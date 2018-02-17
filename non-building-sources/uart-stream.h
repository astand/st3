#pragma once

#include <stdint.h>
#include "../bridge/inc/uart-hal.h"
#include "../buffers/ringbuffer.h"
#include "../abstract/IStreamable.h"
#include "../abstract/IProcessable.h"

class UartStream : public IStreamable, public IProcessable {
 public:
  UartStream(UartHal& uart, RingBuffer& rxb, RingBuffer& txb);

 public: /* IStreamable */
  virtual int32_t Read();

  virtual int32_t Read(uint8_t* buff, int32_t start, int32_t count);

  virtual int32_t Write(uint8_t byte);

  virtual int32_t Write(const uint8_t* buff, int32_t start, int32_t count);

  virtual void FFlushRead();

  virtual void FFlushWrite();

  virtual int32_t CanWrite();

  virtual int32_t CanRead();

 public: /* IProcessable */
  virtual void Process();

 protected:
  UartHal& uart_;
  RingBuffer& rx_;
  RingBuffer& tx_;

 private:
  void BeginTransmitt();
};
