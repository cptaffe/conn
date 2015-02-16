
// Copyright (c) 2015 Connor Taffe

#ifndef CONN_ARGS_H_
#define CONN_ARGS_H_

// argument options (bitmask)
enum args {
	ARG_VERBOSE = 1 << 0,
	ARG_VERSION = 1 << 2
};

char *parse_args(enum args *args, int argc, char *argv[]);

#endif // CONN_ARGS_H_
