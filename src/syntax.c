#include "syntax.h"
#include "utils.h"

#include <assert.h>
#include <string.h>

static void print_arg(FILE* file, const char* name) {
    if (is_upper_case(name))
        fprintf(file, "%s", name);
    else
        fprintf(file, "<%s>", name);
}

static void print_many(FILE* file, const char* sep, const Syntax* syntax) {
    for (const Syntax* elem = syntax; elem; elem = elem->next) {
        print_syntax(file, elem);
        if (elem->next)
            fprintf(file, "%s", sep);
    }
}

void print_syntax(FILE* file, const Syntax* syntax) {
    switch (syntax->tag) {
        case SYNTAX_ROOT:
            fprintf(file, "%s\n\nUsage:\n", syntax->root.info);
            print_many(file, "\n", syntax->root.usages);
            fprintf(file, "\n\nOptions:\n");
            print_many(file, "\n", syntax->root.descs);
            fprintf(file, "\n");
            break;
        case SYNTAX_ERROR:
            fprintf(file, "#error#");
            break;
        case SYNTAX_USAGE:
            fprintf(file, "  %s ", syntax->usage.prog);
            print_many(file, " ", syntax->usage.elems);
            break;
        case SYNTAX_DESC:
            fprintf(file, "  ");
            print_many(file, " ", syntax->desc.elems);
            fprintf(file, "  %s", syntax->desc.info);
            if (syntax->desc.default_val)
                fprintf(file, " # defaults to '%s'", syntax->desc.default_val);
            break;
        case SYNTAX_COMMAND:
            fprintf(file, "%s", syntax->command.name);
            break;
        case SYNTAX_OPTION:
            fprintf(file, "%s%s", syntax->option.is_short ? "-" : "--", syntax->option.name);
            if (syntax->option.arg) {
                fprintf(file, "%c", syntax->option.is_short ? ' ' : '=');
                print_arg(file, syntax->option.arg);
            }
            break;
        case SYNTAX_ARG:
            print_arg(file, syntax->arg.name);
            break;
        case SYNTAX_BRACKETS:
            fprintf(file, "[");
            print_many(file, " ", syntax->brackets.elems);
            fprintf(file, "]");
            break;
        case SYNTAX_PARENS:
            fprintf(file, "(");
            print_many(file, " ", syntax->parens.elems);
            fprintf(file, ")");
            break;
        case SYNTAX_REPEAT:
            print_syntax(file, syntax->repeat.elem);
            fprintf(file, "...");
            break;
        case SYNTAX_STDIN:
            fprintf(file, "-");
            break;
        case SYNTAX_SEP:
            fprintf(file, "--");
            break;
        case SYNTAX_OR:
            print_many(file, " | ", syntax->or_.elems);
            break;
        default:
            break;
    }
}

static void check_usages(Syntax* usages) {
    for (Syntax* usage = usages; usage; usage = usage->next) {
        if (strcmp(usage->usage.prog, usages->usage.prog)) {
            error_at(&usage->range, "expected program name '%s', but got '%s'",
                usages->usage.prog, usage->usage.prog);
        }
    }
}

static void check_descs(const Syntax* descs) {
    for (const Syntax* desc = descs; desc; desc = desc->next) {
        const Syntax* first_opt = desc->desc.elems;
        const Syntax* other_opt = first_opt->next;
        bool has_arg = first_opt->option.arg;

        if (other_opt && has_arg != !!other_opt->option.arg) {
            error_at(&other_opt->range, "option '%s' requires an argument, but option '%s' does not",
                (has_arg ? first_opt : other_opt)->option.name,
                (has_arg ? other_opt : first_opt)->option.name);
        }

        if (!has_arg && desc->desc.default_val) {
            error_at(&desc->range, "option '%s' has no arguments and cannot have a default value",
                first_opt->option.name);
        }
    }
}

void check_syntax(const Syntax* root) {
    assert(root->tag == SYNTAX_ROOT);
    check_usages(root->root.usages);
    check_descs(root->root.descs);
}
