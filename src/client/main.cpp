#include <fmt/core.h>
#include <string>

int main(int argc, char **argv) {
	fmt::println("hi from client");
	fmt::print("Arg Count: {}\t\tValue: {}", argc, std::string(argv[0]));
}
