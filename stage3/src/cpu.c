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
void op_addu1(cpu_state *state, const cpu_instr *const instr);
void op_addu2(cpu_state *state, const cpu_instr *const instr);
void op_addu3(cpu_state *state, const cpu_instr *const instr);
void op_sub1(cpu_state *state, const cpu_instr *const instr);
void op_sub2(cpu_state *state, const cpu_instr *const instr);
void op_sub3(cpu_state *state, const cpu_instr *const instr);
void op_sub4(cpu_state *state, const cpu_instr *const instr);
void op_subu1(cpu_state *state, const cpu_instr *const instr);
void op_subu2(cpu_state *state, const cpu_instr *const instr);
void op_subu3(cpu_state *state, const cpu_instr *const instr);
void op_subu4(cpu_state *state, const cpu_instr *const instr);
void op_mul1(cpu_state *state, const cpu_instr *const instr);
void op_mul2(cpu_state *state, const cpu_instr *const instr);
void op_mul3(cpu_state *state, const cpu_instr *const instr);
void op_mulu1(cpu_state *state, const cpu_instr *const instr);
void op_mulu2(cpu_state *state, const cpu_instr *const instr);
void op_mulu3(cpu_state *state, const cpu_instr *const instr);
void op_div1(cpu_state *state, const cpu_instr *const instr);
void op_div2(cpu_state *state, const cpu_instr *const instr);
void op_div3(cpu_state *state, const cpu_instr *const instr);
void op_div4(cpu_state *state, const cpu_instr *const instr);
void op_divu1(cpu_state *state, const cpu_instr *const instr);
void op_divu2(cpu_state *state, const cpu_instr *const instr);
void op_divu3(cpu_state *state, const cpu_instr *const instr);
void op_divu4(cpu_state *state, const cpu_instr *const instr);
void op_mod1(cpu_state *state, const cpu_instr *const instr);
void op_mod2(cpu_state *state, const cpu_instr *const instr);
void op_mod3(cpu_state *state, const cpu_instr *const instr);
void op_mod4(cpu_state *state, const cpu_instr *const instr);
void op_modu1(cpu_state *state, const cpu_instr *const instr);
void op_modu2(cpu_state *state, const cpu_instr *const instr);
void op_modu3(cpu_state *state, const cpu_instr *const instr);
void op_modu4(cpu_state *state, const cpu_instr *const instr);
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
    { .code = 0x00, .code_str = "NOP",  .mode = NONE,    .func = &op_nop },
    { .code = 0x01, .code_str = "ADD",  .mode = IMM_IMM, .func = &op_add1 },
    { .code = 0x02, .code_str = "ADD",  .mode = REG_IMM, .func = &op_add2 },
    { .code = 0x03, .code_str = "ADD",  .mode = REG_REG, .func = &op_add3 },
    { .code = 0x04, .code_str = "ADDU", .mode = IMM_IMM, .func = &op_addu1 },
    { .code = 0x05, .code_str = "ADDU", .mode = REG_IMM, .func = &op_addu2 },
    { .code = 0x06, .code_str = "ADDU", .mode = REG_REG, .func = &op_addu3 },
    { .code = 0x07, .code_str = "SUB",  .mode = IMM_IMM, .func = &op_sub1 },
    { .code = 0x08, .code_str = "SUB",  .mode = IMM_REG, .func = &op_sub2 },
    { .code = 0x09, .code_str = "SUB",  .mode = REG_IMM, .func = &op_sub3 },
    { .code = 0x0A, .code_str = "SUB",  .mode = REG_REG, .func = &op_sub4 },
    { .code = 0x0B, .code_str = "SUBU", .mode = IMM_IMM, .func = &op_subu1 },
    { .code = 0x0C, .code_str = "SUBU", .mode = IMM_REG, .func = &op_subu2 },
    { .code = 0x0D, .code_str = "SUBU", .mode = REG_IMM, .func = &op_subu3 },
    { .code = 0x0E, .code_str = "SUBU", .mode = REG_REG, .func = &op_subu4 },
    { .code = 0x0F, .code_str = "MUL",  .mode = IMM_IMM, .func = &op_mul1 },
    { .code = 0x10, .code_str = "MUL",  .mode = REG_IMM, .func = &op_mul2 },
    { .code = 0x11, .code_str = "MUL",  .mode = REG_REG, .func = &op_mul3 },
    { .code = 0x12, .code_str = "MULU", .mode = IMM_IMM, .func = &op_mulu1 },
    { .code = 0x13, .code_str = "MULU", .mode = REG_IMM, .func = &op_mulu2 },
    { .code = 0x14, .code_str = "MULU", .mode = REG_REG, .func = &op_mulu3 },
    { .code = 0x15, .code_str = "DIV",  .mode = IMM_IMM, .func = &op_div1 },
    { .code = 0x16, .code_str = "DIV",  .mode = IMM_REG, .func = &op_div2 },
    { .code = 0x17, .code_str = "DIV",  .mode = REG_IMM, .func = &op_div3 },
    { .code = 0x18, .code_str = "DIV",  .mode = REG_REG, .func = &op_div4 },
    { .code = 0x19, .code_str = "DIVU", .mode = IMM_IMM, .func = &op_divu1 },
    { .code = 0x1A, .code_str = "DIVU", .mode = IMM_REG, .func = &op_divu2 },
    { .code = 0x1B, .code_str = "DIVU", .mode = REG_IMM, .func = &op_divu3 },
    { .code = 0x1C, .code_str = "DIVU", .mode = REG_REG, .func = &op_divu4 },
    { .code = 0x1D, .code_str = "MOD",  .mode = IMM_IMM, .func = &op_mod1 },
    { .code = 0x1E, .code_str = "MOD",  .mode = IMM_REG, .func = &op_mod2 },
    { .code = 0x1F, .code_str = "MOD",  .mode = REG_IMM, .func = &op_mod3 },
    { .code = 0x20, .code_str = "MOD",  .mode = REG_REG, .func = &op_mod4 },
    { .code = 0x21, .code_str = "MODU", .mode = IMM_IMM, .func = &op_modu1 },
    { .code = 0x22, .code_str = "MODU", .mode = IMM_REG, .func = &op_modu2 },
    { .code = 0x23, .code_str = "MODU", .mode = REG_IMM, .func = &op_modu3 },
    { .code = 0x24, .code_str = "MODU", .mode = REG_REG, .func = &op_modu4 },
    { .code = 0x25, .code_str = "NOT",  .mode = IMM,     .func = &op_not1 },
    { .code = 0x26, .code_str = "NOT",  .mode = REG,     .func = &op_not2 },
    { .code = 0x27, .code_str = "AND",  .mode = IMM_IMM, .func = &op_and1 },
    { .code = 0x28, .code_str = "AND",  .mode = REG_IMM, .func = &op_and2 },
    { .code = 0x29, .code_str = "AND",  .mode = REG_REG, .func = &op_and3 },
    { .code = 0x2A, .code_str = "OR",   .mode = IMM_IMM, .func = &op_or1 },
    { .code = 0x2B, .code_str = "OR",   .mode = REG_IMM, .func = &op_or2 },
    { .code = 0x2C, .code_str = "OR",   .mode = REG_REG, .func = &op_or3 },
    { .code = 0x2D, .code_str = "XOR",  .mode = IMM_IMM, .func = &op_xor1 },
    { .code = 0x2E, .code_str = "XOR",  .mode = REG_IMM, .func = &op_xor2 },
    { .code = 0x2F, .code_str = "XOR",  .mode = REG_REG, .func = &op_xor3 },
    { .code = 0x30, .code_str = "SHL",  .mode = REG_IMM, .func = &op_shl1 },
    { .code = 0x31, .code_str = "SHL",  .mode = REG_REG, .func = &op_shl2 },
    { .code = 0x32, .code_str = "SHR",  .mode = REG_IMM, .func = &op_shr1 },
    { .code = 0x33, .code_str = "SHR",  .mode = REG_REG, .func = &op_shr2 },
    { .code = 0x34, .code_str = "SET",  .mode = REG_IMM, .func = &op_set1 },
    { .code = 0x35, .code_str = "SET",  .mode = REG_REG, .func = &op_set2 },
};
const cpu_opcode_decl *const OP_TABLE = __OP_TABLE__;
const int OPCODE_COUNT = 54;


static inline
void arith_set(cpu_state *state, t_oparg flag)
{
    // clear all arithmetic flags
    state->flags |= flag;
}

//static inline
//void arith_clear(cpu_state *state, t_oparg flag)
//{
//    // clear all arithmetic flags
//    state->flags &= ~flag;
//}

static inline
void arith_clear_all(cpu_state *state)
{
    // clear all arithmetic flags
    state->flags &= ~FLG__ARITH;
}

static inline
t_oparg adds(cpu_state *state, const t_soparg v1, const t_soparg v2)
{
    // https://www.securecoding.cert.org/confluence/display/seccode/INT32-C.+Ensure+that+operations+on+signed+integers+do+not+result+in+overflow
    t_soparg ret;

    arith_clear_all(state);

    if ((v2 > 0 && v1 > VCPU_INT_MAX - v2) ||
        (v2 < 0 && v1 < VCPU_INT_MIN - v2))
    {
        ret = 0;
        arith_set(state, FLG_OVERFLOW);
    } else {
        ret = v1 + v2;
    }

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    } else if (ret < 0) {
        arith_set(state, FLG_NEGATIVE);
    }

    return (t_oparg) ret;
}

static inline
t_oparg addu(cpu_state *state, const t_oparg v1, const t_oparg v2)
{
    // https://www.securecoding.cert.org/confluence/display/seccode/INT30-C.+Ensure+that+unsigned+integer+operations+do+not+wrap
    t_oparg ret;

    arith_clear_all(state);

    ret = v1 + v2;

    if (VCPU_UINT_MAX - v1 < v2) {
        arith_set(state, FLG_CARRY);
    }
    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    }

    return ret;
}

static inline
t_oparg subs(cpu_state *state, const t_soparg v1, const t_soparg v2)
{
    // https://www.securecoding.cert.org/confluence/display/seccode/INT32-C.+Ensure+that+operations+on+signed+integers+do+not+result+in+overflow
    t_soparg ret;

    arith_clear_all(state);

    if ((v2 > 0 && v1 < VCPU_INT_MIN + v2) ||
        (v2 < 0 && v1 > VCPU_INT_MAX + v2))
    {
        ret = 0;
        arith_set(state, FLG_OVERFLOW);
    } else {
        ret = v1 - v2;
    }

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    } else if (ret < 0) {
        arith_set(state, FLG_NEGATIVE);
    }

    return (t_oparg) ret;
}

static inline
t_oparg subu(cpu_state *state, const t_oparg v1, const t_oparg v2)
{
    // https://www.securecoding.cert.org/confluence/display/seccode/INT30-C.+Ensure+that+unsigned+integer+operations+do+not+wrap
    t_soparg ret;

    arith_clear_all(state);

    ret = (t_soparg) (v1 - v2);

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    } else if (ret < 0) {
        arith_set(state, FLG_NEGATIVE);
    }

    return (t_oparg) ret;
}

static inline
t_oparg muls(cpu_state *state, const t_soparg v1, const t_soparg v2)
{
    // https://www.securecoding.cert.org/confluence/display/seccode/INT32-C.+Ensure+that+operations+on+signed+integers+do+not+result+in+overflow
    t_extsoparg ret;

    arith_clear_all(state);

    ret = (t_extsoparg) v1 * (t_extsoparg) v2;

    if (ret > VCPU_INT_MAX || ret < VCPU_INT_MIN) {
        ret = 0;
        arith_set(state, FLG_OVERFLOW);
    }

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    } else if (ret < 0) {
        arith_set(state, FLG_NEGATIVE);
    }

    return (t_oparg) ret;
}

static inline
t_oparg mulu(cpu_state *state, const t_oparg v1, const t_oparg v2)
{
    // https://www.securecoding.cert.org/confluence/display/seccode/INT30-C.+Ensure+that+unsigned+integer+operations+do+not+wrap
    t_oparg ret;

    arith_clear_all(state);

    if (v2 != 0 && v1 > VCPU_UINT_MAX / v2) {
        ret = 0;
        arith_set(state, FLG_OVERFLOW);
    } else {
        ret = v1 * v2;
    }

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    }

    return ret;
}

static inline
t_oparg divs(cpu_state *state, const t_soparg v1, const t_soparg v2)
{
    // https://www.securecoding.cert.org/confluence/display/seccode/INT32-C.+Ensure+that+operations+on+signed+integers+do+not+result+in+overflow
    t_soparg ret;

    arith_clear_all(state);

    if (v2 == 0) {
        arith_set(state, FLG_CRITICAL);
        return state->ans;
    }

    if (v1 == VCPU_INT_MIN && v2 == -1) {
        ret = 0;
        arith_set(state, FLG_OVERFLOW);
    } else {
        ret = v1 / v2;
    }

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    } else if (ret < 0) {
        arith_set(state, FLG_NEGATIVE);
    }

    return (t_oparg) ret;
}

static inline
t_oparg divu(cpu_state *state, const t_oparg v1, const t_oparg v2)
{
    t_oparg ret;

    arith_clear_all(state);

    // no overflow possible, only division by zero
    if (v2 == 0) {
        arith_set(state, FLG_CRITICAL);
        return state->ans;
    }

    ret = v1 / v2;

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    }

    return ret;
}

static inline
t_oparg mods(cpu_state *state, const t_soparg v1, const t_soparg v2)
{
    t_oparg ret;

    arith_clear_all(state);

    if (v2 == 0) {
        arith_set(state, FLG_CRITICAL);
        return state->ans;
    }

    if (v2 > 0) {
        ret = (t_oparg) (v1 % v2);
        if (ret < 0) {
            ret += v2;
        }
    } else {
        ret = (t_oparg) (-v1 % -v2);
        if (ret < 0) {
            ret += -v2;
        }
    }

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    }

    return ret;
}

static inline
t_oparg modu(cpu_state *state, const t_oparg v1, const t_oparg v2)
{
    t_oparg ret;

    arith_clear_all(state);

    if (v2 == 0) {
        arith_set(state, FLG_CRITICAL);
        return state->ans;
    }

    ret = v1 % v2;

    if (ret == 0) {
        arith_set(state, FLG_ZERO);
    }

    return ret;
}

void cpu_reset_free(cpu_state *const state)
{
    cpu_instr *i, *n;

    for (i = state->instr; i != NULL; i = n) {
        n = i->next;
        free(i);
    }

    state->ans = 0;
    state->flags = 0;
    state->instr = NULL;
    state->instrp = NULL;
}

bool cpu_exec(cpu_state *const state)
{
    cpu_instr *instr;
    cpu_opcode_decl op_decl;

    while (state->instrp != NULL && (state->flags & FLG_CRITICAL) == 0) {
        instr = state->instrp;
        op_decl = OP_TABLE[instr->opcode];

        op_decl.func(state, instr);

        state->instrp = state->instrp->next;
    }

    return ((state->flags & FLG_CRITICAL) == 0);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void op_nop(cpu_state *state, const cpu_instr *const instr)
{
    // do nothing
}

void op_add1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = adds(state, (t_soparg) instr->arg1, (t_soparg) instr->arg2);
}

void op_add2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = adds(state, (t_soparg) *instr->reg1, (t_soparg) instr->arg2);
}

void op_add3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = adds(state, (t_soparg) *instr->reg1, (t_soparg) *instr->reg2);
}

void op_addu1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = addu(state, instr->arg1, instr->arg2);
}

void op_addu2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = addu(state, *instr->reg1, instr->arg2);
}

void op_addu3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = addu(state, *instr->reg1, *instr->reg2);
}

void op_sub1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = subs(state, (t_soparg) instr->arg1, (t_soparg) instr->arg2);
}

void op_sub2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = subs(state, (t_soparg) instr->arg1, (t_soparg) *instr->reg2);
}

void op_sub3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = subs(state, (t_soparg) *instr->reg1, (t_soparg) instr->arg2);
}

void op_sub4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = subs(state, (t_soparg) *instr->reg1, (t_soparg) *instr->reg2);
}

void op_subu1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = subu(state, instr->arg1, instr->arg2);
}

void op_subu2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = subu(state, instr->arg1, *instr->reg2);
}

void op_subu3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = subu(state, *instr->reg1, instr->arg2);
}

void op_subu4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = subu(state, *instr->reg1, *instr->reg2);
}

void op_mul1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = muls(state, (t_soparg) instr->arg1, (t_soparg) instr->arg2);
}

void op_mul2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = muls(state, (t_soparg) *instr->reg1, (t_soparg) instr->arg2);
}

void op_mul3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = muls(state, (t_soparg) *instr->reg1, (t_soparg) *instr->reg2);
}

void op_mulu1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = mulu(state, instr->arg1, instr->arg2);
}

void op_mulu2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = mulu(state, *instr->reg1, instr->arg2);
}

void op_mulu3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = mulu(state, *instr->reg1, *instr->reg2);
}

void op_div1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = divs(state, (t_soparg) instr->arg1, (t_soparg) instr->arg2);
}

void op_div2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = divs(state, (t_soparg) instr->arg1, (t_soparg) *instr->reg2);
}

void op_div3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = divs(state, (t_soparg) *instr->reg1, (t_soparg) instr->arg2);
}

void op_div4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = divs(state, (t_soparg) *instr->reg1, (t_soparg) *instr->reg2);
}

void op_divu1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = divu(state, instr->arg1, instr->arg2);
}

void op_divu2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = divu(state, instr->arg1, *instr->reg2);
}

void op_divu3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = divu(state, *instr->reg1, instr->arg2);
}

void op_divu4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = divu(state, *instr->reg1, *instr->reg2);
}

void op_mod1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = mods(state, (t_soparg) instr->arg1, (t_soparg) instr->arg2);
}

void op_mod2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = mods(state, (t_soparg) instr->arg1, (t_soparg) *instr->reg2);
}

void op_mod3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = mods(state, (t_soparg) *instr->reg1, (t_soparg) instr->arg2);
}

void op_mod4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = mods(state, (t_soparg) *instr->reg1, (t_soparg) *instr->reg2);
}

void op_modu1(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = modu(state, instr->arg1, instr->arg2);
}

void op_modu2(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = modu(state, instr->arg1, *instr->reg2);
}

void op_modu3(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = modu(state, *instr->reg1, instr->arg2);
}

void op_modu4(cpu_state *state, const cpu_instr *const instr)
{
    state->ans = modu(state, *instr->reg1, *instr->reg2);
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
    state->ans = (t_oparg) (*instr->reg1 >> *instr->reg2);
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
