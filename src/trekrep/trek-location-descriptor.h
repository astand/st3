#pragma once

#include <stdint.h>
#include "trekrep/flash-note-mapper/mem-notes-mapper.h"

/* -------------------------------------------------------------------------- *
  0x00FFFFFF (19 bits) - offset of memory in counts of waypionts
  max value is 2 ^ 20 = 1 048 576 -> more then sufficient
  0x7FF00000 (11 bits) - sector num (max value = 2 ^ 11 = 2048) for real 500 sectors
  more then sufficient
  last 31 bit is bit indicate that Table instance is valid

  1000 0000 0010 0000 0000 0000 0100 0000 :
  valid instance, 2 sec, 64 waypionts
 */
class TrekLocationDescriptor : public NotesMapper {

 public:
  TrekLocationDescriptor();
  bool IsValid() const;

  uint32_t Sector() const;
  uint32_t Sector(uint32_t s);

  uint32_t CodeOffset() const;
  uint32_t CodeOffset(uint32_t o);

  bool IsTrueOffset(uint32_t vic) const;

  uint32_t SetAll(uint32_t sec, uint32_t bias);
  bool GetAll(uint32_t& sec, uint32_t& bias);

  uint32_t AddressStart() const;
  uint32_t AddressStart(uint32_t) const;
  uint32_t AddressEnd() const;

  uint32_t ByteSize() {
    return 0;
  }

  void CleanData() {
    value = 0;
  }
 private:
  static uint32_t const MATRIX_SECTOR_M = 0x7ff00000;
  static uint32_t const MATRIX_SECTOR_P = 20;
  static uint32_t const MATRIX_BIAS_M = 0x000FFFFF;
  static uint32_t const MATRIX_BIAS_P = 0;
  static uint32_t const MATRIX_VALID_M = 0x80000000;

  uint32_t value;
};
