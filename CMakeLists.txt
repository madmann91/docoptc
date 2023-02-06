cmake_minimum_required(VERSION 3.9)
project(doctoptc)

add_executable(docoptc
    src/syntax.c
    src/token.c
    src/utils.c
    src/lexer.c
    src/parser.c
    src/mem_pool.c
    src/main.c)

target_compile_options(docoptc PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -pedantic>)
