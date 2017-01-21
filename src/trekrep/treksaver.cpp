#include "treksaver.h"


TrekSaver::TrekSaver(IFlashMemory& drv) : memdrv(drv)
{
}


void TrekSaver::Init()
{
  /* perform search start address update RefrehAddress and init current ID*/
  uint32_t start_page = SearchStartPage();
  SetStartID(ScanPageID(NotesMapper::RollBackPage(start_page)));
  sector_ = start_page;
  address_ = NotesMapper::AddressOfPage(sector_);
  NotesMapper::SetRefreshAddress(address_);
}

void TrekSaver::SaveNote(IFlashStorable& note)
{
  /// check new sector starting (if true clear sector)
  CheckNewPageStarting();
  /// save id and num
  /// save note data
  SaveNewNote(note);
  /// increment num
  ManageSaving();
}

void TrekSaver::CheckNewPageStarting()
{
  if (IsStartPagePosition())
    memdrv.Erase(address_);
}

void TrekSaver::SaveNewNote(IFlashStorable& row)
{
  *(uint16_t*)&headbuf[0] = m_index.GetID();
  *(uint16_t*)&headbuf[2] = m_index.GetNum();
  memdrv.Write(address_, headbuf, 4);
  memdrv.Write(address_ + 4, row);
  /// Save row
}

void TrekSaver::ManageSaving()
{
  m_index.SetNextNum();
  uint32_t num_number_on_page = m_index.GetNum() % NotesMapper::NOTES_PER_PAGE;

  if (num_number_on_page != 0)
  {
    /* use current page */
    address_ += NotesMapper::NOTE_STEP;
  }
  else
    JumpOnNextPage();

  NotesMapper::SetRefreshAddress(address_);
}

void TrekSaver::StartNewNote()
{
  /// set new address (new sector with 0 offset)
  /// set new id (last id + 1 & 0x7FFF)
  /// set num = 0
  SetNextTrek();
}

/* ------------------------------------------------------------------------- */
int32_t TrekSaver::SearchStartPage()
{
  uint16_t scan_id = ScanPageID(0);
  uint16_t previous_id = scan_id;

  if (scan_id == 0xFFFF)
    return 0;

  for (int32_t page = 0; page < NotesMapper::PAGES_COUNT; page++)
  {
    scan_id = ScanPageID(page);

    if (!(scan_id == (previous_id + 1) || (scan_id == previous_id)))
      return page;

    previous_id = scan_id;
  }

  return 0;
}

// bool TrekSaver::SetAddress(uint32_t page, offset_t cellnum)
// {
//   address_ = GetNoteAddress(page, cellnum);
//   sector_ = PageOfAddress(address_);
//   return true;
// }


void TrekSaver::SetNextTrek()
{
  /* perform increment ID and num starts from 0 */
  m_index.SetNextID();

  /* if current addres is start position then no need move to next page */
  if (IsStartPagePosition() == false)
    JumpOnNextPage();
}

void TrekSaver::SetNextNotePosition()
{
  m_index.SetNextNum();
  uint32_t num_number_on_page = m_index.GetNum() % NotesMapper::NOTES_PER_PAGE;

  if (num_number_on_page != 0)
  {
    /* use current page */
    address_ += NotesMapper::NOTE_STEP;
  }
  else
    JumpOnNextPage();
}


void TrekSaver::JumpOnNextPage()
{
  /* select next page */
  sector_ = MxMapper::RollForthPage(sector_);
  address_ = MxMapper::AddressOfPage(sector_);
}

uint16_t TrekSaver::ScanPageID(uint32_t page)
{
  memdrv.Read(MxMapper::AddressOfPage(page), headbuf, 4);
  return (*(uint16_t*)(headbuf));
}
