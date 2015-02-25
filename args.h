
// Copyright (c) 2015 Connor Taffe

#ifndef CONN_ARGS_H_
#define CONN_ARGS_H_

// argument options (bitmask)
enum args {
	ARG_VERBOSE = 1 << 0,
	ARG_VERSION = 1 << 1,
	ARG_BROADCAST = 1 << 2,
};

typedef struct {
	// arguments
	int argc;
	char **argv;
	enum args *args;
	int argi; // current index
	void *func;

	// application specific info
	char *broadcast;
} pa;

int parse_args(pa *a);
void *pa_state_all(pa *a);
void *pa_state_long(pa *a);
void *pa_state_short(pa *a);

pa *pa_init(int argc, char *argv[]);
void pa_free(pa *a);

#endif // CONN_ARGS_H_
