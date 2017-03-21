#pragma once

#include <stdint.h>
#include "common/common-handler.h"

using namespace Rig;

class InfoHandler : public CommonHandler {
 public:
  InfoHandler(IStreamable& strm) : CommonHandler(strm) {
    selfId = INFO;
  }

  virtual int32_t UserIncomeHandler(const RigFrame* in, RigFrame* out);
  virtual int32_t UserProcess(RigFrame* const out, int32_t need_block);

 private:
  const char* toInfoString;

};

