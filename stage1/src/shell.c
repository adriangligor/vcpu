/**
 * shell for hosting, controlling and introspecting the vcpu
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "parser.h"


/**
 * print the ans register of the vcpu
 */
void shell_print_state(const cpu_state *state)
{
    printf("ans=%d\n", state->ans);
}

/**
 * initializes the vcpu state and starts execution
 */
int main(const int argc, const char *argv[])
{
    if (argc < 2) {
        printf("usage: %s file.lst\n", argv[0]);
        exit(1);
    }

    cpu_state state;
    cpu_reset(&state);

    parse_instr(argv[1], &state.instr);

    cpu_exec(&state);

    shell_print_state(&state);
    return(0);
}
