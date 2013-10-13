/**
 * definition of functions and structures for the vcpu
 */

#ifndef VCPU_CPU_H
#define VCPU_CPU_H


#include <stdint.h>


/**
 * type of the vcpu representation of an opcode
 */
typedef uint8_t t_opcode;

/**
 * type of the vcpu representation of an operation argument
 */
typedef int16_t t_oparg;

/**
 * structure representing a single instruction
 */
typedef struct cpu_instr {
    t_opcode opcode;
    t_oparg arg1;
    t_oparg arg2;
} cpu_instr;

/**
 * structure representing the whole state of the vcpu
 */
typedef struct cpu_state {
    t_oparg ans;
    cpu_instr instr;
} cpu_state;


/**
 * reset the cpu state
 */
void cpu_reset(cpu_state *const state);

/**
 * execute the instruction
 */
void cpu_exec(cpu_state *const state);

/**
 * get an opcode from its string mnemonic
 */
const t_opcode cpu_opcode(const char *const opcode_str);

/**
 * get the string mnemonic of an opcode
 */
//const char *cpu_opcode_str(t_opcode opcode);

void cpu_op_nop(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_add(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_sub(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_mul(cpu_state *state, const t_oparg arg1, const t_oparg arg2);
void cpu_op_div(cpu_state *state, const t_oparg arg1, const t_oparg arg2);


#endif
