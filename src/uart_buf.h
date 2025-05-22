#ifndef UART_BUF_H
#define UART_BUF_H

#include <poll.h>
#include <buf.h>
#include <printf.h>
#include <scanf.h>

class UartBuf: public Task, public Printf, public Scanf {
public:
    using WriteFunc = void(*)(const char*, int);
    using ReadFunc = void(*)(char*, int);
private:
    Buf<char> _tx;
    Buf<char> _rx;
    WriteFunc _write_cb;
    bool _lf2crlf = false;
public:
    UartBuf(int buf_size, WriteFunc write_cb);
    Buf<char>& tx() { return _tx; }
    Buf<char>& rx() { return _rx; }

    int getc();
    int gets(char *s, int n);
    void clear_rx();

    void putc(char c);
    void puts(const char *s);
    void flush();

    int read(void *buf, int n);
    int write(const void *buf, int n);

    // 用于串口中断ISR, 或者轮训输入
    void uart_intput(char c);
    void set_lf2crlf_enable(bool b);

    // Printf, Scanf interface
protected:
    void printf_write_char(char c) override;
    int scanf_read_char() override;

private:
    void init() override;
};

extern Shared<UartBuf> uart_controller_start(int buf_size, UartBuf::WriteFunc write_cb);
extern void _test_uart_buf();

#endif // UART_BUF_H
