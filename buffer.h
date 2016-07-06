

#ifndef COMMANDLINE_BUFFER_H_
#define COMMANDLINE_BUFFER_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define CLI_BUF_BUCKET_SIZE 64

struct cli_buf {
    unsigned char *str;
    size_t allocated;
    size_t len;
};

void cli_buf_init(struct cli_buf *buf);

void cli_buf_deinit(struct cli_buf *buf);

void cli_buf_grow(struct cli_buf *buf, size_t len);

void cli_buf_assign(struct cli_buf *buf, const void *str, size_t len);

void cli_buf_insert(struct cli_buf *buf, size_t where, const void *str, size_t len);

void cli_buf_append(struct cli_buf *buf, const void *str, size_t len);

void cli_buf_prepend(struct cli_buf *buf, const void *str, size_t len);

void cli_buf_replace(struct cli_buf *buf, size_t where, const void *str, size_t len);

void cli_buf_erase(struct cli_buf *buf, size_t where, size_t len);

void cli_buf_insert_char(struct cli_buf *buf, size_t where, char c);

void cli_buf_append_char(struct cli_buf *buf, char c);

void cli_buf_prepend_char(struct cli_buf *buf, char c);

void cli_buf_replace_char(struct cli_buf *buf, size_t where, char c);

void cli_buf_erase_char(struct cli_buf *buf, size_t where);

#endif // COMMANDLINE_BUFFER_H_

