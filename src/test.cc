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
  j_["results"] = json::object();
}

void Test::Log() {
  j_["passed"] = success_;
  out_ << j_.dump() << endl;
  if (success_) {
    cout << "Board \033[32mPASSED\033[0m\n\n" << endl;
    return;
  }
  cout << "Board \033[1;31mFAILED\033[0m\a\n\n" << endl;
}

void Test::LogResults(const TestCase& tc) {
  j_["results"][tc.name]["actual"] = tc.actual;
  j_["results"][tc.name]["expected"] = tc.expected;
  j_["results"][tc.name]["epsilon"] = tc.epsilon;
}

void Test::Pass(const TestCase& tc) {
  cout << left << setw(30) << tc.name << ": \033[32mPASSED\033[0m" << endl;
  j_["results"][tc.name]["passed"] = true;
  LogResults(tc);
}

void Test::Fail(const TestCase& tc) {
  cout << left << setw(30) << tc.name
       << ": \033[1;31mFAILED\033[0m, actual = " << tc.actual
       << ", expected = " << tc.expected
       << ", delta = " << abs(tc.expected - tc.actual) << endl;
  success_ = false;
  j_["results"][tc.name]["passed"] = false;
  LogResults(tc);
}
