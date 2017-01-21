#include "noteindex.h"

NoteIndex::NoteIndex()
{
  m_id = PassID(0);
  m_num = 0;
}

void NoteIndex::SetNextID()
{
  /// set next @id and anull @num
  SetID(m_id + 1);
  m_num = 0;
}

void NoteIndex::SetNextNum()
{
  m_num = m_num++;
}
