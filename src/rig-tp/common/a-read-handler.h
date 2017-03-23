#pragma once

#include "abstract/IStreamable.h"
#include "a-rig-handler.h"
#include "rig-tp-types.h"
#include "rig-codes.h"
#include "bid-control.h"

namespace Rig
{
class AReadHandler : public ARigHandler {
 public:
  AReadHandler(IStreamable& strm);

 public:
  virtual HandleResult HandleIncome(const RigFrame* frame);
  virtual void Process();

 protected:
  int32_t fsize;
  BidControl bid;
  uint16_t selfId;
  virtual int32_t UserIncomeHead(const RigFrame* in, RigFrame* const out) = 0;
  virtual int32_t UserProcess(RigFrame* const out, int32_t need_block) = 0;
  virtual int32_t UserIncomeAck(const RigFrame* in, RigFrame* const out) {
    return 0;
  }

 private:
  int32_t kMaxLen;
  uint8_t* const rigFrameBuff_;
  RigFrame* rigFrame;

 private:
  IStreamable& strm_;
};

}
