#include "pipes-maker.h"
#include "utility/bridge/abstract/ISwitchable.h"
#include "utility/streams/spi-block-sender.h"
#include "switch-maker.h"
#include "hdwr/pipes/hw-spi.h"
#include "hdwr/pipes/hw-gps-serial.h"
#include "hdwr/pipes/hw-gsm-serial.h"
#include "equip/flash/mx-25-driver.h"

IFlashMemory& PipesMaker::GetFlashMemory()
{
  static ISwitchable& cs_ = SwitchMaker::GetChipSelect();
  static Mx25drv mxdrv(GetBlockSender(), cs_);
  return mxdrv;
}


IStreamable& PipesMaker::GetGpsStream()
{
  return GetGpsPipe();
}

IStreamable& PipesMaker::GetGsmStream()
{
  return GetGsmPipe();
}

