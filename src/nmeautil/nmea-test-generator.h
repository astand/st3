#pragma once

#include <stdint.h>
#include "abstract/IStreamable.h"
// include raw gps data
#include "nmea-simu.c"

class SinteticStream : public IStreamable {
 public:
  SinteticStream() : kSimDataLen(sizeof(nmea_simu)) {
    nowid = 0;
  }
  virtual int32_t Read() {
    if (nowid == kSimDataLen)
      return -1;

    return nmea_simu[nowid++];
  }

  /// try load to buff[start] as much as possible but no more then count
  virtual int32_t Read(uint8_t* buff, int32_t start, int32_t count) {
    return 0;
  }

  /// @Write - return @d if write to outside pipe success or -1
  virtual int32_t Write(uint8_t byte) {
    return 0;
  }

  /// Try send out data from buff[start] as much as can but no more then count
  virtual int32_t Write(const uint8_t* buff, int32_t start, int32_t count) {
    return 0;
  }

  /// Fflush reading pipe
  virtual void FFlushRead() {}

  /// Fflush writing pipe
  virtual void FFlushWrite() {}

  /// @return count of bytes that can be send by @write operation
  virtual int32_t CanWrite() {
    return 0;
  }

  /// @return count of bytes that can be read by @read operation
  virtual int32_t CanRead() {
    return 0;
  }
 protected:

 private:
  int32_t nowid;
  const int32_t kSimDataLen;

};
