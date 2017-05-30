/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef LITTLELINE_BUFFER_H_
#define LITTLELINE_BUFFER_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * Buffer
 * ------
 *
 * A buffer is an auxiliary object to build and edit text strings. The library
 * handles most strings as raw ``char`` pointers, and only a few of them are in
 * a place where they have to be modified.
 */

/**
 * Number of characters added every time a buffer has to grow
 */
#define LL_BUF_BUCKET_SIZE 64

/**
 * A helper to build text strings 
 */
struct ll_buf {
	/* The string being built */
	char *str;
	/* Number of characters currently allocated */
	size_t allocated;
	/* Number of characters currently used */
	size_t len;
};

/**
 * Initialize buffer 
 */
void ll_buf_init(struct ll_buf *buf);
/**
 * Destroy buffer
 */
void ll_buf_deinit(struct ll_buf *buf);
/**
 * Replaces the contents of ``buf`` with ``len`` characters from ``str``
 */
void ll_buf_assign(struct ll_buf *buf, const void *str, size_t len);
/**
 * Insert ``len`` characters of ``str`` at position ``where`` of ``buf``
 */
void ll_buf_insert(struct ll_buf *buf, size_t where, const void *str, size_t len);
/**
 * Append characters; same as ``ll_buf_insert(buf, buf->len, str, len)``
 */
void ll_buf_append(struct ll_buf *buf, const void *str, size_t len);
/**
 * Prepend characters; Same as ``ll_buf_insert(buf, 0, str, len)``
 */
void ll_buf_prepend(struct ll_buf *buf, const void *str, size_t len);
/**
 * Replace ``len`` characters of ``buf`` starting at ``where`` with the ones
 * from ``str``
 */
void ll_buf_replace(struct ll_buf *buf, size_t where, const void *str, size_t len);
/**
 * Erase ``len`` characters of ``buf`` starting at ``where``
 */
void ll_buf_erase(struct ll_buf *buf, size_t where, size_t len);
/**
 * Insert character; same as ``ll_buf_insert(buf, where, &c, 1)``
 */
void ll_buf_insert_char(struct ll_buf *buf, size_t where, char c);
/**
 * Append character; same as ``ll_buf_append(buf, &c, 1)``
 */
void ll_buf_append_char(struct ll_buf *buf, char c);
/**
 * Prepend character; same as ``ll_buf_prepend(buf, &c, 1)``
 */
void ll_buf_prepend_char(struct ll_buf *buf, char c);
/**
 * Replace character; same as ``ll_buf_replace(buf, where, &c, 1)``
 */
void ll_buf_replace_char(struct ll_buf *buf, size_t where, char c);
/**
 * Erase character; same as ``ll_buf_erase(buf, where, 1)``
 */
void ll_buf_erase_char(struct ll_buf *buf, size_t where);

#endif

