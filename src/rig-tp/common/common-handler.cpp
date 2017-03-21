#include "common-handler.h"
#include "factory/services-factory.h"

namespace Rig
{
static int32_t testCnt = 0;


CommonHandler::CommonHandler(IStreamable& strm) :
  bid(MakeBid()),
  rigFrameBuff_(WorkBuffer()),
  strm_(strm)
{
  rigFrame = (RigFrame*)rigFrameBuff_;
}

HandleResult CommonHandler::HandleIncome(const RigFrame* frame)
{
  if (frame->RigId != selfId)
    return Ignored;

  int32_t ret = UserIncomeHandler(frame, rigFrame);

  if (ret >= 6)
    strm_.Write(rigFrameBuff_, 0, ret);

  return Handled;
}

void CommonHandler::Process()
{
  bid.ProcessControl();

  while (bid.State == Allowed)
  {
    // Send block with number == bid.Send + 1;
    int32_t ret = UserProcess(rigFrame, bid.bidSend + 1);

    if (ret <= 0 || strm_.CanWrite() < (ret + 200))
      // writing isn't performed
      break;

    strm_.Write(rigFrameBuff_, 0, ret);
    bid.ShiftSendBig((ret == 6) ? (true) : (false));
    bid.ProcessControl();
  }

  testCnt++;
}


BidControl& CommonHandler::MakeBid()
{
  static BidControl bControl(8, 3, 10000);
  return bControl;
}

int32_t CommonHandler::UserIncomeHandler(const RigFrame* in, RigFrame* const out)
{
  return 0;
}

int32_t CommonHandler::UserProcess(RigFrame* const out, int32_t need_block)
{
  return 0;
}

}
