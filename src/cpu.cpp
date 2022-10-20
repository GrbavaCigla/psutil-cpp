#include "psutil-cpp/cpu.hpp"

std::optional<std::vector<scputimes>> cpu_times(bool percpu)
{
#ifdef linux
    int CLOCK_TICKS = sysconf(_SC_CLK_TCK);

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
#endif
    return std::nullopt;
}

std::optional<std::vector<scpufreq>> cpu_freq(bool percpu)
{
#ifdef linux
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
#endif
    return std::nullopt;
}

unsigned short int cpu_count(bool logical)
{
    if (logical)
    {
        return sysconf(_SC_NPROCESSORS_ONLN); // TODO: Add fallback if this doesn't work!
    }
    return 0;
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

std::ostream &operator<<(std::ostream &output, const scpufreq &cpufreq)
{
    return output << "scpufreq(current=" << cpufreq.current
                  << ", min=" << cpufreq.min
                  << ", max=" << cpufreq.max
                  << ")";
}