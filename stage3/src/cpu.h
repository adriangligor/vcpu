/**
 * definition of functions and structures for the vcpu
 */

#ifndef VCPU_CPU_H
#define VCPU_CPU_H


#include <stdbool.h>
#include <stdint.h>


/**
 * size of an opcode identifier including \0
 */
#define OPCODE_SZ_SIZE 4

/**
 * smallest unsigned vcpu integer
 */
#define VCPU_UINT_MIN 0

/**
 * largest unsigned vcpu integer
 */
#define VCPU_UINT_MAX UINT16_MAX

/**
 * smallest signed vcpu integer
 */
#define VCPU_INT_MIN INT16_MIN

/**
 * largest signed vcpu integer
 */
#define VCPU_INT_MAX INT16_MAX

/**
 * zero flag bit: a result was zero
 */
#define FLG_ZERO 0x01

/**
 * carry flag bit: an unsigend result is one bit longer than the register
 */
#define FLG_CARRY 0x02

/**
 * negative flag bit: a result was negative
 */
#define FLG_NEGATIVE 0x04

/**
 * overflow flag bit: a signed result does not have the expected sign
 */
#define FLG_OVERFLOW 0x08

/**
 * bitmask covering all arithmetic flags (zero, carry, negative, overflow)
 */
#define FLG__ARITH 0x0F

/**
 * critical halt flag bit: the cpu is in a critical state
 */
#define FLG_CRITICAL 0x80


/**
 * enumeration of addressing modes
 */
typedef enum addr_mode {
    NONE,
    IMM,
    IMM_IMM,
    IMM_REG,
    REG,
    REG_IMM,
    REG_REG
} addr_mode;

/**
 * type of the vcpu representation of an opcode
 */
typedef uint8_t t_opcode;

/**
 * type of the vcpu representation of an operation argument
 */
typedef uint16_t t_oparg;

/**
 * type of the vcpu representation of a signed operation argument
 */
typedef int16_t t_soparg;

/**
 * type of the next larger signed operation argument, for overflowing operations
 */
typedef int32_t t_extsoparg;

/**
 * structure representing a single instruction
 */
typedef struct cpu_instr {
    t_opcode opcode;
    t_oparg arg1;
    t_oparg *reg1;
    t_oparg arg2;
    t_oparg *reg2;
    struct cpu_instr *next;
} cpu_instr;

/**
 * structure representing the whole state of the vcpu
 */
typedef struct cpu_state {
    t_oparg ans;
    t_oparg flags;
    cpu_instr *instr;
    cpu_instr *instrp;
} cpu_state;

/**
 * type of a function implementing a vcpu operation
 */
typedef void (*const t_opfunc)(cpu_state *const, const cpu_instr *const instr);

/**
 * structure representing the definition of a vcpu operation
 */
typedef struct cpu_opcode_decl {
    t_opcode code;
    char code_str[OPCODE_SZ_SIZE];
    addr_mode mode;
    t_opfunc func;
} cpu_opcode_decl;


/**
 * table containing opcode declarations
 */
const cpu_opcode_decl *const OP_TABLE;

/**
 * number of entries in OP_TABLE
 */
const int OPCODE_COUNT;


/**
 * reset the cpu state and free instruction memory
 */
void cpu_reset_free(cpu_state *const state);

/**
 * execute the instruction
 */
bool cpu_exec(cpu_state *const state);


#endif
