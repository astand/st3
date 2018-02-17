#pragma once

#include "mx-25-driver.h"


Mx25drv::Mx25drv(SpiBlockSender& sender, ISwitchable& cs) : sender_(sender), cs_(cs) {}

int32_t Mx25drv::Write(uint32_t addr, const uint8_t* buf, uint32_t len)
{
  uint32_t once_addr = addr;
  uint32_t once_data_len;

  do
  {
    once_data_len = len > 256 ? 256 : len;
    len -= once_data_len;
    Wel();
    cs_.on();
    sender_.SendBl(MX_PP);
    SendAddr(once_addr);

    while (once_data_len--)
      sender_.SendBl(*buf++);

    cs_.off();
    once_addr += 256;
  }
  while (len != 0);

  return 0;
}

int32_t Mx25drv::Write(uint32_t addr, IFlashStorable& store_obj)
{
  return Write(addr, store_obj.DataPointer(), store_obj.DataLenght());
}

int32_t Mx25drv::Read(uint32_t addr, uint8_t* buf, uint32_t len)
{
  WaitWIP();
  cs_.on();
  sender_.SendBl(MX_READ);
  SendAddr(addr);

  for (int i = len; i != 0; --i)
    *buf++ = sender_.SendBl(MX_DUMMY);

  cs_.off();
  return 0;
}

int32_t Mx25drv::Read(uint32_t addr, IFlashStorable& read_obj)
{
  return Read(addr, (uint8_t*)read_obj.DataPointer(), read_obj.DataLenght());
}
/* ------------------------------------------------------------------------- */
/// @EraseSec - perform flushing page that span current @addr value
int32_t Mx25drv::Erase(uint32_t addr)
{
  Wel();
  cs_.on();
  sender_.SendBl(MX_SE);
  SendAddr(addr);
  cs_.off();
  return 0;
}

void Mx25drv::SendAddr(uint32_t addr)
{
  sender_.SendBl((uint8_t)(addr >> 16));
  sender_.SendBl((uint8_t)(addr >> 8));
  sender_.SendBl((uint8_t)(addr));
}

int32_t Mx25drv::WaitWIP()
{
  uint8_t ret;
  cs_.on();
  sender_.SendBl(MX_RDSR);

  do
  {
    ret = sender_.SendBl(MX_DUMMY);
  }
  while (ret & MX_SRWIP);

  cs_.off();
  return (ret);
}

/*
 *
 */
uint8_t Mx25drv::Wel()
{
  uint8_t ret;
  WaitWIP();
  cs_.on();
  ret = sender_.SendBl(MX_WREN);
  cs_.off();
  return ret;
}

/*
 *
 */
int32_t Mx25drv::ReadId(uint8_t* bu)
{
  cs_.on();
  sender_.SendBl(MX_RDID);
  *bu++ = sender_.SendBl(MX_DUMMY);
  *bu++ = sender_.SendBl(MX_DUMMY);
  *bu = sender_.SendBl(MX_DUMMY);
  cs_.off();
  return 0;
}








