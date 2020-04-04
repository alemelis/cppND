#include "format.h"

#include <math.h>

#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
string Format::ElapsedTime(long seconds) {
  int h, m, s;
  h = std::floor(seconds / (60 * 60));
  m = std::floor(seconds / 60 - h * 60);
  s = seconds - h * 60 * 60 - m * 60;
  char buff[100];
  snprintf(buff, sizeof(buff), "%02d:%02d:%02d", h, m, s);
  string time = buff;
  return time;
}
