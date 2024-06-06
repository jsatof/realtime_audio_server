#include <ring_buffer.h>
#include <cstring>
#include <fmt/core.h>

namespace javelin {

RingBuffer::RingBuffer() : head(0), tail(0), size(4096 * 3), buffer(size) {}

RingBuffer::~RingBuffer() {
	buffer.clear();
}

void RingBuffer::write(float *input, const size_t length) {
	// memcpy-ing may be dubious with the containers
	if (head + length >= size) {
		size_t right_part = size - tail;
		std::memcpy(&buffer.data()[tail], input, right_part); // crashes
		std::memcpy(buffer.data(), input + right_part, length - right_part);
	} else {
		std::memcpy(buffer.data() + head, input, length);
	}
	head = tail;
	tail = (tail + length) % size;
}

std::span<float> RingBuffer::read() {
	std::span<float> out;

	if (head > tail) {
		return { buffer.begin(), size };
	} else {
		return { buffer.begin(), size };
	}

	head = tail;
	tail = (tail + head) % size; // wrong

	return out;
}

}

