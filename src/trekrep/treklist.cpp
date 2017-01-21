#include "treklist.h"
#include <string.h>

TrekList::TrekList(IFlashMemory& reader) : m_reader(reader)
{
  m_store_id = 0;
  m_store_num = 0;
}


int32_t TrekList::UploadList(uint16_t startindex, uint16_t maxcount, uint8_t* buf)
{
  int32_t currentindex = 0;

  while ((maxcount != 0))
  {
    if (((currentindex + startindex) >= M_PAGES_COUNT) ||
        (trek_dsc[currentindex + startindex].IsValid() == false))
      break;

    buf += UploadOneTrekInfo(trek_dsc[currentindex + startindex], buf);
    currentindex++;
    maxcount--;
  }

  return (currentindex) * Matrix_item::Lenght();
}


int32_t TrekList::UploadTrek(uint16_t req_id, int32_t tn_offset,
                             uint32_t maxsize,
                             uint8_t* buf)
{
  uint32_t currentaddr, notesize;
  int32_t startoffset = tn_offset;
  notesize = NaviNote::Lenght();
  uint32_t tn_count = maxsize / notesize;

  if (!trek_dsc[req_id].IsValid())
    return 0;

  while (trek_dsc[req_id].IsTrueOffset(tn_offset) && (tn_count--))
  {
    currentaddr = trek_dsc[req_id].AddressStart(tn_offset);
    ReadDataContent(currentaddr + 4, notesize);
    memcpy(buf, readbuf, notesize);
    buf += notesize;
    tn_offset++;
  }

  return (tn_offset - startoffset) * NaviNote::Lenght();
}

int32_t TrekList::GetTrekSize(uint16_t req_id)
{
  if (req_id < M_PAGES_COUNT && trek_dsc[req_id].IsValid())
    return trek_dsc[req_id].CodeOffset() * Navi::Lenght();

  return 0;
}

int32_t TrekList::GetListIndex(uint16_t store_id)
{
  uint16_t id = 0;

  while (trek_dsc[id].IsValid())
  {
    ReadDataContent(trek_dsc[id].AddressStart(), 2);

    if (*(uint16_t*)readbuf == store_id)
      return id;

    id++;
  }

  return -1;
}

int32_t TrekList::UploadOneTrekInfo(TrekLocationDescriptor& trek, uint8_t* buf)
{
  Matrix_item* item = (Matrix_item*)buf;
  uint16_t cur_id;
  uint32_t startdist, enddist;
  uint32_t payloadsize = trek.CodeOffset() * NaviNote::Lenght();
  ReadDataContent(trek.AddressStart(), 2);
  cur_id = *(uint16_t*)readbuf;
  /* get start time and first dist point */
  ReadDataContent(trek.AddressStart() + 4, NaviNote::Lenght());
  item->StartDate((NaviNote*)readbuf);
  startdist = ((NaviNote*)readbuf)->accum_dist;
  /* get end time and last dist point */
  ReadDataContent(trek.AddressEnd() + 4, NaviNote::Lenght());
  item->EndDate((NaviNote*)readbuf);
  enddist = ((NaviNote*)readbuf)->accum_dist;
  item->setParam(cur_id, payloadsize,	(enddist - startdist), enddist);
  return Matrix_item::Lenght();
}

void TrekList::ReadLastNote(uint8_t* buf)
{
  if (!LastNoteValid())
    return;

  ReadDataContent(trek_dsc[0].AddressEnd() + 4, NaviNote::Lenght());
  memcpy(buf, readbuf, NaviNote::Lenght());
}

void TrekList::ReadLastNote(IFlashStorable& read_obj)
{
  if (!LastNoteValid())
    return;

  m_reader.Read(addres_for_last_note + 4, read_obj);
}

int32_t TrekList::RefreshTrekList()
{
  // uint16_t id = 0;
  uint16_t last_id = 0;
  // uint16_t num = 0;
  int32_t idx = 0;
  uint32_t livesec;
  uint16_t liveoffset;
  livesec = NotesMapper::PageOfAddress(NotesMapper::GetRefreshAddress());
  liveoffset = NotesMapper::MemoryOnPage(NotesMapper::GetRefreshAddress());
  liveoffset = liveoffset / NotesMapper::NOTE_STEP;
  bool seek_tail = true;
  CleanList();

  for (int32_t loop = 0; loop < M_PAGES_COUNT; loop++)
  {
    if (seek_tail)
    {
      int32_t ret = ScanTail(livesec, liveoffset);

      if (ret == 0 && m_store_num != 0)
      {
        /* valid id-num pair founded. freeze offset in current list position */
        trek_dsc[idx].CodeOffset(m_store_num + 1);
        last_id = m_store_id;
        seek_tail = false;
      }

      liveoffset = NotesMapper::NOTES_PER_PAGE - 1;
    }

    UpdateIDandNUM(livesec, 0);

    if (m_store_num == 0)
    {
      /* start trek founded. Check that ID coincidence */
      if (last_id == m_store_id)
      {
        /* valid trek - save sector start and move list to next item */
        trek_dsc[idx].Sector(livesec);
        idx++;
      }

      seek_tail = true;
    }

    livesec = MxMapper::RollBackPage(livesec);
  }

  return idx * Matrix_item::Lenght();
}


int32_t TrekList::ScanTail(uint32_t page, uint16_t start_cell_offset)
{
  /* check start of page */
  UpdateIDandNUM(page, 0);

  if (CellClean())
    return -1;

  for (int32_t step = start_cell_offset; step >= 0; step--)
  {
    UpdateIDandNUM(page, step);

    if (CellValid())
      return 0;
  }

  return -1;
}


void TrekList::CleanList()
{
  for (int32_t index = 0; index < M_PAGES_COUNT; index++)
  {
    /* code */
    trek_dsc[index].CleanData();
  }
}


void TrekList::UpdateIDandNUM(uint32_t page, uint16_t cell_on_page)
{
  uint32_t res_addr = NotesMapper::GetNoteAddress(page, cell_on_page);
  m_reader.Read(res_addr, readbuf, 4);
  m_store_id = *(uint16_t*)(readbuf);
  m_store_num = *(uint16_t*)(readbuf + 2);
}


int32_t TrekList::ReadDataContent(uint32_t addr, uint32_t size)
{
  return m_reader.Read(addr, readbuf, size);
}

bool TrekList::LastNoteValid()
{
  bool retval = false;

  if (trek_dsc[0].IsValid())
  {
    addres_for_last_note = trek_dsc[0].AddressEnd();
    retval = true;
  }

  return retval;
}

