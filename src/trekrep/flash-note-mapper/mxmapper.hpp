#pragma once

#include <stdint.h>

// @MxMapper - class that set constraint for memory using, base memory values
// and on that values implements some memory conversions
class MxMapper {
 public:
  /* @PAGES_COUNT - count of pages available for storing trek info */
  static const uint32_t PAGES_COUNT = 500;

  /* @PAGE_SIZE - size in bytes of one page */
  static const uint32_t PAGE_SIZE = 4096;

  /* Comment here */
  static const uint32_t ALL_MEMORY_RANGE = PAGES_COUNT* PAGE_SIZE;

  static uint32_t FIXAddress(uint32_t addr) {
    return (addr % ALL_MEMORY_RANGE);
  }
  static uint32_t FIXPage(uint32_t page) {
    return (page % PAGES_COUNT);
  }

  static uint32_t RollBackPage(uint32_t page) {
    return ((page == 0) ? (PAGES_COUNT - 1) : (page - 1));
  }
  static uint32_t RollForthPage(uint32_t page) {
    return FIXPage(page + 1);
  }

  static uint32_t AddressOfPage(uint32_t page) {
    return (FIXPage(page) * PAGE_SIZE);
  }
  static uint32_t PageOfAddress(uint32_t addr) {
    return (FIXAddress(addr) / PAGE_SIZE);
  }

  static uint32_t MemoryOnPage(uint32_t addr) {
    return (addr % PAGE_SIZE);
  }

  static bool IsPageBeginAddress(uint32_t addr) {
    return (MemoryOnPage(addr) == 0);
  }

 protected:
 private:
  // MxMapper() {}
};
