#ifndef NOW_HPP
#define NOW_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

std::string now_utc_string_get()
{
  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);
  
  std::ostringstream oss;
  oss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%SZ");
  
  return oss.str();
}

#endif //NOW_HPP
