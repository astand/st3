#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "echo-handler.h"
#include "nmeautil/navinote.h"
#include "trekrep/cached-trek.h"

extern CachedTrek cachedTrek;

EchoHandler::EchoHandler(IStreamable& strm) : AReadHandler(strm, Rig::Echo)
{
}

int32_t EchoHandler::UserIncomeHead(const RigFrame* in, int32_t dataSize)
{
  /// echo request has not a data
  *(uint32_t*)(rigFrame->Data) = 0;
  int32_t retcopy = cachedTrek.UpLoad(rigFrame->Data + 4, 0);
  return retcopy + 4;
}


int32_t EchoHandler::UserProcess(int32_t need_block)
{
  return 0;
}
