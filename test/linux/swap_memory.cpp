#include <iostream>
#include <psutil-cpp/mem.hpp>

int main(){
	std::cout << swap_memory().value();
	return 0;
}
