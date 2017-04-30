#pragma once

#include <stdint.h>
#include <stdio.h>
#include "utility/etc/str0.h"
#include "gsm/sett/gprs-param.h"

/* ------------------------------------------------------------------------- */
class SocketBits_c {
  uint32_t b;
  static const uint32_t BITS_MASK = 0x1F;
 public:
  SocketBits_c() {
    b = 0;
  }

  uint32_t Socket();
  uint32_t Socket(uint32_t num);
};

/* ------------------------------------------------------------------------- */
class GeneralSett {
  static const uint8_t TCP_NUMS = 5;
  static const uint16_t ID_LENGHT = 64;
  static const uint32_t CURRENT_SIGN = 0x0700132;
 public:
  uint32_t SIGN;
  char ID[ID_LENGHT];
  // TcpInstance_c IP[TCP_NUMS];
  SocketBits_c sockbit;
  GprsParam gprs[3];

 public:
  GeneralSett();

  bool JCIsSign();

  void JCDefault();
  uint32_t Lenght();
  uint32_t SocketBits(char* s);
};


