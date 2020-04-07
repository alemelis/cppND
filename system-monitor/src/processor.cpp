#include "processor.h"

#include <math.h>

#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  jiffies_ = LinuxParser::Jiffies();
  float tot = ComputeTotalJiffies_();
  float idle = ComputeIdleJiffies_();
  float cpu = (tot - tot_ - idle + idle_) / (tot - tot_);
  if (std::isnan(cpu)) {
    return 0.0;
  }
  return cpu;
}

// https://stackoverflow.com/questions/3221812/how-to-sum-up-elements-of-a-c-vector
float Processor::ComputeTotalJiffies_() {
  float tot;
  for (float& n : jiffies_) tot += n;
  return tot;
}

// https://stackoverflow.com/questions/3221812/how-to-sum-up-elements-of-a-c-vector
float Processor::ComputeIdleJiffies_() {
  return jiffies_[LinuxParser::kIdle_] + jiffies_[LinuxParser::kIOwait_];
}