#include "mxdriver.hpp"


int32_t Mx25drv::Write(uint32_t addr, const uint8_t* buf, uint32_t len)
{
  Wel();
  On();
  SendBl(MX_PP);
  SendAddr(addr);

  while (len--)
    SendBl(*buf++);

  Off();
  return 0;
}

int32_t Mx25drv::Write(uint32_t addr, IFlashStorable& store_obj)
{
  return Write(addr, store_obj.DataPointer(), store_obj.DataLenght());
}

int32_t Mx25drv::Read(uint32_t addr, uint8_t* buf, uint32_t len)
{
  WaitWIP();
  On();
  SendBl(MX_READ);
  SendAddr(addr);

  for (int i = len; i != 0; --i)
    *buf++ = SendBl(MX_DUMMY);

  Off();
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
  On();
  SendBl(MX_SE);
  SendAddr(addr);
  Off();
  return 0;
}

void Mx25drv::SendAddr(uint32_t addr)
{
  SendBl((uint8_t)(addr >> 16));
  SendBl((uint8_t)(addr >> 8));
  SendBl((uint8_t)(addr));
}

int32_t Mx25drv::WaitWIP()
{
  uint8_t ret;
//  Off();
  On();
  SendBl(MX_RDSR);

  do
  {
    ret = SendBl(MX_DUMMY);
  }
  while (ret & MX_SRWIP);

  Off();
  return (ret);
}

/*
 *
 */
uint8_t Mx25drv::Wel()
{
  uint8_t ret;
  WaitWIP();
  On();
  ret = SendBl(MX_WREN);
  Off();
  return ret;
}

/*
 *
 */
int32_t Mx25drv::ReadId(uint8_t* bu)
{
  On();
  SendBl(MX_RDID);
  *bu++ = SendBl(MX_DUMMY);
  *bu++ = SendBl(MX_DUMMY);
  *bu = SendBl(MX_DUMMY);
  Off();
  return 0;
}
