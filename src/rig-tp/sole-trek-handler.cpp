#include "sole-trek-handler.h"
#include "mainconfig.h"
#include "threads/thread-gps.h"
#include "nmeautil/navinote.hpp"

static ITrekList* const trList = FileLink();

SoleTrekHandler::SoleTrekHandler(IStreamable& strm) : AReadHandler(strm)
{
  selfId = SoleTrek;
}

int32_t SoleTrekHandler::UserIncomeHead(const RigFrame* in, int32_t dataSize)
{
  int32_t sole_id = trList->GetListIndex(HWREGH(in->Data));
  int32_t sole_size = 0;

  if (sole_id >= 0)
  {
    sole_size = trList->GetTrekSize(sole_id);
    reqId = sole_id;
  }
  else
  {
    rigFrame->Opc = ERR;
    sprintf((char*)rigFrame->Data, "File not found.");
    sole_size = strlen((char*)rigFrame->Data);
  }

  HWREG(rigFrame->Data) = sole_size;
  return 4;
}


int32_t SoleTrekHandler::UserProcess(int32_t need_block)
{
  return trList->UploadTrek(reqId, (need_block - 1) * 25, (25 * NaviNote::Lenght()), rigFrame->Data);
}
