
// Copyright (c) 2015 Connor Taffe

#include <stdio.h>

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

	// init utf8 parser
	char *str = "Dobar dan! Kako ste? Ja sam Marina\n"
	"Dodigovi\u0107. Kako se Vi zovete?\n";
	utf8_Parser *p = utf8_Parser_init(str);
	uint32_t j, num = 0;
	for(int i = 0; !(j = utf8_Parser_Get(p, &num)); i++){
		printf("%s", (char *) &num);
	}
	printf("%d::%d\n", j, utf8_RuneCount(str));

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

	// cleanup
	pa_free(a);
}
