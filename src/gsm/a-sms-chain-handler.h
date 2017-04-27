#pragma once

#include <stdint.h>
#include "sms-chunks.h"

class ASmsChainHandler {
 public:

 public:
  ASmsChainHandler() {}

 public:
  virtual int32_t HandleIt(const SmsChunkDescriptor& smsdsc, char* ans) = 0;
  void SetSuccessor(ASmsChainHandler* next) {
    next_ = next;
  }

 protected:
  ASmsChainHandler* next_;
};

