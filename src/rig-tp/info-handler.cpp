#include "info-handler.h"
#include <string.h>
#include <stdio.h>

extern const char* BuildInfoString();

int32_t InfoHandler::UserIncomeHandler(const RigFrame* in, RigFrame* out)
{
  if (in->Opc == RRQ)
  {
    if (fsize == 0)
    {
      toInfoString = BuildInfoString();
      fsize = strlen(toInfoString);
    }

    out->Opc = RRQ;
    out->RigId = INFO;
    out->BlockNum = 0;
    *((uint32_t*)out->Data) = fsize;
    bid.StartSending();
    return 6 + 4;
  }
  else if (in->Opc == ACK)
  {
    bid.CheckIncomeBid(in->BlockNum);
    return 0;
  }

  return 0;
}

int32_t InfoHandler::UserProcess(RigFrame* const out, int32_t need_block)
{
  out->Opc = DATA;
  out->RigId = INFO;
  out->BlockNum = need_block;

  if (need_block == 1)
  {
    strcpy((char*)out->Data, toInfoString);
    return 6 + fsize;
  }

  return 6;
}
