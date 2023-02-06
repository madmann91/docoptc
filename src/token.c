#include "token.h"

#include <assert.h>
#include <stdbool.h>

const char* get_token_tag_name(TokenTag tag) {
    switch (tag) {
#define token(tag, str) case TOKEN_##tag: return str;
        TOKEN_LIST(token)
#undef token
        default:
            assert(false && "invalid token");
            return "";
    }
}

size_t get_source_range_len(const SourceRange* range) {
    return range->end.bytes - range->begin.bytes;
}

const char* get_source_range_str(const SourceRange* range, const char* file_data) {
    return file_data + range->begin.bytes;
}
