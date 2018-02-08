#pragma once

#include <stdint.h>
#include "nmeautil/navinote.h"

class CachedTrek {
 public:
  CachedTrek();
  int32_t Add(const NaviNote& item);
  int32_t UpLoad(uint8_t* mem, int32_t maxsize);
  // make loading by number available
 
 private:
  int32_t loaded_cnt_;
  int32_t head_;

};

