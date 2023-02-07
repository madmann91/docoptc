#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "syntax.h"
#include "mem_pool.h"

#include <stdlib.h>

static bool compile_file(const char* file_name) {
    char* file_data = read_file(file_name);
    if (!file_data)
        return false;
    MemPool mem_pool = new_mem_pool();
    Lexer lexer = make_lexer(file_name, file_data);
    Parser parser = make_parser(&mem_pool, &lexer);
    Syntax* syntax = parse(&parser);
    check_syntax(syntax);
    print_syntax(stdout, syntax);
    free(file_data);
    free_mem_pool(&mem_pool);
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    compile_file(argv[1]);
    return 0;
}
