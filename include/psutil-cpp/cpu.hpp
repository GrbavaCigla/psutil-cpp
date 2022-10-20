#pragma once
#include <vector>
#include <ostream>
#include <optional>

#include "psutil-cpp/utils.hpp"

#ifdef linux
#include <unistd.h>
#endif

struct scputimes
{
    float user;
    float nice;
    float system;
    float idle;
    float iowait;
    float irq;
    float softirq;
    float steal;
    float guest;
    float guest_nice;

    scputimes()
    {
        user = 0;
        nice = 0;
        system = 0;
        idle = 0;
        iowait = 0;
        irq = 0;
        softirq = 0;
        steal = 0;
        guest = 0;
        guest_nice = 0;
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

// Operator overload for scputimes struct, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const scputimes &cputimes);

// Operator overload for struct scpufreq, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const scpufreq &cpufreq);

// Operator overload for vector of scputimes structs, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const std::vector<scputimes> &cputimes);

// Operator overload for vector of scpufreq structs, prints in python namedtuple stylem
std::ostream &operator<<(std::ostream &output, const std::vector<scpufreq> &cpufreq);

std::optional<std::vector<scputimes>> cpu_times(bool percpu = false);
std::optional<std::vector<scpufreq>> cpu_freq(bool percpu = false);
unsigned short int cpu_count(bool logical = true);