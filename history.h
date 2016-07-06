
#ifndef COMMANDLINE_HISTORY_H_
#define COMMANDLINE_HISTORY_H_

#include <string.h>
#include <stdlib.h>

struct cl_history {
    char **data;
    size_t allocated;
    size_t size;
    size_t end;
};

void cl_history_init(struct cl_history *hist, size_t max_lines);

void cl_history_deinit(struct cl_history *hist);

void cl_history_push(struct cl_history *hist, const char *line);

const char *cl_history_index(struct cl_history *hist, size_t index);

#endif // COMMANDLINE_HISTORY_H_
