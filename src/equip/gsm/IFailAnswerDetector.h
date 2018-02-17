#pragma once

#include <stdint.h>

class IFailAnswerDetector {
 public:

  /// Detector must determine is there matching between @instr and its
  /// own collection.
  /// @return number of matched string or 0 if not matchs
  virtual int32_t IsAnswerFail(const char* instr, int32_t length) = 0;
};
