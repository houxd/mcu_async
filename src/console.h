/*
 * console.h
 *
 *  Created on: Feb 20, 2024
 *      Author: houxd
 */
#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <poll.h>
#include <str.h>
#include <uart_buf.h>
#include <vec.h>
#include <async.h>

struct CommandEntry;
class Console;

struct Env {
    const Vec<StrView>& args;
    Console *console;
    UartBuf& io() const;
    const CommandEntry* user_cmd_list();
    const CommandEntry* builtin_cmd_list();
    void exit(int status_code);
    friend class Console;
};

struct CommandFunc {
    void (*_sync_cmd)(Env) = nullptr;
    CommandFunc(void (*sync_cmd)(Env)) : _sync_cmd(sync_cmd) {}
#if __cplusplus >= 202002L
    Async<void> (*_async_cmd)(Env) = nullptr;;
    CommandFunc(Async<void> (*async_cmd)(Env)) : _async_cmd(async_cmd) {}
#endif
    void operator()(Env env) const {
        if (_sync_cmd) {
            _sync_cmd(env);
        }
#if __cplusplus >= 202002L
        else if (_async_cmd) {
            _async_cmd(env);
        }
#endif
        else {
            env.exit(1);
        }
    }
    CommandFunc(std::nullptr_t) {}
};

struct CommandEntry {
    const char *name;
    CommandFunc cmd;
    const char *help;
};

class Console : public Task {
    friend class Env;
    static inline const char *CONSOLE_PROMPT = "> ";
    static inline const char *CONSOLE_ISSUE = "Wellcome Console!\n";
    static constexpr int MAX_HISTORY = 20;  // 历史命令最大记录数

  private:
    UartBuf *_buf;
    const CommandEntry *_user_cmd_list;
    Shared<Task> _cmd_thread;
    Str _cmdline;
    Vec<StrView> _args;
    int _status_code = 0;
    
    // 命令历史相关
    Vec<Str> _cmd_history;        // 历史命令列表
    int _history_index = -1;      // 当前历史命令索引，-1表示没有选择历史命令
    
    // 光标相关
    size_t _cursor_pos = 0;       // 光标在当前命令行中的位置
    
    // 特殊按键处理相关
    int _escape_state = 0;        // 转义序列状态
    int _escape_buf[8] = {0};     // 转义序列缓存
    int _escape_len = 0;          // 当前转义序列长度

  public:
    Console(UartBuf *uart_buf, const CommandEntry *cmd_list);
    ~Console();

  private:
    void init() override;
    Vec<StrView> pares_cmd(const StrView &cmd);
    const CommandEntry* find_cmd(const Str& name, const CommandEntry* list);
    
    // 新增方法
    void add_to_history(const Str& cmd);
    void navigate_history(int direction);  // 上下移动历史命令，1表示向上，-1表示向下
    void redisplay_cmdline();              // 重新显示当前命令行
    void move_cursor(int direction);       // 左右移动光标，1表示向右，-1表示向左
    void handle_escape_sequence(char c);   // 处理转义序列
    void handle_normal_char(char c);       // 处理普通字符
    void handle_delete(bool backward);     // 处理删除，backward为true表示向后删除(退格键)
};

extern Shared<Console> console_start(UartBuf *uart_buf, const CommandEntry *cmd_list);

#endif /* CONSOLE_H_ */
