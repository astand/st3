#include <string.h>
#include "cached-trek.h"


static const int32_t kNoteSize = sizeof(NaviNote);
static const int32_t kCacheSize = 10;
static NaviNote notes_[kCacheSize];

CachedTrek::CachedTrek()
{
}


int32_t CachedTrek::Add(const NaviNote& item)
{
  head_ = (head_ + 1) % kCacheSize;
  // copy new item
  notes_[head_] = item;
  loaded_cnt_ = (loaded_cnt_ >= kCacheSize) ? loaded_cnt_ : loaded_cnt_ + 1;
  return 1;
}


int32_t CachedTrek::UpLoad(uint8_t* mem, int32_t maxsize)
{
  int32_t ret = 0;

  if (loaded_cnt_ == 0)
    return ret;

  // get tail index for start uploading from it
  int32_t tail_ = (head_ + 10 - (loaded_cnt_ - 1)) % kCacheSize;

  for (int32_t i = 0; i < loaded_cnt_; i++)
  {
    // copy updated notes to memory
    memcpy(mem + (i * kNoteSize), &notes_[tail_], kNoteSize);
    tail_ = (tail_ + 1) % kCacheSize;
  }

  // update counter to begin from the start
  ret = loaded_cnt_ * kNoteSize;
  loaded_cnt_ = 0;
  return ret;
}
