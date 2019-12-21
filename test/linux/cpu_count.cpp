#include <iostream>
#include <psutil-cpp/linux/psutil.hpp>

int main()
{
	std::cout << cpu_count();
	return 0;
}
