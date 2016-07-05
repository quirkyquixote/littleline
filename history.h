
#ifndef COMMANDLINE_HISTORY_H_
#define COMMANDLINE_HISTORY_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _cli_history {
    char **data;
    size_t allocated;
    size_t size;
    size_t end;
} cli_history;

cli_history *cli_history_new(size_t allocated);

void cli_history_free(cli_history *hist);

void cli_history_push(cli_history *hist, const char *line);

const char *cli_history_index(cli_history *hist, size_t index);

#endif // COMMANDLINE_HISTORY_H_
