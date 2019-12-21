#include <iostream>
#include <psutil-cpp/linux/psutil.hpp>

int main(){
	std::cout << cpu_times() << std::endl;
	std::cout << cpu_times(true);
	return 0;
}
