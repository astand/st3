#pragma once

#include "utility/abstract/IStreamable.h"
#include "utility/abstract/IFlashMemory.h"


class PipesMaker {
 public:
  static IFlashMemory& GetFlashMemory();
  static IStreamable& GetGpsStream();
  static IStreamable& GetGsmStream();

 private:
  PipesMaker();
  PipesMaker(const PipesMaker&);
  PipesMaker& operator=(const PipesMaker&);

};

