/*
 * console.cpp
 *
 *  Created on: Feb 20, 2024
 *      Author: houxd
 */

#include <console.h>
#include <types.h>
#include <poll.h>
#include <timeout.h>

extern void cmd_ps(Env env);
extern void cmd_kill(Env env);
extern void cmd_killall(Env env);

static u32 _total_mem = 0;

static u32 calc_mem_free() {
    constexpr u32 MAX_BLOCK_INDEX = 24; // 32; 
    u32 total = 0;
    u32 block = 1UL << MAX_BLOCK_INDEX;
    void *ptrs[MAX_BLOCK_INDEX];
    u32 ptrs_count = 0;
    
    // 初始化指针数组
    for(u32 i = 0; i < MAX_BLOCK_INDEX; ++i) {
        ptrs[i] = nullptr;
    }

    // 分配内存块直到没有更多的内存
    while (block > 0) {
        void *p = malloc(block);
        if (p) {
            total += block;
            ptrs[ptrs_count++]  = p;
        }
        block >>= 1;
    }

    // 释放已分配的内存块
    for(u32 i = 0; i < ptrs_count; ++i) {
        free(ptrs[i]);
    }

    return total;
}

void cmd_free(Env e) {
    u32 free_ = calc_mem_free();
    e.io().printf("free: %u bytes(%d%%), total: %u bytes\n", (unsigned)free_, int(free_*100/_total_mem), (unsigned)_total_mem);
    e.exit(0);
}

void cmd_pref(Env e) {
    auto c = make_shared<int>(0);
    set_poll([=] { ++(*c); });
    set_interval(1000, [=] {
        e.io().printf("poll freq: %d times/sec\n", *c);
        *c = 0;
    });
}

extern void cmd_test_async(Env e);
extern void cmd_test_promise(Env e);
extern void cmd_help(Env e) {
    e.io().puts("Available commands:\n");
    for (const CommandEntry *entry = e.builtin_cmd_list(); entry->name != nullptr; ++entry) {
        e.io().printf("  %-12s %s\n", entry->name, entry->help);
    }
    for (const CommandEntry *entry = e.user_cmd_list(); entry->name != nullptr; ++entry) {
        e.io().printf("  %-12s %s\n", entry->name, entry->help);
    }
    e.io().flush();
    e.exit(0);
}
static const CommandEntry _builtin_cmd_table[] = {
    {"?", cmd_help, "Show help"},
    {"help", cmd_help, "Show help"},
    {"ps", cmd_ps, "Show thread status"},
    {"kill", cmd_kill, "Kill thread"},
    {"killall", cmd_killall, "Kill all threads"},
    {"free", cmd_free, "Show free heap size"},
    {"pref", cmd_pref, "Show poll frequency"},
    {"test_promise", cmd_test_promise, "Test promise"},
    {"test_async", cmd_test_async, "Test async"},
    {nullptr, nullptr, nullptr} // 结束标志
};

UartBuf &Env::io() const
{
    return *console->_buf;
}

const CommandEntry *Env::user_cmd_list()
{
    return this->console->_user_cmd_list;
}

const CommandEntry *Env::builtin_cmd_list()
{
    return _builtin_cmd_table;
}

void Env::exit(int status_code)
{
    console->_status_code = status_code;
    if(console->_cmd_thread) {
        console->_cmd_thread->terminal();
    }
}

Console::Console(UartBuf *uart_buf, const CommandEntry *cmd_list)
    : _buf(uart_buf), _user_cmd_list(cmd_list), _cursor_pos(0)
{
    _buf->set_lf2crlf_enable(true);
}

Console::~Console()
{
    if(_cmd_thread->is_running()) {
        _cmd_thread->terminal();
        _cmd_thread.reset();
    }
}

void Console::init() {
    _total_mem = calc_mem_free();
    _buf->clear_rx();
    auto version_cmd = find_cmd("version", _user_cmd_list);
    if(version_cmd) {
        version_cmd->cmd(Env{Vec<StrView>{"version"}, this});
    }
    _buf->puts(CONSOLE_ISSUE);
    _buf->puts(CONSOLE_PROMPT);
    _buf->flush();
    _cmd_thread.reset();
    _cmdline.clear();
    _cursor_pos = 0;
    _escape_state = 0;
    _escape_len = 0;
    set_poll([this] {
        if (_cmd_thread) {
            if(_cmd_thread->is_running()) {
                // 如果命令线程正在运行, 则不处理输入只接受ctrl_c
                int res = _buf->getc();
                if (res == 0x3) { // ctrl-c
                    _cmd_thread->terminal();
                }
            } else {
                // 线程已经停止, 准备下一个命令行
                _cmd_thread.reset();
                _cmdline.clear();
                _cursor_pos = 0;
                _escape_state = 0;
                _escape_len = 0;
                _buf->clear_rx();
                _buf->puts(CONSOLE_PROMPT); // 显示提示符
                _buf->flush();
            }
        } else {
            // 接收字符
            int c = _buf->getc();
            if (c == -1) return; // 无字符输入
            //printf("%x\n", c);
            
            if (_escape_state > 0) {
                // 正在处理转义序列
                handle_escape_sequence(c);
                return;
            }
            
            switch (c) {
                case 0x3: // Ctrl-C
                    _buf->puts("^C\n");
                    _buf->puts(CONSOLE_PROMPT);
                    _buf->flush();
                    _cmdline.clear();
                    _cursor_pos = 0;
                    _history_index = -1;
                    break;
                    
                case 0x1B: // ESC
                    _escape_state = 1;
                    _escape_len = 0;
                    break;
                    
                case '\b': // Backspace
                case 0x7F: // Delete
                    handle_delete(true);
                    break;
                    
                case '\r': // Enter
                case '\n': // Enter
                    _buf->puts("\n");
                    if (_cmdline.is_empty()) {
                        _buf->puts(CONSOLE_PROMPT);
                        _buf->flush();
                        break;
                    }
                    _buf->flush();

                    // 添加到历史记录
                    add_to_history(_cmdline);
                    _history_index = -1;

                    // 解析命令参数
                    _args = pares_cmd(_cmdline.trim());

                    if(_args.length()>0) {
                        // 创建命令线程
                        _cmd_thread = start_task([this](Task* cmd_task) {
                            auto cmd = _args[0];
                            cmd_task->set_name(cmd);

                            // 寻找内置命令
                            auto entry = this->find_cmd(cmd, _builtin_cmd_table);
                            if(entry) {
                                entry->cmd(Env{_args, this});
                            }else {
                                entry = this->find_cmd(cmd, _user_cmd_list);
                                if(entry) {
                                    this->_status_code = 0;
                                    entry->cmd(Env{_args, this});
                                }else {
                                    this->_status_code = 1;
                                    printf("Command not found.\n");
                                }
                            }
                        });
                    }else {
                        // empty input?
                        _buf->puts(CONSOLE_PROMPT);
                        _buf->flush();
                    }
                    break;
                    
                default:
                    handle_normal_char(c);
                    break;
            }
        }
    });
}

// 添加命令到历史记录
void Console::add_to_history(const Str& cmd) {
    if (cmd.is_empty()) return;
    
    // 如果命令与最后一条历史命令相同，则不添加
    if (!_cmd_history.empty() && _cmd_history.back() == cmd) {
        return;
    }
    
    _cmd_history.push_back(cmd);
    
    // 如果历史记录超过最大数量，删除最旧的记录
    if (_cmd_history.size() > MAX_HISTORY) {
        _cmd_history.erase(_cmd_history.begin());
    }
}

// 上下浏览历史命令
void Console::navigate_history(int direction) {
    if (_cmd_history.empty()) return;
    
    // 保存当前编辑的命令
    if (_history_index == -1) {
        _history_index = _cmd_history.size();
    }
    
    // 向上浏览历史命令
    if (direction > 0) {
        if (_history_index > 0) {
            _history_index--;
            _cmdline = _cmd_history[_history_index];
        }
    }
    // 向下浏览历史命令
    else if (direction < 0) {
        if (size_t(_history_index) < _cmd_history.size() - 1) {
            _history_index++;
            _cmdline = _cmd_history[_history_index];
        } else {
            _history_index = -1;
            _cmdline.clear();
        }
    }
    
    // 更新光标位置到命令末尾
    _cursor_pos = _cmdline.length();
    
    // 重新显示命令行
    redisplay_cmdline();
}

// 重新显示当前命令行
void Console::redisplay_cmdline() {
    // 先移动到行首
    _buf->puts("\r");
    
    // 清除当前行 - 先计算潜在的最大显示长度
    size_t max_display_length = strlen(CONSOLE_PROMPT);
    
    // 考虑历史命令可能的最大长度
    if (_history_index >= 0 && size_t(_history_index) < _cmd_history.size()) {
        max_display_length += _cmd_history[_history_index].length();
    } else {
        max_display_length += _cmdline.length();
    }
    
    // 为了安全起见，多清除几个字符
    max_display_length += 5;
    
    // 打印空格覆盖整行
    for (size_t i = 0; i < max_display_length; i++) {
        _buf->putc(' ');
    }
    
    // 重新显示提示符和命令
    _buf->puts("\r");
    _buf->puts(CONSOLE_PROMPT);
    _buf->puts(_cmdline.c_str());
    
    // 移动光标到正确位置
    if (_cursor_pos < _cmdline.length()) {
        // 如果光标不在行尾，需要移回到正确位置
        size_t back_count = _cmdline.length() - _cursor_pos;
        for (size_t i = 0; i < back_count; i++) {
            _buf->puts("\b");
        }
    }
    
    _buf->flush();
}

// 左右移动光标
void Console::move_cursor(int direction) {
    if (direction > 0) {
        // 右移光标
        if (_cursor_pos < _cmdline.length()) {
            _cursor_pos++;
            _buf->putc(_cmdline[_cursor_pos-1]);
            _buf->flush();
        }
    } else if (direction < 0) {
        // 左移光标
        if (_cursor_pos > 0) {
            _cursor_pos--;
            _buf->puts("\b");
            _buf->flush();
        }
    }
}

// 处理转义序列
void Console::handle_escape_sequence(char c) {
    // 保存当前转义字符
    _escape_buf[_escape_len++] = c;
    
    // 简单的ANSI转义序列判断
    if (_escape_state == 1) {
        if (c == '[') {
            _escape_state = 2;
            return;
        } else {
            // 不是ANSI序列，放弃
            _escape_state = 0;
            return;
        }
    }
    
    if (_escape_state == 2) {
        switch (c) {
            case 'A': // 上箭头
                _escape_state = 0;
                navigate_history(1); // 向上浏览历史
                break;
                
            case 'B': // 下箭头
                _escape_state = 0;
                navigate_history(-1); // 向下浏览历史
                break;
                
            case 'C': // 右箭头
                _escape_state = 0;
                move_cursor(1); // 右移光标
                break;
                
            case 'D': // 左箭头
                _escape_state = 0;
                move_cursor(-1); // 左移光标
                break;
                
            case '3': // 可能是Delete键的一部分
                _escape_state = 3;
                break;
                
            default:
                if (c >= '0' && c <= '9') {
                    // 可能是更复杂的序列，继续等待
                    return;
                }
                // 不支持的序列，放弃
                _escape_state = 0;
                break;
        }
    }
    
    if (_escape_state == 3 && c == '~') {
        // Delete键
        _escape_state = 0;
        handle_delete(false); // 向前删除
    }
    
    // 防止转义序列过长
    if (_escape_len >= 8) {
        _escape_state = 0;
    }
}

// 处理普通字符输入
void Console::handle_normal_char(char c) {
    if (c >= 32 && c < 127) {
        // 在光标位置插入字符
        if (_cursor_pos == _cmdline.length()) {
            // 在命令行末尾添加字符
            _buf->putc(c);
            _buf->flush();
            _cmdline.append(c);
        } else {
            // 在命令行中间插入字符
            _cmdline.insert(_cursor_pos, 1, c);
            
            // 重新显示插入点之后的部分
            _buf->putc(c);
            for (size_t i = _cursor_pos + 1; i < _cmdline.length(); i++) {
                _buf->putc(_cmdline[i]);
            }
            
            // 将光标移回到插入位置的后一位
            for (size_t i = _cursor_pos + 1; i < _cmdline.length(); i++) {
                _buf->puts("\b");
            }
            _buf->flush();
        }
        _cursor_pos++;
    }
}

// 处理删除字符
void Console::handle_delete(bool backward) {
    if (backward) {
        // Backspace: 删除光标前的字符
        if (_cursor_pos > 0 && !_cmdline.is_empty()) {
            _cmdline.erase(_cursor_pos - 1, 1);
            _cursor_pos--;
            
            // 重新显示修改后的命令行
            _buf->puts("\b");
            for (size_t i = _cursor_pos; i < _cmdline.length(); i++) {
                _buf->putc(_cmdline[i]);
            }
            _buf->putc(' '); // 清除最后一个字符
            
            // 移动光标到正确位置
            for (size_t i = _cursor_pos; i <= _cmdline.length(); i++) {
                _buf->puts("\b");
            }
            _buf->flush();
        }
    } else {
        // Delete: 删除光标处的字符
        if (_cursor_pos < _cmdline.length()) {
            _cmdline.erase(_cursor_pos, 1);
            
            // 重新显示修改后的命令行
            for (size_t i = _cursor_pos; i < _cmdline.length(); i++) {
                _buf->putc(_cmdline[i]);
            }
            _buf->putc(' '); // 清除最后一个字符
            
            // 移动光标到正确位置
            for (size_t i = _cursor_pos; i <= _cmdline.length(); i++) {
                _buf->puts("\b");
            }
            _buf->flush();
        }
    }
}

Vec<StrView> Console::pares_cmd(const StrView &cmd) {
    Vec<StrView> args;
    const char* src = cmd.data();
    size_t len = cmd.size();
    bool in_quotes = false;
    char quote_char = '\0';
    const char* token_start = nullptr;

    // 辅助函数: 将 [start, end) 推入 args
    auto push_token = [&](const char* start, const char* end) {
        if (start && end > start) {
            args.push_back(StrView(start, end - start));
        }
    };

    for (size_t i = 0; i < len; i++) {
        char c = src[i];
        if (!in_quotes) {
            if (c == '"' || c == '\'') {
                // 进入引号区间
                in_quotes = true;
                quote_char = c;
                if (!token_start) {
                    // 若当前还未开始记录，跳过本引号，从下一个字符开始
                    token_start = src + i + 1;
                }
            } else if (isspace((unsigned char)c)) {
                // 推入之前累積的段落
                push_token(token_start, src + i);
                token_start = nullptr;
            } else if (!token_start) {
                // 新的段落起点
                token_start = src + i;
            }
        } else {
            // 位于引号内部
            if (c == quote_char) {
                // 遇到同符号引号则视为结束, 将引号内的区段加入
                in_quotes = false;
                push_token(token_start, src + i);
                token_start = nullptr;
            }
        }
    }
    // 收尾处理
    if (token_start) {
        push_token(token_start, src + len);
    }
    return args;
}

const CommandEntry *Console::find_cmd(const Str &name, const CommandEntry *list)
{
    for (const CommandEntry *entry = list; entry->name != nullptr; ++entry) {
        if (name == entry->name) {
            return entry;
        }
    }
    return nullptr;
}

Shared<Console> console_start(UartBuf* uart_buf, const CommandEntry* cmd_list)
{
    auto res = make_shared<Console>(uart_buf, cmd_list);
    start_task(res);
    return res;
}
