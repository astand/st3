#pragma once

#include <stdint.h>

static const int32_t kDescriptorsCount = 10;

typedef struct
{
  char* text;
  int32_t len;
} SmsChunk;

class SmsChunkDescriptor {
 public:
  SmsChunk desc[kDescriptorsCount];
  int32_t goodCnt;
  void Reset();
  void Parse(char* string, int32_t len);
};

