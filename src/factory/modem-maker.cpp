#include "modem-maker.h"
#include "equip/gsm/at-pipe.h"
#include "equip/gsm/modems/m66-fail-detector.h"
#include "hdwr/pipes/hw-gsm-serial.h"
#include "configs/free-rtos-def.h"
#include "gsm/modem.h"

M66::GsmModem& GetGsmModem()
{
  static AtFramer atf(GetGsmPipe(), 512, osPass);
  static M66FailDetector fd;
  static AtPipe atp(fd, atf);
  static M66::GsmModem my_modem(atp);
  return my_modem;
}


