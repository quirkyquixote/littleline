
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commandline.h"

int main(int arc, char *argv[])
{
    const char *line;

    cl_set_key_bindings(CL_ANSI_KEY_BINDINGS);
    cl_set_history_with_file(10, "history.txt");

    while (line = cl_read(">>")) {
        if (strlen(line) > 0)
            puts(line);
    }

    exit(EXIT_SUCCESS);
}
