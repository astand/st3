#include "sms-chunks.h"

static bool IsSym(const char sym)
{
  uint8_t symbol = sym;
  return (symbol > 0x19 && symbol < 0xFF);
}

void SmsChunkDescriptor::Reset()
{
  for (int i = 0; i < kDescriptorsCount; i++)
  {
    desc[i].text = 0;
    desc[i].len = 0;
  }

  goodCnt = 0;
}


void SmsChunkDescriptor::Parse(char* sms, int32_t len)
{
  Reset();
  desc[goodCnt].text = sms;

  for (int i = 0; (i < len) && (goodCnt < kDescriptorsCount) && IsSym(sms[i]); i++)
  {
    if (sms[i] == '#' || sms[i] == ',')
    {
      sms[i] = '\0';
      ++goodCnt;
      desc[goodCnt].text = sms + i + 1;
    }
    else
    {
      ++desc[goodCnt].len;
    }
  }

  if (desc[goodCnt].len > 0)
    ++goodCnt;
}

