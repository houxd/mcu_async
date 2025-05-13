#include <printf.h>
#include <cctype>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <cstdarg>

static constexpr unsigned PRF_NTOA_BUFSIZ = 32U;
static constexpr unsigned PRF_FTOA_BUFSIZ = 32U;
// flag definitions
static constexpr unsigned FLAGS_ZEROPAD = (1U << 0U);
static constexpr unsigned FLAGS_LEFT = (1U << 1U);
static constexpr unsigned FLAGS_PLUS = (1U << 2U);
static constexpr unsigned FLAGS_SPACE = (1U << 3U);
static constexpr unsigned FLAGS_HASH = (1U << 4U);
static constexpr unsigned FLAGS_UPPERCASE = (1U << 5U);
static constexpr unsigned FLAGS_CHAR = (1U << 6U);
static constexpr unsigned FLAGS_SHORT = (1U << 7U);
static constexpr unsigned FLAGS_LONG = (1U << 8U);
static constexpr unsigned FLAGS_LONG_LONG = (1U << 9U);
static constexpr unsigned FLAGS_PRECISION = (1U << 10U);


template <typename T>
static constexpr T ABS(const T &x) {
    return ((x) > 0 ? (x) : 0 - (x));
}

int Printf::ntoa(char *buf, size_t len, bool negative, unsigned int base, unsigned int prec, unsigned int width, unsigned int flags) {
    size_t nput = 0;
    // pad leading zeros
    if (!(flags & FLAGS_LEFT)) {
        while ((len < prec) && (len < PRF_NTOA_BUFSIZ)) {
            buf[len++] = '0';
        }
        while ((flags & FLAGS_ZEROPAD) && (len < width) &&
               (len < PRF_NTOA_BUFSIZ)) {
            buf[len++] = '0';
        }
    }
    // handle hash
    if (flags & FLAGS_HASH) {
        if (!(flags & FLAGS_PRECISION) && len &&
            ((len == prec) || (len == width))) {
            len--;
            if (len && (base == 16U)) {
                len--;
            }
        }
        if ((base == 16U) && !(flags & FLAGS_UPPERCASE) &&
            (len < PRF_NTOA_BUFSIZ)) {
            buf[len++] = 'x';
        } else if ((base == 16U) && (flags & FLAGS_UPPERCASE) &&
                   (len < PRF_NTOA_BUFSIZ)) {
            buf[len++] = 'X';
        } else if ((base == 2U) && (len < PRF_NTOA_BUFSIZ)) {
            buf[len++] = 'b';
        }
        if (len < PRF_NTOA_BUFSIZ) {
            buf[len++] = '0';
        }
    }
    // handle sign
    if (len && (len == width) &&
        (negative || (flags & FLAGS_PLUS) || (flags & FLAGS_SPACE))) {
        len--;
    }
    if (len < PRF_NTOA_BUFSIZ) {
        if (negative) {
            buf[len++] = '-';
        } else if (flags & FLAGS_PLUS) {
            buf[len++] = '+'; // ignore the space if the '+' exists
        } else if (flags & FLAGS_SPACE) {
            buf[len++] = ' ';
        }
    }
    // pad spaces up to given width
    if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
        for (size_t i = len; i < width; i++) {
            printf_write_char(' ');
            nput++;
        }
    }
    // reverse string
    for (size_t i = 0U; i < len; i++) {
        printf_write_char(buf[len - i - 1U]);
        nput++;
    }
    // append pad spaces up to given width
    if (flags & FLAGS_LEFT) {
        while (nput < width) {
            printf_write_char(' ');
            nput++;
        }
    }
    return nput;
}

void Printf::ltoa(unsigned long value, bool negative, unsigned long base, unsigned int prec, unsigned int width, unsigned int flags) {
    char buf[PRF_NTOA_BUFSIZ];
    size_t len = 0U;
    // no hash for 0 values
    if (!value) {
        flags &= ~FLAGS_HASH;
    }
    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] =
                digit < 10
                    ? '0' + digit
                    : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRF_NTOA_BUFSIZ));
    }
    ntoa(buf, len, negative, (unsigned int)base, prec, width, flags);
}

void Printf::lltoa(unsigned long long value, bool negative, unsigned long long base, unsigned int prec, unsigned int width, unsigned int flags) {
    char buf[PRF_NTOA_BUFSIZ];
    size_t len = 0U;
    // no hash for 0 values
    if (!value) {
        flags &= ~FLAGS_HASH;
    }
    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value) {
        do {
            const char digit = (char)(value % base);
            buf[len++] =
                digit < 10
                    ? '0' + digit
                    : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (value && (len < PRF_NTOA_BUFSIZ));
    }
    ntoa(buf, len, negative, (unsigned int)base, prec, width, flags);
}

int Printf::ftoa(double value, unsigned int prec, unsigned int width, unsigned int flags) {
    size_t nput = 0;
    char buf[PRF_FTOA_BUFSIZ];
    size_t len = 0U;
    double diff = 0.0;
    // if input is larger than thres_max, revert to exponential
    const double thres_max = (double)0x7FFFFFFF;
    // powers of 10
    static const double pow10[] = {1,         10,        100,     1000,
                                   10000,     100000,    1000000, 10000000,
                                   100000000, 1000000000};
    // test for negative
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = 0 - value;
    }
    // set default precision to 6, if not set explicitly
    if (!(flags & FLAGS_PRECISION)) {
        prec = 6U;
    }
    // limit precision to 9, cause a prec >= 10 can lead to overflow errors
    while ((len < PRF_FTOA_BUFSIZ) && (prec > 9U)) {
        buf[len++] = '0';
        prec--;
    }

    int whole = (int)value;
    double tmp = (value - whole) * pow10[prec];
    unsigned long frac = (unsigned long)tmp;
    diff = tmp - frac;

    if (diff > 0.5) {
        ++frac;
        // handle rollover, e.g. case 0.99 with prec 1 is 1.0
        if (frac >= pow10[prec]) {
            frac = 0;
            ++whole;
        }
    } else if ((diff == 0.5) && ((frac == 0U) || (frac & 1U))) {
        // if halfway, round up if odd, OR if last digit is 0
        ++frac;
    }

    // TBD: for very large numbers switch back to native sprintf for
    // exponentials. Anyone want to write code to replace this? Normal
    // printf behavior is to print EVERY whole number digit which can be
    // 100s of characters overflowing your buffers == bad
    if (value > thres_max) {
        return nput;
    }

    if (prec == 0U) {
        diff = value - (double)whole;
        if (diff > 0.5) {
            // greater than 0.5, round up, e.g. 1.6 -> 2
            ++whole;
        } else if ((diff == 0.5) && (whole & 1)) {
            // exactly 0.5 and ODD, then round up
            // 1.5 -> 2, but 2.5 -> 2
            ++whole;
        }
    } else {
        unsigned int count = prec;
        // now do fractional part, as an unsigned number
        while (len < PRF_FTOA_BUFSIZ) {
            --count;
            buf[len++] = (char)(48U + (frac % 10U));
            if (!(frac /= 10U)) {
                break;
            }
        }
        // add extra 0s
        while ((len < PRF_FTOA_BUFSIZ) && (count-- > 0U)) {
            buf[len++] = '0';
        }
        if (len < PRF_FTOA_BUFSIZ) {
            // add decimal
            buf[len++] = '.';
        }
    }
    // do whole part, number is reversed
    while (len < PRF_FTOA_BUFSIZ) {
        buf[len++] = (char)(48 + (whole % 10));
        if (!(whole /= 10)) {
            break;
        }
    }
    // pad leading zeros
    if (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD)) {
        while ((len < width) && (len < PRF_FTOA_BUFSIZ)) {
            buf[len++] = '0';
        }
    }
    // handle sign
    if ((len == width) &&
        (negative || (flags & FLAGS_PLUS) || (flags & FLAGS_SPACE))) {
        len--;
    }
    if (len < PRF_FTOA_BUFSIZ) {
        if (negative) {
            buf[len++] = '-';
        } else if (flags & FLAGS_PLUS) {
            buf[len++] = '+'; // ignore the space if the '+' exists
        } else if (flags & FLAGS_SPACE) {
            buf[len++] = ' ';
        }
    }
    // pad spaces up to given width
    if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
        for (size_t i = len; i < width; i++) {
            printf_write_char(' ');
            nput++;
        }
    }
    // reverse string
    for (size_t i = 0U; i < len; i++) {
        printf_write_char(buf[len - i - 1U]);
        nput++;
    }
    // append pad spaces up to given width
    if (flags & FLAGS_LEFT) {
        while (nput < width) {
            printf_write_char(' ');
            nput++;
        }
    }
    return nput;
}

int Printf::atoi(const char *s, const char **tailptr) {
    char *ep;
    int ret = strtol(s, &ep, 10);
    *tailptr = ep;
    return ret;
}

int Printf::vprintf(const char *fmt, va_list va) {
    if (!fmt || !*fmt) {
        return 0;
    }
    int nput = 0;
    int c;
    unsigned int flags, width, precision;
    while (*fmt) {
        // format specifier?  %[flags][width][.precision][length]
        if (*fmt != '%') {
            printf_write_char(*fmt);
            nput ++;
            fmt++;
            continue;
        }
        fmt++;

        // evaluate flags
        flags = 0U;
        for (;;) {
            switch (*fmt) {
            case '0':
                flags |= FLAGS_ZEROPAD;
                fmt++;
                break;
            case '-':
                flags |= FLAGS_LEFT;
                fmt++;
                break;
            case '+':
                flags |= FLAGS_PLUS;
                fmt++;
                break;
            case ' ':
                flags |= FLAGS_SPACE;
                fmt++;
                break;
            case '#':
                flags |= FLAGS_HASH;
                fmt++;
                break;
            default:
                goto end_flags;
                break;
            }
        };
    end_flags:

        // evaluate width field
        if (isdigit(*fmt)) {
            width = atoi(fmt, &fmt);
        } else if (*fmt == '*') {
            const int w = va_arg(va, int);
            if (w < 0) {
                flags |= FLAGS_LEFT; // reverse padding
                width = (unsigned int)-w;
            } else {
                width = (unsigned int)w;
            }
            fmt++;
        } else {
            width = 0U;
        }

        // evaluate precision field
        precision = 0U;
        if (*fmt == '.') {
            flags |= FLAGS_PRECISION;
            fmt++;
            if (isdigit(*fmt)) {
                precision = atoi(fmt, &fmt);
            } else if (*fmt == '*') {
                const int prec = (int)va_arg(va, int);
                precision = prec > 0 ? (unsigned int)prec : 0U;
                fmt++;
            }
        }

        // evaluate length field
        switch (*fmt) {
        case 'l':
            flags |= FLAGS_LONG;
            fmt++;
            if (*fmt == 'l') {
                flags |= FLAGS_LONG_LONG;
                fmt++;
            }
            break;
        case 'h':
            flags |= FLAGS_SHORT;
            fmt++;
            if (*fmt == 'h') {
                flags |= FLAGS_CHAR;
                fmt++;
            }
            break;
        case 't':
            flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG
                                                        : FLAGS_LONG_LONG);
            fmt++;
            break;
        case 'j':
            flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG
                                                       : FLAGS_LONG_LONG);
            fmt++;
            break;
        case 'z':
            flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG
                                                     : FLAGS_LONG_LONG);
            fmt++;
            break;
        default:
            break;
        }
        // evaluate specifier
        switch ((c = *fmt)) {
        case 'd':
        case 'i':
        case 'u':
        case 'x':
        case 'X':
        case 'o':
        case 'b': {
            // set the base
            unsigned int base;
            if (c == 'x' || c == 'X') {
                base = 16U;
            } else if (c == 'o') {
                base = 8U;
            } else if (c == 'b') {
                base = 2U;
            } else {
                base = 10U;
                flags &= ~FLAGS_HASH; // no hash for dec format
            }
            // uppercase
            if (c == 'X') {
                flags |= FLAGS_UPPERCASE;
            }
            // no plus or space flag for u, x, X, o, b
            if ((c != 'i') && (c != 'd')) {
                flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
            }
            // ignore '0' flag when precision is given
            if (flags & FLAGS_PRECISION) {
                flags &= ~FLAGS_ZEROPAD;
            }
            // convert the integer
            if ((c == 'i') || (c == 'd')) {
                // signed
                if (flags & FLAGS_LONG_LONG) {
                    long long value = va_arg(va, long long);
                    lltoa(ABS(value), value < 0, base, precision, width,
                          flags);
                } else if (flags & FLAGS_LONG) {
                    long value = va_arg(va, long);
                    ltoa(ABS(value), value < 0, base, precision, width,
                         flags);
                } else {
                    int value = va_arg(va, int);
                    ltoa(ABS(value), value < 0, base, precision, width,
                         flags);
                }
            } else {
                // unsigned
                if (flags & FLAGS_LONG_LONG) {
                    lltoa(va_arg(va, unsigned long long), false, base,
                          precision, width, flags);
                } else if (flags & FLAGS_LONG) {
                    ltoa(va_arg(va, unsigned long), false, base, precision,
                         width, flags);
                } else {
                    ltoa(va_arg(va, unsigned int), false, base, precision,
                         width, flags);
                }
            }
            break;
        }
        case 'f':
        case 'F':
            ftoa(va_arg(va, double), precision, width, flags);
            break;
        case 'c': {
            unsigned int l = 1U;
            // pre padding
            if (!(flags & FLAGS_LEFT)) {
                while (l++ < width) {
                    printf_write_char(' ');
                    nput ++;
                }
            }
            // char output
            printf_write_char(char(va_arg(va, int)));
            nput ++;
            // post padding
            if (flags & FLAGS_LEFT) {
                while (l++ < width) {
                    printf_write_char(' ');
                    nput ++;
                }
            }
            break;
        }
        case 's': {
            const char *p = va_arg(va, char *);
            if (!p)
                p = "(null)";
            unsigned int l = strnlen(p, precision ? precision : (size_t)-1);
            // pre padding
            if (flags & FLAGS_PRECISION) {
                l = (l < precision ? l : precision);
            }
            if (!(flags & FLAGS_LEFT)) {
                while (l++ < width) {
                    printf_write_char(' ');
                    nput ++;
                }
            }
            // string output
            while ((*p != 0) &&
                   (!(flags & FLAGS_PRECISION) || precision--)) {
                printf_write_char(*(p++));
                nput ++;
            }
            // post padding
            if (flags & FLAGS_LEFT) {
                while (l++ < width) {
                    printf_write_char(' ');
                    nput ++;
                }
            }
            break;
        }
        case 'p': {
            width = sizeof(void *) * 2U;
            flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
            printf_write_char('0');
            nput ++;
            printf_write_char('x');
            nput ++;
#if __WORDSIZE == 64
            lltoa((long long)va_arg(va, void *), false, 16U, precision,
                  width, flags);
#else
            ltoa((long)va_arg(va, void *), false, 16U, precision, width,
                 flags);
#endif
            break;
        }
        case '%':
            printf_write_char('%');
            nput ++;
            break;
        default:
            printf_write_char(*fmt++);
            nput ++;
            break;
        }
        fmt++;
    }
    return nput;
}

int Printf::printf(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    int n = vprintf(fmt, va);
    va_end(va);
    return n;
}

int Printf::println(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    int n = vprintf(fmt, va);
    printf_write_char('\n');
    n++;
    va_end(va);
    return n;
}


