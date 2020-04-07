#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
  	float active = LinuxParser::ActiveJiffies();
  	float tot = LinuxParser::Jiffies();
  	return (active - (float)active_)/(tot - (float)tot_);
}