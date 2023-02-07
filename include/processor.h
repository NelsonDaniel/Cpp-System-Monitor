#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float _user = 0;
  float _nice = 0;
  float _system = 0;
  float _idle = 0;
  float _iowait = 0;
  float _irq = 0;
  float _softirq = 0;
  float _steal = 0;
  float _guest = 0;
  float _guest_nice =0;  
};

#endif