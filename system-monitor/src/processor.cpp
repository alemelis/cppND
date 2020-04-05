#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// Return the aggregate CPU utilization
float Processor::Utilization() { 
	long active_jiffies = LinuxParser::ActiveJiffies();
	long tot_jiffies = LinuxParser::Jiffies();
	float cpu = ((float) active_jiffies - prev_active_jiffies_) / ((float) tot_jiffies - prev_tot_jiffies_);
	prev_active_jiffies_ = active_jiffies;
	prev_tot_jiffies_ = tot_jiffies;
	return cpu;
}