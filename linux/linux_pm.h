#include <iostream>
#include <vector>

int get_cpu_count();
std::vector<std::string> get_cpu_model(bool);
std::vector<int> get_cpu_freq(bool);
std::vector<int> get_cpu_usage(bool);

int get_total_swap_memory();
int get_cached_swap_memory();
int get_free_swap_memory();

int get_available_memory();
int get_total_memory();
int get_free_memory();
