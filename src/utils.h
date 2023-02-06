#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct SourceRange SourceRange;

char* read_file(const char* file_name);
bool compare_lower_case(const char*, const char*, size_t n);
bool is_upper_case(const char*);
bool is_upper_case_n(const char*, size_t);
void error_at(const SourceRange* pos, const char* format_str, ...);

#endif
