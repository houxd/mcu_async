#include <retarget.h>
#include <types.h>

static UartBuf *_uart_buf = nullptr;
void retarget_stdio(UartBuf *uart_buf) {
    _uart_buf = uart_buf;
}


#ifdef _STM32
extern "C" int _write(int file, char *ptr, int len) {
	if(_uart_buf==nullptr)
		return -1;
	for(int i=0; i<len; i++)
		_uart_buf->putc(ptr[i]);
	_uart_buf->flush();
	return len;
}
extern "C" int _read(int file, char *ptr, int len) {
	for(int i=0; i<len; i++) {
		int c = _uart_buf->getc();
		if(c==-1) {
			return i;
		}
		ptr[i] = (u8)c;
	}
	return len;
}
#endif // STM32


#ifdef _QT
#include <stdarg.h>
#include <QString>
extern "C" int __wrap_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = _uart_buf->vprintf(format, args);
    va_end(args);
    return res;
}
// wrap_scanf not work for gcc ???
#endif // QT

