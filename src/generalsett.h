#pragma once

#include <stdint.h>
#include <stdio.h>
#include "utility/etc/str0.h"

/* ------------------------------------------------------------------------- */
__packed class SocketBits_c {
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
__packed class TcpInstance_c {
 public:
  uint16_t bigto;
  uint16_t silentto;
  uint16_t reconnto;

 public:
  TcpInstance_c();

 public:
  void Default();
  int32_t Update(char* dst);
  void Print(char* dst) const;
  void PrintIP(char* dst) const;
  void PrintPort(char* dst) const;

  int32_t UpdateTim(char* dst);
  void PrintTim(char* dst) const;

  bool TcpValid() const;
  bool IsTimValid(int32_t val) const;
 private:
  uint8_t add[4];
  uint16_t port;
};


/* ------------------------------------------------------------------------- */
__packed class GeneralSett {
  static const uint8_t TCP_NUMS = 5;
  static const uint16_t ID_LENGHT = 64;
  static const uint32_t CURRENT_SIGN = 0x0700031;
 public:
  uint32_t SIGN;
  char ID[ID_LENGHT];
  TcpInstance_c IP[TCP_NUMS];
  SocketBits_c sockbit;
 public:
  GeneralSett();

  bool JCIsSign();

  void JCDefault();
  uint32_t Lenght();
  uint32_t SocketBits(char* s);
};


