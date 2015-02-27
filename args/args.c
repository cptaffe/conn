
// Copyright (c) 2015 Connor Taffe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

void *pa_state_short(pa *a) {
	// offset
	int off;
	if (a->argv[a->argi][0] == '-') {
		off = 1;
	} else {
		off = 0;
	}

	// short options are one character and can be combined
	if (a->argv[a->argi][off] == 0) {
		fprintf(stderr, "(%d): missing short option\n", a->argi);
		return NULL;
	}
	for (int j = off; a->argv[a->argi][j] != 0; j++) {
		const char opt = a->argv[a->argi][j];
		switch (opt) {
			case 'v':
			*a->args |= ARG_VERBOSE;
			break;
			default:
			fprintf(stderr, "(%d, %d): unknown short option '%c'\n", a->argi, j, opt);
			return NULL;
		}
	}
	return &pa_state_all;
}

void *pa_state_address(pa *a) {
	if ((a->argi + 1) < a->argc) {
		a->argi++;
		a->broadcast = a->argv[a->argi];
		return &pa_state_all;
	}
	return NULL;
}

void *pa_state_long(pa *a) {
	const char *opt = &a->argv[a->argi][2];
	if (!strcmp(opt, "version")) {
		*a->args |= ARG_VERSION;
		return &pa_state_all;
	} else if (!strcmp(opt, "broadcast")) {
		*a->args |= ARG_BROADCAST;
		return &pa_state_address;
	} else {
		strnlen(opt, 10) > 10
		? fprintf(stderr, "(%d): unknown long option '%.10s...''\n", a->argi, opt)
		: fprintf(stderr, "(%d): unknown long option '%s'\n", a->argi, opt);
		return NULL;
	}
	return &pa_state_all;
}

void *pa_state_all(pa *a) {
	if ((a->argi + 1) < a->argc) {
		a->argi++;
		if (a->argv[a->argi][0] == '-') {
			// -... option of some kind
			if (a->argv[a->argi][1] == '-') {
				// --... long options
				return &pa_state_long;
			} else {
				// -... short options
				return &pa_state_short;
			}
		} else {
			fprintf(stderr, "(%d): expected '-' prefix, found '%c'\n", a->argi, a->argv[a->argi][0]);
			return NULL;
		}
	} else {
		return NULL;
	}
}

pa *pa_init(int argc, char *argv[]) {
	pa *b = malloc(sizeof(pa));
	if (b) {
		memset(b, 0, sizeof(pa));
		b->argc = argc;
		b->argv = argv;
		b->args = malloc(sizeof(enum args));
		if (b->args) {
			memset(b->args, 0, sizeof(enum args));
		}
		b->func = &pa_state_all;
	}
	return b;
}

void pa_free(pa *a) {
	free(a->args);
	free(a);
}

// parses arguments, alters 'args' accordingly
int parse_args(pa *a) {
	// state machine
	while (a->func) {
		a->func = ((void *(*)(pa *)) a->func)(a);
	}
	return 0;
}
