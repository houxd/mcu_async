/*
 *  Created on: Feb 28, 2024
 *      Author: houxd
 */

#include <poll.h>
#include <console.h>
#include <types.h>

#ifdef QT_CORE_LIB
#include <QDebug>
#include <QString>
#endif

static IdType new_id() {
    static IdType _next_id = 0;
    IdType id = ++_next_id;
    if (id == 0) {
        assert(0 && "IdType overflow");
    }
    return id;
}

Poll::Poll(IdType id) { this->id = id; }

Poll::Poll() { id = 0; }

Poll::Poll(const Poll &other) { id = other.id; }


enum PollFlag {
    PF_ONCE = 1u << 0,
    PF_DELETE = 1u << 1,
};

struct PollNode {
    IdType _id;
    uint _flags;
    PollFunc _cb;
    Shared<Task> _task;
    PollNode(Shared<Task> task, IdType id, uint32_t flags, const PollFunc &cb) {
        _id = id;
        _flags = flags;
        _cb = cb;
        _task = task;
    }
    PollNode(const PollNode &other) {
        _id = other._id;
        _flags = other._flags;
        _cb = other._cb;
        _task = other._task;
    }
};

List<PollNode> _poll_list = {};
Shared<Task> _current_task = nullptr;

Poll set_poll(const PollFunc &cb) {
    IdType id = new_id();
    _poll_list.push_back(PollNode(_current_task, id, 0, cb));
    return Poll{id};
}

Poll set_poll(const PollFunc_1 &cb) {
    auto id = new_id();
    _poll_list.push_back(PollNode(_current_task, id, 0, std::bind(cb, Poll{id})));
    return Poll{id};
}

void set_once(const OnceFunc &cb) {
    // set once
    _poll_list.push_back(PollNode(_current_task, new_id(), PF_ONCE, cb));
}

bool Poll::is_null() const { return id == 0; }

bool Poll::is_not_null() const { return id != 0; }

void Poll::set_null() { id = 0; }

bool Poll::is_active() const {
    if (id == 0)
        return false;
    // 遍历_pollItems, 查找是否有相同的id
    for (auto &item : _poll_list) {
        if (item._id == id) {
            return true;
        }
    }
    return false;
}

void Poll::remove() const {
    if (id == 0)
        return;
    for (auto &item : _poll_list) {
        if (item._id == this->id) {
            item._flags |= PF_DELETE;
            return;
        }
    }
}

void poll() {
    while (1) {
        // 检查并执行回调
        for (auto it = _poll_list.begin(); it != _poll_list.end();) {
            auto &item = *it;
            // 删除标记的回调
            if (item._flags & PF_DELETE) {
                it = _poll_list.erase(it);
            } else {
                _current_task = item._task;
                item._cb();
                _current_task = nullptr;

                // 如果是一次性回调, 则标记删除
                if (item._flags & PF_ONCE) {
                    item._flags |= PF_DELETE;
                }

                ++it;
            }
        }
    }
}

static bool terminal_task_by_id(IdType task_id) {
    bool res = false;
    for (auto &item : _poll_list) {
        if (item._task->task_id() == task_id) {
            item._flags |= PF_DELETE;
            res = true;
        }
    }
    return res;
}

void _set_poll_thread(Shared<Task> task) {
    auto id = new_id();
    Poll main_poll{id};
    _poll_list.push_back(PollNode(task, id, 0, [task, main_poll]{
        // check poll once
        if (task->_run == false) {
            task->_run = true;
            task->init();
        }else {
            // check sub polls
            for (auto &item : _poll_list) {
                if (item._task->_task_id == task->_task_id) {
                    if (item._task->_task_id == item._id) {
                        // 跳过主节点, 主节点仅仅用于监视活动的子节点
                        continue;
                    }
                    if ((item._flags & PF_DELETE) == 0) {
                        // 存在活动的子节点, 则继续轮训
                        return;
                    }
                }
            }

            // 没有活动的子节点, 则删除主节点, 主节点绑定了thread指针, 将被释放
            main_poll.remove();
            task->_run = false;
        }
    }));
    task->_task_id = id;
}

Task::Task() : _task_id(0), _name("noname"), _run(false), _deletors() {}
Task::~Task() {
    for(const auto& d: _deletors) {
        d();
    }
}
void Task::terminal() {
    for (auto &item : _poll_list) {
        if (item._task->_task_id == this->_task_id) {
            if (item._task->_task_id == item._id) {
                // 忽略主节点, 只要删除线程的所有子节点, 他的主节点poll会终止自己
                continue;
            }
            item._flags |= PF_DELETE;
        }
    }
}
bool Task::is_running() { return Poll(this->_task_id).is_active(); }

void Task::install_deletor(const Func<void ()> &deletor)
{
    this->_deletors.push_back(deletor);
}
void start_task(Shared<Task> thread) {
    if(thread==nullptr)
        return;
    _set_poll_thread(thread);
}
Shared<Task> get_task(IdType task_id) {
    for (auto &item : _poll_list) {
        if (item._task->task_id() == task_id) {
            return item._task;
        }
    }
    return nullptr;
}
Shared<Task> get_task(const Str& name) {
    for (auto &item : _poll_list) {
        if (item._task->name() == name) {
            return item._task;
        }
    }
    return nullptr;
}

class _SimpleTask: public Task {
    Func<void(Task*)> _cb;
    void init() override {
        _cb(this);  //单次轮训: 直接调用
    }
public:
    _SimpleTask(const Func<void(Task*)>& cb): Task(), _cb(cb) {}
    ~_SimpleTask() override = default;
};
Shared<Task> start_task(const Func<void(Task*)>& cb) {
    auto thread = make_shared<_SimpleTask>(cb);
    _set_poll_thread(thread);
    return thread;
}

void cmd_ps(Env e) {
    auto& io = e.io();
    auto& args = e.args;
    // 检查 -h --help参数
    if (args.length() > 1 && (args[1] == "-h" || args[1] == "--help")) {
        io.printf("Usage: ps [-p]\n");
        io.printf("Options:\n");
        io.printf("  -p: Print poll list\n");
        return;
    }

    struct TaskInfo {
        IdType _threadId;
        uint _pollIdCount;
        uint _flags;
        Str _name;
    };
    
    List<TaskInfo> thread_info_list;
    for(auto &item : _poll_list) {
        if (item._task->task_id() == item._id) {
            // main thread
            auto it = std::find_if(thread_info_list.begin(), thread_info_list.end(),
                                   [&](const auto &t) { return t._threadId == item._task->task_id(); });
            if (it == thread_info_list.end()) {
                thread_info_list.push_back({
                    item._task->task_id(),
                    1,
                    item._flags,
                    item._task->name()
                });
            }
        }
        else {
            // sub thread
            auto it = std::find_if(thread_info_list.begin(), thread_info_list.end(),
                                   [&](const auto &t) { return t._threadId == item._task->task_id(); });
            if (it != thread_info_list.end()) {
                it->_pollIdCount++;
                it->_flags |= item._flags;
            }
        }
    }

    io.printf("%8s %6s %6s %s\n", "ThreadId", "Flags", "Polls", "NAME" );
    for (auto &item : thread_info_list) {
        io.printf("%llu %6u %6u %s\n", (unsigned long long)item._threadId, item._flags, item._pollIdCount, item._name.c_str());
    }
    
    // 检查 -p 选项打印poll列表
    if (args.length() > 1 && args[1] == "-p") {
        io.printf("Poll List:\n");
        for (auto &item : _poll_list) {
            io.printf("ThreadId: %llu, PollId: %llu, Flags: %u, Name: %s\n", (unsigned long long)item._task->task_id(), 
                    (unsigned long long)item._id, item._flags, item._task->name().c_str());
        }
    }
    io.flush();
    e.exit(0);
}

void cmd_kill(Env e) {
    auto& io = e.io();
    auto& args = e.args;
    if (args.length() < 2) {
        io.printf("Usage: kill <thread_id>\n");
        return;
    }
    IdType task_id = args[0].to_int();
    if(terminal_task_by_id(task_id)) {
        io.printf("Thread %llu killed.\n", (unsigned long long)task_id);
    } else {
        io.printf("Thread %llu not found.\n", (unsigned long long)task_id);
    }
    io.flush();
    e.exit(0);
}

void cmd_killall(Env e) {
    auto& io = e.io();
    auto& args = e.args;
    if(args.length() < 2) {
        io.printf("Usage: killall <thread_name>\n");
        return;
    }
    auto name = args[1];
    for (auto &item : _poll_list) {
        if (item._task->name() == name) {
            if (item._task->task_id() == item._id) {
                // 忽略主节点, 只要删除线程的所有子节点, 他的主节点poll会终止自己
                continue;
            }
            item._flags |= PF_DELETE;
        }
    }
}

void _test_poll() {
    // 测试set_poll
    Shared<int> c = make_shared<int>(0);
    set_poll([c](Poll pid) {
        printf("Poll %llu: count: %d\n", (unsigned long long)pid.id, (*c)++);
        assert(pid.is_active() == true);
        if (*c == 5) {
            pid.remove();
            printf("Poll %llu removed\n", (unsigned long long)pid.id);
            assert(pid.is_active() == false);
        }
    });
    
    // 测试set_poll_once
    set_once([] {
        printf("Poll once\n");
    });
}

Shared<Task> get_current_task()
{
    return _current_task;
}
