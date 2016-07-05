
#include "history.h"

cli_history *cli_history_new(size_t allocated)
{
    cli_history *hist = malloc(sizeof(*hist));
    hist->data = calloc(allocated, sizeof(*hist->data));
    hist->allocated = allocated;
    hist->size = 0;
    hist->end = 0;
    return hist;
}

void cli_history_free(cli_history *hist)
{
    free(hist->data);
    free(hist);
}

void cli_history_push(cli_history *hist, const char *line)
{
    if (hist->size == hist->allocated)
        free(hist->data[hist->end]);
    else
        ++hist->size;
    hist->data[hist->end] = strdup(line);
    if (hist->end == hist->allocated)
        hist->end = 0;
    else
        ++hist->end;
}

const char *cli_history_index(cli_history *hist, size_t index)
{
    if (hist->size < hist->allocated)
        return hist->data[index];
    return hist->data[(hist->end + index) % hist->allocated];
}

