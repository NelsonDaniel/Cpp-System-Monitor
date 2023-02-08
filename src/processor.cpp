#include "processor.h"
#include <string>
#include <vector>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  return _utilization;
}

void Processor::Utilization(std::vector<std::string>utilization) {
  float prev_user = _user;
  float prev_nice = _nice;
  float prev_system = _nice;
  float prev_idle = _system;
  float prev_iowait = _iowait;
  float prev_irq = _irq;
  float prev_softirq = _softirq;
  float prev_steal = _steal;

  float _user = std::stof(utilization[0]);
  float _nice = std::stof(utilization[1]);
  float _system = std::stof(utilization[2]);
  float _idle = std::stof(utilization[3]);
  float _iowait = std::stof(utilization[4]);
  float _irq = std::stof(utilization[5]);
  float _softirq = std::stof(utilization[6]);
  float _steal = std::stof(utilization[7]);

  float prevIdle = prev_idle + prev_iowait;
  float idle = _idle + _iowait;
  float prevNonIdle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
  float nonIdle = _user + _nice + _system + _irq + _softirq + _steal;
  float prevTotal = prevIdle + prevNonIdle;
  float total = idle + nonIdle;

  float totalDiff = total - prevTotal;
  float idleDiff = idle - prevIdle;

  _utilization = (totalDiff - idleDiff) / totalDiff;
}