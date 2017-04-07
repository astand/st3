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
  AReadHandler(IStreamable& strm, uint16_t id);

 public:
  virtual HandleResult HandleIncome(const RigFrame* frame, int32_t dataSize);
  virtual void Process();

 protected:
  int32_t fsize;
  uint16_t selfId;
  BidControl bid;
  RigFrame* rigFrame;
  virtual int32_t UserIncomeHead(const RigFrame* in, int32_t dataSize) = 0;
  virtual int32_t UserProcess(int32_t need_block) = 0;
  virtual int32_t UserIncomeAck(const RigFrame* in) {
    return -1;
  }

 private:
  int32_t kMaxLen;
  uint8_t* const rigFrameBuff_;

 private:
  IStreamable& strm_;
};

}
