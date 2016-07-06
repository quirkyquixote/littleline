
#include "history.h"

void cli_history_init(struct cli_history *hist, size_t allocated)
{
    hist->data = calloc(allocated, sizeof(*hist->data));
    hist->allocated = allocated;
    hist->size = 0;
    hist->end = 0;
}

void cli_history_free(struct cli_history *hist)
{
    free(hist->data);
}

void cli_history_push(struct cli_history *hist, const char *line)
{
    if (hist->size > 0 && strcmp(cli_history_index(hist, hist->size - 1), line) == 0)
        return;
    if (hist->size == hist->allocated)
        free(hist->data[hist->end]);
    else
        ++hist->size;
    hist->data[hist->end] = strdup(line);
    ++hist->end;
    if (hist->end == hist->allocated)
        hist->end = 0;
}

const char *cli_history_index(struct cli_history *hist, size_t index)
{
    if (hist->size < hist->allocated)
        return hist->data[index];
    return hist->data[(hist->end + index) % hist->allocated];
}

