#ifndef __VMASM_H
#define __VMASM_H

#include "vm.h"
#include "parser.h"
Inst *generate_instruction(TokenNode *root);

#endif // __VMASM_H