#pragma once

#include <stdint.h>
#include "sms-chunks.h"

class ASmsChainHandler {
 public:
  ASmsChainHandler() {}

 public:
  /// when it returns 0 that means that next handler must be invoked.
  /// when it returns more 0 - message handled and it answer lenght.
  /// when it returns less 0 - message handled and no answer provided.
  virtual int32_t HandleIt(const SmsChunkDescriptor& smsdsc, char* ans) = 0;
  /// Set descedanted handler.
  void SetSuccessor(ASmsChainHandler* next) {
    next_ = next;
  }

 protected:
  ASmsChainHandler* next_;
};

