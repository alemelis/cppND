#include "processor.h"

#include <iostream>

#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  long active_jiffies = LinuxParser::ActiveJiffies();
  long tot_jiffies = LinuxParser::Jiffies();
  float cpu = ((float)active_jiffies) / ((float)tot_jiffies);
  prev_active_jiffies_ = active_jiffies;
  prev_tot_jiffies_ = tot_jiffies;
  return cpu;
}