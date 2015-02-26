
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
	const uint8_t *str = (const uint8_t *)
	"\uD55C";
	utf8_String *string = utf8_String_init(str);
	utf8_Parser *p = utf8_Parser_init(string);
	utf8_Rune num = 0;
	for(int i = 0; !utf8_Parser_Get(p, &num); i++){
		printf("%s", (char *) &num);
		printf("code-point: U+%X\n", utf8_RuneDecode(num));
	}
	printf("len: %d\n", utf8_RuneCount(string));
	utf8_String_free(string);
	utf8_Parser_free(p);

	char s[] = {0,0,0,0,0};
	 utf8_RuneEncode(0xff, (utf8_Rune *) s);
	printf("test: '%s'\n", s);

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
