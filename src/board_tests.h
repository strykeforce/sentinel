#pragma once

#include "test.h"

namespace sentinel {

class BoardTests {
 private:
  /* data */

 public:
  BoardTests();
  virtual ~BoardTests() = default;

  void TestPotentiometerFail(Test& test);
  void TestPotentiometerPass(Test& test);
};
} /* sentinel */
