#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { 
  return _pid; 
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
  return _cpuUtilization; 
}

// DONE: Return the command that generated this process
string Process::Command() { 
  return _command; 
}

// DONE: Return this process's memory utilization
string Process::Ram() { 
  return _ram; 
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
  return _user; 
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return _upTime; 
}

// DONE: Overload the "less than" comparison operator for Process objects
// DONE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return std::stof(a._ram) < std::stof(_ram); 
}