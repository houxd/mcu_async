#include "log.h"

Log::Log(UartBuf *output, const Str &tag) : _output(output), _tag(tag) {}

void Log::printf_write_char(char c) {
    if (_reset) {
        _reset = false;
        _output->putc('[');
        _output->puts(_tag.c_str());
        _output->putc(']');
        _output->putc(' ');
    }
    if (c == '\n') {
        _reset = true;
    }
    _output->putc(c);
}
