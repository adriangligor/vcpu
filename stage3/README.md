# Virtual CPU (vcpu)

## Stage 1

At this stage, vcpu is able to read one of 5 instructions from a listing and execute it. The listing is a plain text file containing vcpu assembler instructions. The implemented instructions are:

    NOP x y   no operation (ignores both arguments)
    ADD x y   addition
    SUB x y   subtraction
    MUL x y   multiplication
    DIV x y   division

Each instruction operates on two constant values. The result of each operation is written to the special **ANS** register. At this stage, there is no binary representation of the instruction set. The vcpu is just an interpreter.

## Compilation

    make clean shell

Generates the `shell` binary used to execute listings.

## Usage

    ./shell -

Read instructions from stdin and execute them. The shell then prints the content of the ANS register after each successful execution.

    ./shell lst/calc1.lst

Read the listing (really just the first instruction) and execute it. The shell then prints the content of the ANS register before exitting.

## Test

    ./test.py lst/*.lst

Execute each of the specified tests. A test consists of a _pair of files_, a _.lst file_ containing the source code, and a _.ans file_ containing the expected value of the ANS register after the listing has executed.
