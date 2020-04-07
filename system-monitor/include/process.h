#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

class Process {
 public:
  Process(int pid) : pid_(pid) {
    if (pid_ == 0) {
      cmd_ = std::string();
      uid_ = std::string();
      user_ = std::string();
      uptime_ = 0;
      cpu_ = 0.0;
    } else {
      cmd_ = LinuxParser::Command(pid_);
      uid_ = LinuxParser::Uid(pid_);
      user_ = LinuxParser::User(pid_);
      uptime_ = LinuxParser::UpTime(pid_);
      cpu_ = LinuxParser::CpuUtilization(pid_);
    }
  }

  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_;
  std::string cmd_;
  std::string uid_;
  std::string user_;
  long uptime_;
  float cpu_;
};

#endif