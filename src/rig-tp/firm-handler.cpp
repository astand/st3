#include "firm-handler.h"
#include "mainconfig.h"
#include "factory/McuFlashFactory.hpp"
#include "stm32f4xx_flash.h"

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
    is_firm_fail = false;
  }

  return 0;
}


int32_t FirmHandler::UserIncomeData(const RigFrame* in, int32_t dataSize)
{
  int32_t ret = firmsaver->Program(in->Data, dataSize);

  if (ret < 0)
  {
    // programming was finished with errors
    is_firm_fail = true;
  }

  if (dataSize == 0)
  {
    /// firmware ready. need notify any one
    if (callback != 0)
    {
      if (is_firm_fail)
        callback(-1);
      else
      {
        // Place mark for indecating
        // that firmware was loaded successfully
        FLASH_ProgramWord(BOOT_KEY_ADDRESS, 0xdeadbeef);
        callback(fileSize);
      }
    }
  }

  return 0;
}


void FirmHandler::SetCallback(OnLoadedCallback call)
{
  if (call != 0)
    callback = call;
}
