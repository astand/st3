#pragma once

#include <stdint.h>

class AnswerAnalyzer {
 public:
  enum { kOk, kAlter, kError, kUnknown } AnalResault;

 public:
  AnswerAnalyzer() {}


 private:
  AnalResault anres_;
};

