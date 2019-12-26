#include "psutil-cpp/linux/psutil.hpp"

int CLOCK_TICKS = sysconf(_SC_CLK_TCK);
int PAGESIZE = sysconf(_SC_PAGE_SIZE);

svmem virtual_memory()
{
    svmem result = svmem();
    std::map<std::string, unsigned long long> mems = parse_file("/proc/meminfo", true, ": ", 1024);

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
    sswap result = sswap();
    std::map<std::string, unsigned long long> mems = parse_file("/proc/meminfo", true, ": ", 1024);

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

    std::vector<std::string> temp_value;
    std::string line;

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

    if (stat.is_open())
    {
        if (percpu)
        {
            int counter = 0;
            bool done = false;
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

std::vector<scpufreq> cpu_freq(bool percpu)
{

    std::vector<scpufreq> result;
    scpufreq temp_cpufreq;
    if (dexists("/sys/devices/system/cpu/cpufreq/policy0/") || dexists("/sys/devices/system/cpu/cpu0/cpufreq/"))
    {
        std::string curr_path;

        for (int i = 0; i < cpu_count(); i++)
        {
            curr_path = get_path(i);
            if (fexists(curr_path + "scaling_cur_freq"))
            {
                temp_cpufreq.current = std::stof(cat(curr_path + "scaling_cur_freq")[0]) / 1024;
            }
            else
            {
                temp_cpufreq.current = std::stof(cat(curr_path + "cpuinfo_cur_freq")[0]) / 1024;
            }

            temp_cpufreq.min = std::stof(cat(curr_path + "cpuinfo_min_freq")[0]) / 1024;
            temp_cpufreq.max = std::stof(cat(curr_path + "cpuinfo_max_freq")[0]) / 1024;

            result.push_back(temp_cpufreq);
        }
    }
    return result;
}

unsigned short int cpu_count(bool logical)
{
    if (logical)
    {
        return sysconf(_SC_NPROCESSORS_ONLN); // TODO: Add fallback if this doesn't work!
    }
    return 0;
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

std::ostream &operator<<(std::ostream &output, const std::vector<scpufreq> &cpufreq)
{
    output << "{";
    for (int i = 0; i < cpufreq.size() - 1; i++)
    {
        output << cpufreq[i] << ", ";
    }
    output << cpufreq[cpufreq.size() - 1] << "}";
    return output;
}
