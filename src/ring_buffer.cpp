#include <ring_buffer.h>
#include <cstring>
#include <fmt/core.h>

namespace javelin {

RingBuffer::RingBuffer() : head(0), tail(0), size(4096 * 3), buffer(size) {}

RingBuffer::~RingBuffer() {
	buffer.clear();
}

void RingBuffer::write(float *input, const size_t length) {
	if (head + length >= size) {
		size_t right_part = size - tail;
		std::memcpy(&buffer.data()[tail], input, right_part);
		std::memcpy(buffer.data(), input + right_part, length - right_part);
	} else {
		std::memcpy(buffer.data() + head, input, length);
	}
	head = tail;
	tail = (tail + length) % size;
}

void RingBuffer::read(float *output) {
	if (head > tail) {
		size_t right_part = size - head;
		std::memcpy(output, &buffer.data()[head], right_part);
		std::memcpy(output, buffer.data(), size - right_part);
	} else {
		std::memcpy(output, &buffer.data()[head], tail - head);
	}
}

}

