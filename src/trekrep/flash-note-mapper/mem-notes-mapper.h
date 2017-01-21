#pragma once

#include "mxmapper.hpp"

/// @NotesMapper class adds to mx map model NOTE STEP instance, that
/// means size of one trek note in memory including ID (2b) and NUM(2b)
/// fields. On that instance it implements some functions for conversion
/// absolute address to page and offset and backward
class NotesMapper : public MxMapper {
 public:

  static const uint32_t NOTE_STEP = 32;
  static const uint32_t NOTES_PER_PAGE = PAGE_SIZE / NOTE_STEP;

  static uint32_t GetNoteAddress(uint32_t page, uint32_t notes);

  static uint32_t GetRefreshAddress() {
    return m_refresh_address;
  }
  static void SetRefreshAddress(uint32_t addr) {
    m_refresh_address = FIXAddress(addr);
  }

 protected:
  static uint32_t m_refresh_address;
 private:
  static uint32_t NotesToAddressOffset(uint32_t notescount);
};
