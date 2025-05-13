#ifndef ASYNC_H
#define ASYNC_H

#if __cplusplus >= 202002L

#include <coroutine>
#include <exception>
#include <poll.h>
#include <timeout.h>

template <typename T> struct Async {
    struct promise_type {
        T result;

        Async get_return_object() {
            auto handle = std::coroutine_handle<promise_type>::from_promise(*this);
            get_current_task()->install_deletor([handle]{
                handle.destroy();
            });
            return Async{handle};
        }

        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        template <typename U> std::suspend_always yield_value(U &&value) {
            result = std::forward<U>(value);
            return {};
        }
        template <typename U> void return_value(U &&value) {
            result = std::forward<U>(value);
        }

        void unhandled_exception() {
            auto eptr = std::current_exception();
            try {
                if (eptr) {
                    std::rethrow_exception(eptr);
                }
            } catch (const std::exception &e) {
                printf("Async: [unhandled_exception] what(): %s\n", e.what());
            } catch (...) {
                printf("Async: [unhandled_exception] unknown exception\n");
            }
        }
    };
    std::coroutine_handle<promise_type> handle;
    T result() const { return handle.promise().result; }

    auto operator co_await() {
        struct Awaiter {
            std::coroutine_handle<promise_type> handle;
            bool await_ready() const { return handle.done(); }
            void await_suspend(std::coroutine_handle<> awaiting) {
                set_poll([this, awaiting](Poll p) {
                    if (handle.done()) {
                        awaiting.resume();
                        p.remove();
                    }
                });
            }
            T await_resume() {
                return handle.promise().result;
                // T result = handle.promise().result;
                // handle.destroy(); use poll to destroy
                // return result;
            }
        };
        return Awaiter{handle};
    }
};

#include <exception>
template <> struct Async<void> {
    struct promise_type {
        Async get_return_object() {
            auto handle = std::coroutine_handle<promise_type>::from_promise(*this);
            get_current_task()->install_deletor([handle]{
                handle.destroy();
            });
            return Async{handle};
        }

        std::suspend_never initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        void return_void() {}

        void unhandled_exception() {
        	printf("Async: [unhandled_exception] unknown exception\n");
        }
    };
    std::coroutine_handle<promise_type> handle;

    auto operator co_await() {
        struct Awaiter {
            std::coroutine_handle<promise_type> handle;
            bool await_ready() const { return handle.done(); }
            void await_suspend(std::coroutine_handle<> awaiting) {
                set_poll([this, awaiting](Poll p) {
                    if (handle.done()) {
                        awaiting.resume();
                        p.remove();
                    }
                });
            }
            void await_resume() {
                //handle.destroy(); use poll to destroy
            }
        };
        return Awaiter{handle};
    }
};

struct LoopAwaiter {
    Func<bool()> _cb;
    LoopAwaiter(const Func<bool()>& cb): _cb(cb) {}
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
        auto cb = _cb;
        set_poll([cb, h](Poll p) {
            if(cb()==false) {
                h.resume();
                p.remove();
            }
        });
    }
    void await_resume() {}
};
inline LoopAwaiter loop_when(const Func<bool()>& cb) {
    return LoopAwaiter { cb };
}

struct SleepAwaiter {
    u32 _ms;
    SleepAwaiter(u32 ms) : _ms(ms) {}
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
        set_timeout(_ms, [h]() { h.resume(); });
    }
    void await_resume() {}
};

inline SleepAwaiter sleep_ms(u32 ms) { return SleepAwaiter{ms}; }
inline SleepAwaiter sleep_sec(u32 sec) { return SleepAwaiter{sec * 1000}; }

inline void set_once_async(const Func<Async<void>()>& cb) {
    set_once([cb]{
        (void)cb();
    });
}

inline Shared<Task> start_task_async(const Func<Async<void>(Task*)>& cb) {
    return start_task([cb](Task* task){
    	(void)cb(task);
    });
}

#endif // __cplusplus >= 202002L

#endif // ASYNC_H
