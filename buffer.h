

#ifndef LITTLELINE_BUFFER_H_
#define LITTLELINE_BUFFER_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Number of bytes added every time a buffer has to grow */
#define LL_BUF_BUCKET_SIZE 64

/* A helpter to build text strings */
struct ll_buf {
    /* The string being built */
    char *str;
    /* Number of bytes currently allocated */
    size_t allocated;
    /* Number of bytes currently used */
    size_t len;
};

/* Initialize */
void ll_buf_init(struct ll_buf *buf);
/* Deinitialize */
void ll_buf_deinit(struct ll_buf *buf);
/* Replace the contents of buf with len bytes from str */
void ll_buf_assign(struct ll_buf *buf, const void *str, size_t len);
/* Insert len bytes of str at position where of buf */
void ll_buf_insert(struct ll_buf *buf, size_t where, const void *str, size_t len);
/* Same as insert(buf, buf->len, str, len) */
void ll_buf_append(struct ll_buf *buf, const void *str, size_t len);
/* Same as insert(buf, 0, str, len) */
void ll_buf_prepend(struct ll_buf *buf, const void *str, size_t len);
/* Replace len bytes of buf starting at where with the ones from str */
void ll_buf_replace(struct ll_buf *buf, size_t where, const void *str, size_t len);
/* Erase len bytes of buf starting at where */
void ll_buf_erase(struct ll_buf *buf, size_t where, size_t len);
/* Same as insert(buf, where, &c, 1) */
void ll_buf_insert_char(struct ll_buf *buf, size_t where, char c);
/* Same as append(buf, &c, 1) */
void ll_buf_append_char(struct ll_buf *buf, char c);
/* Same as prepend(buf, &c, 1) */
void ll_buf_prepend_char(struct ll_buf *buf, char c);
/* Same as replace(buf, where, &c, 1) */
void ll_buf_replace_char(struct ll_buf *buf, size_t where, char c);
/* Same as erase(buf, where, 1) */
void ll_buf_erase_char(struct ll_buf *buf, size_t where);

#endif

