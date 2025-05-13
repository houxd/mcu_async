#ifndef PROMISE_H
#define PROMISE_H

#include <poll.h>
#include <types.h>

#if __cplusplus >= 202002L
#include <coroutine>
#endif

template <typename T> class Promise {
public:
    using Result = T;
    struct Future {
        Result result;
        bool resolved = false;
    };
    using ResultFunc = Func<void(const Result &)>;
    using ResolveFunc = Func<void(const Result &)>;

    Promise(const Promise &p) : _future(p._future) {}
    Promise(const Func<void(const ResolveFunc &)> &init) {
        _future = std::make_shared<Future>();
        auto weak_future = std::weak_ptr<Future>(_future);
        ResolveFunc resolve_in_future = [weak_future](const Result &result) {
            if (auto future = weak_future.lock()) {
                future->result = result;
                future->resolved = true;
            }
        };
        init(resolve_in_future);
    }
    const Promise &then(const ResultFunc &result_cb) const {
        Shared<Future> future = _future;
        set_poll([future, result_cb](auto poll) {
            if (future->resolved) {
                result_cb(future->result);
                poll.remove();
            }
        });
        return *this;
    }

#if __cplusplus >= 202002L
    // 协程中支持等待Promise
    auto operator co_await() {
        struct Awaiter {
            Promise<T> _promise;
            T _result = T();
            bool await_ready() const {
                return _promise._future->resolved;
            }
            void await_suspend(std::coroutine_handle<> awaiting) {
                _promise.then([this, awaiting](const Result& result){
                    _result = result;
                    awaiting.resume();
                });
            }
            T await_resume() {
                return _result;
            }
        };
        return Awaiter{*this};
    }
#endif // C++20

private:
    Shared<Future> _future;
};

template <> class Promise<void> {
public:
    using Result = char;
    using ResolveFunc = Func<void()>;
    using ResultFunc = Func<void(char)>;
    using VoidResultFunc = Func<void()>;

    Promise(const Promise &p) : _is_resolved(p._is_resolved) {}
    Promise(const Func<void(const ResolveFunc &)> &init) {
        _is_resolved = std::make_shared<bool>(false);
        auto weak_is_resolved = std::weak_ptr<bool>(_is_resolved);
        ResolveFunc resolve_in_future = [weak_is_resolved] {
            if (auto is_resolved = weak_is_resolved.lock()) {
                *is_resolved = true;
            }
        };
        init(resolve_in_future);
    }
    const Promise &then(const ResultFunc &result_cb) const {
        Shared<bool> is_resolved = _is_resolved;
        set_poll([is_resolved, result_cb](auto poll) {
            if (*is_resolved) {
                result_cb(char{});
                poll.remove();
            }
        });
        return *this;
    }
    const Promise &then_(const VoidResultFunc &result_cb) const {
        Shared<bool> is_resolved = _is_resolved;
        set_poll([is_resolved, result_cb](auto poll) {
            if (*is_resolved) {
                result_cb();
                poll.remove();
            }
        });
        return *this;
    }

#if __cplusplus >= 202002L
    // 协程中支持等待Promise
    auto operator co_await() {
        struct Awaiter {
            Promise<void> _promise;
            bool await_ready() const {
                return *_promise._is_resolved;
            }
            void await_suspend(std::coroutine_handle<> awaiting) {
                _promise.then_([awaiting]{
                    awaiting.resume();
                });
            }
            void await_resume() {}
        };
        return Awaiter{*this};
    }
#endif // C++20

private:
    Shared<bool> _is_resolved;
};

template <typename... Promises> auto promise_all(const Promises &...promises) {
    using ResultType = Tuple<typename Promises::Result...>;
    return Promise<ResultType>([=](auto resolve) {
        auto results = std::make_shared<ResultType>(typename Promises::Result()...);
        size_t count = sizeof...(promises);
        auto resolved_count = std::make_shared<size_t>(0);

        handle_promises_impl(results, resolved_count, count, resolve,
                            std::index_sequence_for<Promises...>{},
                            promises...);
    });
}

template <typename ResultTuple, typename... Promises, size_t... Is>
void handle_promises_impl(std::shared_ptr<ResultTuple> results,
                          std::shared_ptr<size_t> resolved_count, size_t count, auto resolve,
                          std::index_sequence<Is...>, const Promises &...promises) {
    // 使用折叠表达式和索引为每个promise创建特定的处理函数
    (promises.then([=](const typename Promises::Result &result) {
        results->template get<Is>() = result;  // 正确地设置结果到对应的元组位置
        (*resolved_count)++;
        if (*resolved_count == count) {
            resolve(*results);
        }
    }),
     ...);
}

#endif // PROMISE_H
