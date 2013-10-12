/**
 * definition of functions and structures for the vcpu
 */

#ifndef VCPU_CPU_H
#define VCPU_CPU_H


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
typedef unsigned char t_opcode;

/**
 * type of the vcpu representation of an operation argument
 */
typedef signed int t_oparg;

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
    int ans;
    cpu_instr *instr;
    cpu_instr *instrp;
} cpu_state;


/**
 * reset the cpu state and free instruction memory
 */
void cpu_reset_free(cpu_state *state);

/**
 * execute the instruction
 */
void cpu_exec(cpu_state *state);

/**
 * fill an instruction from an opcode mnemonic and arguments
 */
const void cpu_opcode(cpu_instr *instr, const char *opcode_str,
                      const char *arg1, const char *arg2, cpu_state *state);

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


#endif
