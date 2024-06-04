#include <server/server.h>

#include <fmt/core.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <vector>

// do everything in main.cpp, generalize later
// we're gonna listen on hardcoded port 34345
// and ping pong

namespace javelin {

static uint32_t next_client_id = 0;

class MyClient {
public:
	uint32_t id;
	int32_t socket;
	std::string address_string;
	struct sockaddr_storage address;
};

void parse_request(const char *buffer, size_t buffer_length) {
	if (buffer || buffer_length) {
		return;
	}
}

void update(const std::vector<MyClient>& clients) {
	for (auto c : clients) {
		fmt::print("sizeof client: {}\n", sizeof(c));
	}
}

void server_main() {
	uint32_t port = 34345;
	const uint32_t MAX_PENDING = 3;
	
	int32_t server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		fmt::print(stderr, "ERROR: failed to create server socket\n");
		return;
	}

	struct sockaddr_in address = {};
	address.sin_family = PF_INET;
	address.sin_port = htons(port); // this can be 0, and will auto assign random port
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr*)&address, sizeof(address))) {
		fmt::print(stderr, "ERROR: failed to bind socket\n");
		return;
	}

	socklen_t address_length = sizeof(address);
	getsockname(server_socket, (struct sockaddr*)&address, &address_length); // in case port == 0
	port = ntohs(address.sin_port);

	if (listen(server_socket, MAX_PENDING)) {
		fmt::print(stderr, "ERROR: failed to listen\n");
		return;
	}

	struct sockaddr_storage client_address;
	socklen_t client_length = sizeof(client_address);
	int32_t client_socket;

	// main loop
	bool quit = false;
	auto client_list = std::vector<MyClient>{};

	while (!quit) {
		client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);
		if (client_socket == -1) {
			fmt::print(stderr, "ERROR: client connection failed\n");
			break;
		}

		// TODO: set nonblocking
		// TODO: tcp no delay or maybe udp instead

		char client_address_string[NI_MAXHOST];
		char client_port_string[NI_MAXSERV];

		int32_t gai_error = getnameinfo((struct sockaddr*)&client_address, client_length,
				client_address_string, sizeof(client_address_string),
				client_port_string, sizeof(client_port_string),
				NI_NUMERICHOST | NI_NUMERICSERV);
		// drop client if error found formatting to string
		if (gai_error) {
			fmt::print(stderr, "ERROR: failed to getnameinfo\n");
			close(client_socket);
			continue;
		}

		auto m_client = MyClient{};
		m_client.address = client_address;
		m_client.address_string = fmt::format("{}:{}", client_address_string, client_port_string);
		m_client.id = next_client_id;
		m_client.socket = client_socket;
		next_client_id++;


		client_list.push_back(m_client);


		//on_client_connect(m_client);


		for (auto client : client_list) {
			char buffer[2048];
			ssize_t request_length = recv(client.socket, buffer, sizeof(buffer), 0);
			if (request_length == -1) {
				fmt::print(stderr, "ERROR: could not recv from client id {}\n", client.id);
				continue;
			}

			fmt::print("connection content: {}\n", buffer);
			parse_request(buffer, sizeof(buffer));
			//construct and send packets

			ssize_t response_length = send(client.socket, buffer, sizeof(buffer), 0);
			if (response_length == -1) {
				fmt::print(stderr, "ERROR: could not send message back to client {}\n", client.id);
				continue;
			}
		}


		// TODO: kick clients with incorrect dates on them
		// TODO: flush packets if its been a long time since we've sent packets to a specific client

		update(client_list);


	}

	close(server_socket);

}

}
