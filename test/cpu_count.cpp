#include <iostream>
#include <psutil-cpp/cpu.hpp>

int main()
{
	std::cout << cpu_count();
	return 0;
}
