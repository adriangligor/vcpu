/**
 * virtual cpu implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"


// forward declarations
void cpu_op_nop(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_add1(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_add2(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_add3(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_sub1(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_sub2(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_sub3(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_sub4(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_mul1(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_mul2(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_mul3(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_div1(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_div2(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_div3(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_div4(cpu_state *state, const t_oparg arg1, const t_oparg arg2);

static const cpu_opcode_decl __OP_TABLE__[] = {
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
const cpu_opcode_decl *const OP_TABLE = __OP_TABLE__;
const int OPCODE_COUNT = 15;


void cpu_free(cpu_instr *const first)
{
    cpu_instr *i, *n;

    for (i = first; i != NULL; i = n) {
        n = i->next;
        free(i);
    }
}

void cpu_reset_free(cpu_state *const state)
{
    cpu_free(state->instr);

    state->ans = 0;
    state->instr = NULL;
    state->instrp = NULL;
}

void cpu_exec(cpu_state *const state)
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
