#pragma once

#include <stdint.h>
#include "trekrep/flash-note-mapper/mem-notes-mapper.h"
#include "noteindex.h"
#include "utility/abstract/IFlashMemory.h"

class TrekSaver {
 public:
  typedef uint16_t offset_t;
 public:
  TrekSaver(IFlashMemory& drv);

 public:
  void Init();
  virtual void SaveNote(IFlashStorable& note);
  virtual void StartNewNote();

  void SetNextTrek();
  void SetNextNotePosition();
  bool IsStartPagePosition() {
    return NotesMapper::IsPageBeginAddress(address_);
  }

  void SetStartID(uint16_t newid) {
    return m_index.SetID(newid);
  }

 private:
  uint32_t address_;
  uint32_t sector_;
  uint8_t headbuf[4];

 private:
  NoteIndex m_index;
  IFlashMemory& memdrv;

 private:
  uint16_t ScanPageID(uint32_t page);
  int32_t SearchStartPage();

  void JumpOnNextPage();
  void CheckNewPageStarting();
  void SaveNewNote( IFlashStorable& note);
  void ManageSaving();

  uint32_t PageTailCells() {
    return (NotesMapper::MemoryOnPage(sector_) / NotesMapper::NOTE_STEP);
  }
};

