#include "board_tests.h"

#include "WPILib.h"

#include "test.h"

using namespace sentinel;

BoardTests::BoardTests() {}

void BoardTests::TestPotentiometerFail(Test& test) {
  ::AnalogInput ai(0);
  double actual = ai.GetVoltage();
  double expected = 3.0;
  double epsilon = 0.1;
  if (std::abs(actual - expected) < epsilon) {
    test.Pass("TestPotentiometerFail", actual, expected, epsilon);
  } else {
    test.Fail("TestPotentiometerFail", actual, expected, epsilon);
  }
}

void BoardTests::TestPotentiometerPass(Test& test) {
  ::AnalogInput ai(0);
  double actual = ai.GetVoltage();
  double expected = 2.8;
  double epsilon = 0.1;
  if (std::abs(actual - expected) < epsilon) {
    test.Pass("TestPotentiometerPass", actual, expected, epsilon);
  } else {
    test.Fail("TestPotentiometerPass", actual, expected, epsilon);
  }
}
