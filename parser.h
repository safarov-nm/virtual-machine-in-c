#ifndef __PARSER_H
#define __PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "vmasmlexer.h"
#include "vmasmlexer.h"

typedef struct TokenNode {
    Token token;           // данные -- токен
    struct TokenNode *next; // указатель на следующий узел
} TokenNode;

TokenNode* parser(Lexer lex);
void append(TokenNode **root, Token token);
void print_list(TokenNode *root);

#endif // __PARSER_H