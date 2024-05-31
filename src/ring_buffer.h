#pragma once

#include <array>
#include <iterator>
#include <vector>

namespace javelin {

//template<typename T>
//concept numeric_type = std::integral<T> || std::floating_point<T>;
class RingBuffer {
public:
	RingBuffer();

	void write(float *input, const size_t size);
	void read(float *output, const size_t size);

	void print();
	size_t head;
	size_t tail;
	const size_t size;
	std::vector<float> buffer;

private:
};

}
