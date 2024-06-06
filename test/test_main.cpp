#include <gtest/gtest.h>
#include <ring_buffer.h>
#include <fmt/core.h>

TEST(AudioServerTestSuite, RingBufferInitTest) {
	auto ring_buffer = javelin::RingBuffer();
	ASSERT_TRUE(ring_buffer.size == 12288);
	ASSERT_TRUE(ring_buffer.head == 0);
	ASSERT_TRUE(ring_buffer.tail == 0);
}

TEST(AudioServerTestSuite, RingBufferNormalInsertTest) {
	auto ring_buffer = javelin::RingBuffer();

	float dummy_data[12];
	ring_buffer.write(dummy_data, sizeof(dummy_data));
	ASSERT_TRUE(ring_buffer.head == 0);
	ASSERT_TRUE(ring_buffer.tail == sizeof(dummy_data));

	ring_buffer.write(dummy_data, sizeof(dummy_data));
	ASSERT_TRUE(ring_buffer.head == sizeof(dummy_data));
	ASSERT_TRUE(ring_buffer.tail == 2 * sizeof(dummy_data));
}

TEST(AudioServerTestSuite, RingBufferLoopbackTest) {
	auto ring_buffer = javelin::RingBuffer();
	ring_buffer.head = 12274;
	ring_buffer.tail = 12280; // 8 before the end

	float dummy_data[12];
	ring_buffer.write(dummy_data, sizeof(dummy_data));
	ASSERT_TRUE(ring_buffer.head = 12280);
	ASSERT_TRUE(ring_buffer.tail == 40);
}

TEST(AudioServerTestSuite, RingBufferFullWriteTest) {
	auto ring_buffer = javelin::RingBuffer();
	float dummy_data[2048];

	size_t loop_count = (ring_buffer.size / sizeof(dummy_data)) + 1; // +1 for wanting the loopback to happen
	for (size_t i = 0; i < loop_count; ++i) {
		ring_buffer.write(dummy_data, sizeof(dummy_data));
	}

	ASSERT_TRUE(ring_buffer.head > ring_buffer.tail);

	size_t right_size = ring_buffer.size - ring_buffer.head;
	size_t left_size = ring_buffer.tail;
	ASSERT_EQ(right_size + left_size, sizeof(dummy_data));

	float read_test_data[2048];
	ring_buffer.read(read_test_data);
	ASSERT_EQ(dummy_data[0], read_test_data[0]);
	ASSERT_EQ(dummy_data[2047], read_test_data[2047]);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
