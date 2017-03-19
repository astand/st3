#pragma once

#include "ARigHandler.h"
#include "abstract/IProcessable.h"

namespace Rig
{
class RigRouter : public IProcessable {
 public:
  RigRouter();

 public:
  virtual void Process();
  // return status of register
  bool RegisterRigHandler(ARigHanlder* handler);
  void UnregisterRigHandler(ARigHanlder* handler);
  void PassRigFrame(const RigFrame* frame);

 protected:

 private:
  static const int32_t kMaxListCount = 16;

 private:
  ARigHanlder* list[kMaxListCount];
  int32_t listCount;
};
// close namespace
}
