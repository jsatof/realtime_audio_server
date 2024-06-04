#include <fmt/core.h>
#include <SDL2/SDL.h>
#include <opus.h>
#include <thread>

#include <server/server.h>
#include <client/client.h>

int main() {
	auto server_thread = std::thread(javelin::server_main);

	javelin::client_main();

	server_thread.join();
}
