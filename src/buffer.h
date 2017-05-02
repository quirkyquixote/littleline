/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */
/**
 * @file
 * Define ll_buf struct and related functions
 */

#ifndef LITTLELINE_BUFFER_H_
#define LITTLELINE_BUFFER_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * Number of characters added every time a buffer has to grow
 * @memberof ll_buf
 */
#define LL_BUF_BUCKET_SIZE 64

/**
 * A helper to build text strings 
 * @ingroup LittleLine
 */
struct ll_buf {
	/** The string being built */
	char *str;
	/** Number of characters currently allocated */
	size_t allocated;
	/** Number of characters currently used */
	size_t len;
};

/**
 * Initialize buffer 
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 */
void ll_buf_init(struct ll_buf *buf);
/**
 * Destroy buffer
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 */
void ll_buf_deinit(struct ll_buf *buf);
/**
 * Replace contents
 *
 * Replaces the contents of _buf_ with _len_ characters from _str_
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param str a string that will be copied to _buf_
 * @param len number of characters to copy
 */
void ll_buf_assign(struct ll_buf *buf, const void *str, size_t len);
/**
 * Insert characters
 *
 * Insert _len_ characters of _str_ at position _where_ of _buf_
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param where first character in _buf_ that will be moved
 * @param str a string that will be copied to _buf_
 * @param len number of characters to copy
 */
void ll_buf_insert(struct ll_buf *buf, size_t where, const void *str, size_t len);
/**
 * Append characters
 *
 * Same as ll_buf_insert(_buf_, _buf->len_, _str_, _len_)
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param str a string that will be copied to _buf_
 * @param len number of characters to copy
 */
void ll_buf_append(struct ll_buf *buf, const void *str, size_t len);
/**
 * Prepend characters
 *
 * Same as ll_buf_insert(_buf_, 0, _str_, _len_)
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param str a string that will be copied to _buf_
 * @param len number of characters to copy
 */
void ll_buf_prepend(struct ll_buf *buf, const void *str, size_t len);
/**
 * Replace characters
 *
 * Replace _len_ characters of _buf_ starting at _where_ with the ones from _str_
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param where first character in _buf_ that will be replaced
 * @param str a string that will be copied to _buf_
 * @param len number of characters to copy
 */
void ll_buf_replace(struct ll_buf *buf, size_t where, const void *str, size_t len);
/**
 * Erase characters
 *
 * Erase _len_ characters of _buf_ starting at _where_
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param where first character in _buf_ that will be erased
 * @param len number of characters to erase
 */
void ll_buf_erase(struct ll_buf *buf, size_t where, size_t len);
/**
 * Insert character
 *
 * Same as ll_buf_insert(_buf_, _where_, &_c_, 1)
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param where first character in _buf_ that will be moved
 * @param c character to insert
 */
void ll_buf_insert_char(struct ll_buf *buf, size_t where, char c);
/**
 * Append character
 *
 * Same as ll_buf_append(_buf_, &_c_, 1)
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param c character to append
 */
void ll_buf_append_char(struct ll_buf *buf, char c);
/**
 * Prepend character
 *
 * Same as ll_buf_prepend(_buf_, &_c_, 1)
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param c character to prepend
 */
void ll_buf_prepend_char(struct ll_buf *buf, char c);
/**
 * Replace character
 *
 * Same as ll_buf_replace(_buf_, _where_, &_c_, 1)
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param where location of the character to replace
 * @param c character to replace
 */
void ll_buf_replace_char(struct ll_buf *buf, size_t where, char c);
/**
 * Erase character
 *
 * Same as ll_buf_erase(_buf_, _where_, 1)
 *
 * @memberof ll_buf
 * @param buf a ll_buf
 * @param where location of the character to erase
 */
void ll_buf_erase_char(struct ll_buf *buf, size_t where);

#endif

