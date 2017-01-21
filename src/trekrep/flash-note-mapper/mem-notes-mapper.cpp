#include "mem-notes-mapper.h"

uint32_t NotesMapper::GetNoteAddress(uint32_t page, uint32_t notes)
{
  uint32_t retval = NotesToAddressOffset(notes) + AddressOfPage(page);
  return FIXAddress(retval);
}


uint32_t NotesMapper::NotesToAddressOffset(uint32_t notescount)
{
  uint32_t spans_pages = notescount / NOTES_PER_PAGE;
  uint32_t left_notes = notescount % NOTES_PER_PAGE;
  return (spans_pages * PAGE_SIZE) + (left_notes * NOTE_STEP);
}

uint32_t NotesMapper::m_refresh_address = 0;
