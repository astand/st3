#pragma once

#include <stdint.h>
#include "navinote.h"
#include "nmea-string-reader.h"

class GpsPositioner {
 public:
  GpsPositioner(NmeaStringReader& posprovider);
  GpsUpdate_t UpdatePosition(GpsPositionData_t* data);

 private:
  NmeaStringReader& reader_;
  static const int32_t kStrLen = 128;
  uint8_t gpstr[kStrLen];
  GpsPositionData_t* posdat_;

 private:
  void UnpackNmeaString();
  void UnpackRMC();
  void UnpackVTG();
  void UnpackGGA();
};
