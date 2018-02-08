#pragma once

#include "utility/abstract/IStreamable.h"
#include "utility/abstract/IFlashMemory.h"
#include "nmeautil/gps-positioner.h"


class PipesMaker {
 public:
  static IFlashMemory& GetFlashMemory();
  static IStreamable& GetGsmStream();
  static GpsPositioner& GetGpsPositioner();

 private:
  PipesMaker();
  PipesMaker(const PipesMaker&);
  PipesMaker& operator=(const PipesMaker&);
};

