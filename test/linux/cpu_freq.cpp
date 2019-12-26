#include <iostream>
#include <psutil-cpp/linux/psutil.hpp>

int main()
{
    std::cout << cpu_freq();
    return 0;
}
