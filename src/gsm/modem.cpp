#include <stdio.h>
#include <string.h>
#include "modem.h"
#include "etc/str0.h"
//#include "equip/gsm/at-cmds/at-commands.h"
#include "configs/dbgconf.h"
#include "equip/gsm/pdu.h"

#define AT_OK   (char *)"OK"

namespace M66
{

const int32_t kDebugLen = 128;
static char cmd_debug[kDebugLen] = { 0 };

const int32_t err_str_len = 3;
const char* err_string[err_str_len] =
{
  "ERROR",
  "NO CARRIER",
  "+CMS ERROR:"
};


static const char* DropLF(const char* in)
{
  int32_t cpy_cnt = strlen(in);;

  if (cpy_cnt > kDebugLen - 1)
  {
    cpy_cnt = kDebugLen - 1;
  }

    memcpy(cmd_debug, in, cpy_cnt - 1);
    cmd_debug[cpy_cnt - 1] = '\0';
  return cmd_debug;
}

static void PrintDebug(const ResponseState& st, const char* answer, int32_t length)
{
  if (st == kNone)
  {
    DBG_Modem("[mdm] ! warning. No answer\n");
  }
  else if (st == kOk)
  {
    DBG_Modem("[mdm] Ack received: %s\n", answer);
  }
  else if (st == kAlter)
  {
    DBG_Modem("[mdm] ! warning. Alternate received: %s\n", answer);
  }
  else if (st == kError)
  {
    DBG_Modem("[mdm] ! error. Received: %s\n", answer);
  }
  else if (st == kUnknown)
  {
    DBG_Modem("[mdm] ! warning. UNKNOWN received: %s\n", answer);
  }

  DBG_Modem("\n\n");
}

static bool WrongAnswer(const ResponseState& st)
{
  return (st == kError || st == kAlter || st == kUnknown);
}
/* ------------------------------------------------------------------------- */
GsmModem::GsmModem(AtPipe& pipe) : State(mdmstate_), /*InetState(qistate_),*/ pipe_(pipe),
  answ_(pipe.answer()), pipeState(pipe.State)
{
}


bool GsmModem::SyncPipe()
{
  mdmstate_ = kNoecho;

  for (int32_t cnt = 0; cnt < 30; cnt++)
  {
    Send("AT\r\n", 20, false);
    AtResponse(AT_OK);

    if (pipeState == kOk)
      break;
  }

  if (pipeState != kOk)
    return false;

  Send("ATE0\r\n", 20, false);
  AtResponse(AT_OK);
  Send("AT+IPR?\r\n", 20, false);
  AtResponse("+IPR: ");
  int32_t ret = str0::Atoi(str0::ToSym(answ_, ' ', 1));

  if (ret != 115200)
  {
    Send("AT+IPR=115200&W\r\n");
    AtResponse(AT_OK);
  }

  for (int32_t cnt = 0; cnt < 30; cnt++)
  {
    Send("AT+CPIN?\r\n", 300);
    AtResponse("+CPIN: READY", 0);

    if (pipeState == kOk)
      break;
  }

  if (pipeState != kOk)
    return false;

  wrong_in_response = false;
  Send("AT&D1\r\n");
  AtResponse(AT_OK);
  Send("AT+CMGF=0\r\n");
  AtResponse(AT_OK);
  Send("AT+QIMODE=1\r\n");
  AtResponse(AT_OK);
  Send("AT+QITCFG=3,2,768,1\r\n");
  AtResponse(AT_OK);
  return (wrong_in_response);
}

void GsmModem::CheckNetwork()
{
  NetworkRegiastration();
  SignalQuality();
  Clock();
}



///@return - 0 if a sms is not available, 1 - if the sms is ready
int32_t GsmModem::ListSMS()
{
  Send("AT+CMGL=4\r\n", 50);
  AtResponse("+CMGL: ", AT_OK);
  return (pipeState == kOk) ? 1 : 0;
}


int32_t GsmModem::ListSMS(char* smstext, int32_t maxlength)
{
  int32_t pdulen;
  int32_t ret = ListSMS();

  if (ret > 0)
  {
    smsid = str0::Atoi(str0::ToSym(answ_, ' ', 1));
    /// parse PDU length
    pdulen = str0::Atoi(str0::ToSym(answ_, 3));
    // sms pdu PAYLOAD receving
    ret = AtResponse(0, 0, 1000);

    if (ret > 0)
    {
      pdulen = (pdulen > maxlength) ? (maxlength) : (pdulen);
      ret = FromPduToAscii(smstext, answ_, pdulen);
    }

    /// Trow all messages until "OK"
    AtResponse("OK", 0, 5000);
    DeleteSMS(smsid);
  }

  return ret;
}

int32_t GsmModem::SendSMS(const char* smstext, int32_t smslen)
{
  int32_t send_ret = 0;
  int32_t text_len = smslen;

  if (smslen < 0)
  {
    text_len = strlen(smstext);
  }

  int32_t pdu_len = FromAsciiToPdu(workbuff + 16, smstext, text_len, 0);
  sprintf(workbuff, "AT+CMGS=%d\r", pdu_len);
  Send(workbuff);
  AtResponse();
  Send(workbuff + 16, 20);
  send_ret = AtResponse("+CMGS: ");
  return send_ret;
}


/* --------------------------------------------------------------------------- */
int32_t GsmModem::Connect(const char* ip, const char* port, const char* apn,
                          const char* usrname, const char* usrpsw)
{
  Scan_QI();
  Configure_QI(apn, usrname, usrpsw);
  Connect_QI(ip, port);

  if (pipeState == kOk)
    mdmstate_ = kConnected;

  return 0;
}

int32_t GsmModem::CloseConnection()
{
  Close_QI();
  return 0;
}


int32_t GsmModem::IsConnectionAlive()
{
  Suspend_QI();

  if (pipeState != kOk)
    return -1;

  CheckNetwork();

  if (ListSMS() != 0)
  {
    /// sms was received ---> exit
    return -3;
  }

  Resume_QI();

  if (pipeState != kOk)
    return -2;

  return 0;
}

/* --------------------------------------------------------------------------- */
void GsmModem::Suspend_QI()
{
  Send("+++", 500);
  AtResponse("OK", 0, 1000);
}


void GsmModem::Resume_QI()
{
  Send("ATO0\r\n");
  AtResponse("CONNECT", "NO CARRIER");
}


void GsmModem::Scan_QI()
{
  /// Query Connection Status of the Current Access (300 ms)
  Send("AT+QISTATE\r\n");
  int32_t retres = AtResponse("STATE: ", "ERROR");

  if (pipeState == kOk)
  {
    CheckConnStatus(answ_, retres);
  }
  else if (WrongAnswer(pipeState))
  {
    qistate_ = kNeedDeactive;
  }
}


void GsmModem::Configure_QI(const char* apn, const char* name, const char* psw)
{
  if (qistate_ == kNeedDeactive)
  {
    /// Deactivate GPRS/CSD PDP Context (40 s)
    Send("AT+QIDEACT\r\n");
    AtResponse("DEACT OK", "ERROR", 40 * 1000);
    qistate_ = kNeedRegister;
  }

  if (qistate_ == kNeedRegister)
  {
    /// Start TCPIP Task and Set APN, User Name and (300 ms)
    Send("AT+QIREGAPP\r\n");
    AtResponse("OK", "ERROR");
    Send("AT+QIACT\r\n");
    AtResponse("OK", "ERROR", 40 * 1000);
    /// Get Local IP Address (300 ms)
    Send("AT+QILOCIP\r\n");
    AtResponse(0, "ERROR");

    if (pipeState != kError || pipeState == kNone)
    {
      /// big error. connection cannot be started
    }
  }
}


void GsmModem::Connect_QI(const char* ip, const char* port)
{
  sprintf(workbuff, "AT+QIOPEN=\"TCP\",\"%s\",\"%s\"\r\n", ip, port);
  // Start up TCP or UDP Connection
  Send(workbuff);
  AtResponse("OK", "CONNECT FAIL");
}


void GsmModem::Close_QI()
{
  Send("AT+QICLOSE\r\n");
  AtResponse("CLOSE OK", "ERROR", 5 * 1000);
  Send("AT+QISTATE\r\n");
  AtResponse("STATE:", "ERROR");

  if (WrongAnswer(pipeState))
    mdmstate_ = kNoecho;
}


void GsmModem::DeleteSMS(int32_t smsid)
{
  sprintf(workbuff, "AT+CMGD=%d\r\n", smsid);
  Send(workbuff, 50);
  AtResponse("OK");
}


void GsmModem::CheckConnStatus(const char* ack, int32_t len)
{
  /// Get actual state string
  const char* qi_stat = str0::ToSym(ack, ' ', 1);
  qistate_ = kNeedDeactive;

  if (memcmp(qi_stat, "IP INI", 6) == 0)
    qistate_ = kNeedRegister; // activation will perform
  else if ((memcmp(qi_stat, "IP START", 8) == 0) ||
           (memcmp(qi_stat, "IP CLOSE", 8) == 0))
    qistate_ = kIdle; // connection will perform immideately
}


/* ------------------------------------------------------------------------- */
int32_t GsmModem::Clock()
{
  Send("AT+CCLK?\r\n");
  int32_t ret = AtResponse("+CCLK: ");
  return ret;
}


void GsmModem::NetworkRegiastration()
{
  Send("AT+CREG?\r\n");
  int32_t resparse = AtResponse("+CREG: ");

  if (resparse > 0)
  {
    int32_t regstatus = str0::Atoi(str0::ToSym(answ_, ',', 1));
    mdmstate_ = (regstatus == 1 || regstatus == 5) ? (kRegOk) : (kNoReg);
  }
}

void GsmModem::SignalQuality()
{
  Send("AT+CSQ\r\n");
  int32_t resparse = AtResponse("+CSQ: ");

  if (resparse > 0)
  {
    csqquality = str0::Atoi(str0::ToSym(answ_, ' ', 1));
  }
}

int32_t GsmModem::Send(const char* cmd, uint32_t cmddelay, bool rx_flush)
{
  DBG_Modem("[mdm] >>> %s\n", DropLF(cmd));
  return pipe_.AtSend(cmd, cmddelay, rx_flush);
}


int32_t GsmModem::AtResponse(const char* oks, const char* alts, int32_t timeout)
{
  int32_t retparse = pipe_.AtRead(oks, alts, timeout);
  PrintDebug(pipeState, answ_, retparse);

  if (WrongAnswer(pipeState))
    wrong_in_response = true;

  return retparse;
}
/* ----------------------------------------------------------------------------------- */

};



