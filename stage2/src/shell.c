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
 * execute the content of a command listing or stdin
 */
void shell(const char *filename)
{
    FILE *fp;
    cpu_state state;

    if (strncmp(filename, "-", 2) == 0) {
        fp = stdin;
    } else {
        fp = fopen(filename, "r");
    }

    if (fp != NULL) {
        printf("reading from file: %s\n", filename);
    } else {
        printf("file not found: %s\n", filename);
        exit(1);
    }

    cpu_reset_free(&state);

    parse_file_malloc(fp, &state);
    cpu_exec(&state);
    shell_print_state(&state);
    cpu_reset_free(&state);

    return;
}

/**
 * initializes the vcpu state and starts execution
 */
int main(const int argc, const char *argv[])
{
    const char *filename = argv[1];

    setvbuf(stdout, NULL, _IONBF, 0); // enable auto-flush of stdout

    if (argc < 2) {
        printf("usage: %s -\n", argv[0]);
        printf("       %s file.lst\n", argv[0]);
        exit(1);
    }

    shell(filename);

    return(0);
}
