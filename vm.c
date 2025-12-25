#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_STACK_SIZE 1024

typedef enum {

	INST_PUSH,
	INST_POP,
	INST_ADD,
	INST_SUB,
	INST_MUL,
	INST_DIV,
	INST_PRINT

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

#define DEF_INST_PUSH(x) {.type = INST_PUSH, .value = x}
#define DEF_INST_POP() {.type = INST_POP}
#define DEF_INST_ADD() {.type = INST_ADD}
#define DEF_INST_SUB() {.type = INST_SUB}
#define DEF_INST_MUL() {.type = INST_MUL}
#define DEF_INST_DIV() {.type = INST_DIV}
#define DEF_INST_PRINT() {.type = INST_PRINT}

Inst program[] = {
	DEF_INST_PUSH(5),
	DEF_INST_PUSH(5),
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
	for (int i = machine->stack_size - 1; i >= 0; --i) {
		printf("%d\n", machine->stack[i]);
	}
}


void write_program_to_file(Machine *machine, char *file_path) {
	FILE *file = fopen(file_path, "wb");

	if (file == NULL) {
		fprintf(stderr, "ERROR: Could not write to file %s\n", file_path);
		exit(1);
	}

	fwrite(machine->instructions, sizeof(machine->instructions[0]), PROGRAM_SIZE, file);

	fclose(file);
}

Machine* read_program_from_file(Machine *machine, char *file_path) {
	FILE *file = fopen(file_path, "rb");

	if (file == NULL) {
		fprintf(stderr, "ERROR: Could not read from file %s\n", file_path);
		exit(1);
	}

	Inst *instructions = malloc(sizeof(Inst) * MAX_STACK_SIZE);

	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	fread(instructions, sizeof(instructions[0]), length / 8, file);
	
	machine->program_size = length / 8;
	machine->instructions = instructions;

	fclose(file);

	return machine;

}

int main() {

	int a, b;

	Machine *loaded_machine = malloc(sizeof(Machine) * MAX_STACK_SIZE);
	loaded_machine->instructions = program;
	write_program_to_file(loaded_machine, "test.vm");
	loaded_machine = read_program_from_file(loaded_machine, "test.vm");

	for (size_t i = 0; i < loaded_machine->program_size; ++i) {
		switch (loaded_machine->instructions[i].type) {
			case INST_PUSH:
				push(loaded_machine, loaded_machine->instructions[i].value);
				break;
			case INST_POP:
				pop(loaded_machine);
				break;
			case INST_ADD:
				a = pop(loaded_machine);
				b = pop(loaded_machine);
				push(loaded_machine, a + b);
				break;
			case INST_SUB:
				a = pop(loaded_machine);
				b = pop(loaded_machine);
				push(loaded_machine, b - a);
				break;
			case INST_MUL:
				a = pop(loaded_machine);
				b = pop(loaded_machine);
				push(loaded_machine, a * b);
				break;
			case INST_DIV:
				a = pop(loaded_machine);
				b = pop(loaded_machine);
				push(loaded_machine, b / a);
				break;
			case INST_PRINT:
				printf("%d\n", pop(loaded_machine));
				break;
		}
	}

	print_stack(loaded_machine);

	return 0;
}