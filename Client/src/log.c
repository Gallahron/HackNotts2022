#include <stdio.h>
#include <stdarg.h>

#include "log.h"

#define FILE_INFO stdout
#define FILE_ERR stderr
#define FILE_DEBUG stderr

void print_info(const char* fmt, ...);
void print_err(const char* fmt, ...);
void print_debug(const char* fmt, ...);

void print_info(const char* fmt, ...)
{
#if PRINT_LEVEL >= PRINT_LEVEL_INFO
	fputs("Info: ", FILE_INFO);
	va_list va_ptr;
	va_start(va_ptr, fmt);
	vfprintf(FILE_INFO, fmt, va_ptr);
	va_end(va_ptr);
	putc('\n', FILE_INFO);
#endif
}

void print_err(const char* fmt, ...)
{
#if PRINT_LEVEL >= PRINT_LEVEL_ERR
	fputs("Error: ", FILE_ERR);
	va_list va_ptr;
	va_start(va_ptr, fmt);
	vfprintf(FILE_ERR, fmt, va_ptr);
	va_end(va_ptr);
	putc('\n', FILE_ERR);
#endif
}

void print_debug(const char* fmt, ...)
{
#if PRINT_LEVEL >= PRINT_LEVEL_DEBUG
	fputs("Debug: ", FILE_DEBUG);
	va_list va_ptr;
	va_start(va_ptr, fmt);
	vfprintf(FILE_DEBUG, fmt, va_ptr);
	va_end(va_ptr);
	putc('\n', FILE_DEBUG);
#endif
}
