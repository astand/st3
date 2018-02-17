#include "uart-stream.h"

UartStream::UartStream(UartHal& uart, RingBuffer& rxb, RingBuffer& txb) :
  uart_(uart), rx_(rxb), tx_(txb)
{
}


int32_t UartStream::Read()
{
  return rx_.Get();
}


int32_t UartStream::Read(uint8_t* buff, int32_t start, int32_t count)
{
  int32_t readCnt = 0;
  int32_t ret;

  do
  {
    ret = Read();

    if (ret < 0)
      break;

    buff[start + readCnt] = static_cast<uint8_t>(ret);
    ++readCnt;

    if (readCnt >= count)
      break;
  }
  while (true);

  return readCnt;
}


int32_t UartStream::Write(uint8_t byte)
{
  int32_t ret = tx_.Put(byte);

  if (ret >= 0)
    BeginTransmitt();

  return ret;
}


int32_t UartStream::Write(const uint8_t* buff, int32_t start, int32_t count)
{
  if (start < 0 || count <= 0)
    /// Sending cannot be performed
    return -1;

  int32_t send_cnt = 0;

  do
  {
    if (Write(buff[send_cnt + start]) < 0)
    {
      /// Writing operation was not performed
      return send_cnt;
    }

    send_cnt++;
  }
  while (--count);

  return send_cnt;
}


int32_t UartStream::CanWrite()
{
  return tx_.GetFree();
}

int32_t UartStream::CanRead()
{
  return rx_.GetUsed();
}


void UartStream::FFlushRead()
{
  rx_.Flush();
}


void UartStream::FFlushWrite()
{
  tx_.Flush();
}


void UartStream::Process()
{
  int32_t inb = -1;

  if (uart_.is_TXE())
  {
    if (tx_.IsEmpty())
    {
      uart_.clear_TXE();
      uart_.disable_TXEI();
    }
    else
    {
      uart_.flow_byte(tx_.Get());
    }
  }

  if (uart_.is_byte_received())
  {
    inb = uart_.get_dr();
    rx_.Put(inb);
  }
}


void UartStream::BeginTransmitt()
{
  if (uart_.is_TXEI_enabled() == false)
  {
    uart_.enable_TXEI();
  }
}
