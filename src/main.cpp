#include <fmt/core.h>
#include <SDL2/SDL.h>
#include <opus.h>

#include <server/server.h>
#include <client/client.h>

int main() {
	if (SDL_Init(SDL_INIT_AUDIO)) {
		fmt::print(stderr, "ERROR: Could not init sdl\n{}\n", SDL_GetError());
		return 1;
	}

	//javelin::server_main(); // TODO: run in separate thread
	javelin::client_main();

	SDL_Quit();
}
