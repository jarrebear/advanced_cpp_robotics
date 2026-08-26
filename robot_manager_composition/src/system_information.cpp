#include "robot_manager_composition/system_information.hpp"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>

#include <sys/sysinfo.h>

void ComputerUnit::print_info() {
  printf("Total CPU usage: %.2f%%\n", cpu_sys_usage);

  try {
    mem_usage_percentage = static_cast<double>(mem_usage) /
                           static_cast<double>(max_memory) * 100.0;

    printf("Total memory usage: %.2f%%\n", mem_usage_percentage);
  } catch (const std::logic_error &e) {
    std::cerr << e.what() << std::endl;
  }
}

struct SystemInformation::PImpl : public ISystemInformation {
public:
  PImpl() {
    // Read the first CPU statistics
    readCpuStats(m_lastTotalUser, m_lastTotalUserLow, m_lastTotalSys,
                 m_lastTotalIdle);
  }

  ~PImpl() override = default;

  int64_t GetTotalMemory() override {
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    long long totalPhysMem = memInfo.totalram;
    totalPhysMem *= memInfo.mem_unit;

    return totalPhysMem;
  }

  int64_t GetTotalUsageMemory() override {
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    long long physMemUsed = memInfo.totalram - memInfo.freeram;

    physMemUsed *= memInfo.mem_unit;

    return physMemUsed;
  }

  double GetCpuTotalUsage() override {
    // Wait a short time between two CPU samples
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    unsigned long long totalUser;
    unsigned long long totalUserLow;
    unsigned long long totalSys;
    unsigned long long totalIdle;

    readCpuStats(totalUser, totalUserLow, totalSys, totalIdle);

    unsigned long long userDiff = totalUser - m_lastTotalUser;

    unsigned long long userLowDiff = totalUserLow - m_lastTotalUserLow;

    unsigned long long sysDiff = totalSys - m_lastTotalSys;

    unsigned long long idleDiff = totalIdle - m_lastTotalIdle;

    unsigned long long totalDiff = userDiff + userLowDiff + sysDiff + idleDiff;

    // Save current values for the next call
    m_lastTotalUser = totalUser;
    m_lastTotalUserLow = totalUserLow;
    m_lastTotalSys = totalSys;
    m_lastTotalIdle = totalIdle;

    if (totalDiff == 0) {
      return 0.0;
    }

    double cpuUsage = static_cast<double>(userDiff + userLowDiff + sysDiff) /
                      static_cast<double>(totalDiff);

    return cpuUsage * 100.0;
  }

private:
  void readCpuStats(unsigned long long &user, unsigned long long &userLow,
                    unsigned long long &sys, unsigned long long &idle) {
    FILE *file = fopen("/proc/stat", "r");

    if (file == nullptr) {
      user = userLow = sys = idle = 0;
      return;
    }

    fscanf(file, "cpu %llu %llu %llu %llu", &user, &userLow, &sys, &idle);

    fclose(file);
  }

private:
  unsigned long long m_lastTotalUser = 0;
  unsigned long long m_lastTotalUserLow = 0;
  unsigned long long m_lastTotalSys = 0;
  unsigned long long m_lastTotalIdle = 0;
};

SystemInformation::SystemInformation() { m_impl = new PImpl(); }

SystemInformation::~SystemInformation() { delete m_impl; }

int64_t SystemInformation::GetTotalMemory() { return m_impl->GetTotalMemory(); }

int64_t SystemInformation::GetTotalUsageMemory() {
  return m_impl->GetTotalUsageMemory();
}

double SystemInformation::GetCpuTotalUsage() {
  return m_impl->GetCpuTotalUsage();
}