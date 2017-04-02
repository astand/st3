#include "info-handler.h"
#include <string.h>
#include <stdio.h>

extern const char* BuildInfoString();

InfoHandler::InfoHandler(IStreamable& strm) : AReadHandler(strm)
{
  selfId = Rig::Info;
}

int32_t InfoHandler::UserIncomeHead(const RigFrame* in, int32_t dataSize)
{
  if (fsize == 0)
  {
    toInfoString = BuildInfoString();
    fsize = strlen(toInfoString);
  }

  *((uint32_t*)rigFrame->Data) = fsize;
  return 4;
}


int32_t InfoHandler::UserProcess(int32_t need_block)
{
  if (need_block == 1)
  {
    strcpy((char*)rigFrame->Data, toInfoString);
    return fsize;
  }

  return 0;
}
