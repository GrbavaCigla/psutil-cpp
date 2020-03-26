#ifndef PSUTIL_HPP
#define PSUTIL_HPP

#include <iostream>
#include "../utils.hpp"
#include <fstream>
#include <map>
#include <sys/sysinfo.h>
#include <unistd.h>

struct svmem
{
    unsigned long long total;
    unsigned long long available;
    float percent;
    unsigned long long used;
    unsigned long long free;
    unsigned long long active;
    unsigned long long inactive;
    unsigned long long buffers;
    unsigned long long cached;
    unsigned long long shared;
    unsigned long long slab;

    svmem()
    {
        total = 0;
        available = 0;
        percent = 0;
        used = 0;
        free = 0;
        active = 0;
        inactive = 0;
        buffers = 0;
        cached = 0;
        shared = 0;
        slab = 0;
    }
};

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

// Operator overload for svmem struct, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const svmem &vmem);

// Operator overload for scputimes struct, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const scputimes &cputimes);

// Operator overload for vector of scputimes structs, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const std::vector<scputimes> &cputimes);

// Operator overload for vector of scpufreq structs, prints in python namedtuple stylem
std::ostream &operator<<(std::ostream &output, const std::vector<scpufreq> &cpufreq);

// CPU
std::vector<scputimes> cpu_times(bool percpu = false);
std::vector<scpufreq> cpu_freq(bool percpu = false);
unsigned short int cpu_count(bool logical = true);

// System Memory
svmem virtual_memory();
sswap swap_memory();

#endif