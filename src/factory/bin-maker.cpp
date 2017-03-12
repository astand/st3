#include "bin-maker.h"
#include "hdwr/pipes/hw-gsm-serial.h"
#include "equip/pipes/bin-pipe.h"


IStreamable& GetBinPipe()
{
  static Stuff::BinaryPipe bp_(GetGsmPipe());
  return bp_;
}


