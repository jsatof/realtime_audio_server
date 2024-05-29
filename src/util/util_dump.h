#pragma once

#include <sys/socket.h>
#include <netinet/tcp.h>
#include <fcntl.h>

int8_t socket_set_nonblocking(int32_t fd) {
	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1) {
		return -1;
	}
	return 0;
}
