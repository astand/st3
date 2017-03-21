#include "bid-control.h"


BidControl::BidControl(int32_t window, int32_t resend, int32_t wait_to) :
  State(state_)
{
  window_ = window;
  resend_ = resend;
  timeout_ = wait_to;
}

void BidControl::StopSending()
{
}


void BidControl::StartSending()
{
  bidLast = -1;
  bidAck = 0;
  bidSend = 0;
  state_ = Allowed;
  tim_.Start(timeout_);
}


void BidControl::CheckIncomeBid(int32_t recBid)
{
  if (recBid == bidAck + 1)
  {
    bidAck = recBid;
  }

  if (bidAck == bidLast)
  {
    state_ = Finished;
  }
}


void BidControl::ShiftSendBig(bool is_last)
{
  bidSend += 1;
  resended_ = 0;

  if (is_last && bidLast == -1)
  {
    bidLast = bidSend;
  }

  tim_.Start(timeout_);
}

void BidControl::ProcessControl()
{
  if (tim_.Elapsed())
  {
    // resend action
    resended_ += 1;

    if (resended_ >= resend_)
    {
      state_ = Idle;
      return;
    }

    state_ = Allowed;
    bidSend = bidAck;
  }

  if (bidSend > (bidAck + window_) || bidLast == bidSend)
  {
    state_ = Denied;
  }
}
