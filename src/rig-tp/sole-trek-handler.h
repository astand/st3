#pragma once

#include <stdint.h>
#include "common/a-read-handler.h"

using namespace Rig;

class SoleTrekHandler : public AReadHandler {
 public:
  SoleTrekHandler(IStreamable& strm);
  virtual int32_t UserIncomeHead(const RigFrame* in, RigFrame* const out);
  virtual int32_t UserProcess(RigFrame* const out, int32_t need_block);
 private:
  int32_t reqId;
};


