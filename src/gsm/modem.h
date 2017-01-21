#pragma once

#include <stdint.h>
#include "equip/gsm/at-pipe.h"
#include "equip/gsm/modemstate.h"
#include "equip/gsm/modemanswerstate.h"
#include "equip/gsm/IConnectionParams.h"
#include "abstract/IStreamable.h"

namespace M66
{

class GsmModem {
 public:
  const ModemState& State;

 public:
  GsmModem(AtPipe& pipe);
  bool SyncPipe();
  void CheckNetwork();
  int32_t ListSMS();
  int32_t ListSMS(char* smstext, int32_t maxlength);
  int32_t SendSMS(const char* number, const char* smstext, int32_t textlen);
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

