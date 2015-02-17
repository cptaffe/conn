
// Copyright (c) 2015 Connor Taffe

#include <stdio.h>
#include <string.h>

#include "args.h"

// error setup
char error_array[120]; // never longer than 120 chars.
char *error_string = (char *) error_array;

struct pa {
	// arguments
	int argc;
	char **argv;
	enum args *args;
	char *err; // error string
	int argi; // current index
};

static int pa_short(struct pa *a, int off) {
	// short options are one character and can be combined
	if (a->argv[a->argi][off] == 0) {
		sprintf(a->err, "(%d): missing short option", a->argi);
		return 1;
	}
	for (int j = off; a->argv[a->argi][j] != 0; j++) {
		const char opt = a->argv[a->argi][j];
		switch (opt) {
			case 'v':
			*a->args |= ARG_VERBOSE;
			break;
			default:
			sprintf(a->err, "(%d, %d): unknown short option '%c'", a->argi, j, opt);
			return 1;
		}
	}
	return 0;
}

static int pa_long(struct pa *a) {
	const char *opt = &a->argv[a->argi][2];
	if (!strcmp(opt, "version")) {
		*a->args |= ARG_VERSION;
	} else if (!strcmp(opt, "broadcast")) {
		*a->args |= ARG_BROADCAST;
	} else {
		strnlen(opt, 10) > 10
		? sprintf(a->err, "(%d): unknown long option '%.10s...''", a->argi, opt)
		: sprintf(a->err, "(%d): unknown long option '%s'", a->argi, opt);
		return 1;
	}
	return 0;
}

static int pa_all(struct pa *a) {
	for (a->argi = 1; a->argi < a->argc; a->argi++) {

		// -... option of some kind
		if (a->argv[a->argi][0] == '-') {
			// --... long options
			if (a->argv[a->argi][1] == '-') {
				if (pa_long(a)) {
					return 1;
				}
			}

			// -... short options
			else {
				if (pa_short(a, 1)) {
					return 1;
				}
			}
		} else {
			// treat non '-' prefixed args as '-' prefixed
			if (pa_short(a, 0)) {
				return 1;
			}
		}
	}
	return 0;
}

// parses arguments, alters 'args' accordingly
char * parse_args(enum args *args, int argc, char *argv[]) {
	struct pa a = {
		.err = error_string,
		.argc = argc,
		.argv = argv,
		.args = args
	};
	if (pa_all(&a)) {
		return a.err;
	} else {
		return NULL;
	}
}
