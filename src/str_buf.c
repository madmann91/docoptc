#include "str_buf.h"

#include <string.h>
#include <stdlib.h>

StrBuf make_str_buf(void) {
    return (StrBuf) { .data = NULL, .size = 0, .cap = 0 };
}

static void grow_buf(StrBuf* buf, size_t len) {
    size_t new_cap = buf->cap * 2;
    new_cap = new_cap < len ? len : new_cap;
    buf->cap = new_cap;
    buf->data = realloc(buf->data, new_cap);
}

void append_str(StrBuf* buf, const char* str, size_t len) {
    if (buf->cap < buf->size + len)
        grow_buf(buf, buf->size + len);
    memcpy(buf->data + buf->size, str, len);
    buf->size += len;
}

void append_char(StrBuf* buf, const char c) {
    if (buf->cap < buf->size + 1)
        grow_buf(buf, buf->size + 1);
    buf->data[buf->size++] = c;
}

void free_str_buf(StrBuf* buf) {
    free(buf->data);
}
