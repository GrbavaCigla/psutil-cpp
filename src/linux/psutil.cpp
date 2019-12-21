#include "psutil-cpp/linux/psutil.hpp"

int CLOCK_TICKS = sysconf(_SC_CLK_TCK);
int PAGESIZE = sysconf(_SC_PAGE_SIZE);

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

std::vector<scputimes> cpu_times(bool percpu)
{
    std::ifstream stat("/proc/stat");
    std::string line;
    std::vector<std::string> temp_value;
    std::vector<scputimes> result;
    scputimes temp_scputimes = scputimes();
    int counter = 0;
    bool done = false;
    if (stat.is_open())
    {
        if (percpu)
        {
            while (std::getline(stat, line))
            {
                if (counter != 0 && !done)
                {
                    line = trim_double_spaces(line);
                    temp_value = split_by_delim(line, " ");

                    if (temp_value[0].substr(0, 3) != "cpu")
                    {
                        done = true;
                    }
                    temp_scputimes.user = std::stof(temp_value[1]) / (float)CLOCK_TICKS;
                    temp_scputimes.nice = std::stof(temp_value[2]) / (float)CLOCK_TICKS;
                    temp_scputimes.system = std::stof(temp_value[3]) / (float)CLOCK_TICKS;
                    temp_scputimes.idle = std::stof(temp_value[4]) / (float)CLOCK_TICKS;
                    temp_scputimes.iowait = std::stof(temp_value[5]) / (float)CLOCK_TICKS;
                    temp_scputimes.irq = std::stof(temp_value[6]) / (float)CLOCK_TICKS;
                    temp_scputimes.softirq = std::stof(temp_value[7]) / (float)CLOCK_TICKS;
                    temp_scputimes.steal = std::stof(temp_value[8]) / (float)CLOCK_TICKS;
                    temp_scputimes.guest = std::stof(temp_value[9]) / (float)CLOCK_TICKS;
                    temp_scputimes.guest_nice = std::stof(temp_value[10]) / (float)CLOCK_TICKS;
                    result.push_back(temp_scputimes);
                }
                counter++;
            }
        }
        else
        {
            std::getline(stat, line);
            line = trim_double_spaces(line);
            temp_value = split_by_delim(line, " ");
            temp_scputimes.user = std::stof(temp_value[1]) / (float)CLOCK_TICKS;
            temp_scputimes.nice = std::stof(temp_value[2]) / (float)CLOCK_TICKS;
            temp_scputimes.system = std::stof(temp_value[3]) / (float)CLOCK_TICKS;
            temp_scputimes.idle = std::stof(temp_value[4]) / (float)CLOCK_TICKS;
            temp_scputimes.iowait = std::stof(temp_value[5]) / (float)CLOCK_TICKS;
            temp_scputimes.irq = std::stof(temp_value[6]) / (float)CLOCK_TICKS;
            temp_scputimes.softirq = std::stof(temp_value[7]) / (float)CLOCK_TICKS;
            temp_scputimes.steal = std::stof(temp_value[8]) / (float)CLOCK_TICKS;
            temp_scputimes.guest = std::stof(temp_value[9]) / (float)CLOCK_TICKS;
            temp_scputimes.guest_nice = std::stof(temp_value[10]) / (float)CLOCK_TICKS;
            result.push_back(temp_scputimes);
        }

        stat.close();
    }
    return result;
}

unsigned short int cpu_count(bool logical)
{
    if (logical)
    {
        return sysconf(_SC_NPROCESSORS_ONLN);
    }
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
                  << ")";
}

std::ostream &operator<<(std::ostream &output, const scputimes &cputimes)
{
    return output << "scputimes(user=" << cputimes.user
                  << ", nice=" << cputimes.nice
                  << ", system=" << cputimes.system
                  << ", idle=" << cputimes.idle
                  << ", iowait=" << cputimes.iowait
                  << ", irq=" << cputimes.irq
                  << ", softirq=" << cputimes.softirq
                  << ", steal=" << cputimes.steal
                  << ", guest=" << cputimes.guest
                  << ", guest_nice=" << cputimes.guest_nice
                  << ")";
}

std::ostream &operator<<(std::ostream &output, const std::vector<scputimes> &cputimes)
{
    output << "{";
    for (int i = 0; i < cputimes.size() - 1; i++)
    {
        output << cputimes[i] << ", ";
    }
    output << cputimes[cputimes.size() - 1] << "}";
    return output;
}