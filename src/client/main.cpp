#include <client/client.h>

#include <fmt/core.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <opus.h>
#include <opusfile.h>

#include <string>

namespace javelin {

int client_main() {
	const uint32_t port = 34345;	
	const char *message = "Scooby Doo";

	int32_t client_socket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address = {};
	address.sin_family = PF_INET;
	address.sin_port = htons(port); // this can be 0, and will auto assign random port
	address.sin_addr.s_addr = INADDR_ANY;

	if (connect(client_socket, (struct sockaddr*)&address, sizeof(address))) {
		fmt::print(stderr, "ERROR: failled to connect to socket\n");
		return 1;
	}

	ssize_t bytes_sent = send(client_socket, message, sizeof(message), 0);
	if (bytes_sent == -1) {
		fmt::print(stderr, "ERROR: failed to send message\n");
		close(client_socket);
		return 1;
	}

	char buffer[2048];
	ssize_t response = recv(client_socket, buffer, sizeof(buffer), 0);
	if (response == -1) {
		fmt::print(stderr, "ERROR: could not receive response from server\n");
		close(client_socket);
		return 1;
	}


	fmt::print("Received message from server: {}\n", buffer);


	close(client_socket);

	return 0;
}

}
