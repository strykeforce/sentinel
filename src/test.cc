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
  strftime(buf, sizeof buf, "%FT%T%Z", localtime(&now));

  j_["timestamp"] = buf;
  j_["results"] = json::array();
}

void Test::Log() {
  j_["passed"] = success_;
  out_ << j_.dump(4) << endl;
  if (success_) {
    cout << "Board PASSED\n\n" << endl;
    return;
  }
  cout << "Board FAILED\n\n" << endl;
}

namespace {
void LogResults(json& tr, const TestCase& tc) {
  tr["test"] = tc.name;
  tr["actual"] = tc.actual;
  tr["expected"] = tc.expected;
  tr["epsilon"] = tc.epsilon;
}
}

void Test::Pass(const TestCase& tc) {
  cout << tc.name << ": PASSED" << endl;
  json tr;
  tr["passed"] = true;
  LogResults(tr, tc);
  j_["results"].push_back(tr);
}

void Test::Fail(const TestCase& tc) {
  cout << tc.name << ": FAILED, actual = " << tc.actual
       << ", expected = " << tc.expected << ", epsilon = " << tc.epsilon
       << endl;
  success_ = false;
  json tr;
  tr["passed"] = false;
  LogResults(tr, tc);
  j_["results"].push_back(tr);
}
