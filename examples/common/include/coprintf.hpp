#ifndef COPRINTF_HPP
#define COPRINTF_HPP

#ifdef __gnu_linux__

#include <stdarg.h>
#include <memory>


class CoPrintfBaseMessage
{
public:
    virtual ~CoPrintfBaseMessage() = default;
    virtual void write(const void* data, size_t size) = 0;
    virtual void flush() {}
};

std::unique_ptr<CoPrintfBaseMessage> coprintf_make_message();

void coprintf(const char* fmt, ...);
void covprintf(const char* fmt, va_list args);

#else

void coprintf(const char* fmt, ...);
void covprintf(const char* fmt, ...);

#endif



#endif // COPRINTF_HPP
