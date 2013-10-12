/**
 * virtual cpu implementation
 */

#include <stdio.h>
#include <stdbool.h>
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
typedef void (*t_opfunc)(cpu_state *, const t_oparg, const t_oparg);

/**
 * structure representing the definition of a vcpu operation
 */
typedef struct cpu_opcode_decl {
    t_opcode code;
    char code_str[OPCODE_SZ_SIZE];
    addr_mode mode;
    t_opfunc func;
} cpu_opcode_decl;


const cpu_opcode_decl OP_TABLE[] = {
    { .code = 0x00, .code_str = "NOP", .mode = NONE,    .func = &cpu_op_nop },
    { .code = 0x01, .code_str = "ADD", .mode = IMM_IMM, .func = &cpu_op_add1 },
    { .code = 0x02, .code_str = "ADD", .mode = REG_IMM, .func = &cpu_op_add2 },
    { .code = 0x03, .code_str = "ADD", .mode = REG_REG, .func = &cpu_op_add3 },
    { .code = 0x04, .code_str = "SUB", .mode = IMM_IMM, .func = &cpu_op_sub1 },
    { .code = 0x05, .code_str = "SUB", .mode = IMM_REG, .func = &cpu_op_sub2 },
    { .code = 0x06, .code_str = "SUB", .mode = REG_IMM, .func = &cpu_op_sub3 },
    { .code = 0x07, .code_str = "SUB", .mode = REG_REG, .func = &cpu_op_sub4 },
    { .code = 0x08, .code_str = "MUL", .mode = IMM_IMM, .func = &cpu_op_mul1 },
    { .code = 0x09, .code_str = "MUL", .mode = REG_IMM, .func = &cpu_op_mul2 },
    { .code = 0x0A, .code_str = "MUL", .mode = REG_REG, .func = &cpu_op_mul3 },
    { .code = 0x0B, .code_str = "DIV", .mode = IMM_IMM, .func = &cpu_op_div1 },
    { .code = 0x0C, .code_str = "DIV", .mode = IMM_REG, .func = &cpu_op_div2 },
    { .code = 0x0D, .code_str = "DIV", .mode = REG_IMM, .func = &cpu_op_div3 },
    { .code = 0x0E, .code_str = "DIV", .mode = REG_REG, .func = &cpu_op_div4 }
};
const int OPCODE_COUNT = 15;


addr_mode arg_mode(const char *arg)
{
    if (arg == NULL) {
        return NONE;
    } else if (strncmp(arg, "ANS", OPCODE_SZ_SIZE) == 0) {
        return REG;
    } else {
        return IMM;
    }
}

addr_mode args_mode(const char *arg1, const char *arg2)
{
    const addr_mode arg1_mode = arg_mode(arg1);
    const addr_mode arg2_mode = arg_mode(arg2);

    if (arg1_mode == NONE && arg2_mode == NONE) {
        return NONE;
    } else if (arg1_mode == IMM && arg2_mode == NONE) {
        return IMM;
    } else if (arg1_mode == REG && arg2_mode == NONE) {
        return REG;
    } else if (arg1_mode == IMM && arg2_mode == IMM) {
        return IMM_IMM;
    } else if (arg1_mode == IMM && arg2_mode == REG) {
        return IMM_REG;
    } else if (arg1_mode == REG && arg2_mode == IMM) {
        return REG_IMM;
    } else if (arg1_mode == REG && arg2_mode == REG) {
        return REG_REG;
    } else {
        printf("vcpu fatal error: illegal argument combination %s,%s\n",
               arg1, arg2);
        exit(1);
    }
}

const t_opcode cpu_find_opcode(const char *opcode_str, const char *arg1,
                               const char *arg2)
{
    const addr_mode mode = args_mode(arg1, arg2);
    cpu_opcode_decl op;
    bool op_match;
    bool mode_match;
    int i;

    for (i = 0; i < OPCODE_COUNT; i++) {
        op = OP_TABLE[i];

        op_match = (strncmp(opcode_str, op.code_str, OPCODE_SZ_SIZE) == 0);
        mode_match = (mode == op.mode);

        if (op_match && mode_match) {
            return op.code;
        }
    }

    printf("vcpu fatal error: unknown instruction %s[mode:%d]\n", opcode_str, mode);
    exit(1);
}

void cpu_free(cpu_instr *first)
{
    cpu_instr *i;
    cpu_instr *n;

    for (i = first; i != NULL; i = n) {
        n = i->next;
        free(i);
    }
}

void cpu_reset_free(cpu_state *state)
{
    cpu_free(state->instr);

    state->ans = 0;
    state->instr = NULL;
    state->instrp = NULL;
}

void cpu_exec(cpu_state *state)
{
    cpu_instr *instr;
    cpu_opcode_decl op_decl;

    while (state->instrp != NULL) {
        instr = state->instrp;
        op_decl = OP_TABLE[instr->opcode];

        op_decl.func(state, instr->arg1, instr->arg2);

        state->instrp = state->instrp->next;
    }
}

const void cpu_opcode(cpu_instr *instr, const char *opcode_str,
                      const char *arg1, const char *arg2, cpu_state *state)
{
    const t_opcode opcode = cpu_find_opcode(opcode_str, arg1, arg2);
    const addr_mode arg1_mode = arg_mode(arg1);
    const addr_mode arg2_mode = arg_mode(arg2);

    instr->opcode = opcode;
    instr->next = NULL;

    if (arg1_mode == NONE) {
        instr->arg1 = 0;
        instr->reg1 = NULL;
    } else if (arg1_mode == IMM) {
        instr->arg1 = atoi(arg1);
        instr->reg1 = NULL;
    } else if (arg1_mode == REG) {
        instr->arg1 = 0;
        instr->reg1 = &state->ans;
    }

    if (arg2_mode == NONE) {
        instr->arg2 = 0;
        instr->reg2 = NULL;
    } else if (arg2_mode == IMM) {
        instr->arg2 = atoi(arg2);
        instr->reg2 = NULL;
    } else if (arg1_mode == REG) {
        instr->arg2 = 0;
        instr->reg2 = &state->ans;
    }
}

const char *cpu_opcode_str(const t_opcode opcode)
{
    if (opcode >= OPCODE_COUNT) {
        printf("vcpu fatal error: illegal opcode %d\n", opcode);
        exit(1);
    }

    return OP_TABLE[opcode].code_str;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void cpu_op_nop(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    // do nothing
}

void cpu_op_add1(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 + arg2;
}

void cpu_op_add2(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = state->ans + arg2;
}

void cpu_op_add3(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = state->ans + state->ans;
}

void cpu_op_sub1(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 - arg2;
}

void cpu_op_sub2(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 - state->ans;
}

void cpu_op_sub3(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = state->ans - arg2;
}

void cpu_op_sub4(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = state->ans - state->ans;
}

void cpu_op_mul1(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 * arg2;
}

void cpu_op_mul2(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = state->ans * arg2;
}

void cpu_op_mul3(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = state->ans * state->ans;
}

void cpu_op_div1(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 / arg2;
}

void cpu_op_div2(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = arg1 / state->ans;
}

void cpu_op_div3(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = state->ans / arg2;
}

void cpu_op_div4(cpu_state *state, const t_oparg arg1, const t_oparg arg2)
{
    state->ans = state->ans / state->ans;
}

#pragma clang diagnostic pop
