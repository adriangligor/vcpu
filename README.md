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
        (0x08)   4byte (reg, imm)   (?? how many bits)
        (0x09)   2byte (reg, reg)   (?? signed/unsigned, overflow)
    
    DIV (0x09)   division                   ?
    MOD (0x0A)   modulo                     ?
    
    AND and
    OR  or
    XOR exclusive or
    NOT not
    
    SHL shift left
    SHR shift right
    
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
    
    STR store to register (?? bytes, words)
    CPR copy register
    STM store to memory
    LDM load from memory

