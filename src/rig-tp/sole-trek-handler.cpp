#include "sole-trek-handler.h"
#include "mainconfig.h"
#include "threads/thread-gps.h"
#include "nmeautil/navinote.hpp"

static ITrekList* const trList = FileLink();

SoleTrekHandler::SoleTrekHandler(IStreamable& strm) : AReadHandler(strm)
{
  selfId = SoleTrek;
}

int32_t SoleTrekHandler::UserIncomeHead(const RigFrame* in, RigFrame* const out)
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
    out->Opc = ERR;
    sprintf((char*)out->Data, "File not found.");
    sole_size = strlen((char*)out->Data);
  }

  HWREG(out->Data) = sole_size;
  return 4;
}


int32_t SoleTrekHandler::UserProcess(RigFrame* const out, int32_t need_block)
{
  return trList->UploadTrek(reqId, (need_block - 1) * 25, (25 * NaviNote::Lenght()), out->Data);
}
