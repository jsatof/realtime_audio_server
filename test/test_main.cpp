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
	ASSERT_TRUE(ring_buffer.tail == 4);


}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
