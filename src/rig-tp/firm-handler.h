#pragma once

#include "common/a-write-handler.h"

typedef void (*OnLoadedCallback)(int32_t size);

using namespace Rig;

class FirmHandler : public AWriteHandler {
 public:
  FirmHandler(IStreamable& strm);
  virtual int32_t UserIncomeHead(const RigFrame* in, int32_t dataSize);
  virtual int32_t UserIncomeData(const RigFrame* in, int32_t dataSize);
  void SetCallback(OnLoadedCallback call);

 private:
  OnLoadedCallback callback;
  bool is_firm_fail;
};
