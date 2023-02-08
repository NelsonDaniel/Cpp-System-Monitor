#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

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

// DONE: Read and return the system memory utilization
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
  return (ActiveJiffies() + IdleJiffies()) / sysconf(_SC_CLK_TCK);
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return UpTime() * sysconf(_SC_CLK_TCK);
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVED: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
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

      uptime = (utime + stime + cutime + cstime);      
  }

  return uptime ;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long activeJiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      string line;
      long value;
      std::getline(filestream, line);
      std::istringstream linestream(line);

      // first ten values are part of active jiffies
      for (int i = 1 ; i <= 10; i++) {
        linestream >> value;
        activeJiffies += value;    
      }    
  }

  return activeJiffies; 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idleJiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      string line;
      long ignore;
      long idle;
      long iowait;
      std::getline(filestream, line);
      std::istringstream linestream(line);

      for (int i = 1 ; i <= 3; i++) {
        linestream >> ignore;
      }

      linestream >> idle >> iowait;

      idleJiffies = idle + iowait;      
  }

  return idleJiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> utilization; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key;   
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          for (int i = 0; i < 10; i++) {            
            string value;   
            linestream >> value;
            utilization.push_back(value);
          }            
        }        
      }
    }    
  }

  return utilization;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int count = 0; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key;
      float value;      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          count = value;   
        }        
      }
    }    
  }

  return count;
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

// DONE: Read and return the memory used by a process
// DONE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { 
  string ram = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatusFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key;
      double value;      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          std::stringstream stream;
          stream << std::fixed << std::setprecision(3) << value / 1000.0; 
          ram = stream.str();
        }        
      }
    }    
  }

  return ram; 
}

// DONE: Read and return the user ID associated with a process
// DONE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string uid = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatusFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key;
      string value;      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;   
        }        
      }
    }    
  }

  return uid;  
}

// DONE: Read and return the user associated with a process
// DONE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string username = "";
  std::ifstream filestream("/etc/passwd");
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string current_username;
      string ignore; 
      string current_uid;     
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);      
      while (linestream >> current_username >> ignore >> current_uid) {
        if (current_uid == Uid(pid)) {
          username = current_username;   
        }        
      }
    }    
  }

  return username; 
}

// DONE: Read and return the uptime of a process
// DONE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {   
  return ActiveJiffies(pid) / sysconf(_SC_CLK_TCK);
}

