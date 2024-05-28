#pragma once

#include <atomic>
#include <thread>
#include <vector>

namespace javelin {

class Server {
public:
	constexpr bool init();
	bool start();
	bool stop();

	//javelin::Allocator arena;
	//javelin::Allocator start_lifetime;

	uint32_t port;
	uint32_t socket;
	uint32_t next_client_id;
	uint8_t context;

	//std::vector<Client> clients;
	//std::vector<Client> client_remove_queue;

	std::atomic<bool> ready;
	std::atomic<bool> quit;
	std::atomic<bool> paused;

	std::mutex mutex;
	std::thread thread;

	const char error[256];

	//Ticker tick;
	//Session session;
	//std::vector<Packet> packets;
};

}
