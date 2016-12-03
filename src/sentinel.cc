#include "sentinel.h"

#include "WPILib.h"
#include "cpptoml/cpptoml.h"
#include "spdlog/spdlog.h"
#include "talon/settings.h"

#include "board_tests.h"
#include "test.h"

using namespace sentinel;

::CANTalon* Sentinel::uut_talon = nullptr;
::CANTalon* Sentinel::sensor_talon = nullptr;

Sentinel::Sentinel() {}

void Sentinel::RobotInit() {
  uut_talon = new ::CANTalon(1);
  sensor_talon = new ::CANTalon(2);
  auto config = cpptoml::parse_file("sentinel.toml")->get_table("SENTINEL");
  auto talon_settings = sidewinder::talon::Settings::Create(config, "both");
  auto logger = spdlog::stdout_color_st("Sentinel");
  logger->set_level(spdlog::level::info);
  talon_settings->LogConfig(logger);
  talon_settings->Configure(uut_talon);
  talon_settings->SetMode(uut_talon);
  talon_settings->Configure(sensor_talon);
  talon_settings->SetMode(sensor_talon);
}
void Sentinel::Disabled() {}

void Sentinel::OperatorControl() {
  // reload config each time enabled, throws exception if cannot parse config
  auto config = cpptoml::parse_file("sentinel.toml")->get_table("SENTINEL");
  auto tester = std::make_shared<sentinel::Test>();
  BoardTests uut(config, tester);
  uut.RunTests();
  tester->Log();
}

START_ROBOT_CLASS(sentinel::Sentinel)
