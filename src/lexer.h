#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct Lexer {
    const char* file_name;
    const char* file_data;
    SourcePos pos;
} Lexer;

Lexer make_lexer(const char* file_name, const char* file_data);
size_t eat_spaces(Lexer* lexer);
void skip_line(Lexer* lexer);
Token lex(Lexer* lexer);

#endif
