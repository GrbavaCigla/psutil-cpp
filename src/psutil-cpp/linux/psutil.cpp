#include "psutil.hpp"

svmem virtual_memory()
{
    std::ifstream file("/proc/meminfo");
    std::string line;
    std::vector<std::string> temp_value;
    svmem result = svmem();
    std::map<std::string, unsigned long long> mems;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            line = trim_double_spaces(line);
            temp_value = split_by_delim(line, std::string(" "));

            mems[temp_value[0].substr(0, temp_value[0].length() - 1)] = std::stoull(temp_value[1]) * 1024;
        }
        file.close();
    }
    result.total = mems["MemTotal"];
    result.free = mems["MemFree"];
    result.buffers = mems["Buffers"];
    result.cached = mems["Cached"];
    result.shared = mems["Shmem"];
    result.active = mems["Active"];
    result.inactive = mems["Inactive"];
    result.slab = mems["Slab"];
    result.available = mems["MemAvailable"];
    if (!result.cached)
    {
        result.cached = mems["SReclaimable"];
    }
    if (!result.shared)
    {
        result.shared = mems["MemShared"];
    }
    if (!result.inactive)
    {
        result.inactive = mems["Inact_dirty"] + mems["Inact_clean"] + mems["Inact_laundry"];
    }
    result.used = result.total - result.free - result.cached - result.buffers;
    if (result.used < 0)
    {
        result.used = result.total - result.free;
    }
    if (!result.available)
    {
        result.available = result.free + result.buffers + result.cached; // TODO: Replace with calculate_avail_memory(), when it is made
    }
    if (result.available < 0)
    {
        result.available = 0;
    }
    if (result.available > result.total)
    {
        result.available = result.free;
    }
    result.percent = usage_percent(result.used, result.total);

    return result;
}

svmem swap_memory()
{
}

int main()
{
    std::cout << virtual_memory().percent << std::endl;
    return 0;
}