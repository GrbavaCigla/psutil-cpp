#include "psutil-cpp/utils.hpp"

bool are_spaces(char lhs, char rhs)
{
    return (lhs == rhs) && (lhs == ' ');
}

// Removes double spaces from string, example is /proc/meminfo
std::string trim_double_spaces(std::string input)
{
    std::string::iterator new_end = std::unique(input.begin(), input.end(), are_spaces);
    input.erase(new_end, input.end());
    return input;
}

// Splits string by delimiter, first argument is string, second is delimiter
std::vector<std::string> split_by_delim(std::string string, std::string delimiter)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> array;
    while ((pos = string.find(delimiter)) != std::string::npos)
    {

        token = string.substr(0, pos);
        array.push_back(token);
        string.erase(0, pos + delimiter.length());
    }
    array.push_back(string);
    return array;
}

float usage_percent(unsigned long long used, unsigned long long total, int _round)
{
    if (total == 0)
    {
        return 0;
    }
    float ret = ((float)used / (float)total) * 100.0;
    if (_round != -1)
    {
        unsigned long rounding_places = pow(10, _round);
        ret = std::round(ret * rounding_places) / rounding_places;
    }
    return ret;
}

// TODO: Implement this
float calculate_avail_memory()
{
    return 0;
}

std::map<std::string, unsigned long long> parse_file(std::string path, bool trim_lines, std::string delimiter, int value_multiplier)
{
    std::ifstream file(path);
    std::string line;
    std::vector<std::string> temp_value;
    std::map<std::string, unsigned long long> mems;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (trim_lines)
            {
                line = trim_double_spaces(line);
            }

            temp_value = split_by_delim(line, std::string(delimiter));

            mems[temp_value[0]] = std::stoull(temp_value[1]) * value_multiplier;
        }
        file.close();
    }
    return mems;
}

std::ostream &operator<<(std::ostream &output, const sswap &swap)
{
    return output << "sswap(total=" << swap.total
                  << ", used=" << swap.used
                  << ", free=" << swap.free
                  << ", percent=" << swap.percent
                  << ", sin=" << swap.sin
                  << ", sout=" << swap.sout
                  << ")";
}

std::ostream &operator<<(std::ostream &output, const scpufreq &cpufreq)
{
    return output << "scpufreq(current=" << cpufreq.current
                  << ", min=" << cpufreq.min
                  << ", max=" << cpufreq.max
                  << ")";
}