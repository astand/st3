#pragma once

#include "serv-param.h"
#include "tim-param.h"

class SmsChunkDescriptor;

class GprsParam {
 public:
  GprsParam() {}
  void Default();
  int32_t Parse(const SmsChunkDescriptor& dsc);
  int32_t Print(char* str);

 public:
  ServParam serv;
  TimParam tim;

 private:
  int32_t ParseServ(const SmsChunkDescriptor& dsc);
  int32_t ParseTim(const SmsChunkDescriptor& dsc);
};
