#include <client/client.h>
#include <ring_buffer.h>

#include <fmt/core.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <opus.h>
#include <opusfile.h>
#include <SDL2/SDL.h>

#include <string>
#include <vector>

namespace javelin {

void populate_samples(const std::string &path) {
	int opus_error;
	OggOpusFile *file = op_open_file(path.c_str(), &opus_error);
	if (opus_error) {
		fmt::print(stderr, "ERROR: Could not open OggOpusFile\n");
		return;
	}

	size_t samples_left = op_pcm_total(file, -1); // use a negative _li value to read the whole stream length
	size_t sample_counter = 0;

	float load_buffer[2048];

	while (samples_left > 0) {
		const uint32_t samples_read = op_read_float_stereo(file, load_buffer, sizeof(load_buffer) * sizeof(load_buffer[0]));
		samples_left -= samples_read;
		sample_counter += samples_read;

		// TODO: load these chunks into the buffer

		fmt::print("status: {} samples read\n", samples_read);
	}

	op_free(file);
}

void audio_callback(void *, uint8_t *stream, int stream_length) {
	std::vector<float> samples;
	if (samples.size() == 0) {
		return;
	}

	uint32_t length = std::min(uint32_t(stream_length), uint32_t(samples.size()));

	SDL_memcpy(stream, samples.data(), length);
	//audio_buffer->data += 1;
	//audio_buffer->length -= length;
}

void play_audio(float *, const SDL_AudioSpec &) {
	// TODO: maybe make a client argument or file chooser and check path validity
	//const char *path = "bets_are_off.opus"; // have to run from project root




}

int client_main() {
	const uint32_t port = 34345;	
	const char *message = "Scooby Doo";

	int32_t client_socket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address = {};
	address.sin_family = PF_INET;
	address.sin_port = htons(port); // this can be 0, and will auto assign random port
	address.sin_addr.s_addr = INADDR_ANY;

	if (SDL_Init(SDL_INIT_AUDIO)) {
		fmt::print(stderr, "ERROR: Could not init sdl\n{}\n", SDL_GetError());
		return 1;
	}


// SDL doesnt want its user to fill the missing fields
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	SDL_AudioSpec obtained_spec;
	SDL_AudioSpec requested_spec = {
		.freq = 48000,
		.format = AUDIO_F32,
		.channels = 2,
		.samples = 2048,
		.callback = audio_callback,
		.userdata = nullptr,
	};
#pragma GCC diagnostic pop

	SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(NULL, false, &requested_spec, &obtained_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
	SDL_PauseAudioDevice(audio_device, false);
	play_audio(nullptr, obtained_spec);

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
	SDL_CloseAudioDevice(audio_device);
	SDL_Quit();
	return 0;
}


}
