#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "echo-handler.h"
#include "nmeautil/navinote.hpp"

extern Navi scoor;
static const NaviNote& echoNote = scoor;
static const int32_t echoSize = sizeof(NaviNote);

EchoHandler::EchoHandler(IStreamable& strm) : AReadHandler(strm, Rig::Echo)
{
}

int32_t EchoHandler::UserIncomeHead(const RigFrame* in, int32_t dataSize)
{
  /// echo request has not a data
  *(uint32_t*)(rigFrame->Data) = 0;
  memcpy(rigFrame->Data + 4, (const void*) & (echoNote), echoSize);
  return echoSize + 4;
}


int32_t EchoHandler::UserProcess(int32_t need_block)
{
  return 0;
}
