
#include "buffer.h"

cli_buf *cli_buf_new(void)
{
    cli_buf *buf = malloc(sizeof(*buf));
    buf->str = calloc(CLI_BUF_BUCKET_SIZE, 1);
    buf->allocated = CLI_BUF_BUCKET_SIZE;
    buf->len = 0;
    return buf;
}

void cli_buf_free(cli_buf *buf)
{
    if (buf->str)
        free(buf->str);
    free(buf);
}

void cli_buf_grow(cli_buf *buf, size_t len)
{
    if (buf->allocated < len) {
        do
            buf->allocated += CLI_BUF_BUCKET_SIZE;
        while(buf->allocated < len);
        buf->str = realloc(buf->str, buf->allocated);
    }
}

void cli_buf_assign(cli_buf *buf, const void *str, size_t len)
{
    cli_buf_grow(buf, len + 1);
    memcpy(buf->str, str, len + 1);
    buf->len = len;
}

void cli_buf_insert(cli_buf *buf, size_t where, const void *str, size_t len)
{
    assert(buf->str);
    assert(where <= buf->len);
    cli_buf_grow(buf, buf->len + len + 1);
    memmove(buf->str + (where + len),
            buf->str + where,
            buf->len - where + 1);
    memcpy(buf->str + where, str, len);
    buf->len += len;
}

void cli_buf_append(cli_buf *buf, const void *str, size_t len)
{
    cli_buf_insert(buf, buf->len, str, len);
}

void cli_buf_prepend(cli_buf *buf, const void *str, size_t len)
{
    cli_buf_insert(buf, 0, str, len);
}

void cli_buf_replace(cli_buf *buf, size_t where, const void *str, size_t len)
{
    assert(buf->str);
    assert(where + len <= buf->len);
    memcpy(buf->str + where, str, len);
}

void cli_buf_erase(cli_buf *buf, size_t where, size_t len)
{
    assert(buf->str);
    assert(where + len <= buf->len);
    memmove(buf->str + where,
            buf->str + (where + len),
            buf->len - where + len + 1);
    buf->len -= len;
}

void cli_buf_insert_char(cli_buf *buf, size_t where, char c)
{
    cli_buf_insert(buf, where, &c, 1);
}

void cli_buf_append_char(cli_buf *buf, char c)
{
    cli_buf_append(buf, &c, 1);
}

void cli_buf_prepend_char(cli_buf *buf, char c)
{
    cli_buf_prepend(buf, &c, 1);
}

void cli_buf_replace_char(cli_buf *buf, size_t where, char c)
{
    cli_buf_replace(buf, where, &c, 1);
}

void cli_buf_erase_char(cli_buf *buf, size_t where)
{
    cli_buf_erase(buf, where, 1);
}


