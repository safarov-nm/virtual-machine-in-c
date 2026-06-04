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
            printf("TYPE NONE\n");
            break;
        case TYPE_NOP:
            printf("TYPE NOP\n");
            break;
        case TYPE_PUSH:
            printf("TYPE PUSH\n");
            break;
        case TYPE_POP:
            printf("TYPE POP\n");
            break;
        case TYPE_DUP:
            printf("TYPE DUP\n");
            break;
        case TYPE_INDUP:
            printf("TYPE INDUP\n");
            break;
        case TYPE_SWAP:
            printf("TYPE SWAP\n");
            break;
        case TYPE_INSWAP:
            printf("TYPE INSWAP\n");
            break;
        case TYPE_ADD:
            printf("TYPE ADD\n");
            break;
        case TYPE_SUB:
            printf("TYPE SUB\n");
            break;
        case TYPE_MUL:
            printf("TYPE MUL\n");
            break;
        case TYPE_DIV:
            printf("TYPE DIV\n");
            break;
        case TYPE_MOD:
            printf("TYPE MOD\n");
            break;
        case TYPE_CMPE:
            printf("TYPE CMPE\n");
            break;
        case TYPE_CMPNE:
            printf("TYPE CMPNE\n");
            break;
        case TYPE_CMPG:
            printf("TYPE CMPG\n");
            break;
        case TYPE_CMPL:
            printf("TYPE CMPL\n");
            break;
        case TYPE_CMPGE:
            printf("TYPE CMPGE\n");
            break;
        case TYPE_CMPLE:
            printf("TYPE CMPLE\n");
            break;
        case TYPE_JMP:
            printf("TYPE JMP\n");
            break;
        case TYPE_ZJMP:
            printf("TYPE ZJMP\n");
            break;
        case TYPE_NZJMP:
            printf("TYPE NZJMP\n");
            break;
        case TYPE_PRINT:
            printf("TYPE PRINT\n");
            break;
        case TYPE_HALT:
            printf("TYPE HALT\n");
            break;
        case TYPE_INT:
            printf("TYPE INT\n");
            break;
        }

    }

}


TokenNode* parser(Lexer lex) {
    
    TokenNode *root = NULL;

    generate_list(&root, &lex);

    print_list(root);

    return root;
}