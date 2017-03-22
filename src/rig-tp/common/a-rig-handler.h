#pragma once

#include <stdint.h>
#include "rig-tp-types.h"

namespace Rig
{

class ARigHandler {
 public:
  virtual HandleResult HandleIncome(const RigFrame* frame) = 0;
  virtual void Process() = 0;
};

}
