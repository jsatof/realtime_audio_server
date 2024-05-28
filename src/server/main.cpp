#include <fmt/core.h>

int main(int argc, char **argv) {
	fmt::println("hi from server");
	fmt::print("Arg Count: {}\t\tValue: {}", argc, argv[0]);
}
