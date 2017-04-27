#include "gprs-param.h"
#include "../sms-chunks.h"

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
  return 1;
}

