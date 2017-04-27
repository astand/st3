#pragma once

#include "serv-param.h"
#include "tim-param.h"

class GprsParam {
 public:
  GprsParam() {}
  void Clean() {
    serv.ResetAll();
    tim.Reset();
  }

 public:
  ServParam serv;
  TimParam tim;
};
