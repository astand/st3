#pragma once

#include "common/ARigHandler.h"

class InfoHandler : public ARigHanlder {
 public:
  InfoHandler() {}

 public:
  virtual HandleResult HandleIncome(const RigFrame* frame);
  virtual void Process();

 private:

};

