#include <stdio.h>

#include "vmasm.h"
#include "parser.h"
#include "vm.h"

#define MAX_PROGRAM_SIZE 1024

Inst *generate_instruction(TokenNode *root) {

    Inst *program = malloc(sizeof(Inst) * MAX_PROGRAM_SIZE);
    int program_size = 0;

    TokenNode *current = root;

    while (current != NULL) {
        switch(current->token.type) {
        case TYPE_NONE:
            assert(false && "Token should not be none\n");
            break;
        case TYPE_NOP:
            program[program_size++] = DEF_INST_NOP();
            break;
        case TYPE_PUSH:
            current = current->next;
            program[program_size++] = DEF_INST_PUSH(atoi(current->token.text));
            break;
        case TYPE_POP:
            program[program_size++] = DEF_INST_POP();
            break;
        case TYPE_DUP:
            program[program_size++] = DEF_INST_DUP();
            break;
        case TYPE_INDUP:
            current = current->next;
            program[program_size++] = DEF_INST_INDUP(atoi(current->token.text));
            break;
        case TYPE_SWAP:
            program[program_size++] = DEF_INST_SWAP();
            break;
        case TYPE_INSWAP:
            current = current->next;
            program[program_size++] = DEF_INST_INSWAP(atoi(current->token.text));
            break;
        case TYPE_ADD:
            program[program_size++] = DEF_INST_ADD();
            break;
        case TYPE_SUB:
            program[program_size++] = DEF_INST_SUB();
            break;
        case TYPE_MUL:
            program[program_size++] = DEF_INST_MUL();
            break;
        case TYPE_DIV:
            program[program_size++] = DEF_INST_DIV();
            break;
        case TYPE_MOD:
            program[program_size++] = DEF_INST_MOD();
            break;
        case TYPE_CMPE:
            program[program_size++] = DEF_INST_CMPE();
            break;
        case TYPE_CMPNE:
            program[program_size++] = DEF_INST_CMPNE();
            break;
        case TYPE_CMPG:
            program[program_size++] = DEF_INST_CMPG();
            break;
        case TYPE_CMPL:
            program[program_size++] = DEF_INST_CMPL();
            break;
        case TYPE_CMPGE:
            program[program_size++] = DEF_INST_CMPGE();
            break;
        case TYPE_CMPLE:
            program[program_size++] = DEF_INST_CMPLE();
            break;
        case TYPE_JMP:
            current = current->next;
            program[program_size++] = DEF_INST_JMP(atoi(current->token.text));
            break;
        case TYPE_ZJMP:
            current = current->next;
            program[program_size++] = DEF_INST_ZJMP(atoi(current->token.text));
            break;
        case TYPE_NZJMP:
            current = current->next;
            program[program_size++] = DEF_INST_NZJMP(atoi(current->token.text));
            break;
        case TYPE_PRINT:
            program[program_size++] = DEF_INST_PRINT();
            break;
        case TYPE_HALT:
            program[program_size++] = DEF_INST_HALT();
            break;
        case TYPE_INT:
            // INT должен был быть обработан инструкциями выше
            assert(false && "Should not be INT\n");
            break;
        }

        current = current->next;
    }

    return program;
}

int main() {

    Lexer lex = lexer();

    TokenNode *root = parser(lex);
    // print_list(root); 

    Inst *program = generate_instruction(root);

    Machine *loaded_machine = malloc(sizeof(Machine));
    if (loaded_machine == NULL) {
        fprintf(stderr, "ERROR: Couldn't allocate memory for the machine\n");
        exit(1);
    }

    loaded_machine->instructions = program;
    loaded_machine->program_size = MAX_PROGRAM_SIZE / sizeof(program[0]);

    write_program_to_file(loaded_machine, "test.vm");

    loaded_machine = read_program_from_file(loaded_machine, "test.vm");

    run_instructions(loaded_machine);

    print_stack(loaded_machine);

    free(loaded_machine->instructions);
    free(loaded_machine);

    return 0;
}