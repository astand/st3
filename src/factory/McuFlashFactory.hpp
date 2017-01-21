#pragma once

#include <stdint.h>
#include "utility/abstract/io/ISectorWriter.h"

class FlashFactory {
 public:

  static ISectorWriter* GetSectorWriter();

 protected:

  FlashFactory() {}

  static ISectorWriter* m_sw_instance;

 private:

  static void CreateHardwareDriver();
};

