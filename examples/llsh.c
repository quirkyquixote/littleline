
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../littleline.h"

int main(int arc, char *argv[])
{
	const char *line;

	ll_set_key_bindings(LL_ANSI_KEY_BINDINGS);
	ll_set_history_with_file(10, "history.txt");

	while (line = ll_read(">>")) {
		if (strlen(line) > 0)
			puts(line);
	}

	exit(EXIT_SUCCESS);
}
