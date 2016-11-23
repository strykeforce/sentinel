#pragma once

#include "WPILib.h"

#define CATCH_CONFIG_RUNNER
#include "catch/catch.hpp"

namespace sentinel {

class Sentinel : public ::SampleRobot {
 private:
  Catch::Session session_;

 public:
  Sentinel();
  Sentinel(const Sentinel&) = delete;
  Sentinel& operator=(const Sentinel&) = delete;
  virtual ~Sentinel() = default;

  virtual void RobotInit() override;
  virtual void Disabled() override;
  virtual void OperatorControl() override;
};

} /* sentinel */
