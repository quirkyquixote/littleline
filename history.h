
#ifndef COMMANDLINE_HISTORY_H_
#define COMMANDLINE_HISTORY_H_

#include <string.h>
#include <stdlib.h>

struct cli_history {
    char **data;
    size_t allocated;
    size_t size;
    size_t end;
};

void cli_history_init(struct cli_history *hist, size_t max_lines);

void cli_history_deinit(struct cli_history *hist);

void cli_history_push(struct cli_history *hist, const char *line);

const char *cli_history_index(struct cli_history *hist, size_t index);

#endif // COMMANDLINE_HISTORY_H_
