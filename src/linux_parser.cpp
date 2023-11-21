#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::atof;
using std::string;
using std::to_string;
using std::vector;

// DONE: Added comments to the example on how to read data from the filesystem
// Retrieve the operating system name from the filesystem
string LinuxParser::OperatingSystem() {
  // Variables to hold the line read from file, key and value pairs
  string line, key, value;
  std::ifstream filestream(kOSPath);  // Open the file containing OS info
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // Replace certain characters for parsing
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      // Read key-value pairs
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;  // Return the pretty name of the OS
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
// Retrieve the kernel version from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);  // Open version file
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >>
        kernel;  // Parse os, version, and kernel info
  }
  return kernel;  // Return the kernel version
}

// BONUS: Update this to use std::filesystem
// Get a list of all process IDs
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());  // Open /proc directory
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {  // Check if the entry is a directory
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(),
                      isdigit)) {  // Check if directory name is a digit
        int pid = stoi(filename);
        pids.push_back(pid);  // Add the PID to the list
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float total = 0.0, free = 0.0;
  std::ifstream filestream(kProcDirectory +
                           kMeminfoFilename);  // Open memory info file
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key;
      float value;
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      // Parse total and free memory
      while (linestream >> key >> value) {
        if (key == "MemTotal") total = value;
        if (key == "MemFree") free = value;
      }
    }
  }
  return ((total - free) / total);  // Calculate utilization
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  return Jiffies() / sysconf(_SC_CLK_TCK);
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long totalJiffies = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    string line;
    long totalSeconds;
    long ignore;
    long idleSeconds;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> totalSeconds;
    linestream >> ignore;
    linestream >> idleSeconds;
    totalJiffies = totalSeconds * sysconf(_SC_CLK_TCK);
  }

  return totalJiffies;
}

// DONE: Read and return the number of active jiffies for a PID
// Calculate the number of active jiffies (time measurements) for a given PID
long LinuxParser::ActiveJiffies(int pid) {
  long active_jiffies = 0;
  // Open the file that contains stats for the given PID
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" +
                           kStatFilename);
  if (filestream.is_open()) {
    string line;
    long unsigned utime, stime, cutime, cstime;
    string ignore;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    // Skip initial values in the line, focus on the ones representing time
    for (int i = 1; i <= 13; i++) {
      linestream >> ignore;
    }
    // Read user and system times for the process and its children
    linestream >> utime;
    linestream >> stime;
    linestream >> cutime;
    linestream >> cstime;

    active_jiffies = (utime + stime + cutime + cstime);
  }

  return active_jiffies;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long activeJiffies = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    string line;
    long totalSeconds;
    long ignore;
    long idleSeconds;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> totalSeconds;
    linestream >> ignore;
    linestream >> idleSeconds;
    activeJiffies = (totalSeconds - idleSeconds)*sysconf(_SC_CLK_TCK);
  }

  return activeJiffies;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idleJiffies = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    string line;
    long totalSeconds;
    long idleSeconds;
    long ignore;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> totalSeconds;
    linestream >> ignore;
    linestream >> idleSeconds;
    idleJiffies = idleSeconds * sysconf(_SC_CLK_TCK);
  }

  return idleJiffies;
}

// DONE: Read and return CPU utilization
// Read and return CPU utilization information
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
          // Collect the 10 values representing CPU utilization
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
string LinuxParser::Command(int pid) {
  string command = "";
  // Open the file containing the command line for the process
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);  // Read the command line
    std::istringstream linestream(line);
    linestream >> command;  // Extract the command
  }
  return command;
}


// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string ram = "0.0";
  // Open the file containing the process status
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" +
                           kStatusFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key;
      double value;
      std::istringstream linestream(line);
      // Parse the VmSize (Virtual Memory Size) value
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          std::stringstream stream;
          stream << std::fixed << std::setprecision(3)
                 << value / 1000.0;  // Convert from KB to MB
          ram = stream.str();
        }
      }
    }
  }
  return ram;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid = "";
  // Open the file containing the process status
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" +
                           kStatusFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string key, value;
      std::istringstream linestream(line);
      // Parse the Uid value
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;  // Extract the user ID
        }
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string username = "";
  // Open the system's password file to get user information
  std::ifstream filestream("/etc/passwd");
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      string current_username, ignore, current_uid;
      std::replace(line.begin(), line.end(), ':',
                   ' ');  // Replace ':' for parsing
      std::istringstream linestream(line);
      // Parse username and UID from the file
      while (linestream >> current_username >> ignore >> current_uid) {
        if (current_uid == Uid(pid)) {
          username = current_username;  // Match UID with the process's UID
        }
      }
    }
  }
  return username;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long starttime = 0;
  // Open the file containing the process stats
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" +
                           kStatFilename);
  if (filestream.is_open()) {
    string line, ignore;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    // Skip initial values to focus on the start time (22nd value)
    for (int i = 1; i <= 21; i++) {
      linestream >> ignore;
    }

    linestream >> starttime;  // Read the start time of the process
  }

  auto system_uptime = LinuxParser::UpTime();  // Get system uptime
  auto starttime_seconds =
      starttime / sysconf(_SC_CLK_TCK);  // Convert starttime to seconds
  auto uptime = system_uptime - starttime_seconds;  // Calculate process uptime

  return uptime;
}