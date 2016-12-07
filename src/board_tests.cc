#include "board_tests.h"

#include <chrono>
#include <memory>
#include <thread>
#include <vector>
#include "WPILib.h"
#include "cpptoml/cpptoml.h"

#include "sentinel.h"
#include "test.h"

namespace {
// test head control digital outputs
enum DioConstants {
  kDIOQuadBHiLow = 0,
  kDIOQuadBEnable,
  kDIOQuadAHiLow,
  kDIOQuadAEnable,
  kDIOQuadIHiLow,
  kDIOQuadIEnable,
  kDIOLimitFwd,
  kDIOLimitRev,
  kDIOAnalog1,
  kDIOAnalog2,
  kDIOCount,
};

// current measurements
enum AinConstants {
  kI5vChan = 0,
  kI3vChan,
};
}

using namespace sentinel;
using namespace std;

BoardTests::BoardTests(std::shared_ptr<cpptoml::table> config,
                       std::shared_ptr<Test> tester)
    : config_(config),
      tester_(tester),
      digital_outputs_(),
      current_5v_(kI5vChan),
      current_3v_(kI3vChan),
      talon_wait_(config_->get_as<uint>("talon_wait_duration").value_or(0)) {
  // initialize digital outputs and set all to output low
  for (uint32_t i = 0; i < kDIOCount; i++) {
    digital_outputs_.push_back(make_unique<::DigitalOutput>(i));
    digital_outputs_.back()->Set(0);
  }

  ConfigTalon(Sentinel::uut_talon);
  ConfigTalon(Sentinel::sensor_talon);
}

void BoardTests::ConfigTalon(::CANTalon* talon) {
  auto rate = config_->get_as<uint>("frame_rate").value_or(20);
  talon->SetStatusFrameRateMs(::CANTalon::StatusFrameRateGeneral, rate);
  talon->SetStatusFrameRateMs(::CANTalon::StatusFrameRateFeedback, rate);
  talon->SetStatusFrameRateMs(::CANTalon::StatusFrameRateQuadEncoder, rate);
  talon->SetStatusFrameRateMs(::CANTalon::StatusFrameRateAnalogTempVbat, rate);
}

// initialize each test case
void BoardTests::Setup() {
  for (const auto& d : digital_outputs_) {
    d->Set(0);
  }

  // disconnect quad encoder
  digital_outputs_[kDIOQuadAEnable]->Set(1);
  digital_outputs_[kDIOQuadBEnable]->Set(1);
  digital_outputs_[kDIOQuadIEnable]->Set(1);
  // turn off all leds
  digital_outputs_[kDIOQuadAHiLow]->Set(1);
  digital_outputs_[kDIOQuadBHiLow]->Set(1);
  digital_outputs_[kDIOQuadIHiLow]->Set(1);
}

// run all tests
void BoardTests::RunTests() {
  Setup();
  TestAnalogEncoderInput();
  Setup();
  TestQuadEncoderLogic();
  Setup();
  TestLimitSwitches();
  Setup();
  TestQuadEncoderCount();
}

// test UUT 5v and 3.3v supply currents
void BoardTests::TestCurrent(string name, double i5, double i5e, double i3,
                             double i3e) {
  TestCase tc;
  auto name_suffix = config_->get_as<string>("5v_name").value_or("NA");
  tc.name = name + name_suffix;
  tc.actual = static_cast<double>(current_5v_.GetVoltage());
  tc.expected = i5;
  tc.epsilon = i5e;

  if (abs(tc.actual - tc.expected) <= tc.epsilon) {
    tester_->Pass(tc);
  } else {
    tester_->Fail(tc);
  }

  name_suffix = config_->get_as<string>("3v_name").value_or("NA");
  tc.name = name + name_suffix;
  tc.actual = static_cast<double>(current_3v_.GetVoltage());
  tc.expected = i3;
  tc.epsilon = i3e;

  if (abs(tc.actual - tc.expected) <= tc.epsilon) {
    tester_->Pass(tc);
  } else {
    tester_->Fail(tc);
  }
}

// test Talon analog encoder input
void BoardTests::TestAnalogEncoderInput() {
  auto name = config_->get_as<string>("analog_low_name").value_or("NA");
  auto expected = config_->get_as<uint>("analog_low").value_or(0);
  TestAnalogEncoderInputLevel(name, 0, 0, expected);

  name = config_->get_as<string>("analog_med_low_name").value_or("NA");
  expected = config_->get_as<uint>("analog_med_low").value_or(0);
  TestAnalogEncoderInputLevel(name, 1, 0, expected);

  name = config_->get_as<string>("analog_med_high_name").value_or("NA");
  expected = config_->get_as<uint>("analog_med_high").value_or(0);
  TestAnalogEncoderInputLevel(name, 0, 1, expected);

  name = config_->get_as<string>("analog_high_name").value_or("NA");
  expected = config_->get_as<uint>("analog_high").value_or(0);
  TestAnalogEncoderInputLevel(name, 1, 1, expected);
}

// helper function for testing Talon analog encoder input
void BoardTests::TestAnalogEncoderInputLevel(string name, uint a1, uint a2,
                                             uint expected) {
  TestCase tc;
  tc.name = name;

  digital_outputs_[kDIOAnalog1]->Set(a1);
  digital_outputs_[kDIOAnalog2]->Set(a2);
  this_thread::sleep_for(talon_wait_);

  tc.actual = static_cast<double>(Sentinel::uut_talon->GetAnalogInRaw());
  tc.expected = static_cast<double>(expected);
  tc.epsilon =
      static_cast<double>(config_->get_as<uint>("analog_epsilon").value_or(0));

  if (abs(tc.actual - tc.expected) <= tc.epsilon) {
    tester_->Pass(tc);
  } else {
    tester_->Fail(tc);
  }
}

void BoardTests::TestQuadEncoderLogic() {
  auto i5 = config_->get_as<double>("quad_5v_current").value_or(0.0);
  auto i5e = config_->get_as<double>("quad_5v_epsilon").value_or(0.0);
  auto i3 = config_->get_as<double>("quad_3v_current").value_or(0.0);
  auto i3e = config_->get_as<double>("quad_3v_epsilon").value_or(0.0);

  for (uint i = 0; i < 6; i++) {
    TestCase tc;
    tc.epsilon = 0.0;
    string name;
    switch (i) {
      case 0:
        // A low
        name = config_->get_as<string>("quad_logic_a_name").value_or("NA");
        tc.name = name + "Low";
        digital_outputs_[kDIOQuadAHiLow]->Set(0);
        this_thread::sleep_for(talon_wait_);
        tc.actual =
            static_cast<double>(Sentinel::uut_talon->GetPinStateQuadA());
        tc.expected = 0.0;
        TestCurrent(tc.name, i5, i5e, i3, i3e);
        break;
      case 1:
        // A high
        name = config_->get_as<string>("quad_logic_a_name").value_or("NA");
        tc.name = name + "High";
        digital_outputs_[kDIOQuadAHiLow]->Set(1);
        this_thread::sleep_for(talon_wait_);
        tc.actual =
            static_cast<double>(Sentinel::uut_talon->GetPinStateQuadA());
        tc.expected = 1.0;
        break;
      case 2:
        // B low
        name = config_->get_as<string>("quad_logic_b_name").value_or("NA");
        tc.name = name + "Low";
        digital_outputs_[kDIOQuadBHiLow]->Set(0);
        this_thread::sleep_for(talon_wait_);
        tc.actual =
            static_cast<double>(Sentinel::uut_talon->GetPinStateQuadB());
        tc.expected = 0.0;
        TestCurrent(tc.name, i5, i5e, i3, i3e);
        break;
      case 3:
        // B high
        name = config_->get_as<string>("quad_logic_b_name").value_or("NA");
        tc.name = name + "High";
        digital_outputs_[kDIOQuadBHiLow]->Set(1);
        this_thread::sleep_for(talon_wait_);
        tc.actual =
            static_cast<double>(Sentinel::uut_talon->GetPinStateQuadB());
        tc.expected = 1.0;
        break;
      case 4:
        // I low
        name = config_->get_as<string>("quad_logic_i_name").value_or("NA");
        tc.name = name + "Low";
        digital_outputs_[kDIOQuadIHiLow]->Set(0);
        this_thread::sleep_for(talon_wait_);
        tc.actual =
            static_cast<double>(Sentinel::uut_talon->GetPinStateQuadIdx());
        tc.expected = 0.0;
        TestCurrent(tc.name, i5, i5e, i3, i3e);
        break;
      case 5:
        // I high
        name = config_->get_as<string>("quad_logic_i_name").value_or("NA");
        tc.name = name + "High";
        digital_outputs_[kDIOQuadIHiLow]->Set(1);
        this_thread::sleep_for(talon_wait_);
        tc.actual =
            static_cast<double>(Sentinel::uut_talon->GetPinStateQuadIdx());
        tc.expected = 1.0;
        break;
    }
    if (tc.actual == tc.expected) {
      tester_->Pass(tc);
    } else {
      tester_->Fail(tc);
    }
  }  // for loop
}

void BoardTests::TestLimitSwitches() {
  auto i5 = config_->get_as<double>("limit_5v_current").value_or(0.0);
  auto i5e = config_->get_as<double>("limit_5v_epsilon").value_or(0.0);
  auto i3 = config_->get_as<double>("limit_3v_current").value_or(0.0);
  auto i3e = config_->get_as<double>("limit_3v_epsilon").value_or(0.0);

  TestCase tc;
  tc.epsilon = 0.0;
  auto name = config_->get_as<string>("limit_name").value_or("NA");
  tc.name = name + "FwdOpen";
  tc.expected = 0.0;

  // verify forward limit switch open
  digital_outputs_[kDIOLimitFwd]->Set(0);
  this_thread::sleep_for(talon_wait_);
  tc.actual =
      static_cast<double>(Sentinel::uut_talon->IsFwdLimitSwitchClosed());
  if (tc.actual == tc.expected) {
    tester_->Pass(tc);
  } else {
    tester_->Fail(tc);
  }

  tc.name = name + "FwdClosed";
  tc.expected = 1.0;

  // close forward limit switch
  digital_outputs_[kDIOLimitFwd]->Set(1);
  this_thread::sleep_for(talon_wait_);
  tc.actual =
      static_cast<double>(Sentinel::uut_talon->IsFwdLimitSwitchClosed());
  if (tc.actual == tc.expected) {
    tester_->Pass(tc);
  } else {
    tester_->Fail(tc);
  }

  // test current with forward LED lit
  TestCurrent(tc.name, i5, i5e, i3, i3e);

  // open forward limit switch to turn off LED
  digital_outputs_[kDIOLimitFwd]->Set(0);

  tc.name = name + "RevOpen";
  tc.expected = 0.0;
  // verify reverse limit switch open
  digital_outputs_[kDIOLimitRev]->Set(0);
  this_thread::sleep_for(talon_wait_);
  tc.actual =
      static_cast<double>(Sentinel::uut_talon->IsRevLimitSwitchClosed());
  if (tc.actual == tc.expected) {
    tester_->Pass(tc);
  } else {
    tester_->Fail(tc);
  }

  tc.name = name + "RevClosed";
  tc.expected = 1.0;
  // close reverse limit switch
  digital_outputs_[kDIOLimitRev]->Set(1);
  this_thread::sleep_for(talon_wait_);
  tc.actual =
      static_cast<double>(Sentinel::uut_talon->IsRevLimitSwitchClosed());
  if (tc.actual == tc.expected) {
    tester_->Pass(tc);
  } else {
    tester_->Fail(tc);
  }

  // test current with reverse LED lit
  TestCurrent(tc.name, i5, i5e, i3, i3e);
}

void BoardTests::TestQuadEncoderCount() {
  // connect quad encoder
  digital_outputs_[kDIOQuadAEnable]->Set(0);
  digital_outputs_[kDIOQuadBEnable]->Set(0);
  digital_outputs_[kDIOQuadIEnable]->Set(0);

  TestCase tc;
  tc.name = "QuadEncoderCount";
  Sentinel::uut_talon->SetPosition(0.0);
  Sentinel::sensor_talon->SetPosition(0.0);
  auto v = config_->get_as<double>("quad_count_run_voltage").value_or(0.0);
  Sentinel::uut_talon->Set(v);
  this_thread::sleep_for(chrono::seconds(2));
  Sentinel::uut_talon->Set(0.0);
  this_thread::sleep_for(chrono::milliseconds(500));
  tc.actual = Sentinel::uut_talon->GetPosition();
  tc.expected = Sentinel::sensor_talon->GetPosition();
  tc.epsilon = config_->get_as<double>("quad_count_epsilon").value_or(0.0);
  if (abs(tc.actual - tc.expected) <= tc.epsilon) {
    tester_->Pass(tc);
  } else {
    tester_->Fail(tc);
  }
}
