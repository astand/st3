#include "track-list-handler.h"
#include "threads/thread-gps.h"

static ITrekList* const trList = FileLink();

TrekListHandler::TrekListHandler(IStreamable& strm) : AReadHandler(strm, Rig::TrekList)
{
}

int32_t TrekListHandler::UserIncomeHead(const RigFrame* in, int32_t dataSize)
{
  fsize = trList->RefreshTrekList();
  *((uint32_t*)rigFrame->Data) = fsize;
  return 4;
}


int32_t TrekListHandler::UserProcess(int32_t need_block)
{
  int32_t blksize = trList->UploadList((need_block - 1) * 25, 25, rigFrame->Data);
  return blksize;
}
