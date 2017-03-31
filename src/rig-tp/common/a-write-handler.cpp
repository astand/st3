#include "a-write-handler.h"
#include "factory/services-factory.h"
#include "dbgconf.h"

namespace Rig
{

AWriteHandler::AWriteHandler(IStreamable& strm) : strm_(strm),
  buff_(WorkBuffer())
{
  rigFrame = (RigFrame*)buff_;
}

HandleResult AWriteHandler::HandleIncome(const RigFrame* frame, int32_t dataSize)
{
  int32_t ret = 0;

  if (frame->RigId != selfId)
    return Ignored;

  rigFrame->RigId = selfId;

  if (frame->Opc == WRQ)
  {
    rigFrame->Opc = WRQ;
    rigFrame->BlockNum = 0;
    ret = UserIncomeHead(frame, dataSize);

    if (ret > 0)
    {
      if (rigFrame->Opc == ERR)
      {
      }
      else
      {
        ack_block = 1;
      }
    }
  }
  else if (frame->Opc == DATA)
  {
    rigFrame->Opc = ACK;

    if (rigFrame->BlockNum == ack_block)
    {
      rigFrame->BlockNum = ack_block;
      ack_block++;
      ret = UserIncomeData(frame, dataSize);
    }
  }

  if (ret >= 0)
  {
    strm_.Write(buff_, 0, ret + 6);
  }

  return Handled;
}


void AWriteHandler::Process() {}

}
