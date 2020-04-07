#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
 	Processor() {
 		tot_ = LinuxParser::Jiffies();
 		active_ = LinuxParser::ActiveJiffies();
 	}
  float Utilization();

 private:
 	long tot_;
 	long active_;
};

#endif