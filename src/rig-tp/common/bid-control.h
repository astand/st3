#pragma once

#include <stdint.h>
#include "timers/timers.h"

typedef enum
{
  Allowed,
  Denied,
  Finished,
  Idle
}
BidState;

class BidControl {
 public:
  const BidState& State;
  int32_t bidSend;
  int32_t bidAck;

 public:
  BidControl() : State(state_) {}
  void Config(int32_t window, int32_t resend, int32_t wait_to);
  void StopSending();
  void ResetAll();
  void CheckIncomeBid(int32_t recBid);
  void ShiftSendBig(bool is_last = false);
  void ProcessControl();

 private:
  int32_t bidWait;
  int32_t bidLast;
  BidState state_;

  int32_t window_;
  int32_t resend_;
  int32_t resended_;
  int32_t timeout_;
  DTimers::Timer tim_;
};
