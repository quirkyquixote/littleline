
#ifndef COMMANDLINE_HISTORY_H_
#define COMMANDLINE_HISTORY_H_

#include <string.h>
#include <stdlib.h>

/* Fixed-size circular list to store text strings */
struct cl_history {
    /* All strings stored here */
    char **data;
    /* Total capacity */
    size_t allocated;
    /* Number of strings currently held */
    size_t size;
    /* Index of the element after the last one */
    size_t end;
};

/* Initialize */
void cl_history_init(struct cl_history *hist, size_t max_lines);
/* Deinitialize */
void cl_history_deinit(struct cl_history *hist);
/* Push a copy of line into the list; if size reaches allocated, it will push
 * out the oldest stored string. */
void cl_history_push(struct cl_history *hist, const char *line);
/* Return the string whose index is given, counting from the oldest one still
 * stored in the list. */
const char *cl_history_index(struct cl_history *hist, size_t index);

#endif // COMMANDLINE_HISTORY_H_
