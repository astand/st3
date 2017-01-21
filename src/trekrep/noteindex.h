#pragma once

#include <stdint.h>

class NoteIndex {
 public:
  NoteIndex();
  void SetNextID();
  void SetNextNum();
  void SetID(uint16_t newid) {
    m_id = PassID(newid);
  }
  uint16_t GetNum() {
    return m_num;
  }
  uint16_t GetID() {
    return m_id;
  }

 protected:

 private:
  static const uint16_t PASSING_FILTER = 0x7FFF;

  uint16_t m_id;
  uint16_t m_num;

  uint16_t PassID(uint16_t dirty_id) {
    return dirty_id & PASSING_FILTER;
  }
};
