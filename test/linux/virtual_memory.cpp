#include <iostream>
#include <psutil-cpp/mem.hpp>

int main(){
	std::cout << virtual_memory().value();
	return 0;
}
