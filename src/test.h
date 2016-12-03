#pragma once

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

namespace sentinel {

struct TestCase {
  std::string name;
  uint32_t channel;
  double actual, expected, epsilon;
};

/** Logs test results.
 */
class Test {
 public:
  Test();
  Test(const Test&) = delete;
  Test& operator=(const Test&) = delete;
  virtual ~Test() = default;

  void Pass(const TestCase& tc);
  void Fail(const TestCase& tc);
  void Log();

 private:
  nlohmann::json j_;
  std::fstream out_;
  bool success_ = true;
};
} /* sentinel */
