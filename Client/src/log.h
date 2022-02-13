#ifndef __LOG_H__
#define __LOG_H__

#define PRINT_LEVEL_NONE 0
#define PRINT_LEVEL_INFO 1
#define PRINT_LEVEL_ERR 2
#define PRINT_LEVEL_DEBUG 3
#define PRINT_LEVEL PRINT_LEVEL_DEBUG

void print_info(const char* fmt, ...);
void print_err(const char* fmt, ...);
void print_debug(const char* fmt, ...);

#endif
