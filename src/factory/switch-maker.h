#pragma once

#include "utility/bridge/abstract/ISwitchable.h"

class SwitchMaker {
 public:
  static ISwitchable& GetGsmLedBlinker();
  static ISwitchable& GetLed2();

  static ISwitchable& GetGsmEn();
  static ISwitchable& GetGpsEn();
  static ISwitchable& GetPwrKey();

  static ISwitchable& GetChipSelect();
  static ISwitchable& GetDCDSignal();

 private:
  SwitchMaker() {}
  SwitchMaker(const SwitchMaker&);
  SwitchMaker& operator=(SwitchMaker&);
};


