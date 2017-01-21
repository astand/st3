#pragma once

#include <stdint.h>
#include "equip/gsm/at-pipe.h"
#include "equip/gsm/modemstate.h"
#include "equip/gsm/modemanswerstate.h"
#include "equip/gsm/IConnectionParams.h"
#include "abstract/IStreamable.h"

namespace M66
{


/// @CMD_DELAY - collection of timings for waiting before sending CMD to modem
/// unit - <ms>
// #define CMD_DELAY20       ((uint32_t) 20)
// #define CMD_DELAY50       ((uint32_t) 50)

#define CMD_DELAY_SIM     ((uint32_t) 500)

/// @WAIT - collection of timings for waiting different answers
/// from modem. unit - <ms>
// #define WAIT_ANSWER15     ((uint32_t)150)
// #define WAIT_ANSWER30     ((uint32_t)300)

#define WAIT_CLOCK        ((uint32_t)500)
#define WAIT_ECHO         ((uint32_t)200)

#define WAIT_SMS_LIST     ((uint32_t)700)
#define WAIT_SMS_DEL      ((uint32_t)3000)

#define WAIT_CG_ATT       ((uint32_t) 10000)
#define WAIT_CG_CONN      ((uint32_t) 10000)
#define WAIT_CG_DISCONN   ((uint32_t) 5000)

#define WAIT_PRESUSPEND   ((uint32_t) 1000)
#define WAIT_POSTSUSPEND  ((uint32_t) 1500)

#define WAIT_RESTORE      ((uint32_t) 1500)


#define WAIT_CG_CLOSE     ((uint32_t) 5000)


class GsmModem {
  // typedef uint16_t oswait_t;
 public:
  const ModemState& State;
  // const GprsState& InetState;

 public:
  // const AtPipe::strbuf_t* AtData();
  /// @GsmModem - take stream and pointer to task context switch
  // GsmModem(IStreamable& str, ISwitchable& powerpin, void (*waiter)());
  GsmModem(AtPipe& pipe);


  /// @TickCounters - must be call every 1 ms, for handle inner timings
//  void TickCounters();

  bool SyncPipe();
  void CheckNetwork();
  int32_t ListSMS();
  int32_t ListSMS(char* smstext, int32_t maxlength);
  /* --------------------------------------------------------------------------- */
  int32_t Connect(const char* ip, const char* port, const char* apn = 0,
                  const char* usrname = 0, const char* usrpsw = 0);

  int32_t CloseConnection();
  int32_t IsConnectionAlive();
  /* --------------------------------------------------------------------------- */

  int32_t CSQ() {
    return csqquality;
  }

 private:
  static const int32_t SEC_TO_TICKS = 1000;
  static const uint32_t session_sched = 16000; // 15 sec for regular request in connection mode
  static const uint32_t idle_sched = 6000; // 5 sec for waiting regular reg and csq state request
  static const uint32_t session_init_time = 20; // 20 sec for first waiting session start
  static const int32_t session_silent_time = 60;

  // static const int32_t err_str_len;
  // static const char* err_string[];

  static const int32_t maxlenght_pCG = 32;
  static const int32_t workbuf_lenght = 256;

 private:
  // ModemOperateState opstate;
  // ModemAnswerState ackstate;
  AtPipe& pipe_;
  AtPipe::pipe_t answ_;
  ModemState mdmstate_;
  GprsState qistate_;

  int32_t regstatus;
  int32_t csqquality;
  uint32_t smsid;

  char workbuff[workbuf_lenght];
  bool wrong_in_response;

 private:
  int32_t AtResponse(const char* okanswer = 0, const char* altanswer = 0, int32_t timeout = 300);
  int32_t Send(const char* cmd, uint32_t cmddelay = 20, bool rx_flush = true);
  // bool IsAnswerFail(const AtFramer::strbuf_t* answ, int32_t lenght);
  const ResponseState& pipeState;

 private:

  /// GPRS Methods
 private:
  void Scan_QI();
  void Configure_QI(const char* apn, const char* name, const char* psw);
  void Connect_QI(const char* ip, const char* port);
  void Close_QI();

  void Suspend_QI();
  void Resume_QI();

 private:
  void CheckConnStatus(const char* ack, int32_t len);

  /// Common methods
 private:
  void NetworkRegiastration();
  void SignalQuality();
  int32_t Clock();

};


};

