#pragma once

#include <stdint.h>
#include "utility/etc/str0.h"
#include "clnd.hpp"
#include "speedsnaper.hpp"
#include "utility/abstract/IFlashStorable.h"

/* ------------------------------------------------------------------------- *
** single note (waypiont) of full trek file
** including latitude and long titude date time etc
** current Lenght = 24. Spare will use for ADC1 input soon
** next will deploy two more fields for other ADC2-ADC3 inputs
** ------------------------------------------------------------------------- */
__packed class NaviNote {
 public:
  static const uint8_t NAVI_VER = 1;
  static const uint8_t MEM_STEP_ADD = 64;

  clnd0 clnd;
  int16_t altitude;

  int32_t lafull;
  int32_t lofull;
  uint16_t spd;
  uint32_t accum_dist;
  uint16_t adcs[3];

  /* */
  NaviNote() { };
  /* */
  void PrsDate(const char* buf);
  void PrsTime(const char* buf);
  int32_t SetL(const char* buf, uint8_t l);

  uint8_t* Base() {
    return (uint8_t*)(this);
  }
  static uint16_t Lenght() {
    return sizeof(NaviNote);
  }

 protected:
  void MathDistance(float vic);
};

/* ------------------------------------------------------------------------- *
**  Navi including methods and base constants for handle
**  incoming GPS information and interpretate it for NaviNote view
** ------------------------------------------------------------------------- */
class Navi : public NaviNote, public  IFlashStorable {

 public:
  virtual const uint8_t* DataPointer() {
    return Base();
  }
  virtual uint32_t DataLenght() {
    return Lenght();
  }

  SpeedSnaper mvdetector;
  static const int8_t MAXNMEA = 80;

  static const int8_t P_RMC_VALID	= 2;
  static const int8_t P_RMC_TIME = 1;
  static const int8_t P_RMC_DATE = 9;
  static const int8_t	P_RMC_SPEED = 7;
  static const int8_t	P_RMC_LAT = 3;
  static const int8_t	P_RMC_LATNS = 4;
  static const int8_t	P_RMC_LONG = 5;
  static const int8_t	P_RMC_LONGEW = 6;
  static const int8_t	P_VTG_VALID  = 9;
  static const int8_t	P_VTG_SPD = 7;
  static const int8_t P_GGA_ALT = 9;

  static const uint16_t FIX_SPD_VAL = (30 * 100);

  static const uint32_t RMC_PREFIX = 0x2C434D52;
  static const uint32_t VTG_PREFIX = 0x2C475456;
  static const uint32_t GGA_PREFIX = 0x2C414747;
  static const uint32_t GLL_PREFIX = 0x2C4C4C47;

  bool rmcvalid;
  uint16_t fixspd;
  float intermediate_dist;

  Navi();

  void Copy(Navi* patient);

  uint32_t MathTo(uint32_t firstto);

  int16_t SetLatitude(const char*);
  int16_t SetLongitude(const char*);
  void FreezeFixSpd();
  void FreezeDistance();
  bool RMCParse(const char*);
  bool VTGParse(const char*);
  bool GGAParse(const char*);

  bool Valid();
};
