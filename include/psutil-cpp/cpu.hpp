#pragma once
#include <vector>
#include <ostream>
#include <optional>
#include <chrono>

#include "psutil-cpp/utils.hpp"

#ifdef linux
#include <unistd.h>
#endif

using PID = uint32_t;
using PTime = std::chrono::system_clock::time_point;

/**
 * Represents single system process and CPU related data for this process
 */
struct process {
  process() = default;

  explicit process(PID pid);

  PID pid{0};                    // field[0]
  std::string name;              // field[1]
  std::string path;
  std::string cmdline_cmd;
  bool exists{false};
  PTime time;
  char status{0};                // field[2]
  PID ppid{0};                   // field[3]
  uint32_t ttynr{0};             // field[6]
  float utime{0};                // field[13]
  float stime{0};                // field[14]
  float children_utime{0};       // field[15]
  float children_stime{0};       // field[16]
  float create_time{0};          // field[21]
  uint8_t cpu_num{0};            // field[38]
  uint32_t blkio_ticks{0};       // field[41]  # aka 'delayacct_blkio_ticks'
  float iowait{0};               // calculated
  float cpu_percent{0};          // calculated
  // stores previous values
  float last_stime{-1};
  float last_utime{-1};
  PTime last_time;

  void reset();

  void update(PID pid);

  float get_cpu_percent(uint32_t interval_mlsec = 0);
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