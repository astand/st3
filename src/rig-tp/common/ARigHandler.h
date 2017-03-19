#pragma once

#include <stdint.h>
#include "rig-tp-types.h"

class ARigHanlder {
 public:
  virtual HandleResult HandleIncome(const RigFrame* frame) = 0;
  virtual void Process() = 0;

 protected:
  RigTxState rigState;
};
