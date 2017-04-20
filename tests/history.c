
#include <stdio.h>

#include "../src/history.h"

static const char *strings1[] = {
	"this", "is", "a", "simple", "test", NULL
};

static const char *strings2[] = {
	"this", "is", "a", "test", "for", "overflow", NULL
};

int main(int argc, char *argv[])
{
	struct ll_history hist;
	int i;
	const char *line;

	ll_history_init(&hist, 5);
	for (i = 0; strings1[i]; ++i)
		ll_history_push(&hist, strings1[i]);
	for (i = 0; strings1[i]; ++i) {
		line = ll_history_index(&hist, i);
		if (strcmp(line, strings1[i]) != 0) {
			fprintf(stderr, "On test #1: expected \"%s\", got \"%s\"\n",
					strings1[i], line);
			exit(EXIT_FAILURE);
		}
	}
	ll_history_deinit(&hist);

	ll_history_init(&hist, 5);
	for (i = 0; strings2[i]; ++i)
		ll_history_push(&hist, strings2[i]);
	for (i = 0; strings2[i + 1]; ++i) {
		line = ll_history_index(&hist, i);
		if (strcmp(line, strings2[i + 1]) != 0) {
			fprintf(stderr, "On test #2: expected \"%s\", got \"%s\"\n",
					strings2[i + 1], line);
			exit(EXIT_FAILURE);
		}
	}
	ll_history_deinit(&hist);

	exit(EXIT_SUCCESS);
}
