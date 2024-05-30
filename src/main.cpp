#include <fmt/core.h>
#include <SDL2/SDL.h>
#include <opus.h>

#include <server/server.h>
#include <client/client.h>

int main() {
	//javelin::server_main(); // TODO: run in separate thread
	javelin::client_main();
}
