
// Copyright (c) 2015 Connor Taffe

#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "sock.h"
#include "utf8.h"

// following GNU coding standards, this is the accepted version format.
char *version_text =
"chat v0.1\n"
"Copyright (c) 2015, Connor Taffe.\n"
"License: BSD 2-Clause <http://opensource.org/licenses/BSD-2-Clause>\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.";

// entry point
int main(int argc, char *argv[]) {
	// init argument parser
	pa *a = pa_init(argc, argv);
	if (!a) {
		return 1;
	}

	// parse args
	int err = parse_args(a);
	if (err) {
		pa_free(a);
		return 1;
	}

	printf("input unicode hex values (U+XXXXXX)\n");

	// init utf8 parser
	size_t len = 10;
	char *str = malloc(len);
	size_t slen;
	while ((slen = getline(&str, &len, stdin)) != -1){
		// get rid of optional newline
		for (int i = 0; i < slen; i++) {
			if (str[i] == '\n') {
				str[i] = 0;
				break;
			}
		}

		// get numeric input
		char *end = &str[slen];
		int32_t i = strtol(str, &end, 16);

		// encode to utf-8
		char strng[5] = {0};
		*((utf8_rune *) strng) = utf8_encode(i);
		printf("got: %#x->%d->'%s'\n", i, i,
		(*((utf8_rune *) strng) >= 0) ? (char *) strng : "not in utf-8");
	}

	// respond to specific arguments
	if (*a->args & ARG_VERSION) {
		puts(version_text);
	}
	if (*a->args & ARG_VERBOSE) {
		puts("verbose: finished parsing arguments");
	}
	if (*a->args & ARG_BROADCAST) {
		printf("broadcasting to address `%s`\n", a->broadcast);
		sock_broadcast(a->broadcast, 0xbad, "This is a test.");
	}
	free(str);

	// cleanup
	pa_free(a);
}
