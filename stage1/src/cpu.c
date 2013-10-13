/**
 * virtual cpu implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"


/**
 * size of an opcode identifier including \0
 */
#define OPCODE_SZ_SIZE 4

/**
 * type of a function implementing a vcpu operation
 */
typedef void (*const t_opfunc)(cpu_state *const, const t_oparg, const t_oparg);

/**
 * structure representing the definition of a vcpu operation
 */
typedef struct cpu_opcode_decl {
    char opcode_str[OPCODE_SZ_SIZE];
    t_opcode opcode;
    t_opfunc op_func;
} cpu_opcode_decl;


const cpu_opcode_decl OP_TABLE[] = {
    { .opcode_str = "NOP", .opcode = 0, .op_func = &cpu_op_nop },
    { .opcode_str = "ADD", .opcode = 1, .op_func = &cpu_op_add },
    { .opcode_str = "SUB", .opcode = 2, .op_func = &cpu_op_sub },
    { .opcode_str = "MUL", .opcode = 3, .op_func = &cpu_op_mul },
    { .opcode_str = "DIV", .opcode = 4, .op_func = &cpu_op_div }
};
const int OPCODE_COUNT = 5;


void cpu_reset(cpu_state *const state)
{
    const cpu_instr nop = { .opcode = 0, .arg1 = 0, .arg2 = 0 };

    state->ans = 0;
    state->instr = nop;
}

void cpu_exec(cpu_state *const state)
{
    cpu_instr instr = state->instr;
    cpu_opcode_decl op_decl = OP_TABLE[instr.opcode];
    op_decl.op_func(state, instr.arg1, instr.arg2);
}

const t_opcode cpu_opcode(const char *const opcode_str)
{
    cpu_opcode_decl op;
    int i;
    
    for (i = 0; i < OPCODE_COUNT; i++) {
        op = OP_TABLE[i];
        if (strncmp(opcode_str, op.opcode_str, OPCODE_SZ_SIZE) == 0) {
            return op.opcode;
        }
    }

    printf("vcpu fatal error: unknown mnemonic %s\n", opcode_str);
    exit(1);
}

//const char *cpu_opcode_str(const t_opcode opcode)
//{
//    if (opcode >= OPCODE_COUNT) {
//        printf("vcpu fatal error: illegal opcode %i\n", opcode);
//        exit(1);
//    }
//
//    return OP_TABLE[opcode].opcode_str;
//}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void cpu_op_nop(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    // do nothing
}
#pragma clang diagnostic pop

void cpu_op_add(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 + arg2;
}

void cpu_op_sub(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 - arg2;
}

void cpu_op_mul(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 * arg2;
}

void cpu_op_div(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 / arg2;
}
