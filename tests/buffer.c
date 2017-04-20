

#include "../src/buffer.h"

int main (int argc, char *argv[])
{
	struct ll_buf buf;
	const char *s1;
	const char *s2;
	const char *s3;

	ll_buf_init(&buf);

	if (buf.len != 0 || strcmp(buf.str, "") != 0)
		exit(EXIT_FAILURE);

	s1 = "hola";
	ll_buf_assign(&buf, s1, strlen(s1));
	if (buf.len != strlen(s1) || strcmp(buf.str, s1) != 0)
		exit(EXIT_FAILURE);

	s1 = "string bigger than the buffer bucket size, that is 64 bytes by default";
	ll_buf_assign(&buf, s1, strlen(s1));
	if (buf.len != strlen(s1) || strcmp(buf.str, s1) != 0)
		exit(EXIT_FAILURE);

	s1 = "foo";
	s2 = "bar";
	s3 = "foobar";
	ll_buf_assign(&buf, s1, strlen(s1));
	ll_buf_append(&buf, s2, strlen(s2));
	if (buf.len != strlen(s3) || strcmp(buf.str, s3) != 0)
		exit(EXIT_FAILURE);

	s1 = "foo";
	s2 = "bar";
	s3 = "barfoo";
	ll_buf_assign(&buf, s1, strlen(s1));
	ll_buf_prepend(&buf, s2, strlen(s2));
	if (buf.len != strlen(s3) || strcmp(buf.str, s3) != 0)
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}
