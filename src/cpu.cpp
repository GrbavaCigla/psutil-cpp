#include <psutil-cpp/cpu.hpp>
#include <iostream>
#include <cassert>

#define MUTE_PSUTIL_LOGS

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
    if (dexists("/sys/devices/system/cpu/cpufreq/policy0/").value() || dexists("/sys/devices/system/cpu/cpu0/cpufreq/").value())
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

const std::string& get_proc_path() {
  static const std::string proc_path = std::getenv("PROC_PATH") ? std::getenv("PROC_PATH") : "/proc";
  return proc_path;
}

process::process(PID pid) {
  update(pid);
}

void process::reset() {
  pid = 0;
  name.clear();
  path.clear();
  cmdline_cmd.clear();
  exists = false;
  status = 0;
  ppid = 0;
  ttynr = 0;
  utime = 0;
  stime = 0;
  children_utime = 0;
  children_stime = 0;
  create_time = 0;
  cpu_num = 0;
  blkio_ticks = 0;
  iowait = 0;
}

void process::update(PID pid) {
  const auto &proc_path = get_proc_path();
  path = proc_path + "/" + std::to_string(pid) + "/stat";
  this->pid = pid;

  auto str = file_to_string(path);
  if (str.empty()) {
    std::cerr << "psutil: can not find process stat file with path " << path << std::endl;
    reset();
    this->pid = pid;
    return;
  }
  // TODO: optimize
  str = replace_all(str, "((", "(");
  str = replace_all(str, "))", ")");
  auto stat = split_by_delim(str, ")");
  assert(stat.size() == 2);
  auto stat2 = split_by_delim(stat[1], " ");

  PID proc_pid = std::stoi(stat[0]);
  if (pid != proc_pid) {
    std::cerr << "psutil: stat pid[" << proc_pid << "] != requested pid[" << pid << "]" << std::endl;
    reset();
    this->pid = pid;
    return;
  }

  cmdline_cmd = file_to_string(proc_path + "/" + std::to_string(pid) + "/cmdline");
  time = std::chrono::system_clock::now();
  name = split_by_delim(stat[0], "(")[1];
  status = stat2[1][0];
  ppid = std::stoi(stat2[2]);
  ttynr = std::stoi(stat2[5]);
  utime = std::stof(stat2[12]);
  stime = std::stof(stat2[13]);
  children_utime = std::stof(stat2[14]);
  children_stime = std::stof(stat2[15]);
  create_time = std::stof(stat2[20]);
  cpu_num = std::stoi(stat2[37]);
  blkio_ticks = std::stoi(stat2[40]);
  exists = true;

  // calc cpu times
  static const long kClockTicks = sysconf(_SC_CLK_TCK);
  utime = float(utime) / kClockTicks;
  stime = float(stime) / kClockTicks;
  children_utime = float(children_utime) / kClockTicks;
  children_stime = float(children_stime) / kClockTicks;
  iowait = float(blkio_ticks) / kClockTicks;
}

/**
  Return a float representing the current process CPU utilization as a percentage.

  When *interval* is 0.0 or None (default) compares process times
  to system CPU times elapsed since last call, returning
  immediately (non-blocking). That means that the first time
  this is called it will return a meaningful 0.0 value.

  When *interval* is > 0.0 compares process times to system CPU
  times elapsed before and after the interval (blocking).

  In this case is recommended for accuracy that this function
  be called with at least 0.1 seconds between calls.

  A value > 100.0 can be returned in case of processes running
  multiple threads on different CPU cores.

  The returned value is explicitly NOT split evenly between
  all available logical CPUs. This means that a busy loop process
  running on a system with 2 logical CPUs will be reported as
  having 100% CPU utilization instead of 50%.
*/
float process::get_cpu_percent(uint32_t interval_mlsec) {
  if (pid == 0) {
    std::cerr << "psutil: get_cpu_percent() can not be run. Process PID is [0]" << std::endl;
    return 0;
  }

  float stime1 = 0l, utime1 = 0, stime2 = 0, utime2 = 0;
  PTime time1, time2;
  if (interval_mlsec > 0) {
    update(pid);
    if (!exists) {
      return 0;
    }
    stime1 = stime;
    utime1 = utime;
    time1 = time;
    sleep_mlsec(interval_mlsec);
    update(pid);
    if (!exists) {
      return 0;
    }
    stime2 = stime;
    utime2 = utime;
    time2 = time;
  } else {
    if (last_utime == -1 || last_stime == -1) {
      last_stime = stime;
      last_utime = utime;
      last_time = time;
      return 0;
    }
    utime1 = last_utime;
    stime1 = last_stime;
    time1 = last_time;
    update(pid);
    if (!exists) {
      return 0;
    }
    utime2 = utime;
    stime2 = stime;
    time2 = time;
    last_stime = stime;
    last_utime = utime;
    last_time = time;
  }

  auto delta_proc = (utime2 - utime1) + (stime2 - stime1);
  delta_proc = delta_proc < 0 ? 0 : delta_proc;
  auto delta_mksec = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count();
  auto delta_time = static_cast<float>(delta_mksec / 1000000.0 * cpu_count()); // NOLINT(cppcoreguidelines-narrowing-conversions)
  if (delta_time < 1) {
#ifndef MUTE_PSUTIL_LOGS
    std::cerr << std::fixed <<  "pid={" << pid << "}  delta_time={" << delta_time << "} is too small\n";
#endif
    cpu_percent = 0;
    return cpu_percent;
  }
  auto overall_cpus_percent = delta_time == 0 ? 0 : ((delta_proc / delta_time) * 100);
  auto single_cpu_percent = overall_cpus_percent * (float) cpu_count();
  // TODO: round value
  cpu_percent = ceilf(single_cpu_percent * 100) / 100;

#ifndef MUTE_PSUTIL_LOGS
  if(single_cpu_percent > 0) {
    std::cout << "--------------------------------------------\n";
    std::cout << std::fixed <<  "pid={" << pid << "}  pt1.system={" << stime1 << "}  pt1.user={" << utime1 << "}\n";
    std::cout << std::fixed <<  "pid={" << pid << "}  pt2.system={" << stime2 << "}  pt2.user={" << utime2 << "}\n";
    auto tt1 = std::chrono::duration_cast<std::chrono::microseconds>(time1.time_since_epoch()).count();
    auto tt2 = std::chrono::duration_cast<std::chrono::microseconds>(time2.time_since_epoch()).count();
    std::cout << std::fixed <<  "st1={" << tt1 << "}  st2={" << tt2 << "}\n";
    std::cout << std::fixed <<  "delta_time={" << delta_time << "}\n";
    std::cout << std::fixed <<  "delta_proc={" << delta_proc << "}\n";
    std::cout << std::fixed << "single_cpu_percent={" << cpu_percent << "}\n";
  }
#endif

  return cpu_percent;
}