
#include "history.h"

#include <string.h>

#include "buffer.h"

void ll_history_init(struct ll_history *hist, size_t allocated)
{
    hist->data = calloc(allocated, sizeof(*hist->data));
    hist->allocated = allocated;
    hist->size = 0;
    hist->end = 0;
}

void ll_history_deinit(struct ll_history *hist)
{
    free(hist->data);
}

void ll_history_push(struct ll_history *hist, const char *line)
{
    const char *ptr;

    for (ptr = line; *ptr && isspace(*ptr); ++ptr)
        continue;
    if ((strlen(ptr) == 0) || ((hist->size > 0)
         && (strcmp(ll_history_index(hist, hist->size - 1), line) == 0)))
        return;
    if (hist->size == hist->allocated)
        free(hist->data[hist->end]);
    else
        ++hist->size;
    hist->data[hist->end] = malloc(strlen(line) + 1);
    strcpy(hist->data[hist->end], line);
    ++hist->end;
    if (hist->end == hist->allocated)
        hist->end = 0;
}

const char *ll_history_index(struct ll_history *hist, size_t index)
{
    if (hist->size < hist->allocated)
        return hist->data[index];
    return hist->data[(hist->end + index) % hist->allocated];
}

