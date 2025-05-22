#ifndef LOG_H
#define LOG_H

#include <printf.h>
#include <uart_buf.h>

class Log : public Printf {
    UartBuf *_output;
    Str _tag;
    bool _reset = true;

public:
    Log(UartBuf *output, const Str &tag);

    // Printf interface
protected:
    void printf_write_char(char c) override;
};

#endif // LOG_H
