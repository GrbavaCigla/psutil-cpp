#include <iostream>
#include <psutil-cpp/cpu.hpp>

int main(){
	std::cout << cpu_freq().value() << std::endl;
	return 0;
}
