#pragma once

#include "abstract/IStreamable.h"
#include "a-rig-handler.h"
#include "rig-tp-types.h"
#include "rig-codes.h"

namespace Rig
{

class AWriteHandler : public ARigHandler {
 public:
  AWriteHandler(IStreamable& strm);

 public:
  virtual HandleResult HandleIncome(const RigFrame* frame, int32_t dataSize);
  virtual void Process();

 public:
  /// implementation is provided by user. It must return int value as
  /// result of operation
  /// @UserIncomeHead - when return -1 user doesn't accept request (no answer)
  virtual int32_t UserIncomeHead(const RigFrame* in, int32_t dataSize) = 0;
  /// @UserIncomeData - when return -1 user doesn't accept request (no answer)
  virtual int32_t UserIncomeData(const RigFrame* in, int32_t dataSize) = 0;

 protected:
  uint16_t selfId;
  int32_t fileSize;
  RigFrame* rigFrame;

 private:
  IStreamable& strm_;
  uint8_t* const buff_;
  uint16_t ack_block;
};

// namespace
}
