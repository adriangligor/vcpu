/**
 * definition of functions for the source listing parser
 */

#ifndef VCPU_PARSER_H
#define VCPU_PARSER_H


#include "cpu.h"


/**
 * parses an instruction list from the given file into the state
 */
void parse_instr_malloc(FILE *fp, cpu_state *state);

/**
 * parses an instruction from the given line
 */
cpu_instr *parse_line_malloc(char *line, cpu_state *state, cpu_instr *prev);


#endif
