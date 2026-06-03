#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define MAX_STACK_SIZE 1024

typedef enum {
    INST_NOP = 0,
    INST_PUSH,
    INST_POP,
    INST_DUP,
    INST_SWAP,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_MOD,
    INST_CMPE,
    INST_CMPNE,
    INST_CMPG,
    INST_CMPGE,
    INST_CMPL,
    INST_CMPLE,
    INST_JMP,
    INST_ZJMP,
    INST_NZJMP,

    INST_PRINT,
    INST_HALT
} Inst_Set;

typedef struct {
    Inst_Set type;
    int value;
} Inst;

typedef struct {
    int stack[MAX_STACK_SIZE];
    int stack_size;
    size_t program_size;
    Inst *instructions;
} Machine;

#define DEF_INST_NOP(x) {.type = INST_NOP}
#define DEF_INST_PUSH(x) {.type = INST_PUSH, .value = x}
#define DEF_INST_POP() {.type = INST_POP}
#define DEF_INST_DUP() {.type = INST_DUP}
#define DEF_INST_SWAP() {.type = INST_SWAP}
#define DEF_INST_ADD() {.type = INST_ADD}
#define DEF_INST_SUB() {.type = INST_SUB}
#define DEF_INST_MUL() {.type = INST_MUL}
#define DEF_INST_DIV() {.type = INST_DIV}
#define DEF_INST_MOD() {.type = INST_MOD}
#define DEF_INST_CMPE() {.type = INST_CMPE}
#define DEF_INST_CMPNE() {.type = INST_CMPNE}
#define DEF_INST_CMPG() {.type = INST_CMPG}
#define DEF_INST_CMPGE() {.type = INST_CMPGE}
#define DEF_INST_CMPL() {.type = INST_CMPL}
#define DEF_INST_CMPLE() {.type = INST_CMPLE}
#define DEF_INST_JMP(x) {.type = INST_JMP, .value = x}
#define DEF_INST_ZJMP(x) {.type = INST_ZJMP, .value = x}
#define DEF_INST_NZJMP(x) {.type = INST_NZJMP, .value = x}
#define DEF_INST_PRINT() {.type = INST_PRINT}
#define DEF_INST_HALT(x) {.type = INST_HALT}

Inst program[] = {


	DEF_INST_PUSH(5),
	DEF_INST_PUSH(10),
	DEF_INST_JMP(0)

};

#define PROGRAM_SIZE (sizeof(program)/sizeof(program[0]))

void push(Machine *machine, int value) {
    if (machine->stack_size >= MAX_STACK_SIZE) {
        fprintf(stderr, "ERROR: Stack overflow\n");
        exit(1);
    }
    machine->stack[machine->stack_size] = value;
    machine->stack_size++;
}

int pop(Machine *machine) {
    if (machine->stack_size <= 0) {
        fprintf(stderr, "ERROR: Stack underflow\n");
        exit(1);
    }
    machine->stack_size--;
    return machine->stack[machine->stack_size];
}

void print_stack(Machine *machine) {

    // if (machine->stack_size == 0) return;

    putchar('\n');
    printf("------ START OF STACK\n");
    // printf("Stack (size = %d): ", machine->stack_size);
    for (int i = machine->stack_size - 1; i >= 0; i--) {
        printf("%d\n", machine->stack[i]);
    }
    printf("------ END OF STACK\n");
    putchar('\n');
}

void write_program_to_file(Machine *machine, char *file_path) {
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not write to file %s\n", file_path);
        exit(1);
    }

    fwrite(machine->instructions, sizeof(Inst), machine->program_size, file);
    fclose(file);
}

Machine* read_program_from_file(char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not read from file %s\n", file_path);
        exit(1);
    }

    // Создаем новую машину
    Machine *machine = malloc(sizeof(Machine));
    if (!machine) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        exit(1);
    }
    
    // Инициализируем стек
    machine->stack_size = 0;
    
    // Получаем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Расчет размера программы
    machine->program_size = file_size / sizeof(Inst);
    
    // Выделяем память под инструкции
    machine->instructions = malloc(file_size);
    if (!machine->instructions) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        free(machine);
        exit(1);
    }
    
    size_t read_count = fread(machine->instructions, sizeof(Inst), 
                             machine->program_size, file);
    if (read_count != machine->program_size) {
        fprintf(stderr, "ERROR: Failed to read program\n");
        free(machine->instructions);
        free(machine);
        exit(1);
    }
    
    fclose(file);
    return machine;
}

int main() {
    int a, b;

    _Bool flag; // Убираем лишние выводы стека, когда совершается прыжок

    // Создаем машину для записи
    Machine *machine_to_write = malloc(sizeof(Machine));
    machine_to_write->stack_size = 0;
    machine_to_write->program_size = PROGRAM_SIZE;
    machine_to_write->instructions = program;
    
    // Записываем программу в файл
    write_program_to_file(machine_to_write, "test.vm");
    
    // Загружаем программу из файла в новую машину
    Machine *loaded_machine = read_program_from_file("test.vm");
    
    printf("Loaded program with %zu instructions\n", loaded_machine->program_size);
    
    // Исполняем программу
    for (size_t i = 0; i < loaded_machine->program_size; ++i) {
        flag = 0;
        // print_stack(loaded_machine);
        switch (loaded_machine->instructions[i].type) {
            case INST_NOP:
                continue;
                break;
            case INST_PUSH:
                push(loaded_machine, loaded_machine->instructions[i].value);
                // printf("PUSH %d\n", loaded_machine->instructions[i].value);
                break;
            case INST_POP:
                // printf("POP: %d\n", pop(loaded_machine));
                pop(loaded_machine);
                break;
            case INST_DUP:
                a = pop(loaded_machine);
                push(loaded_machine, a);
                push(loaded_machine, a);
                break;
            case INST_SWAP:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a);
                push(loaded_machine, b);
                break;
            case INST_ADD:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a + b);
                // printf("ADD: %d + %d = %d\n", a, b, a + b);
                break;
            case INST_SUB:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a - b);
                // printf("SUB: %d - %d = %d\n", a, b, a - b);
                break;
            case INST_MUL:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a * b);
                //printf("MUL: %d * %d = %d\n", a, b, a * b);
                break;
            case INST_DIV:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                if (b == 0) {
                    fprintf(stderr, "ERROR: Division by zero\n");
                    exit(1);
                }
                push(loaded_machine, a / b);
                // printf("DIV: %d / %d = %d\n", a, b, a / b);
                break;
            case INST_MOD:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a % b);
                // printf("MOD: %d % %d = %d\n", a, b, a % b);
                break;
            case INST_CMPE:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a == b);
                // printf("EQUAL: %d == %d = %d\n", a, b, a == b);
                break;
            case INST_CMPNE:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a != b);
                // printf("NOT EQUAL: %d != %d = %d\n", a, b, a != b);
                break;
            case INST_CMPG:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a > b);
                // printf("GREAT: %d > %d = %d\n", a, b, a > b);
                break;
            case INST_CMPGE:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a >= b);
                // printf("GREAT: %d >= %d = %d\n", a, b, a >= b);
                break;
            case INST_CMPL:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a < b);
                // printf("LESS: %d < %d = %d\n", a, b, a < b);
                break;
            case INST_CMPLE:
                a = pop(loaded_machine);
                b = pop(loaded_machine);
                push(loaded_machine, a <= b);
                // printf("LESS: %d <= %d = %d\n", a, b, a <= b);
                break;
            case INST_JMP:
                flag = 1;
                if (loaded_machine->instructions[i].value < 0 || loaded_machine->instructions[i].value >= (int)loaded_machine->program_size) {
                    fprintf(stderr,  "ERROR: Jump out of bounds\n");
                    exit(1);
                }

                i = loaded_machine->instructions[i].value - 1; // -1 компенсирует i++ в for

                break;
            case INST_ZJMP:
                flag = 1;
                if (pop(loaded_machine) == 0){
                    if (loaded_machine->instructions[i].value < 0 || loaded_machine->instructions[i].value >= (int)loaded_machine->program_size) {
                        fprintf(stderr,  "ERROR: Jump out of bounds\n");
                        exit(1);
                    }
                    i = loaded_machine->instructions[i].value - 1; // loaded_machine->instructions[i].value = target
                }
                break;
            case INST_NZJMP:
                flag = 1;
                if (pop(loaded_machine) != 0){
                    if (loaded_machine->instructions[i].value < 0 || loaded_machine->instructions[i].value >= (int)loaded_machine->program_size) {
                        fprintf(stderr,  "ERROR: Jump out of bounds\n");
                        exit(1);
                    }
                    i = loaded_machine->instructions[i].value - 1;
                }
                break;
            case INST_PRINT:
                // flag = 1;
                // printf("PRINT TOP STACK: %d\n", loaded_machine->stack_size == 0 ? INT_MIN : loaded_machine->stack[loaded_machine->stack_size - 1]);
                printf("PRINT: %d\n", pop(loaded_machine));
                break;
            case INST_HALT:
                i = loaded_machine->program_size;
                break;
        }
        if (!flag) print_stack(loaded_machine);
    }
    



    // print_stack(loaded_machine);

    // Освобождаем память
    free(machine_to_write);
    free(loaded_machine->instructions);
    free(loaded_machine);
    
    return 0;
}
