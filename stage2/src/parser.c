/**
 * parser for assisting the shell with reading source listings
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


int chomp(char *str)
{
    if (!str || !*str) {
        return 0;
    }

    while (str[1]) {
        ++str;
    }

    if (*str!='\n') {
        return 0;
    }

    *str = 0;
    return '\n';
}

void parse_file_malloc(FILE *fp, cpu_state *state)
{
    char *line;
    size_t len;
    int line_nr;
    cpu_instr *instr;

    line = NULL;
    instr = NULL;
    line_nr = 1;
    while (getline(&line, &len, fp) != -1) {
        chomp(line);
        printf("%d: %s\n", line_nr, line);

        instr = parse_line_malloc(line, state, instr);
        if (state->instr == NULL) {
            state->instr = instr;
            state->instrp = instr;
        }

        line_nr++;
    }

    if (ferror(fp)) {
        printf("error while reading\n");
        exit(1);
    }

    free(line);
    fclose(fp);
}

cpu_instr *parse_line_malloc(char *line, cpu_state *state, cpu_instr *prev)
{
    char *tk_opcode;
    char *tk_arg1;
    char *tk_arg2;
    int i;
    cpu_instr *instr;

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
    for (i = 0; tk_arg1[i] != '\0'; i++) {
        tk_arg1[i] = (char) toupper(tk_arg1[i]);
    }
    for (i = 0; tk_arg2[i] != '\0'; i++) {
        tk_arg2[i] = (char) toupper(tk_arg2[i]);
    }

    instr = malloc(sizeof(cpu_instr));

    cpu_opcode(instr, tk_opcode, tk_arg1, tk_arg2, state);

    if (prev != NULL) {
        prev->next = instr;
    }
    
    return instr;
}
