#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {    
  float total = 0.0;
  float free = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key;
      float value;      
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {          
          total = value;
        }

        if (key == "MemFree") {          
          free = value;
        }        
      }
    }
  }
  
  return ((total - free) / total);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {   
  long uptime = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
      string line;
      std::getline(filestream, line);  
      std::istringstream linestream(line);
      linestream >> uptime;
    }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return Pids().size(); 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int count = 0; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key;
      float value;      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          count = value;   
        }        
      }
    }    
  }

  return count;
}

// DONE: Read and return the command associated with a process
// DONE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string command = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" + kCmdlineFilename);
  if (filestream.is_open()) {
      string line;
      std::getline(filestream, line);  
      std::istringstream linestream(line);
      linestream >> command;
    }

  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// DONE: Read and return the uptime of a process
// DONE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long uptime = 0;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatFilename);
  if (filestream.is_open()) {
      string line;
      long unsigned utime = 0;
      long unsigned stime = 0 ;
      long unsigned cutime = 0 ;
      long unsigned cstime = 0 ;
      string ignore;
      std::getline(filestream, line);
      std::istringstream linestream(line);

      // pattern: "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %lu %lu"
      for (int i = 1 ; i <= 13; i++) {
        linestream >> ignore;
      }

      linestream >> utime >> stime >> cutime >> cstime;
      uptime = (utime + stime + cutime + cstime)/sysconf(_SC_CLK_TCK);      
  }

  return uptime;
}

