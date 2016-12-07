#pragma once

#include <memory>
#include <vector>
#include "WPILib.h"
#include "cpptoml/cpptoml.h"

#include "test.h"
namespace sentinel {

class BoardTests {
 private:
  std::shared_ptr<cpptoml::table> config_;
  std::shared_ptr<Test> tester_;
  std::vector<std::unique_ptr<::DigitalOutput>> digital_outputs_;
  ::AnalogInput current_5v_, current_3v_;
  std::chrono::milliseconds talon_wait_;

  void ConfigTalon(::CANTalon* talon);
  void Setup();
  void TestCurrent(std::string name, double i5, double i5e, double i3,
                   double i3e);
  void TestAnalogEncoderInput();
  void TestAnalogEncoderInputLevel(std::string name, uint a1, uint a2,
                                   uint expected);
  void TestQuadEncoderLogic();
  void TestLimitSwitches();
  void TestQuadEncoderCount();

 public:
  BoardTests(std::shared_ptr<cpptoml::table> config,
             std::shared_ptr<Test> tester);
  virtual ~BoardTests() = default;
  void RunTests();
};

} /* sentinel */
