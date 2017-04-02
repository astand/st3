#include "firm-handler.h"
#include "factory/McuFlashFactory.hpp"

static ISectorWriter* firmsaver = FlashFactory::GetSectorWriter();

FirmHandler::FirmHandler(IStreamable& strm) : AWriteHandler(strm)
{
  selfId = Rig::Firmware;
}

int32_t FirmHandler::UserIncomeHead(const RigFrame* in, int32_t dataSize)
{
  fileSize = 0;

  if (dataSize >= 4)
  {
    fileSize = *(int32_t*)(in->Data);
  }

  if (fileSize > 0)
  {
    /// firmware file will be received
    firmsaver->Erase();
  }

  return 0;
}


int32_t FirmHandler::UserIncomeData(const RigFrame* in, int32_t dataSize)
{
  firmsaver->Program(in->Data, dataSize);

  if (dataSize == 0)
  {
    /// firmware ready. need notify any one
    if (callback != 0)
      callback(fileSize);
  }

  return 0;
}


void FirmHandler::SetCallback(OnLoadedCallback call)
{
  if (call != 0)
    callback = call;
}
