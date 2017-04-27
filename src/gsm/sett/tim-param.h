#pragma once

#include <stdint.h>

typedef struct
{
  uint16_t Big;
  uint16_t Silent;
  uint16_t ReConnect;
  uint16_t Reserved0;
  uint16_t Reserved1;
  uint16_t Reserved2;

  void Reset() {
    Big = 7200;
    Silent = 45;
    ReConnect = 40;
  }

} TimParam;


