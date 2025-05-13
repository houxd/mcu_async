#include "async.h"
#include <console.h>
#include <promise.h>


#if __cplusplus >= 202002L

Async<char> func2(Env e) {
    e.io().printf("ddddd1\n");
    co_await sleep_ms(2000);
    e.io().printf("ddddd2\n");
    co_return 'b';
}

static auto start_int_promise(int x) {
    return Promise<int>([x](auto resolve) { set_timeout(1000, [=] { resolve(x); }); });
}

Async<int> func1(Env e) {
    static int cc = 0;
    co_await loop_when([e]{
        if(++cc>100) {
            return false;
        }else {
            e.io().printf("loop%d\n", cc);
            return true;
        }
    });
    e.io().printf("hello1\n");
    co_await sleep_ms(1000);
    e.io().printf("hello2\n");
    co_await func2(e);
    int x = co_await start_int_promise(97);
    e.io().printf("%d\n", x);
    co_return 9;
}


void cmd_test_async(Env e) {
    func1(e);
}

#endif // __cplusplus >= 202002L
