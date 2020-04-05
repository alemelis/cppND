#include "linux_parser.h"

#include <dirent.h>
#include <math.h>
#include <unistd.h>

#include <iostream>
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
  long user, nice, sys, idle, iowait, irq, softirq, steal;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> sys >> idle >> iowait >>
             irq >> softirq >> steal) {
        if (kind_of_jiffies == "total") {
          return user + nice + sys + irq + softirq + steal + idle + iowait;
        } else if (kind_of_jiffies == "idle") {
          return idle + iowait;
        } else if (kind_of_jiffies == "active") {
          return user + nice + sys + irq + softirq + steal;
        }
      }
    }
  }
  return 0;
}

// http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
// https://www.cplusplus.com/reference/string/stol/
long parseStat(string filename, int idx) {
  string line;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  std::stringstream ss(line);
  std::string token;
  std::vector<string> vec;
  int counter = 0;
  while (getline(ss, token, ' ')) {
    if (counter == idx) {
      return std::stol(token);
    }
    counter++;
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

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return readMultiLiner<int>(kProcDirectory + kStatFilename, "processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return readMultiLiner<int>(kProcDirectory + kStatFilename, "procs_running");
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) {
  long utime =
      parseStat(kProcDirectory + std::to_string(pid) + kStatFilename, 13);
  long stime =
      parseStat(kProcDirectory + std::to_string(pid) + kStatFilename, 14);
  long total_time = (utime + stime) / sysconf(_SC_CLK_TCK);
  long seconds = LinuxParser::UpTime(pid);
  float cpu = 100.0 * total_time / seconds;
  return cpu;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  return readOneLiner<string>(kProcDirectory + std::to_string(pid) +
                              kCmdlineFilename);
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  int vmem = readMultiLiner<int>(
      kProcDirectory + std::to_string(pid) + kStatusFilename, "VmSize:");
  char buff[100];
  snprintf(buff, sizeof(buff), "%6.2f", vmem / 1000.0);
  string ram = buff;
  return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  int uid = readMultiLiner<int>(
      kProcDirectory + std::to_string(pid) + kStatusFilename, "Uid:");
  return std::to_string(uid);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// Read and return the uptime of a process
// http://man7.org/linux/man-pages/man5/proc.5.html
long LinuxParser::UpTime(int pid) {
  long val =
      parseStat(kProcDirectory + std::to_string(pid) + kStatFilename, 21);
  return LinuxParser::UpTime() - val / sysconf(_SC_CLK_TCK);
}
