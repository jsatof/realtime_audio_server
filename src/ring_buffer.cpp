#include <ring_buffer.h>
#include <cstring>
#include <fmt/core.h>

namespace javelin {

RingBuffer::RingBuffer() : head(0), tail(0), size(4096 * 3), buffer(size) {}

void RingBuffer::write(float *input, const size_t length) {
	// memcpy-ing may be dubious with the containers

	size_t right_part = length - tail;
	if (right_part > length) {
		fmt::print("HUH??\n");
	}

	if (head + length >= this->size) {
		std::memcpy(buffer.data() + tail, input, right_part); // crashes
		std::memcpy(buffer.data(), input + right_part, length - right_part);
	} else {
		std::memcpy(buffer.data() + head, input, length);
	}
	head = tail;
	tail = (tail + length) % this->size;
}

void RingBuffer::read(float *, const size_t ) {
	
}


void RingBuffer::print() {
	fmt::print("Size: {}\n", size);
	fmt::print("[");
	for (auto s : buffer) {
		fmt::print("{}, ", s);
	}
	fmt::print("]\n");
}

}

