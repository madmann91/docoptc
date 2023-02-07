#ifndef SYNTAX_H
#define SYNTAX_H

#include "token.h"

#include <stdio.h>
#include <stdbool.h>

typedef struct Syntax Syntax;

typedef enum {
    SYNTAX_ERROR,
    SYNTAX_ROOT,
    SYNTAX_USAGE,
    SYNTAX_DESC,
    SYNTAX_COMMAND,
    SYNTAX_OPTION,
    SYNTAX_ARG,
    SYNTAX_BRACKETS,
    SYNTAX_PARENS,
    SYNTAX_REPEAT,
    SYNTAX_STDIN,
    SYNTAX_SEP,
    SYNTAX_OR
} SyntaxTag;

struct Syntax {
    SyntaxTag tag;
    SourceRange range;
    Syntax* next;
    union {
        struct {
            const char* info;
            Syntax* usages;
            Syntax* descs;
        } root;
        struct {
            const char* prog;
            Syntax* elems;
        } usage;
        struct {
            Syntax* elems;
            const char* info;
            const char* default_val;
        } desc;
        struct {
            const char* name;
        } command;
        struct {
            bool is_short;
            const char* name;
            const char* arg;
        } option;
        struct {
            const char* name;
        } arg;
        struct {
            Syntax* elems;
        } or_;
        struct {
            Syntax* elems;
        } brackets, parens;
        struct {
            Syntax* elem;
        } repeat;
    };
};

void print_syntax(FILE*, const Syntax*);
void check_syntax(const Syntax*);

#endif
