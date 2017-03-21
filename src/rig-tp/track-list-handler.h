#pragma once

#include <stdint.h>
#include "common/common-handler.h"

using namespace Rig;

class TrackListHandler : public CommonHandler {
 public:
  TrackListHandler(IStreamable& strm) : CommonHandler(strm) {
    selfId = TRACKLIST;
  }

 public:
  virtual int32_t UserIncomeHandler(const RigFrame* in, RigFrame* const out);
  virtual int32_t UserProcess(RigFrame* const out, int32_t need_block);

 private:

};
