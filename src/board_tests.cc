#include "board_tests.h"

#include <vector>
#include "WPILib.h"

#include "test.h"

using namespace sentinel;
using namespace std;

vector<TestCase> tests = {
    // name, channel, actual, expected, epsilon,
    {
        "TestPotentiometerFail", 0, 0.0, 3.0, 0.1,
    },
    {
        "TestPotentiometerPass", 0, 0.0, 2.8, 0.1,
    },
};

void sentinel::RunTests(Test& test) {
  for (auto& tc : tests) {
    ::AnalogInput ai(tc.channel);
    tc.actual = ai.GetVoltage();
    if (abs(tc.actual - tc.expected) <= tc.epsilon) {
      test.Pass(tc);
    } else {
      test.Fail(tc);
    }
  }
}
