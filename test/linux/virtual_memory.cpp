#include <iostream>
#include <psutil-cpp/linux/psutil.hpp>

int main(){
	std::cout << virtual_memory();
	return 0;
}
