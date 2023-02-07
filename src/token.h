#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define TOKEN_LIST(f) \
    f(UNKNOWN, "invalid token") \
    f(END, "end-of-file") \
    f(NL, "new line") \
    f(COMMA, "','") \
    f(OR, "'|'") \
    f(DOTS, "'...'") \
    f(COLON, "':'") \
    f(IDENT, "identifier") \
    f(DASH, "'-'") \
    f(DDASH, "'--'") \
    f(SOPT, "short option") \
    f(LOPT, "long option") \
    f(DELIMARG, "delimited argument") \
    f(UPPERARG, "uppercase argument") \
    f(USAGE, "usage section start") \
    f(LANGLE, "'<'") \
    f(RANGLE, "'>'") \
    f(LBRACKET, "'['") \
    f(RBRACKET, "']'") \
    f(LPAREN, "'('") \
    f(RPAREN, "')'")

typedef struct SourcePos {
    uint32_t row, col;
    size_t bytes;
} SourcePos;

typedef struct SourceRange {
    const char* file_name;
    SourcePos begin, end;
} SourceRange;

typedef enum {
#define token(tag, str) TOKEN_##tag,
    TOKEN_LIST(token)
#undef token
} TokenTag;

typedef struct {
    TokenTag tag;
    SourceRange range;
    bool is_separated;
} Token;

const char* get_token_tag_name(TokenTag);
size_t get_source_range_len(const SourceRange*);
const char* get_source_range_str(const SourceRange*, const char* file_data);

#endif
