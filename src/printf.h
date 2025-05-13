/*
 *  Copyright (C) 2020 by houxd
 *  All rights reserved.
 */
#ifndef PRINTF_H
#define PRINTF_H

#include <cstdlib>
#include <cstdarg>

class Printf {
private:
    int ntoa(char *buf, size_t len, bool negative, unsigned int base,
             unsigned int prec, unsigned int width, unsigned int flags);

    void ltoa(unsigned long value, bool negative, unsigned long base,
              unsigned int prec, unsigned int width, unsigned int flags);

    void lltoa(unsigned long long value, bool negative, unsigned long long base,
               unsigned int prec, unsigned int width, unsigned int flags);

    int ftoa(double value, unsigned int prec, unsigned int width,
             unsigned int flags);

    static int atoi(const char *s, const char **tailptr);

protected:
    virtual void printf_write_char(char c) = 0;

public:
    int vprintf(const char *fmt, va_list va);
    int printf(const char *fmt, ...) __attribute__((format(printf, 2, 3)));
    int println(const char *fmt, ...) __attribute__((format(printf, 2, 3)));
};

#endif // PRINTF_H
