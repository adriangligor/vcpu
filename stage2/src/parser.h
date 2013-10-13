/**
 * definition of functions for the source listing parser
 */

#ifndef VCPU_PARSER_H
#define VCPU_PARSER_H


#include "cpu.h"


/**
 * parses an instruction list from the given file into the state
 */
void parse_file_malloc(FILE *const fp, cpu_state *const state);


#endif
