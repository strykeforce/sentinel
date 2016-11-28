
#include "WPILib.h"

#include "board_tests.h"
#include "test.h"

namespace sentinel {
namespace robot {
class Sentinel : public ::SampleRobot {
 private:
  /* data */

 public:
  Sentinel() {}

  void RobotInit() {}

  void Disabled() {}

  void OperatorControl() {
    sentinel::Test test;
    BoardTests uut;
    uut.TestPotentiometerFail(test);
    uut.TestPotentiometerPass(test);
    test.Log();
  }
};

} /* robot */
} /* sentinel */

START_ROBOT_CLASS(sentinel::robot::Sentinel)
