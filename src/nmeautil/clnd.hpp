#pragma once

#include <stdint.h>

// ----------------------------------------------------------------------------
// Instance for time handle
// include overloaded methods for compare and equal operations
// current size = 1 * 6 = 6B
// ----------------------------------------------------------------------------

__packed class clnd0 {
  typedef uint8_t clnd0_t;
 public:
  clnd0();
  clnd0_t year;
  clnd0_t month;
  clnd0_t day;

  clnd0_t hr;
  clnd0_t min;
  clnd0_t sec;
  friend bool operator> (const clnd0& left, const clnd0& right);
  clnd0& operator=(const clnd0& right);
  static int32_t Lenght();
};
