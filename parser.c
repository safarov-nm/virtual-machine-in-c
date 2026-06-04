#include "parser.h"

void append(TokenNode **head, Token token) {
    
    // Создаем новый узел
    TokenNode *new_node = malloc(sizeof(TokenNode));
    if (!new_node) {
        fprintf(stderr, "ERROR: Out of memory\n");
        exit(1);
    }
    
    new_node->token = token;
    new_node->next = NULL;
    
    // Если список пуст, новый узел становится головой
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    // Иначе идем в конец и добавляем
    TokenNode *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

void print_list(TokenNode *root) {

    if (root == NULL) {
        return;
    }

    TokenNode *current = root;

    while (current != NULL) {
        print_token(current->token);
        current = current->next;
    }

}

void generate_list(TokenNode **root, Lexer *lex) {

    for (int i = 0; i < lex->size; ++i) {

        switch(lex->tokens[i].type) {
        case TYPE_NONE:
            assert(false && "Token should not be none\n");
            break;
        case TYPE_NOP:
            append(root, lex->tokens[i]);
            break;
        case TYPE_PUSH:
            append(root, lex->tokens[i]);
            i++;
            if (lex->tokens[i].type != TYPE_INT) {
                fprintf(stderr, "ERROR: Expected type INT!\n");
                exit(1);
            }
            append(root, lex->tokens[i]);
            break;
        case TYPE_POP:
            append(root, lex->tokens[i]);
            break;
        case TYPE_DUP:
            append(root, lex->tokens[i]);
            break;
        case TYPE_INDUP:
            append(root, lex->tokens[i]);
            i++;
            if (lex->tokens[i].type != TYPE_INT) {
                fprintf(stderr, "ERROR: Expected type INT!\n");
                exit(1);
            }
            append(root, lex->tokens[i]);
            break;
        case TYPE_SWAP:
            append(root, lex->tokens[i]);
            break;
        case TYPE_INSWAP:
            append(root, lex->tokens[i]);
            i++;
            if (lex->tokens[i].type != TYPE_INT) {
                fprintf(stderr, "ERROR: Expected type INT!\n");
                exit(1);
            }
            append(root, lex->tokens[i]);
            break;
        case TYPE_ADD:
            append(root, lex->tokens[i]);
            break;
        case TYPE_SUB:
            append(root, lex->tokens[i]);
            break;
        case TYPE_MUL:
            append(root, lex->tokens[i]);
            break;
        case TYPE_DIV:
            append(root, lex->tokens[i]);
            break;
        case TYPE_MOD:
            append(root, lex->tokens[i]);
            break;
        case TYPE_CMPE:
            append(root, lex->tokens[i]);
            break;
        case TYPE_CMPNE:
            append(root, lex->tokens[i]);
            break;
        case TYPE_CMPG:
            append(root, lex->tokens[i]);
            break;
        case TYPE_CMPL:
            append(root, lex->tokens[i]);
            break;
        case TYPE_CMPGE:
            append(root, lex->tokens[i]);
            break;
        case TYPE_CMPLE:
            append(root, lex->tokens[i]);
            break;
        case TYPE_JMP:
            append(root, lex->tokens[i]);
            break;
        case TYPE_ZJMP:
            append(root, lex->tokens[i]);
            break;
        case TYPE_NZJMP:
            append(root, lex->tokens[i]);
            break;
        case TYPE_PRINT:
            append(root, lex->tokens[i]);
            break;
        case TYPE_HALT:
            append(root, lex->tokens[i]);
            break;
        case TYPE_INT:
            append(root, lex->tokens[i]);
            break;
        }

    }

}


TokenNode* parser(Lexer lex) {
    
    TokenNode *root = NULL;

    generate_list(&root, &lex);

    return root;
}