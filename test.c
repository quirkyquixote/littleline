
#include <stdio.h>
#include <stdlib.h>

#include "commandline.h"

int main(int arc, char *argv[])
{
    cli_set_key_bindings(CLI_ANSI_KEY_BINDINGS);
    cli_set_history(10);
    cli_set_history_file("history.txt");

    const char *line;
    while (line = cli_read(">>")) {
        if (strlen(line) > 0)
            puts(line);
    }

    exit(EXIT_SUCCESS);
}
