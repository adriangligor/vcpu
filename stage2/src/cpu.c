/**
 * virtual cpu implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"


// forward declarations
void op_nop(cpu_state *state, const cpu_instr *const instr);
void op_add1(cpu_state *state, const cpu_instr *const instr);
void op_add2(cpu_state *state, const cpu_instr *const instr);
void op_add3(cpu_state *state, const cpu_instr *const instr);
void op_sub1(cpu_state *state, const cpu_instr *const instr);
void op_sub2(cpu_state *state, const cpu_instr *const instr);
void op_sub3(cpu_state *state, const cpu_instr *const instr);
void op_sub4(cpu_state *state, const cpu_instr *const instr);
void op_mul1(cpu_state *state, const cpu_instr *const instr);
void op_mul2(cpu_state *state, const cpu_instr *const instr);
void op_mul3(cpu_state *state, const cpu_instr *const instr);
void op_div1(cpu_state *state, const cpu_instr *const instr);
void op_div2(cpu_state *state, const cpu_instr *const instr);
void op_div3(cpu_state *state, const cpu_instr *const instr);
void op_div4(cpu_state *state, const cpu_instr *const instr);
void op_mod1(cpu_state *state, const cpu_instr *const instr);
void op_mod2(cpu_state *state, const cpu_instr *const instr);
void op_mod3(cpu_state *state, const cpu_instr *const instr);
void op_mod4(cpu_state *state, const cpu_instr *const instr);
void op_not1(cpu_state *state, const cpu_instr *const instr);
void op_not2(cpu_state *state, const cpu_instr *const instr);
void op_and1(cpu_state *state, const cpu_instr *const instr);
void op_and2(cpu_state *state, const cpu_instr *const instr);
void op_and3(cpu_state *state, const cpu_instr *const instr);
void op_or1(cpu_state *state, const cpu_instr *const instr);
void op_or2(cpu_state *state, const cpu_instr *const instr);
void op_or3(cpu_state *state, const cpu_instr *const instr);
void op_xor1(cpu_state *state, const cpu_instr *const instr);
void op_xor2(cpu_state *state, const cpu_instr *const instr);
void op_xor3(cpu_state *state, const cpu_instr *const instr);
void op_shl1(cpu_state *state, const cpu_instr *const instr);
void op_shl2(cpu_state *state, const cpu_instr *const instr);
void op_shr1(cpu_state *state, const cpu_instr *const instr);
void op_shr2(cpu_state *state, const cpu_instr *const instr);
void op_set1(cpu_state *state, const cpu_instr *const instr);
void op_set2(cpu_state *state, const cpu_instr *const instr);

static const cpu_opcode_decl __OP_TABLE__[] = {
    { .code = 0x00, .code_str = "NOP", .mode = NONE,    .func = &op_nop },
    { .code = 0x01, .code_str = "ADD", .mode = IMM_IMM, .func = &op_add1 },
    { .code = 0x02, .code_str = "ADD", .mode = REG_IMM, .func = &op_add2 },
    { .code = 0x03, .code_str = "ADD", .mode = REG_REG, .func = &op_add3 },
    { .code = 0x04, .code_str = "SUB", .mode = IMM_IMM, .func = &op_sub1 },
    { .code = 0x05, .code_str = "SUB", .mode = IMM_REG, .func = &op_sub2 },
    { .code = 0x06, .code_str = "SUB", .mode = REG_IMM, .func = &op_sub3 },
    { .code = 0x07, .code_str = "SUB", .mode = REG_REG, .func = &op_sub4 },
    { .code = 0x08, .code_str = "MUL", .mode = IMM_IMM, .func = &op_mul1 },
    { .code = 0x09, .code_str = "MUL", .mode = REG_IMM, .func = &op_mul2 },
    { .code = 0x0A, .code_str = "MUL", .mode = REG_REG, .func = &op_mul3 },
    { .code = 0x0B, .code_str = "DIV", .mode = IMM_IMM, .func = &op_div1 },
    { .code = 0x0C, .code_str = "DIV", .mode = IMM_REG, .func = &op_div2 },
    { .code = 0x0D, .code_str = "DIV", .mode = REG_IMM, .func = &op_div3 },
    { .code = 0x0E, .code_str = "DIV", .mode = REG_REG, .func = &op_div4 },
    { .code = 0x0F, .code_str = "MOD", .mode = IMM_IMM, .func = &op_mod1 },
    { .code = 0x10, .code_str = "MOD", .mode = IMM_REG, .func = &op_mod2 },
    { .code = 0x11, .code_str = "MOD", .mode = REG_IMM, .func = &op_mod3 },
    { .code = 0x12, .code_str = "MOD", .mode = REG_REG, .func = &op_mod4 },
    { .code = 0x13, .code_str = "NOT", .mode = IMM,     .func = &op_not1 },
    { .code = 0x14, .code_str = "NOT", .mode = REG,     .func = &op_not2 },
    { .code = 0x15, .code_str = "AND", .mode = IMM_IMM, .func = &op_and1 },
    { .code = 0x16, .code_str = "AND", .mode = REG_IMM, .func = &op_and2 },
    { .code = 0x17, .code_str = "AND", .mode = REG_REG, .func = &op_and3 },
    { .code = 0x18, .code_str = "OR",  .mode = IMM_IMM, .func = &op_or1 },
    { .code = 0x19, .code_str = "OR",  .mode = REG_IMM, .func = &op_or2 },
    { .code = 0x1A, .code_str = "OR",  .mode = REG_REG, .func = &op_or3 },
    { .code = 0x1B, .code_str = "XOR", .mode = IMM_IMM, .func = &op_xor1 },
    { .code = 0x1C, .code_str = "XOR", .mode = REG_IMM, .func = &op_xor2 },
    { .code = 0x1D, .code_str = "XOR", .mode = REG_REG, .func = &op_xor3 },
    { .code = 0x1E, .code_str = "SHL", .mode = REG_IMM, .func = &op_shl1 },
    { .code = 0x1F, .code_str = "SHL", .mode = REG_REG, .func = &op_shl2 },
    { .code = 0x20, .code_str = "SHR", .mode = REG_IMM, .func = &op_shr1 },
    { .code = 0x21, .code_str = "SHR", .mode = REG_REG, .func = &op_shr2 },
    { .code = 0x22, .code_str = "SET", .mode = REG_IMM, .func = &op_set1 },
    { .code = 0x23, .code_str = "SET", .mode = REG_REG, .func = &op_set2 },
};
const cpu_opcode_decl *const OP_TABLE = __OP_TABLE__;
const int OPCODE_COUNT = 36;


void cpu_reset_free(cpu_state *const state)
{
    cpu_instr *i, *n;

    for (i = state->instr; i != NULL; i = n) {
        n = i->next;
        free(i);
    }

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

        op_decl.func(state, instr);

        state->instrp = state->instrp->next;
    }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void op_nop(cpu_state *state, const cpu_instr *const instr)
{
    // do nothing
}

void op_add1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 + instr->arg2;
}

void op_add2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 + instr->arg2;
}

void op_add3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 + *instr->reg2;
}

void op_sub1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 - instr->arg2;
}

void op_sub2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 - *instr->reg2;
}

void op_sub3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 - instr->arg2;
}

void op_sub4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 - *instr->reg2;
}

void op_mul1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 * instr->arg2;
}

void op_mul2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 * instr->arg2;
}

void op_mul3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 * *instr->reg2;
}

void op_div1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 / instr->arg2;
}

void op_div2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 / *instr->reg2;
}

void op_div3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 / instr->arg2;
}

void op_div4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 / *instr->reg2;
}

void op_mod1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 % instr->arg2;
}

void op_mod2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 % *instr->reg2;
}

void op_mod3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 % instr->arg2;
}

void op_mod4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 % *instr->reg2;
}

void op_not1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = ~instr->arg1;
}

void op_not2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = ~*instr->reg1;
}

void op_and1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 & instr->arg2;
}

void op_and2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 & instr->arg2;
}

void op_and3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 & *instr->reg2;
}

void op_or1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 | instr->arg2;
}

void op_or2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 | instr->arg2;
}

void op_or3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 | *instr->reg2;
}

void op_xor1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = instr->arg1 ^ instr->arg2;
}

void op_xor2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 ^ instr->arg2;
}

void op_xor3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 ^ *instr->reg2;
}

void op_shl1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = (t_oparg) (*instr->reg1 << instr->arg2);
}

void op_shl2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = (t_oparg) (*instr->reg1 << *instr->reg2);
}

void op_shr1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = (t_oparg) (*instr->reg1 >> instr->arg2);
}

void op_shr2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = *instr->reg1 >> *instr->reg2;
}

void op_set1(cpu_state *state, const cpu_instr *const instr)
{
    *instr->reg2 = instr->arg2;
}

void op_set2(cpu_state *state, const cpu_instr *const instr)
{
    *instr->reg2 = *instr->reg1;
}

#pragma clang diagnostic pop
