#pragma once

#include "a-rig-handler.h"
#include "abstract/IProcessable.h"

namespace Rig
{
class RigRouter : public IProcessable {
 public:
  RigRouter();

 public:
  virtual void Process();
  // return status of register
  bool RegisterRigHandler(ARigHandler* handler);
  void UnregisterRigHandler(ARigHandler* handler);
  void PassRigFrame(const RigFrame* frame);

 protected:

 private:
  static const int32_t kMaxListCount = 16;

 private:
  ARigHandler* list[kMaxListCount];
  int32_t listCount;
};
// close namespace
}
