/**
 * shell for hosting, controlling and introspecting the vcpu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
 * execute a line from a command listing
 */
void shell_file(const char *filename)
{
    cpu_state state;
    cpu_reset(&state);
    
    parse_instr(filename, &state.instr);
    cpu_exec(&state);
    shell_print_state(&state);

    return;
}

/**
 * execute a line from stdin
 */
void shell_stdin()
{
    cpu_state state;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != -1) {
        cpu_reset(&state);

        parse_instr_line(line, &state.instr);
        cpu_exec(&state);
        shell_print_state(&state);
    }

    return;
}

/**
 * initializes the vcpu state and starts execution
 */
int main(const int argc, const char *argv[])
{
    const char *filename;

    setvbuf(stdout, NULL, _IONBF, 0); // enable auto-flush of stdout

    if (argc < 2) {
        printf("usage: %s -\n", argv[0]);
        printf("       %s file.lst\n", argv[0]);
        exit(1);
    }

    filename = argv[1];
    if (strncmp(filename, "-", 2) == 0) {
        shell_stdin();
    } else {
        shell_file(filename);
    }

    return(0);
}
