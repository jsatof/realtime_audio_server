#include <server.h>

#include <fmt/core.h>
#include <sys/socket.h>

#include <util_dump.h>

namespace javelin {

void Server::create_session() {

}

// sets the server configuration
void Server::init() const {
	const char *port_env = std::getenv("JAVELIN_PORT");
	if (!port_env) {
		// TODO: make a logger
		fmt::print(stderr, "ERROR: port environment variable isnt set\n");
		quit = true;
		return;
	}

	uint32_t port_number = std::stoul(port_env);
	if (port_number <= 1000 || port_number >= 55000) {
		// TODO: make a logger
		fmt::print(stderr, "ERROR: port environment variable isnt in a range [1001, 54999]\n");
		quit = true;
		return;
	}

	port = port_number;
	fmt::print("port number found: {}\n", port);

	next_client_id = 1;

	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		// TODO: make a logger
		fmt::print(stderr, "ERROR: failed to create a socket_fd\n");
		quit = true;
		return;
	}

	if (socket_set_nonblocking(socket_fd)) {
		// TODO: make a logger
		fmt::print(stderr, "ERROR: failed to make socket nonblocking\n");
		quit = true;
		return;
	}

	/*
	if (bind(socket_fd, sizeof(addr))) {
		// TODO: make a logger
		fmt::print(stderr, "ERROR: failed to bind() socket\n");
		quit = true;
		return;
	}
	*/
}

// true means successful return, false means some failure
int8_t Server::start() const {
	init();
	if (quit) {
		return -1;
	}
	return 0;
}

int8_t Server::stop() const {
	return 0;
}

}
