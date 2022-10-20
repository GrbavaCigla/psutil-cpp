#pragma once

#include <ostream>
#include <optional>
#include <map>

#include "psutil-cpp/utils.hpp"

#ifdef linux
#include <sys/sysinfo.h>
#endif

// Used for swap
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

// Operator overload for svmem struct, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const svmem &vmem);

// Operator overload for struct sswap, prints in python namedtuple style
std::ostream &operator<<(std::ostream &output, const sswap &swap);

// System Memory
std::optional<svmem> virtual_memory();
std::optional<sswap> swap_memory();
