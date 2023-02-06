#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct Syntax  Syntax;
typedef struct MemPool MemPool;
typedef struct Lexer   Lexer;

typedef struct Parser {
    Lexer* lexer;
    MemPool* mem_pool;
    SourcePos prev_end;
    Token ahead;
} Parser;

Parser make_parser(MemPool*, Lexer*);
Syntax* parse(Parser*);

#endif
