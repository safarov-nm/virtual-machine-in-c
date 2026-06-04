#include <stdio.h>

#include "vmasm.h"
#include "parser.h"

int main() {

    Lexer lex = lexer();

    parser(lex);


    return 0;
}