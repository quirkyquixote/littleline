

#ifndef COMMANDLINE_BUFFER_H_
#define COMMANDLINE_BUFFER_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define CL_BUF_BUCKET_SIZE 64

struct cl_buf {
    unsigned char *str;
    size_t allocated;
    size_t len;
};

void cl_buf_init(struct cl_buf *buf);

void cl_buf_deinit(struct cl_buf *buf);

void cl_buf_grow(struct cl_buf *buf, size_t len);

void cl_buf_assign(struct cl_buf *buf, const void *str, size_t len);

void cl_buf_insert(struct cl_buf *buf, size_t where, const void *str, size_t len);

void cl_buf_append(struct cl_buf *buf, const void *str, size_t len);

void cl_buf_prepend(struct cl_buf *buf, const void *str, size_t len);

void cl_buf_replace(struct cl_buf *buf, size_t where, const void *str, size_t len);

void cl_buf_erase(struct cl_buf *buf, size_t where, size_t len);

void cl_buf_insert_char(struct cl_buf *buf, size_t where, char c);

void cl_buf_append_char(struct cl_buf *buf, char c);

void cl_buf_prepend_char(struct cl_buf *buf, char c);

void cl_buf_replace_char(struct cl_buf *buf, size_t where, char c);

void cl_buf_erase_char(struct cl_buf *buf, size_t where);

#endif // COMMANDLINE_BUFFER_H_

