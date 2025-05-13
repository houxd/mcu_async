/*
 *  Created on: Feb 28, 2024
 *      Author: houxd
 */

#ifndef POLL_H
#define POLL_H

#include <types.h>
#include <str.h>

using IdType = uint64_t;

struct Poll {
    IdType id = 0;
public:
    Poll();
    Poll(IdType id);
    Poll(const Poll& other);
    Poll& operator=(const Poll& other) {
        id = other.id;
        return *this;
    }
    bool is_null() const;
    bool is_not_null() const;
    void set_null();
    bool is_active() const;
    void remove() const;
};
using PollFunc = Func<void()>;
using PollFunc_1 = Func<void(Poll pid)>;
extern Poll set_poll(const PollFunc &cb);
extern Poll set_poll(const PollFunc_1 &cb);
using OnceFunc = Func<void()>;
extern void set_once(const OnceFunc& cb);
extern void poll();


class Task {
    IdType _task_id;
    Str _name;
    bool _run;
    List<Func<void()>> _deletors;
    friend void _set_poll_thread(Shared<Task> thread);
public:
    Task();
    virtual ~Task();
    Str name() const { return _name; }
    void set_name(const Str& name) { _name = name; }
    IdType task_id() const { return _task_id; }
    void terminal();
    bool is_running();
    void install_deletor(const Func<void()>& deletor);
protected:
    virtual void init() {}
};
extern void start_task(Shared<Task> thread);
extern Shared<Task> start_task(const Func<void(Task*)>& cb);
extern Shared<Task> get_task(IdType task_id);
extern Shared<Task> get_task(const Str& name);
extern Shared<Task> get_current_task();

extern void _test_poll();

#endif // POLL_H
