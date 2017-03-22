#include "track-list-handler.h"
#include "threads/thread-gps.h"

static ITrekList* const trList = FileLink();

TrekListHandler::TrekListHandler(IStreamable& strm) : AReadHandler(strm)
{
  selfId = TrekList;
}

int32_t TrekListHandler::UserIncomeHead(const RigFrame* in, RigFrame* const out)
{
  fsize = trList->RefreshTrekList();
  *((uint32_t*)out->Data) = fsize;
  return 4;
}


int32_t TrekListHandler::UserProcess(RigFrame* const out, int32_t need_block)
{
  int32_t blksize = trList->UploadList((need_block - 1) * 25, 25, out->Data);
  return blksize;
}
