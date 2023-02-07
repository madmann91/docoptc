#include "utils.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>

char* read_file(const char* file_name) {
    static const size_t chunk_size = 1024;

    FILE* file = fopen(file_name, "r");
    size_t pos = 0, cap = chunk_size;
    char* buf = malloc(cap);

    while (true) {
        size_t count = fread(buf + pos, 1, cap - pos, file);
        pos += count;
        if (count < cap - pos)
            break;
        cap *= 2;
        buf = realloc(buf, cap);
    }
    fclose(file);

    buf = realloc(buf, pos + 1);
    buf[pos] = 0;
    return buf;
}

bool compare_lower_case(const char* str, const char* ref, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (tolower(str[i]) != ref[i])
            return false;
    }
    return true;
}

bool is_upper_case(const char* str) {
    while (*str) {
        if (toupper(*str) != *str)
            return false;
        str++;
    }
    return true;
}

bool is_upper_case_n(const char* str, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (toupper(str[i]) != str[i])
            return false;
    }
    return true;
}

void error_at(const SourceRange* range, const char* format_str, ...) {
    va_list args;
    va_start(args, format_str);
    fprintf(stderr, "error in %s(%"PRIu32":%"PRIu32" - %"PRIu32":%"PRIu32"): ",
        range->file_name, range->begin.row, range->begin.col, range->end.row, range->end.col);
    vfprintf(stderr, format_str, args);
    va_end(args);
    fprintf(stderr, "\n");
}
