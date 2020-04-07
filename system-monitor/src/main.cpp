#include <iostream>

#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  //   std::cout << "Active Jiffies: " << LinuxParser::ActiveJiffies() << "\n";
  // std::cout << "Total Jiffies: " << LinuxParser::Jiffies() << "\n";

  // std::cout << "CPU Utilization: " << system.Cpu().Utilization() << "\n";

  // std::cout <<
  // NCursesDisplay::ProgressBar(system.Cpu().Utilization()).c_str() << "\n";
  NCursesDisplay::Display(system);
}