#include "test.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "nlohmann/json.hpp"

using namespace sentinel;
using namespace std;
using json = nlohmann::json;

Test::Test() : out_("sentinel.json", ios_base::app | ios_base::out) {
  time_t now;
  time(&now);
  char buf[100];
  strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));

  j_["timestamp"] = buf;
  j_["results"] = json::array();
}

void Test::Log() {
  j_["passed"] = success_;
  out_ << j_.dump(4) << endl;
  if (success_) {
    cout << "Board PASSED" << endl;
    return;
  }
  cout << "Board FAILED" << endl;
}

namespace {
void LogResults(json& tr, std::string name, double actual, double expected,
                double epsilon) {
  tr["test"] = name;
  tr["actual"] = actual;
  tr["expected"] = expected;
  tr["epsilon"] = epsilon;
}
}

void Test::Pass(std::string msg, double actual, double expected,
                double epsilon) {
  cout << msg << ": PASSED" << endl;
  json tr;
  tr["passed"] = true;
  LogResults(tr, msg, actual, expected, epsilon);
  j_["results"].push_back(tr);
}

void Test::Fail(std::string msg, double actual, double expected,
                double epsilon) {
  cout << msg << ": FAILED, actual = " << actual << ", expected = " << expected
       << ", epsilon = " << epsilon << endl;
  success_ = false;
  json tr;
  tr["passed"] = false;
  LogResults(tr, msg, actual, expected, epsilon);
  j_["results"].push_back(tr);
}
