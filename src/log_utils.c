#include "log_utils.h"
#include <stdio.h>
#include <stdarg.h>

void log_error(char* format, ...) {
    fprintf(stderr, "[ERROR] ");
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

void log_info(char* format, ...) {
    fprintf(stderr, "[INFO] ");
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}
