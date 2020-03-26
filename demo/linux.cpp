#include <psutil-cpp/linux/psutil.hpp>
#include <iostream>

int main()
{
	std::cout << virtual_memory() << std::endl; // Returns svmem struct
	std::cout << swap_memory() << std::endl;	// Returns sswap struct
	std::cout << cpu_times()[0] << std::endl;   // Returns std::vector with only one element (scputimes struct)
	std::cout << cpu_times(true) << std::endl;  // Returns std::vector of scputimes structs
	std::cout << cpu_count() << std::endl;      // Returns number cpus. When logical is false (cpu_count(false)) currently doesn't work!

	return 0;
}
