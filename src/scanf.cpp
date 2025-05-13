#include "scanf.h"
#include <cctype>

int Scanf::vscanf(const char *fmt, va_list args) {
    int matches = 0;
    while (*fmt) {
        if (*fmt != '%') {
            // 略过普通字符，不进行匹配
            if (*fmt == (char)scanf_read_char()) {
                fmt++;
            } else {
                // 未匹配则直接返回
                return matches;
            }
        } else {
            fmt++; // 跳过 '%'
            // 简化处理：只示例 %d, %f, %s

            switch (*fmt) {
            case 'd': {
                // 读取整数
                int val = 0;
                bool neg = false;
                int c = scanf_read_char();
                // 判断正负号
                if (c == '-') {
                    neg = true;
                    c = scanf_read_char();
                }
                // 读取数字
                if (!isdigit(c)) {
                    return matches;
                }
                val = (c - '0');
                while (true) {
                    c = scanf_read_char();
                    if (!isdigit(c)) {
                        // 回退一个字符或自行处理
                        break;
                    }
                    val = val * 10 + (c - '0');
                }
                *(va_arg(args, int*)) = neg ? -val : val;
                matches++;
                break;
            }
            case 'f': {
                // 读取浮点数（演示用，较为简化）
                double val = 0.0;
                bool neg = false;
                bool decimalFound = false;
                double fracFactor = 0.1;
                int c = scanf_read_char();
                if (c == '-') {
                    neg = true;
                    c = scanf_read_char();
                }
                while (isdigit(c) || c == '.') {
                    if (c == '.') {
                        decimalFound = true;
                    } else {
                        if (!decimalFound) {
                            val = val * 10 + (c - '0');
                        } else {
                            val = val + (c - '0') * fracFactor;
                            fracFactor /= 10.0;
                        }
                    }
                    c = scanf_read_char();
                }
                *(va_arg(args, double*)) = neg ? -val : val;
                matches++;
                break;
            }
            case 's': {
                // 读取字符串
                char *str = va_arg(args, char*);
                int i = 0;
                while (true) {
                    int c = scanf_read_char();
                    if (c == '\0' || isspace(c)) {
                        break;
                    }
                    str[i++] = (char)c;
                }
                str[i] = '\0';
                matches++;
                break;
            }
            default:
                // 其他占位符暂不处理
                break;
            }
            fmt++;
        }
    }
    return matches;
}
