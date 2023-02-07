#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, std::string user, std::string command, float cpuUtilization, std::string ram, long int upTime) {
    _pid = pid;
    _user = user;
    _command = command;
    _cpuUtilization = cpuUtilization;
    _ram = ram;
    _upTime = upTime;
  }
    
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int _pid;                               // TODO: See src/process.cpp
  std::string _user;                      // TODO: See src/process.cpp
  std::string _command;                   // TODO: See src/process.cpp
  float _cpuUtilization;                  // TODO: See src/process.cpp
  std::string _ram;                       // TODO: See src/process.cpp
  long int _upTime;                       // TODO: See src/process.cpp
};

#endif