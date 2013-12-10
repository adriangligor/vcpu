# Virtual CPU (vcpu)

## Features

* 16bit CPU
* 16 general purpose registers Rx0 - RxF
* 3 special registers:
    Rx0/ACC/ANS: accumulator (aka answer, stores arithmetic results)
    PRC: program counter (next instruction address)
    FLG: flags (bitwise - carry, error, negative, zero, overflow)
* 64k total memory, byte aligned
* byte = 8bit, word = 16bit
* _? byte order ?_
* instruction set:
    variable width
    first byte represents the opcode/variant
    registers are represented by 1byte
    immediate values are 2byte
    arithmetics support for signed & unsigned integers

## Instruction set

    NOP (0x00)   1byte              no operation

    ADD (0x01)   5byte (imm, imm)   addition with carry
        (0x02)   4byte (reg, imm)   (?? signed/unsigned)
        (0x03)   2byte (reg, reg)   (?? negative)

    SUB (0x04)   5byte (imm, imm)   subtraction with carry
        (0x05)   4byte (reg, imm)   (?? signed/unsigned)
        (0x06)   4byte (imm, reg)   (?? overflow, negative)
        (0x07)   2byte (reg, reg)   

    MUL (0x08)   5byte (imm, imm)   multiplication
        (0x09)   4byte (reg, imm)   (?? how many bits)
        (0x0A)   2byte (reg, reg)   (?? signed/unsigned, overflow)

    DIV (0x0B)   5byte (imm, imm)   division
        (0x0C)   4byte (imm, reg)   (?? how many bits)
        (0x0D)   4byte (reg, imm)   (?? signed/unsigned, nan)
        (0x0E)   2byte (reg, reg)

    MOD (0x0F)   5byte (imm, imm)   modulo
        (0x10)   4byte (imm, reg)   (?? how many bits)
        (0x11)   4byte (reg, imm)   (?? signed/unsigned, nan)
        (0x12)   2byte (reg, reg)

    NOT (0x13)   3byte (imm)        bitwise not
        (0x14)   2byte (reg)

    AND (0x15)   5byte (imm, imm)   bitwise and
        (0x16)   4byte (reg, imm)
        (0x17)   2byte (reg, reg)

    OR  (0x18)   5byte (imm, imm)   bitwise or
        (0x19)   4byte (reg, imm)
        (0x1A)   2byte (reg, reg)

    XOR (0x1B)   5byte (imm, imm)   bitwise exclusive or
        (0x1C)   4byte (reg, imm)
        (0x1D)   2byte (reg, reg)

    SHL (0x1E)   4byte (reg, imm)   bitwise shift left
        (0x1F)   2byte (reg, reg)

    SHR (0x20)   4byte (reg, imm)   bitwise shift right
        (0x21)   2byte (reg, reg)

    CPR (0x22)   2byte (reg, reg)   copy register value (source, target)



    STR store to register (?? bytes, words)
    STM store to memory
    LDM load from memory


    JMP jump
    CMP compare
    JEQ jump if equal
    JNE jump if not equal
    JGR jump if greater

    (a == b): CMP a b. JEQ $dest.
    (a != b): CMP a b. JNE $dest.
    (a > b) : CMP a b. JGR $dest.
    (a < b) : CMP b a. JGR $dest.
    (a >= b): CMP a b. JGR $dest. JEQ $dest. (or JEQ+JGR)
    (a <= b): CMP b a. JGR $dest. JEQ $dest. (or JEQ+JGR)

