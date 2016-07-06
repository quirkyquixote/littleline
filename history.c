
#include "history.h"

#include "buffer.h"

void cli_history_init(struct cli_history *hist, size_t allocated)
{
    hist->data = calloc(allocated, sizeof(*hist->data));
    hist->allocated = allocated;
    hist->size = 0;
    hist->end = 0;
    hist->file = NULL;
}

int cli_history_init_with_file(struct cli_history *hist, size_t max_lines,
        const char *path)
{
    cli_history_init(hist, max_lines);
    FILE *file = fopen(path, "a+");
    if (file == NULL) {
        perror(path);
        return -1;
    }
    setvbuf(file, NULL, _IONBF, 0);
    fseek(file, SEEK_SET, 0);
    int ch;
    struct cli_buf buf;
    cli_buf_init(&buf);
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            cli_history_push(hist, buf.str);
            cli_buf_assign(&buf, "", 0);
        } else {
            cli_buf_append_char(&buf, ch);
        }
    }
    cli_buf_deinit(&buf);
    hist->file = file;
    return 0;
}

void cli_history_deinit(struct cli_history *hist)
{
    if (hist->file)
        fclose(hist->file);
    free(hist->data);
}

void cli_history_push(struct cli_history *hist, const char *line)
{
    if (hist->file)
        fprintf(hist->file, "%s\n", line);
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

