

#ifndef COMMANDLINE_BUFFER_H_
#define COMMANDLINE_BUFFER_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define CLI_BUF_BUCKET_SIZE 64

typedef struct _cli_buf {
    unsigned char *str;
    size_t allocated;
    size_t len;
} cli_buf;

cli_buf *cli_buf_new(void);

void cli_buf_free(cli_buf *buf);

void cli_buf_grow(cli_buf *buf, size_t len);

void cli_buf_assign(cli_buf *buf, const void *str, size_t len);

void cli_buf_insert(cli_buf *buf, size_t where, const void *str, size_t len);

void cli_buf_append(cli_buf *buf, const void *str, size_t len);

void cli_buf_prepend(cli_buf *buf, const void *str, size_t len);

void cli_buf_replace(cli_buf *buf, size_t where, const void *str, size_t len);

void cli_buf_erase(cli_buf *buf, size_t where, size_t len);

void cli_buf_insert_char(cli_buf *buf, size_t where, char c);

void cli_buf_append_char(cli_buf *buf, char c);

void cli_buf_prepend_char(cli_buf *buf, char c);

void cli_buf_replace_char(cli_buf *buf, size_t where, char c);

void cli_buf_erase_char(cli_buf *buf, size_t where);

#endif // COMMANDLINE_BUFFER_H_

