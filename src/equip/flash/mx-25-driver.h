#pragma once

#include <stdint.h>
#include "abstract/IFlashMemory.h"
#include "abstract/IFlashStorable.h"
#include "streams/spi-block-sender.h"
#include "bridge/abstract/ISwitchable.h"

class Mx25drv : public IFlashMemory {
 public:
  static const uint32_t MX_JCONF_A = (511) * 4096;

 public:
  Mx25drv(SpiBlockSender& sender, ISwitchable& cs);

 public:
  virtual int32_t Write(uint32_t addr, const uint8_t* buf, uint32_t len);
  virtual int32_t Write(uint32_t addr, IFlashStorable& store_obj);
  virtual int32_t Read(uint32_t addr, uint8_t* buf, uint32_t len);
  virtual int32_t Read(uint32_t addr, IFlashStorable& read_obj);
  virtual int32_t Erase(uint32_t addr);


 private:
  static const uint8_t MX_WREN = 0x06;
  static const uint8_t MX_WRDI = 0x04;
  static const uint8_t MX_RDSR = 0x05;
  static const uint8_t MX_READ = 0x03;
  static const uint8_t MX_SE = 0x20;
  static const uint8_t MX_PP = 0x02;
  static const uint8_t MX_RDID = 0x9f;
  static const uint8_t MX_REMS = 0x90;

  static const uint8_t MX_SRWIP = 0x01;
  static const uint8_t MX_SRWEL = 0x02;

  static const uint8_t MX_DUMMY = 0xFF;

 private:
  SpiBlockSender& sender_;
  ISwitchable& cs_;

 private:
  void SendAddr(uint32_t addr);
  int32_t ReadId(uint8_t* buf);
  int32_t WaitWIP();
  uint8_t Wel();

};

