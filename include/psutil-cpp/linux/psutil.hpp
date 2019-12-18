#ifndef PSUTIL_H
#define PSUTIL_H

#include <iostream>
#include "../utils.hpp"
#include <fstream>
#include <map>

struct svmem
{
    unsigned long long total;
    unsigned long long available;
    unsigned long long percent;
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

// System Memory
svmem virtual_memory();
svmem swap_memory();

#endif