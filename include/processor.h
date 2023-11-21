#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  void Utilization(std::vector<std::string> utilization);

  // DONE: Declare any necessary private members
 private:
  float _utilization;
  float _user = 0;
  float _nice = 0;
  float _system = 0;
  float _idle = 0;
  float _iowait = 0;
  float _irq = 0;
  float _softirq = 0;
  float _steal = 0;
};

#endif