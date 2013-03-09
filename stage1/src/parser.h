/**
 * definition of functions for the source listing parser
 */

#ifndef VCPU_PARSER_H
#define VCPU_PARSER_H


#include "cpu.h"


/**
 * parses an instruction from the given file
 */
void parse_instr(const char *filename, cpu_instr *instr);

/**
 * parses an instruction from the given line
 */
void parse_instr_line(char *line, cpu_instr *instr);


#endif
