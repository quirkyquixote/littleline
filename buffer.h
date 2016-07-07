

#ifndef COMMANDLINE_BUFFER_H_
#define COMMANDLINE_BUFFER_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Number of bytes added every time a buffer has to grow */
#define CL_BUF_BUCKET_SIZE 64

/* A helpter to build text strings */
struct cl_buf {
    /* The string being built */
    unsigned char *str;
    /* Number of bytes currently allocated */
    size_t allocated;
    /* Number of bytes currently used */
    size_t len;
};

/* Initialize */
void cl_buf_init(struct cl_buf *buf);
/* Deinitialize */
void cl_buf_deinit(struct cl_buf *buf);
/* Replace the contents of buf with len bytes from str */
void cl_buf_assign(struct cl_buf *buf, const void *str, size_t len);
/* Insert len bytes of str at position where of buf */
void cl_buf_insert(struct cl_buf *buf, size_t where, const void *str, size_t len);
/* Same as insert(buf, buf->len, str, len) */
void cl_buf_append(struct cl_buf *buf, const void *str, size_t len);
/* Same as insert(buf, 0, str, len) */
void cl_buf_prepend(struct cl_buf *buf, const void *str, size_t len);
/* Replace len bytes of buf starting at where with the ones from str */
void cl_buf_replace(struct cl_buf *buf, size_t where, const void *str, size_t len);
/* Erase len bytes of buf starting at where */
void cl_buf_erase(struct cl_buf *buf, size_t where, size_t len);
/* Same as insert(buf, where, &c, 1) */
void cl_buf_insert_char(struct cl_buf *buf, size_t where, char c);
/* Same as append(buf, &c, 1) */
void cl_buf_append_char(struct cl_buf *buf, char c);
/* Same as prepend(buf, &c, 1) */
void cl_buf_prepend_char(struct cl_buf *buf, char c);
/* Same as replace(buf, where, &c, 1) */
void cl_buf_replace_char(struct cl_buf *buf, size_t where, char c);
/* Same as erase(buf, where, 1) */
void cl_buf_erase_char(struct cl_buf *buf, size_t where);

#endif // COMMANDLINE_BUFFER_H_

