#include <lib.h>
#include <console.h>
#include <uart_buf.h>
#include <async.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int setup_nonblocking_stdin() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

int nonblocking_getchar() {
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) <= 0) {
        return EOF;
    }
    return c;
}

void cmd_version(Env e) {
    e.io().printf("MCU Async demo v1.0\n");
    e.exit(0);
}

void cmd_echo(Env e) {
    if (e.args.size() < 2) {
        e.io().printf("Usage: echo <text>\n");
        e.exit(1);
        return;
    }
    
    for (size_t i = 1; i < e.args.size(); i++) {
        e.io().printf("%s ", e.args[i].data());
    }
    e.io().printf("\n");
    e.exit(0);
}

#if __cplusplus >= 202002L
Async<void> cmd_async_demo(Env e) {
    e.io().printf("async demo start...\n");
    
    for (int i = 1; i <= 5; i++) {
        e.io().printf("count: %d\n", i);
        co_await sleep_ms(1000);
    }
    
    e.io().printf("async demo end.\n");
    e.exit(0);
}
#endif

// 用户命令表
const CommandEntry user_cmd_list[] = {
    {"version", cmd_version, "show version"},
    {"echo", cmd_echo, "echo text"},
#if __cplusplus >= 202002L
    {"async_demo", cmd_async_demo, "demo async command"},
#endif
    {nullptr, nullptr, nullptr} // end flag
};


void main_task(Task *task) {
    printf("========== MCU Console Async Example ==========\n");

    // setup non-blocking stdin, emulate UART
    setup_nonblocking_stdin();

    // create uart controller
    auto uart = uart_controller_start(1024, [](const char* data, int size){
        // handle uart tx data
        for (int i = 0; i < size; i++) {
            putchar(data[i]);
        }
    });

    // set a poll task to handle non-blocking stdin input
    set_poll([=] {
        int c = nonblocking_getchar();
        if (c != EOF) {
            uart->uart_intput(c);
        }
    });
    
    // start console
    auto console = console_start(uart.get(), user_cmd_list);
}

int main() {

    start_task(main_task)->set_name("main");

    poll(); // handle poll tasks, noreturn
}
