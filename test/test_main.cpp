#include <stdio.h>

#include <types.h>

// extern void _test_types();
// extern void _test_poll();
// extern void _test_timeout();
// extern void _test_buf();
// extern void _test_str();
// extern void _test_uart_buf();
// extern void _test_promise();

int main() {
    printf("========== Lib MCU Async Test ==========\n");

    _test_enum();
    
    printf("========== Test End ==========\n");
    return 0;
}
