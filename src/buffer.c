/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#include "buffer.h"

void ll_buf_init(struct ll_buf *buf)
{
	buf->str = calloc(LL_BUF_BUCKET_SIZE, 1);
	buf->allocated = LL_BUF_BUCKET_SIZE;
	buf->len = 0;
}

void ll_buf_deinit(struct ll_buf *buf)
{
	free(buf->str);
}

void ll_buf_grow(struct ll_buf *buf, size_t len)
{
	if (buf->allocated < len) {
		do
			buf->allocated += LL_BUF_BUCKET_SIZE;
		while(buf->allocated < len);
		buf->str = realloc(buf->str, buf->allocated);
	}
}

void ll_buf_assign(struct ll_buf *buf, const void *str, size_t len)
{
	ll_buf_grow(buf, len + 1);
	memcpy(buf->str, str, len + 1);
	buf->len = len;
}

void ll_buf_insert(struct ll_buf *buf, size_t where, const void *str, size_t len)
{
	assert(where <= buf->len);
	ll_buf_grow(buf, buf->len + len + 1);
	memmove(buf->str + (where + len), buf->str + where, buf->len - where + 1);
	memcpy(buf->str + where, str, len);
	buf->len += len;
}

void ll_buf_append(struct ll_buf *buf, const void *str, size_t len)
{
	ll_buf_insert(buf, buf->len, str, len);
}

void ll_buf_prepend(struct ll_buf *buf, const void *str, size_t len)
{
	ll_buf_insert(buf, 0, str, len);
}

void ll_buf_replace(struct ll_buf *buf, size_t where, const void *str, size_t len)
{
	assert(where + len <= buf->len);
	memcpy(buf->str + where, str, len);
}

void ll_buf_erase(struct ll_buf *buf, size_t where, size_t len)
{
	assert(where + len <= buf->len);
	memmove(buf->str + where, buf->str + (where + len), buf->len - where + len + 1);
	buf->len -= len;
}

void ll_buf_insert_char(struct ll_buf *buf, size_t where, char c)
{
	ll_buf_insert(buf, where, &c, 1);
}

void ll_buf_append_char(struct ll_buf *buf, char c)
{
	ll_buf_append(buf, &c, 1);
}

void ll_buf_prepend_char(struct ll_buf *buf, char c)
{
	ll_buf_prepend(buf, &c, 1);
}

void ll_buf_replace_char(struct ll_buf *buf, size_t where, char c)
{
	ll_buf_replace(buf, where, &c, 1);
}

void ll_buf_erase_char(struct ll_buf *buf, size_t where)
{
	ll_buf_erase(buf, where, 1);
}


