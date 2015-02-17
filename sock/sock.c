
// Copyright (c) 2015 Connor Taffe

#include <sys/socket.h>	// socket, bind
#include <arpa/inet.h>	// struct sockaddr_in
#include <string.h>

#include "sock.h"

int sock_broadcast(char *ip, unsigned short port, char *msg) {

	// new socket
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) { return 1; }

	int bcp = 1; // broadcast permissions
	int err = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &bcp, sizeof(bcp));
	if (err < 0) { return 2; }

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = inet_addr(ip),
		.sin_port = htons(port)
	};

	int len = strlen(msg);
	while (1) {
		err = sendto(sock, msg, len, 0, (struct sockaddr *) &addr, sizeof(addr));
		if (err != len) { return 3; }
	}
}
