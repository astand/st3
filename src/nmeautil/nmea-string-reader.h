#pragma once

#include <stdint.h>
#include "abstract/IStreamable.h"
#include "etc/str0.h"

typedef uint16_t csm_t;

// this class performs reading income stream and trys to parse
// valid NMEA string
// Poll function @TryReadNmea retruns len of parsed valid string
class NmeaStringReader {
 public:
  NmeaStringReader(IStreamable& strm) : strm_(strm) {}
  int32_t TryReadNmea(uint8_t* outbuf, int32_t maxlen) {
    int32_t sym;
    sym = strm_.Read();

    if (sym < 0)
      return 0;

    if (sym == '$')
      bytec_ = 0;
    else if (sym == 0x0D) {
      /* stop msg */
      uint8_t csm = MathNmeaCsm(outbuf, bytec_ - 3);
      uint8_t real = ReadNmeaCsm(outbuf, bytec_);

      if (csm == real)
        return (bytec_ - 3);
      else
        return -1;
    }
    else
      outbuf[bytec_++] = static_cast<uint8_t>(sym);

    return 0;
  }

 private:
  IStreamable& strm_;
  uint16_t bytec_;

 private:
  uint16_t MathNmeaCsm(const uint8_t inb[], uint16_t len) {
    uint16_t res = 0;

    for (uint32_t i = 0; i < len; res ^= inb[i++]);

    return res;
  }

  uint16_t ReadNmeaCsm(const uint8_t inb[], uint16_t len) {
    uint16_t val = 0;
    val = str0::Atoh(inb[len - 2]) * 16;
    val += str0::Atoh(inb[len - 1]);
    return val;
  }
};
