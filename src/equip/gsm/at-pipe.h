#pragma once

#include <stdint.h>
#include "equip/gsm/IFailAnswerDetector.h"
#include "equip/gsm/at-framer.h"
#include "equip/gsm/modemanswerstate.h"


class AtPipe {
 public:
  typedef const char* pipe_t;
 public:
  AtPipe(IFailAnswerDetector& fail, AtFramer& atf) :
    State(atstate_), failer_(fail), framer_(atf) {}

 public:
  const ResponseState& State;

 public:
  int32_t AtSend(pipe_t str, int32_t ms_delay = 20, bool rx_flush = true);
  int32_t AtRead(pipe_t ok = 0, pipe_t alt = 0, int32_t resp_time = 300);
  pipe_t answer() {
    return framer_.Answer();
  }
 private:
  IFailAnswerDetector& failer_;
  AtFramer& framer_;
  ResponseState atstate_;
};


