#pragma once

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

namespace sentinel {

/** Logs test results.
 */
class Test {
 public:
  Test();
  virtual ~Test() = default;
  void Pass(std::string msg, double actual, double expected, double epsilon);
  void Fail(std::string msg, double actual, double expected, double epsilon);
  void Log();

 private:
  nlohmann::json j_;
  std::fstream out_;
  bool success_ = true;
};
} /* sentinel */
