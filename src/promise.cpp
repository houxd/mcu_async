#include "promise.h"
#include <console.h>
#include <poll.h>
#include <timeout.h>
#include <async.h>

static auto start_int_promise(int x) {
    return Promise<int>([x](auto resolve) { set_timeout(1000, [=] { resolve(x); }); });
}

static auto start_void_promise() {
    return Promise<void>([](auto resolve) { set_timeout(1000, [=] { resolve(); }); });
}

static auto start_multi_promise(char x, double y) {
    return Promise<Tuple<char, double>>(
        [=](auto resolve) { set_timeout(1000, [=] { resolve({x, y}); }); });
}

void cmd_test_promise(Env e) {
    start_int_promise(8).then([=](int result) {
        e.io().printf("result: %d, except: 8\n", result);
    });

    start_void_promise().then_([=] { // void类型需要用 then_, 使用then需要char类型参数占位
        printf("void promise resolved\n");
    });

    start_multi_promise(3, 7.5).then([=](auto res) {
        e.io().printf("result: %c, %f, except: 3, 7.5\n", res._0(), res._1());
    });

    {
        auto p1 = start_int_promise(99);
        auto p2 = start_void_promise();
        promise_all(p1, p2).then([=](auto res) {
            e.io().printf("promise_all result: %d, except: 99\n", res._0());
        });
    }
}
