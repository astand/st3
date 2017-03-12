#pragma once

#include <stdint.h>
#include "ITrekList.h"
#include "trekrep/trek-location-descriptor.h"
#include "trekrep/flash-note-mapper/mem-notes-mapper.h"
#include "../nmeautil/nmeautil.hpp"
#include "utility/abstract/IFlashMemory.h"

class TrekList : public ITrekList {

 public:
  TrekList( IFlashMemory& reader);
 public: /* ITrekList interface */
  virtual int32_t RefreshTrekList();
  virtual int32_t UploadList(uint16_t, uint16_t, uint8_t*);
  virtual int32_t UploadTrek(uint16_t req_id, int32_t cell_offset, uint32_t maxsize, uint8_t* buf);
  virtual int32_t GetTrekSize(uint16_t req_id);
  virtual int32_t GetListIndex(uint16_t store_id);

 public:
  void ReadLastNote(uint8_t* buf);
  void ReadLastNote(IFlashStorable& obj);

 private:
  static const int32_t M_PAGES_COUNT = NotesMapper::PAGES_COUNT;

 private:
  uint16_t m_store_id;
  uint16_t m_store_num;
  uint32_t addres_for_last_note;

 private:
  uint8_t readbuf[NotesMapper::NOTE_STEP];
  TrekLocationDescriptor trek_dsc[NotesMapper::PAGES_COUNT];
  IFlashMemory& m_reader;

 private:
  int32_t RefreshList(uint32_t start_page, uint16_t cell_num);

  bool CellClean() {
    return (m_store_id == 0xFFFF);
  }
  bool CellValid() {
    return (m_store_id != 0xFFFF && m_store_num != 0xFFFF);
  }

  int32_t ScanTail(uint32_t page, uint16_t start_cell_offset);
  void CleanList();
  /// Load info in buf.
  /// @return liaded memory size (bytes)
  int32_t UploadOneTrekInfo(TrekLocationDescriptor& trek, uint8_t* buf);
  int32_t ReadDataContent(uint32_t addr, uint32_t size);
  void UpdateIDandNUM(uint32_t page, uint16_t cell_on_page);
  bool LastNoteValid();

};

