
#include "buffer.h"

void cl_buf_init(struct cl_buf *buf)
{
    buf->str = calloc(CL_BUF_BUCKET_SIZE, 1);
    buf->allocated = CL_BUF_BUCKET_SIZE;
    buf->len = 0;
}

void cl_buf_deinit(struct cl_buf *buf)
{
    free(buf->str);
}

void cl_buf_grow(struct cl_buf *buf, size_t len)
{
    if (buf->allocated < len) {
        do
            buf->allocated += CL_BUF_BUCKET_SIZE;
        while(buf->allocated < len);
        buf->str = realloc(buf->str, buf->allocated);
    }
}

void cl_buf_assign(struct cl_buf *buf, const void *str, size_t len)
{
    cl_buf_grow(buf, len + 1);
    memcpy(buf->str, str, len + 1);
    buf->len = len;
}

void cl_buf_insert(struct cl_buf *buf, size_t where, const void *str, size_t len)
{
    assert(where <= buf->len);
    cl_buf_grow(buf, buf->len + len + 1);
    memmove(buf->str + (where + len), buf->str + where, buf->len - where + 1);
    memcpy(buf->str + where, str, len);
    buf->len += len;
}

void cl_buf_append(struct cl_buf *buf, const void *str, size_t len)
{
    cl_buf_insert(buf, buf->len, str, len);
}

void cl_buf_prepend(struct cl_buf *buf, const void *str, size_t len)
{
    cl_buf_insert(buf, 0, str, len);
}

void cl_buf_replace(struct cl_buf *buf, size_t where, const void *str, size_t len)
{
    assert(where + len <= buf->len);
    memcpy(buf->str + where, str, len);
}

void cl_buf_erase(struct cl_buf *buf, size_t where, size_t len)
{
    assert(where + len <= buf->len);
    memmove(buf->str + where, buf->str + (where + len), buf->len - where + len + 1);
    buf->len -= len;
}

void cl_buf_insert_char(struct cl_buf *buf, size_t where, char c)
{
    cl_buf_insert(buf, where, &c, 1);
}

void cl_buf_append_char(struct cl_buf *buf, char c)
{
    cl_buf_append(buf, &c, 1);
}

void cl_buf_prepend_char(struct cl_buf *buf, char c)
{
    cl_buf_prepend(buf, &c, 1);
}

void cl_buf_replace_char(struct cl_buf *buf, size_t where, char c)
{
    cl_buf_replace(buf, where, &c, 1);
}

void cl_buf_erase_char(struct cl_buf *buf, size_t where)
{
    cl_buf_erase(buf, where, 1);
}


