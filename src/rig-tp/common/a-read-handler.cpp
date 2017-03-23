#include "a-read-handler.h"
#include "factory/services-factory.h"
#include "dbgconf.h"

namespace Rig
{

AReadHandler::AReadHandler(IStreamable& strm) :
  rigFrameBuff_(WorkBuffer()),
  strm_(strm)
{
  rigFrame = (RigFrame*)rigFrameBuff_;
  bid.Config(8, 3, 10000);
}

HandleResult AReadHandler::HandleIncome(const RigFrame* frame)
{
  if (frame->RigId != selfId)
    return Ignored;

  int32_t ret = 0;
  rigFrame->RigId = selfId;
  DBG_Rig("[RigPack] : INCOME <<< OPC=%04d RIGID=%04d NUM=%04d\n",
          frame->Opc, frame->RigId, frame->BlockNum);

  if (frame->Opc == RRQ)
  {
    // preset head, user can change its values if it required
    rigFrame->Opc = RRQ;
    rigFrame->BlockNum = 0;
    ret = UserIncomeHead(frame, rigFrame);

    if (ret > 0)
    {
      if (rigFrame->Opc == ERR)
      {
        // it doesn't need to handle process DATA
      }
      else
      {
        // client accepted head request
        bid.ResetAll();
      }
    }
  }
  else if (frame->Opc == ACK)
  {
    bid.CheckIncomeBid(frame->BlockNum);
    ret = UserIncomeAck(frame, rigFrame);
  }

  if (ret > 0)
  {
    DBG_Rig("[RigPack] : SENDING >>> OPC: %4d  RIGID: %4d  NUM: %4d Len[%5d]\n",
            rigFrame->Opc, rigFrame->RigId, rigFrame->BlockNum, ret);
    strm_.Write(rigFrameBuff_, 0, ret + 6);
  }

  return Handled;
}

void AReadHandler::Process()
{
  bid.ProcessControl();
  rigFrame->Opc = DATA;
  rigFrame->RigId = selfId;

  while (bid.State == Allowed)
  {
    // preset head, user can change its values if it required
    rigFrame->BlockNum = bid.bidSend + 1;
    // user load data and return data size (if less then 0 - no need sending)
    int32_t ret = UserProcess(rigFrame, bid.bidSend + 1);

    if (ret < 0 || strm_.CanWrite() < (ret + 200))
      // writing isn't performed
      break;

    DBG_Rig("[RigPack] : SENDING >>> OPC: %4d  RIGID: %4d  NUM: %4d Len[%5d]\n",
            rigFrame->Opc, rigFrame->RigId, rigFrame->BlockNum, ret);
    strm_.Write(rigFrameBuff_, 0, ret + 6);
    bid.ShiftSendBig((ret == 0) ? (true) : (false));
    bid.ProcessControl();
  }
}
// namespace close
}
