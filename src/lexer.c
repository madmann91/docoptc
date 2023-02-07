#include "lexer.h"
#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

Lexer make_lexer(const char* file_name, const char* file_data) {
    return (Lexer) {
        .file_name = file_name,
        .file_data = file_data,
        .pos = {
            .row = 1,
            .col = 1,
            .bytes = 0
        }
    };
}

static inline char peek_char(const Lexer* lexer) {
    return lexer->file_data[lexer->pos.bytes];
}

static inline bool eof_reached(const Lexer* lexer) {
    return peek_char(lexer) == 0;
}

static inline void skip_char(Lexer* lexer) {
    lexer->pos.col++;
    if (peek_char(lexer) == '\n') {
        lexer->pos.row++;
        lexer->pos.col = 1;
    }
    lexer->pos.bytes++;
}

static inline bool accept_char(Lexer* lexer, char c) {
    if (peek_char(lexer) == c) {
        skip_char(lexer);
        return true;
    }
    return false;
}

static inline bool accept_str(Lexer* lexer, const char* str) {
    SourcePos begin = lexer->pos;
    while (true) {
        if (*str == 0)
            return true;
        if (!accept_char(lexer, *(str++)))
            break;
    }
    lexer->pos = begin;
    return false;
}

static inline bool accept_ident(Lexer* lexer) {
    if (isalpha(peek_char(lexer)) || peek_char(lexer) == '_') {
        do {
            skip_char(lexer);
        } while (isalnum(peek_char(lexer)) || peek_char(lexer) == '_');
        return true;
    }
    return false;
}

static bool accept_arg(Lexer* lexer, char sep, char other_sep) {
    SourcePos begin = lexer->pos;
    if (!accept_char(lexer, sep) && other_sep != 0 && !accept_char(lexer, other_sep))
        return false;

    SourcePos after_sep = lexer->pos;
    const char* str = lexer->file_data + after_sep.bytes;
    if (accept_ident(lexer) && is_upper_case_n(str, lexer->pos.bytes - begin.bytes))
        return true;
    lexer->pos = after_sep;
    if (accept_char(lexer, '<') && accept_ident(lexer) && accept_char(lexer, '>'))
        return true;
    lexer->pos = begin;
    return false;
}

size_t eat_spaces(Lexer* lexer) {
    size_t count = 0;
    while (peek_char(lexer) == ' ' || peek_char(lexer) == '\t')
        skip_char(lexer), count++;
    return count;
}

void skip_line(Lexer* lexer) {
    while (!eof_reached(lexer) && peek_char(lexer) != '\n')
        skip_char(lexer);
}

static inline Token make_token(Lexer* lexer, const SourcePos* begin, bool is_separated, TokenTag tag) {
    return (Token) {
        .tag = tag,
        .is_separated = is_separated,
        .range = {
            .file_name = lexer->file_name,
            .begin = *begin,
            .end = lexer->pos
        }
    };
}

Token lex(Lexer* lexer) {
    bool is_separated = eat_spaces(lexer) >= 2;

    SourcePos begin = lexer->pos;
    if (eof_reached(lexer))
        return make_token(lexer, &begin, is_separated, TOKEN_END);

    if (accept_char(lexer, '\n')) return make_token(lexer, &begin, is_separated, TOKEN_NL);
    if (accept_char(lexer, '['))  return make_token(lexer, &begin, is_separated, TOKEN_LBRACKET);
    if (accept_char(lexer, ']'))  return make_token(lexer, &begin, is_separated, TOKEN_RBRACKET);
    if (accept_char(lexer, '('))  return make_token(lexer, &begin, is_separated, TOKEN_LPAREN);
    if (accept_char(lexer, ')'))  return make_token(lexer, &begin, is_separated, TOKEN_RPAREN);
    if (accept_char(lexer, '|'))  return make_token(lexer, &begin, is_separated, TOKEN_OR);
    if (accept_str(lexer, "...")) return make_token(lexer, &begin, is_separated, TOKEN_DOTS);
    if (accept_char(lexer, ':'))  return make_token(lexer, &begin, is_separated, TOKEN_COLON);
    if (accept_char(lexer, '='))  return make_token(lexer, &begin, is_separated, TOKEN_COLON);
    if (accept_char(lexer, ','))  return make_token(lexer, &begin, is_separated, TOKEN_COMMA);

    if (accept_ident(lexer)) {
        size_t len = lexer->pos.bytes - begin.bytes;
        const char* str = lexer->file_data + begin.bytes;
        bool is_usage = len == 5 && compare_lower_case(str, "usage", 5) && accept_char(lexer, ':');
        return make_token(lexer, &begin, is_separated,
            is_usage ? TOKEN_USAGE :
            is_upper_case_n(str, len) ? TOKEN_UPPERARG : TOKEN_IDENT);
    }

    if (accept_char(lexer, '<')) {
        bool ok = true;
        ok &= accept_ident(lexer);
        ok &= accept_char(lexer, '>');
        return make_token(lexer, &begin, is_separated, ok ? TOKEN_DELIMARG : TOKEN_UNKNOWN);
    }

    if (accept_char(lexer, '-')) {
        if (accept_ident(lexer)) {
            accept_arg(lexer, ' ', 0);
            return make_token(lexer, &begin, is_separated, TOKEN_SOPT);
        }
        if (accept_char(lexer, '-')) {
            if (accept_ident(lexer)) {
                accept_arg(lexer, '=', ' ');
                return make_token(lexer, &begin, is_separated, TOKEN_LOPT);
            }
            return make_token(lexer, &begin, is_separated, TOKEN_DDASH);
        }
        return make_token(lexer, &begin, is_separated, TOKEN_DASH);
    }

    skip_char(lexer);
    return make_token(lexer, &begin, is_separated, TOKEN_UNKNOWN);
}
