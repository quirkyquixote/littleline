
#include "buffer.h"

void cli_buf_init(struct cli_buf *buf)
{
    buf->str = calloc(CLI_BUF_BUCKET_SIZE, 1);
    buf->allocated = CLI_BUF_BUCKET_SIZE;
    buf->len = 0;
}

void cli_buf_deinit(struct cli_buf *buf)
{
    if (buf->str)
        free(buf->str);
}

void cli_buf_grow(struct cli_buf *buf, size_t len)
{
    if (buf->allocated < len) {
        do
            buf->allocated += CLI_BUF_BUCKET_SIZE;
        while(buf->allocated < len);
        buf->str = realloc(buf->str, buf->allocated);
    }
}

void cli_buf_assign(struct cli_buf *buf, const void *str, size_t len)
{
    cli_buf_grow(buf, len + 1);
    memcpy(buf->str, str, len + 1);
    buf->len = len;
}

void cli_buf_insert(struct cli_buf *buf, size_t where, const void *str, size_t len)
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

void cli_buf_append(struct cli_buf *buf, const void *str, size_t len)
{
    cli_buf_insert(buf, buf->len, str, len);
}

void cli_buf_prepend(struct cli_buf *buf, const void *str, size_t len)
{
    cli_buf_insert(buf, 0, str, len);
}

void cli_buf_replace(struct cli_buf *buf, size_t where, const void *str, size_t len)
{
    assert(buf->str);
    assert(where + len <= buf->len);
    memcpy(buf->str + where, str, len);
}

void cli_buf_erase(struct cli_buf *buf, size_t where, size_t len)
{
    assert(buf->str);
    assert(where + len <= buf->len);
    memmove(buf->str + where,
            buf->str + (where + len),
            buf->len - where + len + 1);
    buf->len -= len;
}

void cli_buf_insert_char(struct cli_buf *buf, size_t where, char c)
{
    cli_buf_insert(buf, where, &c, 1);
}

void cli_buf_append_char(struct cli_buf *buf, char c)
{
    cli_buf_append(buf, &c, 1);
}

void cli_buf_prepend_char(struct cli_buf *buf, char c)
{
    cli_buf_prepend(buf, &c, 1);
}

void cli_buf_replace_char(struct cli_buf *buf, size_t where, char c)
{
    cli_buf_replace(buf, where, &c, 1);
}

void cli_buf_erase_char(struct cli_buf *buf, size_t where)
{
    cli_buf_erase(buf, where, 1);
}


