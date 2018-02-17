#pragma once

#include <stdint.h>
#include "abstract/IStreamable.h"
#include "abstract/ICrcCheckable.h"
#include "framer/stuff-framer.h"
#include "framer/crcs/table0-crc.h"

namespace Stuff
{

class BinaryPipe : public IStreamable {
 public:
  BinaryPipe(IStreamable& stream);


 public:
  virtual int32_t Read();

  virtual int32_t Read(uint8_t* buff, int32_t start, int32_t count);

  virtual int32_t Write(uint8_t byte);

  virtual int32_t Write(const uint8_t* buff, int32_t start, int32_t count);

  virtual void FFlushRead() {
    strm_.FFlushRead();
  }

  virtual void FFlushWrite() {
    strm_.FFlushWrite();
  }

  virtual int32_t CanWrite() {
    return strm_.CanWrite();
  }

  virtual int32_t CanRead() {
    return strm_.CanRead();
  }

 protected:
  IStreamable& strm_;

 private:
  Table0_CRC crc_;
  StuffFramer framer_;
};

};

