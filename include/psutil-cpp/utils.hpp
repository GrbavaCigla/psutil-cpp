#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <optional>

#ifdef linux
#include <sys/types.h>
#include <sys/stat.h>
#endif

// check if lhs and rhs (chars) are spaces (' ')
bool are_spaces(char, char);

// Removes double spaces from string, example is /proc/meminfo
std::string trim_double_spaces(std::string input);

// Check if directory exists
std::optional<bool> dexists(std::string path);

// Check if file exists
bool fexists(const std::string &filename);

// Splits string by delimiter, first argument is string, second is delimiter
std::vector<std::string> split_by_delim(std::string string, std::string delimiter);

// Calculate usage percentage
float usage_percent(unsigned long long used, unsigned long long total, int _round = -1);

// TODO: Implement this
float calculate_avail_memory();

// Parse file, example of file:
// bla       1
// bla_adw   3
// bla       300
// Output with trim and delimiter set to " " in std::map
// bla:1
// bla_adw:3
// bla:300
std::map<std::string, unsigned long long> parse_file(std::string path, bool trim_lines, std::string delimiter, int value_multiplier);

// Returns contents of file
std::vector<std::string> cat(std::string path);

// Returns valid path for cpu_freq
std::string get_path(int num);

void sleep_mlsec(uint32_t millisec);

bool is_only_numbers(const std::string &str);

std::vector<std::string> get_dirs_list(const std::string &path);

std::string file_to_string(const std::string &filename);

std::string replace_all(std::string str, const std::string &from, const std::string &to);

template<typename T>
T round2(const T number) {
  return round(number * 100) / 100;
}

#endif
