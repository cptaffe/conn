
// Copyright (c) 2015 Connor Taffe

#include <stdio.h>

#include "args.h"
#include "sock.h"

// following GNU coding standards, this is the accepted version format.
char *version_text =
"chat v0.1\n"
"Copyright (c) 2015, Connor Taffe.\n"
"License: BSD 2-Clause <http://opensource.org/licenses/BSD-2-Clause>\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.";

// entry point
int main(int argc, char *argv[]) {
	enum args args = 0;
	char *err = parse_args(&args, argc, argv);
	if (err) {
		printf("err: %s\n", err);
		return 1;
	}

	if (args & ARG_VERSION) {
		puts(version_text);
	}
	if (args & ARG_VERBOSE) {
		puts("finished parsing arguments");
	}
	if (args & ARG_BROADCAST) {
		sock_broadcast("255.255.255.255", 0xbad, "This is a test.");
	}
}
