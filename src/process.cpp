#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return _pid; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  auto process_uptime = LinuxParser::UpTime(_pid);
  auto system_uptime = LinuxParser::UpTime();
  float utilization = (process_uptime * 1.0) / system_uptime;
  return utilization;
}

// DONE: Return the command that generated this process
string Process::Command() { return _command; }

// DONE: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(_pid); }

// DONE: Return the user (name) that generated this process
string Process::User() { return _user; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

// DONE: Overload the "less than" comparison operator for Process objects
// DONE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return std::stof(a.Ram()) < std::stof(Ram());
}