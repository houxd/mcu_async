#include <uart_buf.h>
#include <timeout.h>

#ifdef _QT
#include <QDebug>
#endif

void _test_uart_buf()
{
#ifdef _QT

#endif
}


Shared<UartBuf> uart_controller_start(int buf_size, UartBuf::WriteFunc write_cb)
{
    auto res = make_shared<UartBuf>(buf_size, write_cb);
    start_task(res);
    return res;
}

UartBuf::UartBuf(int buf_size, WriteFunc write_cb)
    : _tx(buf_size), _rx(buf_size), _write_cb(write_cb)
{
    assert(_write_cb);
}

int UartBuf::getc() {
    return scanf_read_char();
}

int UartBuf::gets(char *s, int n) {
    int i = 0;
    while (i < n - 1) {
        int res = this->scanf_read_char();
        if (res != -1) {
            char c = (unsigned char)res;
            if (c == '\n') {
                break;
            }
            s[i++] = c;
        } else {
            break;
        }
    }
    s[i] = '\0';
    return i;
}

void UartBuf::clear_rx() {
    _rx.reset();
}

void UartBuf::putc(char c) { printf_write_char(c); }

void UartBuf::puts(const char *s) {
    const char *p = s;
    char c;
    while ((c = *p++)) {
        printf_write_char(c);
    }
}

void UartBuf::flush() {
    if(_tx.size()) {
        int n = _tx.size();
        char *data = new char[n];
        int m = _tx.copy_to(data, n);
        _tx.pop(m);
        _write_cb(data, m);
        delete[] data;
    }
}

void UartBuf::uart_intput(char c) {
    _rx.push(c);
}

void UartBuf::set_lf2crlf_enable(bool b) {
    _lf2crlf = b;
}

void UartBuf::printf_write_char(char c) {
    if(c=='\n') {
        if(_lf2crlf) {
            _tx.push('\r');
            _tx.push('\n');
        }else {
            _tx.push('\n');
        }
        flush();
    }else {
        _tx.push(c);
        if(_tx.is_full()) {
            flush();
        }
    }
}

int UartBuf::scanf_read_char() {
    if(_rx.is_empty())
        return -1;
    unsigned char c = _rx.front();
    _rx.pop();
    return c;
}

void UartBuf::init() {
    set_poll([this]{
        flush();
    });
}
