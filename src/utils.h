#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct SourceRange SourceRange;

typedef struct StrBuf {
    char* data;
    size_t cap, size;
} StrBuf;

char* read_file(const char* file_name);
bool compare_lower_case(const char*, const char*, size_t n);
bool is_upper_case(const char*);
bool is_upper_case_n(const char*, size_t);
void error_at(const SourceRange* pos, const char* format_str, ...);

StrBuf make_str_buf(void);
void free_str_buf(StrBuf*);
void append_str(StrBuf*, const char*, size_t);
void append_char(StrBuf*, const char);

#endif
