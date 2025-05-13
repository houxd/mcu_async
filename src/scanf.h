#ifndef SCANF_H
#define SCANF_H

#include <cstdarg>

class Scanf {
protected:
    // 派生类需要实现此函数以读取单个字符，若无数据则返回EOF(-1)
    virtual int scanf_read_char() = 0;

    int vscanf(const char *fmt, va_list args);

public:
    int scanf(const char *fmt, ...) __attribute__((format(scanf, 2, 3))) {
        va_list args;
        va_start(args, fmt);
        int ret = vscanf(fmt, args);
        va_end(args);
        return ret;
    }
};

#endif // SCANF_H
