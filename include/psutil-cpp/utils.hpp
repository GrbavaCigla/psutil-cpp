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

// Operator overload for struct sswap, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const sswap &swap);

#endif