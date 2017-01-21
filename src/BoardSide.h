#ifndef __BOARDERSIDE_H__
#define __BOARDERSIDE_H__



#include <stdint.h>
#define DATA_OUT  ((uint16_t)1 << 14)
/*
* OPC
* */
#define OPC_RRQ   ((uint16_t) 1)
#define OPC_WRQ   ((uint16_t) 2)
#define OPC_DATA  ((uint16_t) 3)
#define OPC_ACK   ((uint16_t) 4)
#define OPC_ERR   ((uint16_t) 5)

/*
* ID
* */
#define ID_NONE           ((uint16_t) 0)
#define ID_GET_FILENOTES  ((uint16_t) 1)
#define ID_GET_TRACK      ((uint16_t) 2)
#define ID_GET_INFO       ((uint16_t) 4)
#define ID_GET_ECHO       ((uint16_t) 128)
#define ID_FIRMWARE       ((uint16_t) 3 | DATA_OUT)

/*
* -------------------------------------------------------------------------- */

/*
* */
typedef struct
{
  uint16_t opc;
  uint16_t blockid;
  uint8_t  msg[1020];
} GTftp2_t;
/*
* -------------------------------------------------------------------------- */

/*
* */
class GTftp2Blk {
 public:

  static uint16_t const RESEND = 2;
  static uint16_t const WINDOWS = 8;
  /* @bidsend
   * number of currently sending packet, values havn't to
   * be more then bidack more then @bidwindow
   **/
  uint16_t bidsend;



  /* @bidwindow - size of gap beetwen @bidsend and @bidack
   * expirience shows that value = 4 best
   **/
  uint16_t bidwindow;



  /* @resend_cnt - count of resending action
   *
   **/
  uint16_t resend_cnt;



  /* @bidack - number of last acknowledged block
   *
   **/
  uint16_t bidack;

  /* @bidlast - number of last block
   *
   **/
  uint16_t bidlast;




  GTftp2Blk(uint16_t id) {
    bidack = id;
  }
  GTftp2Blk();

  void ResetResend();



  bool IsBlockValid(uint16_t id, uint16_t windsize);
  bool BlockCheck(uint16_t id);

  void Anew();

  bool RollBackBidsend();
  bool IsBlockSendPermitted();
  bool Last();

  bool FixLastBid(int32_t size);
 private:
  void FixLastBid() {
    bidlast = bidsend;
  }
};

/*
* */
enum ReceiveResult
{
  Free = (0),
  WaitData = (1 << 1),
  WaitAck = (1 << 2),
  SendData = (1 << 3),

  BlockWrong = (1 << 15)
};
/*
* -------------------------------------------------------------------------- */

/*
* */
class ReceiveStatus {

  ReceiveResult _stat;

  uint32_t WAITFLAGS();

 public:
  ReceiveStatus();
  void DropSendAction();
};
class CommonFileSize {
  int32_t _full_file_size;
  int32_t _success_passed;
 public:
  CommonFileSize() {
    ANew(0);
  }
  int32_t LenghtFile(int32_t v) {
    return _full_file_size = SizeCorrect(v);
  }
  int32_t LenghtFile() {
    return _full_file_size;
  }



  int32_t ANew(int32_t fsize);

  int32_t AddPassing(int32_t bsize);

  int32_t Passed() {
    return _success_passed;
  }


 private :
  int32_t Passed(int32_t v) {
    return _success_passed = SizeCorrect(v);
  }

  int32_t SizeCorrect(int32_t v) {
    return (v > 0) ? v : 0;
  }





//  public override string ToString()
//  {
//    //string message = String.Format(" Passed {0:6}b/{1:6}b", LenghtPassed, LenghtFile);
//    return (String.Format(" Passed: {0,8}b / {1}b.", Passed, LenghtFile));
//  }
};

/*
* -------------------------------------------------------------------------- */
class BoardSideInst {

 public:

  uint16_t _fid;
  uint16_t _trcid;
  ReceiveStatus recres;
  GTftp2Blk bchief;
  GTftp2_t* come;
  GTftp2_t* from;
  CommonFileSize FInsize;

  /*
  * */

  /*
  * */

  /*
  * */

  /*
  * */
  BoardSideInst() {};

  /*
  * */
  void ResetAll();

  void headUp();
  void headUp(uint16_t bid);
  void headUp(uint16_t bid, uint16_t opc);
  void headError(uint16_t bid) {
    headUp(bid, OPC_ERR);
  }


  void headDataUp() {
    headUp(bchief.bidsend, OPC_DATA);
  }

  void headAckUp() {
    headUp(come->blockid, OPC_ACK);
  }
  void headAckUp(uint16_t bid) {
    headUp(bid, OPC_ACK);
  }
  /*
  * */
  uint16_t Options();
  uint16_t Options(uint16_t val);

  /* set size of transmitting file to @from message
  * */
  void FSize(int32_t v) {
    (*(volatile uint32_t*)(from->msg + 2)) = v;
  }
  /*
  * Read size of receiving file  */
  int32_t FSize();
  /*
  * */
  uint16_t setFid();
  uint16_t setFid(uint16_t v) {
    return _fid = v;
  }
  uint16_t getFid() {
    return _fid;
  }
  bool getFid(uint16_t mask) {
    return (getFid() & mask);
  }
  /*
  * if it is 0 fileID (Fid) or File with OUT flag not implement sendAction */
  bool SendActionDenied() {
    return (getFid(DATA_OUT) || getFid() == 0);
  }
  /*
  * */
  bool Validation();

  /*
  * */
  uint16_t NeedTreck() {
    return _trcid;
  }
  uint16_t NeedTreck(uint16_t v) {
    return (_trcid = v);
  }
};



#endif
