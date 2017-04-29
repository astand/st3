#include <cstdio>
#include <cstring>
#include "gprs-param.h"
#include "../sms-chunks.h"
#include "etc/str0.h"

void GprsParam::Default()
{
  serv.ResetAll();
  tim.Reset();
}

int32_t GprsParam::Parse(const SmsChunkDescriptor& dsc)
{
  char parse_type = dsc.desc[0].text[1];

  if (parse_type == '0')
  {
    /// server parsing
    return ParseServ(dsc);
  }
  else if (parse_type == '1')
  {
    /// timing parsing
    return ParseTim(dsc);
  }

  return 0;
}


int32_t GprsParam::Print(char* str)
{
  std::sprintf(str, "Serv:%s,%s,%s. Tim > %d,%d,%d. ", serv.GetParametr(kServer),
               serv.GetParametr(kPort), serv.GetParametr(kAPN),
               tim.Big, tim.Silent, tim.ReConnect);
  return std::strlen(str);
}


int32_t GprsParam::ParseServ(const SmsChunkDescriptor& dsc)
{
  int i = 1;

  while (dsc.desc[i].len > 0)
  {
    serv.UpdateParametr((ParamPosition)(i - 1), dsc.desc[i].text, dsc.desc[i].len);
    i++;
  }

  return 1;
}


int32_t GprsParam::ParseTim(const SmsChunkDescriptor& dsc)
{
  if (dsc.goodCnt < 4)
    return -1;

  tim.Big = str0::Atoi(dsc.desc[1].text);
  tim.Silent = str0::Atoi(dsc.desc[2].text);
  tim.ReConnect = str0::Atoi(dsc.desc[3].text);
  return 1;
}

