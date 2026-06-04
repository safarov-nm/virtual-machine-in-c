#ifndef __PARSER_H
#define __PARSER_H

#include "vmasmlexer.h"

typedef struct TokenNode {
    Token token;           // данные -- токен
    struct TokenNode *next; // указатель на следующий узел
} TokenNode;

void parser(Lexer lex);

#endif // __PARSER_H