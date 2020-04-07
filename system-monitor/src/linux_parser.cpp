#include "linux_parser.h"

#include <dirent.h>
#include <math.h>
#include <unistd.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T readMultiLiner(string filename, string key_str) {
  string line, key;
  T value{0};
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
  return value;
}

template <typename T>
T readOneLiner(string filename) {
  T value{0};
  string line;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

// http://man7.org/linux/man-pages/man5/proc.5.html
// http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
// https://www.cplusplus.com/reference/string/stol/
long getStatAtId(int pid, int idx) {
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  std::stringstream ss(line);
  std::string token;
  int count = 0;
  while (getline(ss, token, ' ')) {
    if (count == idx) {
      return std::stol(token);
    }
    count++;
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

// Read and return the jiffies for the system
std::vector<float> LinuxParser::Jiffies() {
  string line, key;
  std::vector<float> jiffies;
  string user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> sys >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
    if (key == "cpu") {
      jiffies = {std::stof(user),      std::stof(nice),   std::stof(sys),
                 std::stof(idle),      std::stof(iowait), std::stof(irq),
                 std::stof(softirq),   std::stof(steal),  std::stof(guest),
                 std::stof(guest_nice)};
    }
  }
  return jiffies;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return readMultiLiner<int>(kProcDirectory + kStatFilename, "processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return readMultiLiner<int>(kProcDirectory + kStatFilename, "procs_running");
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  return readOneLiner<string>(kProcDirectory + std::to_string(pid) +
                              kCmdlineFilename);
}

// Read and return the memory used by a process
// https://stackoverflow.com/a/29200671
string LinuxParser::Ram(int pid) {
  float ram =
      readMultiLiner<float>(
          kProcDirectory + std::to_string(pid) + kStatusFilename, "vmSize:") /
      1000.0;
  std::stringstream stream;
  stream << std::fixed << std::setprecision(3) << ram;
  std::string s = stream.str();
  return s;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return std::to_string(readMultiLiner<int>(
      kProcDirectory + std::to_string(pid) + kStatusFilename, "Uid:"));
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user_id = LinuxParser::Uid(pid);

  string line, x, uid, user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (uid == user_id) {
          return user;
        }
      }
    }
  }
  return "?";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  if (pid == 0) {
    return 0;
  }
  long stat21 = getStatAtId(pid, 21);
  return LinuxParser::UpTime() - stat21 / sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid) {
  if (pid == 0) {
    return 0;
  }
  long stat13 = getStatAtId(pid, 13);
  long stat14 = getStatAtId(pid, 14);
  long stat15 = getStatAtId(pid, 15);
  long stat16 = getStatAtId(pid, 16);

  float tot_time = (float)(stat13 + stat14 + stat15 + stat16);
  float seconds = LinuxParser::UpTime(pid);

  return (tot_time / sysconf(_SC_CLK_TCK)) / seconds;
}