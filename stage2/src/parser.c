/**
 * parser for assisting the shell with reading source listings
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


int chomp(char *const str)
{
    char *c = str;

    if (!str || !*str) {
        return 0;
    }

    while (c[1]) {
        ++c;
    }

    if (*c != '\n') {
        return 0;
    }

    *c = 0;
    return '\n';
}

addr_mode arg_mode(const char *const arg)
{
    if (arg == NULL) {
        return NONE;
    } else if (strncmp(arg, "ANS", OPCODE_SZ_SIZE) == 0) {
        return REG;
    } else {
        return IMM;
    }
}

addr_mode args_mode(const char *const arg1, const char *const arg2)
{
    const addr_mode arg1_mode = arg_mode(arg1);
    const addr_mode arg2_mode = arg_mode(arg2);

    switch (arg1_mode) {
        case NONE:
            switch(arg2_mode) {
                case NONE: return NONE;
                default: break;
            }
        case IMM:
            switch(arg2_mode) {
                case NONE: return IMM;
                case IMM: return IMM_IMM;
                case REG: return IMM_REG;
                default: break;
            }
        case REG:
            switch(arg2_mode) {
                case NONE: return REG;
                case IMM: return REG_IMM;
                case REG: return REG_REG;
                default: break;
            }
        default:
            break;
    }

    printf("unexpected address mode: %s %s\n", arg1, arg2);
    exit(1);
}

void arg_mode_decode_instr(t_oparg *const argp, t_oparg **const regp,
                           char const *const arg, cpu_state *const state)
{
    const addr_mode mode = arg_mode(arg);

    switch (mode) {
        case NONE:
            *argp = 0;
            *regp = NULL;
            return;
        case IMM:
            *argp = (t_oparg) atoi(arg);
            *regp = NULL;
            return;
        case REG:
            *argp = 0;
            *regp = &state->ans;
            return;
        default:
            printf("vcpu fatal error: decoding illegal mode: %i\n", mode);
            exit(1);
    }
}

t_opcode find_opcode(const char *const opcode_str,
                     const char *const arg1, const char *const arg2)
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

    printf("vcpu fatal error: unknown instruction %s[mode:%i]\n",
        opcode_str, mode);
    exit(1);
}

void opcode(cpu_instr *const instr, const char *const opcode_str,
            const char *const arg1, const char *const arg2,
            cpu_state *const state)
{
    const t_opcode opcode = find_opcode(opcode_str, arg1, arg2);

    instr->opcode = opcode;
    instr->next = NULL;

    arg_mode_decode_instr(&instr->arg1, &instr->reg1, arg1, state);
    arg_mode_decode_instr(&instr->arg2, &instr->reg2, arg2, state);
}

//const char *parse_opcode_str(const t_opcode opcode)
//{
//    if (opcode >= OPCODE_COUNT) {
//        printf("vcpu fatal error: illegal opcode %i\n", opcode);
//        exit(1);
//    }
//
//    return OP_TABLE[opcode].code_str;
//}

cpu_instr *parse_line_malloc(char *const line, cpu_state *const state)
{
    char *tk_opcode;
    char *tk_arg1;
    char *tk_arg2;
    int i;
    cpu_instr *instr;

    tk_opcode = strtok(line, " ");
    tk_arg1 = strtok(NULL, " ");
    tk_arg2 = strtok(NULL, " ");

    if (tk_opcode == NULL) {
        printf("error parsing line: %s\n", line);
        exit(1);
    }

    for (i = 0; tk_opcode[i] != '\0'; i++) {
        tk_opcode[i] = (char) toupper(tk_opcode[i]);
    }
    for (i = 0; tk_arg1 != NULL && tk_arg1[i] != '\0'; i++) {
        tk_arg1[i] = (char) toupper(tk_arg1[i]);
    }
    for (i = 0; tk_arg2 != NULL && tk_arg2[i] != '\0'; i++) {
        tk_arg2[i] = (char) toupper(tk_arg2[i]);
    }

    instr = malloc(sizeof(cpu_instr));

    opcode(instr, tk_opcode, tk_arg1, tk_arg2, state);

    return instr;
}

void parse_file_malloc(FILE *const fp, cpu_state *const state)
{
    char *line;
    size_t len;
    int line_nr;
    cpu_instr *instr, *prev;

    line = NULL;
    prev = NULL;
    line_nr = 1;
    while (getline(&line, &len, fp) != -1) {
        chomp(line);
        printf("%i: %s\n", line_nr, line);

        instr = parse_line_malloc(line, state);
        if (state->instr == NULL) {
            state->instr = instr;
            state->instrp = instr;
        }

        if (prev != NULL) {
            prev->next = instr;
        }
        prev = instr;

        line_nr++;
    }

    if (ferror(fp)) {
        printf("error while reading\n");
        exit(1);
    }

    free(line);
    fclose(fp);
}
