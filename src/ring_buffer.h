#pragma once

#include <array>
#include <iterator>
#include <vector>
#include <span>

namespace javelin {

//template<typename T>
//concept numeric_type = std::integral<T> || std::floating_point<T>;
class RingBuffer {
public:
	RingBuffer();
	~RingBuffer();

	void write(float *input, const size_t size);
	std::span<float> read();

	size_t head;
	size_t tail;

	const size_t size;
	std::vector<float> buffer;
};

}
