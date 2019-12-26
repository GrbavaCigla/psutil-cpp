#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

// check if lhs and rhs (chars) are spaces (' ')
bool are_spaces(char, char);

// Removes double spaces from string, example is /proc/meminfo
std::string trim_double_spaces(std::string input);

// Check if directory exists
bool dexists(std::string path);

// Check if file exists
bool fexists(const std::string &filename);

// Splits string by delimiter, first argument is string, second is delimiter
std::vector<std::string> split_by_delim(std::string string, std::string delimiter);

// Calculate usage percentage
float usage_percent(unsigned long long used, unsigned long long total, int _round = -1);

// TODO: Implement this
float calculate_avail_memory();

//Used for swap
struct sswap
{
    unsigned long long total;
    unsigned long long free;
    unsigned long long used;
    float percent;
    unsigned long long sin;
    unsigned long long sout;

    sswap()
    {
        total = 0;
        free = 0;
        used = 0;
        percent = 0;
        sin = 0;
        sout = 0;
    }
};

struct scpufreq
{
    float current;
    float min;
    float max;

    scpufreq(float current_par = 0, float min_par = 0, float max_par = 0)
    {
        current = current_par;
        min = min_par;
        max = max_par;
    }
};

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

// Operator overload for struct sswap, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const sswap &swap);

// Operator overload for struct scpufreq, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const scpufreq &cpufreq);

#endif