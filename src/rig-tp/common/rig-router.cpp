#include "rig-router.h"

namespace Rig
{
RigRouter::RigRouter()
{
  for (int32_t i = 0; i < kMaxListCount; i++)
  {
    // clear all pointers
    list[i] = (ARigHandler*)0;
    listCount = 0;
  }
}

void RigRouter::Process()
{
  // Call process for all registered RigHandlers
  for (int i = 0; i < listCount; i++)
  {
    list[i]->Process();
  }
}

bool RigRouter::RegisterRigHandler(ARigHandler* handler)
{
  if (listCount == kMaxListCount)
    return false;

  list[listCount++] = handler;
  return true;
}


void RigRouter::UnregisterRigHandler(ARigHandler* handler)
{
  // Not Implemented Yet
  // It must scan list and remove handler if it was found
  // THIS MUST GARANTEE THAT LIST HAVE NOT EMPTY CELLS !!!
}


void RigRouter::PassRigFrame(const RigFrame* frame)
{
  for (int i = 0; i < listCount; i++)
  {
    HandleResult ret = list[i]->HandleIncome(frame);

    if (ret == Handled)
      return;
  }
}
// close namespace
}
