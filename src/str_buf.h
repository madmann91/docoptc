#ifndef STR_BUF_H
#define STR_BUF_H

#include <stddef.h>

typedef struct StrBuf {
    char* data;
    size_t cap, size;
} StrBuf;

StrBuf make_str_buf(void);
void free_str_buf(StrBuf*);
void append_str(StrBuf*, const char*, size_t);
void append_char(StrBuf*, const char);

#endif
