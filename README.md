# MCU Promise/Async Library

MCU Promise/Async Library is an asynchronous runtime library designed for microcontrollers, implementing a lightweight asynchronous programming model using C++20 coroutines, making microcontroller development more efficient and manageable.

## Features

- **Promise/Future Pattern**: Similar to JavaScript Promises, facilitating asynchronous operations
- **Lightweight Task Model**: Task scheduling system based on polling
- **Serial Console**: Built-in command-line interface with history and command completion
- **C++20 Coroutine Support**: Simplifies asynchronous code writing using modern C++ features **(Not fully implemented, Memory Leaks)**
- **Built-in Utility Libraries**:
  - `Poll`/`Task`: Polling tasks
  - `Timeout`: Timed tasks
  - `Promise`: Similar to JavaScript Promises, for use when coroutines are not supported
  - `Stream`: Passes multiple asynchronous values when coroutines are not supported
  - `Tuple`/`Vec<T>`/`Str`/`StrView`: Standard library wrappers for ease of use
  - `Buf<T>`: Circular buffer
  - `UartBuf`: Serial buffer handling
  - `Printf`/`Scanf`: Formatted input/output
  - `retarget`: Redirects printf to UartBuf
  - `Console`: Support for a simple console
  - `Async`: C++ coroutine async function support **(TODO)**


### Building the Library

```bash
git clone https://github.com/yourusername/mcu_async.git
cd mcu_async
mkdir build && cd build
cmake ..
make
```

## Usage

### 1. Using Promises

```cpp
#include <promise.h>

auto start_request() {
    return Promise<int>([](auto resolve) {
        set_timeout(1000, [=] {
            resolve(42);  // Resolve the Promise after 1 second
        });
    });
}

auto wait_request() {
    return Promise<void>([](auto resolve) {
        start_request().then([resolve](int result){
        	printf("Received result: %d\n", result);
            resolve(char{});
        });
    });
}

void use_promise() {
    wait_request().then([](auto) {
        printf("all promise done %d\n");
    });
}

void main() {
    use_promise();
    poll(); // Start the asynchronous polling environment
}
```

### 2. Use Task

```cpp

void wathc_dog_task(Task *self) {
    MX_IWDG_Init();
	set_poll([]{
        HAL_IWDG_Refresh(&hiwdg);
    });
}

static Shared<UartBuf> _uart2_buf; // console uart
static uint8_t _uart2_rx_byte;

void uart2_start() {
    _uart2_buf = uart_controller_start(128, [](const char *data, int n) {
        HAL_UART_Transmit(&huart2, (uint8_t *)data, n, HAL_MAX_DELAY);
    });
    _uart2_buf->set_name("console_uart2");
    HAL_UART_Receive_IT(&huart2, &_uart2_rx_byte, 1);
}

void main() {
    uart2_start();
	retarget_stdio(_uart2_buf.get());
	console_start(_uart2_buf.get(), _cmd_table)->set_name("console");
    // create a task
    auto task = start_task("wathc_dog_task", wathc_dog_task);
    set_interval(30000, [task]{
        task.stop(); // stop task after 30 seconds.
    });
    poll(); // Start the asynchronous polling environment
}

// UART2 Rx Interrupt callback
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart == &huart2) {
        _uart2_buf->uart_intput(_uart2_rx_byte);
        HAL_UART_Receive_IT(&huart2, &_uart2_rx_byte, 1);
    } 
}
```

- Use console to management tasks, kill watchdog task can make mcu restart.

  ![image-task](https://raw.githubusercontent.com/houxd/mcu_promise/refs/heads/master/examples/image-task.png)

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

// Command table
const CommandEntry user_cmd_list[] = {
    {"hello", cmd_hello, "Print welcome message"},
    {nullptr, nullptr, nullptr}  // End marker
};

// Start the console
auto uart = uart_controller_start(1024, uart_write);
auto console = console_start(uart.get(), user_cmd_list);
```

## API Reference

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

## 

### Async API(TODO)

- `Async<T>` - Coroutine return type
- `co_await` - Wait for asynchronous operation
- `co_return` - Return asynchronous result
- `sleep_ms(ms)` - Asynchronous wait in milliseconds
- `sleep_sec(sec)` - Asynchronous wait in seconds
- `loop_when(condition)` - Loop while condition is true

## License

This project is licensed under the MIT License - see the LICENSE file for details.
