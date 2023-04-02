#include <iostream>
#include <psutil-cpp/cpu.hpp>

int main(){
	std::cout << cpu_times().value() << std::endl;
	std::cout << cpu_times(true).value();
	return 0;
}
