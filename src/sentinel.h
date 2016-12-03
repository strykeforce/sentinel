#include "WPILib.h"

#include "board_tests.h"

namespace sentinel {

class Sentinel : public ::SampleRobot {
 public:
  static ::CANTalon* uut_talon;
  static ::CANTalon* sensor_talon;

  Sentinel();
  void RobotInit() override;
  void Disabled() override;
  void OperatorControl() override;
  //
  //  private:
  //   BoardTests uut_;
};

} /* sentinel */
