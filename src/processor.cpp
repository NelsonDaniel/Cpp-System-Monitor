#include "processor.h"

#include <string>
#include <vector>

// Return the aggregate CPU utilization
float Processor::Utilization() {
  // Returns the calculated CPU utilization value
  return _utilization;
}

// Update the CPU utilization based on the current utilization vector
void Processor::Utilization(std::vector<std::string> utilization) {
  // Store previous CPU times for calculation
  auto prev_user = _user;
  auto prev_nice = _nice;
  auto prev_system = _system;
  auto prev_idle = _idle;
  auto prev_iowait = _iowait;
  auto prev_irq = _irq;
  auto prev_softirq = _softirq;
  auto prev_steal = _steal;

  // Update current CPU times from the passed utilization vector
  _user = std::stof(utilization[0]);
  _nice = std::stof(utilization[1]);
  _system = std::stof(utilization[2]);
  _idle = std::stof(utilization[3]);
  _iowait = std::stof(utilization[4]);
  _irq = std::stof(utilization[5]);
  _softirq = std::stof(utilization[6]);
  _steal = std::stof(utilization[7]);

  // Calculate total idle and non-idle times in the previous and current state
  auto prevIdle = prev_idle + prev_iowait;
  auto idle = _idle + _iowait;
  auto prevNonIdle = prev_user + prev_nice + prev_system + prev_irq +
                     prev_softirq + prev_steal;
  auto nonIdle = _user + _nice + _system + _irq + _softirq + _steal;

  // Calculate total and difference in total and idle times
  auto prevTotal = prevIdle + prevNonIdle;
  auto total = idle + nonIdle;
  auto totalDiff = total - prevTotal;
  auto idleDiff = idle - prevIdle;

  // Calculate the CPU utilization
  _utilization = (totalDiff - idleDiff) / totalDiff;
}
