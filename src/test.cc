#include "catch/catch.hpp"

#include "WPILib.h"
#include "spdlog/spdlog.h"

TEST_CASE("potentiometer voltage in range", "[test]") {
  auto logger = spdlog::get("sentinel");
  ::AnalogInput ai{0};
  auto v = ai.GetVoltage();
  logger->info("voltage = {}", v);
  CHECK(v == Approx(3.0).epsilon(0.1));
}
