#pragma once

#include <stdint.h>
#include "equip/gsm/IFailAnswerDetector.h"

class M66FailDetector : public IFailAnswerDetector {
 public:
  M66FailDetector() {}

 public:
  virtual int32_t IsAnswerFail(const char* instr, int32_t length);
};

