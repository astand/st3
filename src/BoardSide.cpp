#include "BoardSide.h"

#define GTFTP_ERR_NOFILE
/*
 *  25 * 24 = 600
 */
#define DATA_BLOCK_DT       ((uint32_t)600)



/*
* */
GTftp2Blk::GTftp2Blk()
{
  Anew();
}

/*
* */
void GTftp2Blk::ResetResend()
{
  resend_cnt = RESEND;
}

/*
* */
bool GTftp2Blk::IsBlockValid(uint16_t id, uint16_t windsize)
{
  return (bidack == (id - windsize));
}


/*
* */
bool GTftp2Blk::BlockCheck(uint16_t bid)
{
  if (IsBlockValid(bid, 1))
  {
    bidack += 1;
    return true;
  }

  return false;
}

/*
* */
void GTftp2Blk::Anew()
{
  bidsend = 1;
  resend_cnt = RESEND;
  bidack = 0;
  bidlast = 0;
  ResetResend();
}

/*
* */
bool GTftp2Blk::RollBackBidsend()
{
  if (resend_cnt == 0)
    return false;
  else
    resend_cnt--;

  bidlast = 0;
  bidsend = bidack + 1;
  return true;
}
/*
* */
bool GTftp2Blk::IsBlockSendPermitted()
{
  return ((bidsend <= (bidack + WINDOWS)) && (bidlast != bidsend));
}

/*
* */
bool GTftp2Blk::Last()
{
  return (bidack == bidlast);
}
/*
* */
bool GTftp2Blk::FixLastBid(int32_t size)
{
  if (size > 0)
  {
    bidsend++;
    return false;
  }
  else
    bidlast = bidsend;

  return true;
}
/*
* */

/*
* */

/*
* */

/*
* */

/*
* */

/*
* */

/*
* */

/*
* */
ReceiveStatus::ReceiveStatus()
{
  _stat = Free;
}
/*
* */
uint32_t ReceiveStatus::WAITFLAGS()
{
  return static_cast<uint32_t>( WaitAck | WaitData | SendData);
}
/*
* */
void ReceiveStatus::DropSendAction()
{
  uint32_t ret = static_cast<uint32_t>(_stat);
  ret &= ~WAITFLAGS();
  _stat = static_cast<ReceiveResult>(ret);
}
/*
* */

/*
* */
/*
* */

/*
* */

/*
* */

/*
* */
void BoardSideInst::ResetAll()
{
  setFid(0);
  bchief.Anew();
}

/*
* */
void BoardSideInst::headUp()
{
  headUp(come->blockid, come->opc);
}
/*
* */
void BoardSideInst::headUp(uint16_t bid)
{
  headUp(bid, come->opc);
}
/*
* */
void BoardSideInst::headUp(uint16_t bid, uint16_t opc)
{
  from->blockid = bid;
  from->opc = opc;
}
/*
* */
uint16_t BoardSideInst::Options()
{
  return (*(volatile uint16_t*)(come->msg));
}
/*
* */
uint16_t BoardSideInst::Options(uint16_t val)
{
  return (*(volatile uint16_t*)(from->msg)) = val;
}
/*
* */
uint16_t BoardSideInst::setFid()
{
  return (_fid = (come->blockid));
}
/*
* */
bool BoardSideInst::Validation()
{
  if (bchief.BlockCheck(come->blockid))
  {
    bchief.ResetResend();
    return true;
  }

  return false;
}

/*
* */
int32_t BoardSideInst::FSize()
{
  return FInsize.ANew(*(volatile uint32_t*)(come->msg + 2));
}
/*
* */

/*
* */

/*
* */

/*
* */

/*
* */

/*
* -------------------------------------------------------------------------- */

/*
* */
int32_t CommonFileSize::ANew(int32_t v)
{
  LenghtFile(v);
  Passed(0);
  return LenghtFile();
}
/*
* */
int32_t CommonFileSize::AddPassing(int32_t blksize)
{
  int32_t ret = Passed() + blksize;

  if (ret > LenghtFile() || ret < 0)
  {
    /* overflow occasion */
  }
  else
    Passed(ret);

  return Passed();
}
/*
* */

/*
* */

/*
* */

/*
* */


