#pragma once

#include <stdint.h>
#include "common/a-read-handler.h"

using namespace Rig;

class SoleTrekHandler : public AReadHandler {
 public:
  SoleTrekHandler(IStreamable& strm);
  virtual int32_t UserIncomeHead(const RigFrame* in, int32_t dataSize);
  virtual int32_t UserProcess(int32_t need_block);
 private:
  int32_t reqId;
};


