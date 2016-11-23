#include "sentinel.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "WPILib.h"
#include "catch/catch.hpp"
#include "spdlog/spdlog.h"

using namespace sentinel;

Sentinel::Sentinel() {
  auto t = std::time(nullptr);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d-%H-%M-%S", std::localtime(&t));
  std::ostringstream oss;
  oss << mbstr << "_sentinel_log.txt";
  logger_ = spdlog::basic_logger_mt("sentinel", oss.str());
}

void Sentinel::RobotInit() {
  Catch::ConfigData config;
  config.showSuccessfulTests = true;
  session_.useConfigData(config);
}
void Sentinel::Disabled() {
  std::cout << "Disable to reset, Enable to run tests.\n" << std::endl;
  logger_->flush();
}
void Sentinel::OperatorControl() { session_.run(); }

START_ROBOT_CLASS(Sentinel)
