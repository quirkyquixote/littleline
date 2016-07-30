
#include "history.h"

#include <string.h>
#include <stdio.h>

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

void ll_history_clear(struct ll_history *hist)
{
	int i;

	for (i = 0; i < hist->allocated; ++i) {
		if (hist->data[i] != NULL) {
			free(hist->data[i]);
			hist->data[i] = NULL;
		}
	}
	hist->size = 0;
	hist->end = 0;
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

int ll_history_read(struct ll_history *hist, const char *path)
{
	FILE *f;
	int c;
	struct ll_buf buf;

	f = fopen(path, "r");
	if (f == NULL) {
		perror(path);
		return -1;
	}
	ll_history_clear(hist);
	ll_buf_init(&buf);
	while ((c = fgetc(f)) != EOF) {
		if (c == '\n') {
			if (buf.len > 0) {
				ll_history_push(hist, buf.str);
				ll_buf_assign(&buf, "", 0);
			}
		} else {
			ll_buf_append_char(&buf, c);
		}
	}
	ll_buf_deinit(&buf);
	fclose(f);
	return 0;
}

int ll_history_write(struct ll_history *hist, const char *path)
{
	FILE *f;
	int i;

	f = fopen(path, "w");
	if (f == NULL) {
		perror(path);
		return -1;
	}
	for (i = 0; i < hist->size; ++i)
		fprintf(f, "%s\n", ll_history_index(hist, i));
	fclose(f);
	return 0;
}

