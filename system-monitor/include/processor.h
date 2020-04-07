#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  Processor() {
    jiffies_ = LinuxParser::Jiffies();
    tot_ = ComputeTotalJiffies_();
    idle_ = ComputeIdleJiffies_();
  }
  float Utilization();

 private:
  std::vector<float> jiffies_;
  float ComputeTotalJiffies_();
  float ComputeIdleJiffies_();
  float tot_;
  float idle_;
};

#endif