#include "sentinel.h"

#include <iostream>
#include "WPILib.h"
#include "catch/catch.hpp"

using namespace sentinel;

Sentinel::Sentinel() {}

void Sentinel::RobotInit() {}
void Sentinel::Disabled() {
  std::cout << "Disable to reset, Enable to run tests.\n" << std::endl;
}
void Sentinel::OperatorControl() { session_.run(); }

START_ROBOT_CLASS(Sentinel)
