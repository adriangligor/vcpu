/**
 * parser for assisting the shell with reading source listings
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void parse_instr(FILE *fp, cpu_instr *instr)
{
    char *line = NULL;
    size_t len;

    if (getline(&line, &len, fp) != -1) {
        printf("> %s", line);
        parse_instr_line(line, instr);
    }

    if (ferror(fp)) {
        printf("error while reading\n");
        exit(1);
    }

    free(line);
    fclose(fp);
}

void parse_instr_line(char *line, cpu_instr *instr)
{
    int i;
    char *tk_opcode;
    char *tk_arg1;
    char *tk_arg2;

    tk_opcode = strtok(line, " ");
    tk_arg1 = strtok(NULL, " ");
    tk_arg2 = strtok(NULL, " ");

    if (tk_opcode == NULL || tk_arg1 == NULL || tk_arg2 == NULL) {
        printf("error parsing line: %s\n", line);
        exit(1);
    }

    for (i = 0; tk_opcode[i] != '\0'; i++) {
        tk_opcode[i] = (char) toupper(tk_opcode[i]);
    }

    instr->opcode = cpu_opcode(tk_opcode);
    instr->arg1 = (t_oparg) atoi(tk_arg1);
    instr->arg2 = (t_oparg) atoi(tk_arg2);
}
