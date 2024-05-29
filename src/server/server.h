#pragma once

#include <atomic>
#include <thread>
#include <vector>

namespace javelin {

class Server {
public:
	int8_t start() const;
	int8_t stop() const;

	//javelin::Allocator arena;
	//javelin::Allocator start_lifetime;

	// TODO: 'mutable' is a hack, probably should be in a constructor
	uint32_t mutable port;
	uint32_t mutable next_client_id;
	uint8_t mutable context;
	int32_t mutable socket_fd;

	//std::vector<Client> clients;
	//std::vector<Client> client_remove_queue;

	std::atomic<bool> mutable ready;
	std::atomic<bool> mutable quit;
	std::atomic<bool> mutable paused;

	std::mutex mutable mutex; // locked whenever this object's state is modified
	std::thread mutable thread;

	const char error[256];

	//Ticker tick;
	//Session session;
	//std::vector<Packet> packets;

private:
	void init() const;
	void create_session();
};

}
