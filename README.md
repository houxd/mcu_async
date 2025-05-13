# MCU Async Library

MCU Async is an asynchronous runtime library designed for microcontrollers, implementing a lightweight asynchronous programming model using C++20 coroutines, making microcontroller development more efficient and manageable.

## Features

- **Event-based Asynchronous Programming Model**: Implements cooperative multitasking without an RTOS
- **C++20 Coroutine Support**: Simplifies asynchronous code writing using modern C++ features
- **Promise/Future Pattern**: Similar to JavaScript Promises, facilitating asynchronous operations
- **Lightweight Thread Model**: Task scheduling system based on polling
- **Serial Console**: Built-in command-line interface with history and command completion
- **Built-in Utility Libraries**:
  - `Poll`/`Task`: Polling tasks
  - `Timeout`: Timed tasks
  - `Promise`: Similar to JavaScript Promises, for use when coroutines are not supported
  - `Stream`: Passes multiple asynchronous values when coroutines are not supported
  - `Async`: C++ coroutine async function support
  - `Tuple`/`Vec<T>`/`Str`/`StrView`: Standard library wrappers for ease of use
  - `Buf<T>`: Circular buffer
  - `UartBuf`: Serial buffer handling
  - `Printf`/`Scanf`: Formatted input/output
  - `retarget`: Redirects printf to UartBuf
  - `Console`: Support for a simple console


### Building the Library

```bash
git clone https://github.com/yourusername/mcu_async.git
cd mcu_async
mkdir build && cd build
cmake ..
make
```

## Usage

### 1. Creating Asynchronous Tasks

```cpp
#include <async.h>

// Basic coroutine function
Async<int> calculate(int x) {
    co_await sleep_ms(1000);  // Wait asynchronously for 1 second
    co_return x * 2;          // Return the result
}

// Using coroutines
Async<void> main_task() {
    printf("Calculation starting\n");
    int result = co_await calculate(21);
    printf("Calculation result: %d\n", result);
}

void main() {
    main_task();
    poll(); // Start the asynchronous polling environment
}
```

### 2. Using Promises

```cpp
#include <promise.h>

auto start_request() {
    return Promise<int>([](auto resolve) {
        set_timeout(1000, [=] {
            resolve(42);  // Resolve the Promise after 1 second
        });
    });
}

void use_promise() {
    start_request().then([](int result) {
        printf("Received result: %d\n", result);
    });
}

void main() {
    use_promise();
    poll(); // Start the asynchronous polling environment
}
```

### 3. Setting up Polling and Timers

```cpp
// Set up polling
set_poll([](Poll p) {
    printf("Polling in progress\n");
    if (/* some condition */) {
        p.remove();  // Stop polling
    }
});

// Set up a timer
set_timeout(1000, [] {
    printf("Executed after 1 second\n");
});

// Set up an interval
set_interval(500, [](Timeout t) {
    static int count = 0;
    printf("Executing every 500ms: %d\n", count++);
    if (count >= 10) {
        t.stop();  // Stop the interval
    }
});
```

### 4. Creating Console Commands

```cpp
#include <console.h>

// Synchronous command
void cmd_hello(Env e) {
    e.io().printf("Hello, World!\n");
    e.exit(0);
}

// Asynchronous command
Async<void> cmd_async_hello(Env e) {
    e.io().printf("Asynchronous command starting\n");
    co_await sleep_ms(1000);
    e.io().printf("Asynchronous command completed\n");
    e.exit(0);
}

// Command table
const CommandEntry user_cmd_list[] = {
    {"hello", cmd_hello, "Print welcome message"},
    {"async_hello", cmd_async_hello, "Print asynchronous message"},
    {nullptr, nullptr, nullptr}  // End marker
};

// Start the console
auto uart = uart_controller_start(1024, uart_write);
auto console = console_start(uart.get(), user_cmd_list);
```

## API Reference

### Async API

- `Async<T>` - Coroutine return type
- `co_await` - Wait for asynchronous operation
- `co_return` - Return asynchronous result
- `sleep_ms(ms)` - Asynchronous wait in milliseconds
- `sleep_sec(sec)` - Asynchronous wait in seconds
- `loop_when(condition)` - Loop while condition is true

### Promise API

- `Promise<T>` - Promise type
- `promise.then(callback)` - Add success callback
- `promise_all(p1, p2, ...)` - Wait for multiple Promises

### Task API

- `Task` - Task object
- `start_task(callback)` - Start task
- `start_task_async(callback)` - Start asynchronous task
- `get_task(id)` - Get task
- `get_current_task()` - Get current task

### Polling and Timer API

- `set_poll(callback)` - Set polling
- `set_once(callback)` - Set one-time polling
- `set_timeout(ms, callback)` - Set timeout
- `set_interval(ms, callback)` - Set interval
- `set_once_async(callback)` - Set asynchronous one-time polling

### Console API

- `console_start(uart, cmd_list)` - Start console
- `CommandEntry` - Command entry
- `Env` - Command environment

### Utility Library API

- `Vec<T>` - Dynamic array
- `Str` - String
- `StrView` - String view
- `Buf<T>` - Circular buffer
- `UartBuf` - Serial buffer
- `Printf` - Formatted output
- `Scanf` - Formatted input

## License

This project is licensed under the MIT License - see the LICENSE file for details.
