#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <chrono>

#include "now.hpp"

struct logger_t {
  std::ofstream ofs;
  
  auto info(const std::string & msg)
  {
    ofs << now_utc_string_get() << " | " << msg << "\n";
    ofs.flush();
  }
};

#endif //LOGGER_HPP
