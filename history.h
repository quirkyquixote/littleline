
#ifndef LITTLELINE_HISTORY_H_
#define LITTLELINE_HISTORY_H_

#include <string.h>
#include <stdlib.h>

/* Fixed-size circular list to store text strings */
struct ll_history {
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
void ll_history_init(struct ll_history *hist, size_t max_lines);
/* Deinitialize */
void ll_history_deinit(struct ll_history *hist);
/* Remove all elements from the history */
void ll_history_clear(struct ll_history *hist);
/* Push a copy of line into the list; if size reaches allocated, it will push
 * out the oldest stored string. */
void ll_history_push(struct ll_history *hist, const char *line);
/* Return the string whose index is given, counting from the oldest one still
 * stored in the list. */
const char *ll_history_index(struct ll_history *hist, size_t index);
/* Read the history from a file */
int ll_history_read(struct ll_history *hist, const char *path);
/* Save the history to a file */
int ll_history_write(struct ll_history *hist, const char *path);

#endif
