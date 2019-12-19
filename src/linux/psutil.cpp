#include "psutil-cpp/linux/psutil.hpp"

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
    result.percent = usage_percent(result.used, result.total, -1);

    return result;
}

sswap swap_memory()
{
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    std::vector<std::string> temp_value;
    sswap result = sswap();
    std::map<std::string, unsigned long long> mems;

    if (meminfo.is_open())
    {
        while (std::getline(meminfo, line))
        {
            line = trim_double_spaces(line);
            temp_value = split_by_delim(line, " ");

            mems[temp_value[0].substr(0, temp_value[0].length() - 1)] = std::stoull(temp_value[1]) * 1024;
        }
        meminfo.close();
    }
    result.total = mems["SwapTotal"];
    result.free = mems["SwapFree"];

    if (!result.total || !result.free)
    {
        struct sysinfo info;
        if (sysinfo(&info) == 0)
        {
            result.total = info.totalswap;
            result.free = info.freeswap;
        }
    }

    result.used = result.total - result.free;
    result.percent = usage_percent(result.used, result.total, 1);

    std::ifstream vmstat("/proc/vmstat");

    if (vmstat.is_open())
    {
        while (std::getline(vmstat, line))
        {
            temp_value = split_by_delim(line, " ");
            if (line.substr(0, 7) == "pswpin ")
            {
                result.sin = std::stoull(temp_value[1]) * 4096;
            }
            if (line.substr(0, 7) == "pswpout")
            {
                result.sout = std::stoull(temp_value[1]) * 4096;
            }
        }
        vmstat.close();
    }
    return result;
}

std::ostream &operator<<(std::ostream &output, const svmem &vmem)
{
    return output << "svmem(total=" << vmem.total
                  << ", available=" << vmem.available
                  << ", percent=" << vmem.percent
                  << ", used=" << vmem.used
                  << ", free=" << vmem.free
                  << ", active=" << vmem.active
                  << ", inactive=" << vmem.inactive
                  << ", buffers=" << vmem.buffers
                  << ", cached=" << vmem.cached
                  << ", shared=" << vmem.shared
                  << ", slab=" << vmem.slab
                  << ")" << std::endl;
}