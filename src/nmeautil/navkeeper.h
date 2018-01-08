#pragma once

#include <stdint.h>
#include "navinote.h"
#include "speedsnaper.hpp"

class NavKeeper {
 public:
  NavKeeper(NaviNote* noteref);

 public:
  void HandleGpsData(GpsPositionData_t* data);
  SpeedSnaper mvdetector;
  static const uint16_t FIX_SPD_VAL = (30 * 100);

  bool rmcvalid;
  uint16_t fixspd;
  uint16_t fixkurs;
  uint16_t liveKurs;
  double intermediate_dist;

  void InitFromRestored();

  uint32_t MathTo(uint32_t firstto);
  int32_t CoerseChanged();
  void FreezeFixSpd();
  void FreezeDistance();

 private: 
  NaviNote* note_;
};
