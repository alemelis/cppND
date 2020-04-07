#include "linux_parser.h"

#include <dirent.h>
#include <math.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T readMultiLiner(string filename, string key_str) {
  string line, key;
  T value;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == key_str) {
          return value;
        }
      }
    }
  }
  return T{0};
}

template <typename T>
T readOneLiner(string filename) {
  T value;
  string line;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

long parseJiffies(string filename, string kind_of_jiffies) {
  string line, key;
  long user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> sys >> idle >> iowait >>
             irq >> softirq >> steal >> guest >> guest_nice) {
        if (kind_of_jiffies == "total") {
          return user + nice + sys + irq + softirq + steal + idle + iowait +
                 guest + guest_nice;
        } else if (kind_of_jiffies == "idle") {
          return idle + iowait;
        } else if (kind_of_jiffies == "active") {
          return user + nice + sys + irq + softirq + steal + guest + guest_nice;
        }
      }
    }
  }
  return 0;
}

// An example of how to read data from the filesystem
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

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memtotal =
      readMultiLiner<float>(kProcDirectory + kMeminfoFilename, "MemTotal:");
  float memfree =
      readMultiLiner<float>(kProcDirectory + kMeminfoFilename, "MemFree:");

  return (memtotal - memfree) / memtotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  return std::round(readOneLiner<long>(kProcDirectory + kUptimeFilename));
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return parseJiffies(kProcDirectory + kStatFilename, "total");
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return parseJiffies(kProcDirectory + kStatFilename, "active");
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  return parseJiffies(kProcDirectory + kStatFilename, "idle");
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return readMultiLiner<int>(kProcDirectory + kStatFilename, "processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return readMultiLiner<int>(kProcDirectory + kStatFilename, "procs_running");
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { return readOneLiner<string>(kProcDirectory + std::to_string(pid) + kCmdlineFilename); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }