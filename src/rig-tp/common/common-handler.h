#pragma once

#include "abstract/IStreamable.h"
#include "rig-tp-types.h"
#include "rig-codes.h"
#include "bid-control.h"

namespace Rig
{
class CommonHandler {
 public:
  CommonHandler(IStreamable& strm);

 public:
  virtual HandleResult HandleIncome(const RigFrame* frame);
  virtual void Process();

 protected:
  int32_t fsize;
  ID selfId;
  BidControl& bid;
  virtual int32_t UserIncomeHandler(const RigFrame* in, RigFrame* const out);
  virtual int32_t UserProcess(RigFrame* const out, int32_t need_block);
  virtual BidControl& MakeBid();

 private:
  int32_t kMaxLen;
  uint8_t* const rigFrameBuff_;
  RigFrame* rigFrame;

 private:
  IStreamable& strm_;
};

}
