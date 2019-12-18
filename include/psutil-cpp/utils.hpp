#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

// check if lhs and rhs (chars) are spaces (' ')
bool are_spaces(char, char);

// Removes double spaces from string, example is /proc/meminfo
std::string trim_double_spaces(std::string);

// Splits string by delimiter, first argument is string, second is delimiter
std::vector<std::string> split_by_delim(std::string, std::string);

float usage_percent(unsigned long long, unsigned long long, int);

// TODO: Implement this
float calculate_avail_memory();

#endif