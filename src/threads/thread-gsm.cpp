#include "thread-gsm.h"
#include "threads/thread-gps.h"
#include "configs/free-rtos-def.h"
#include "configs/dbgconf.h"
#include "Configs/MainConfig.h"
#include "Configs/Version.h"
#include "rtos-init.h"
#include "rig-tp/rig-inc.h"
#include "utility/timers/timer.h"
#include "factory/modem-maker.h"
#include "factory/bin-maker.h"
#include "factory/services-factory.h"
#include "generalsett.h"
#include "nmeautil/nmeautil.hpp"
#include "factory/switch-maker.h"
#include "factory/pipes-maker.h"
#include "gsm/sett/gprs-param.h"
#include "gsm/sms-delegate.h"


#define GPRS_ACTIVE_TIMEOUT   15000
#define NET_REG_SEC_TIMEOUT   180

using namespace M66;
using namespace Timers;

static Timer sessTim;
static Timer igsmTim;
static Timer gregTim;
static Timer failTim;


static IStreamable& binPipe = GetBinPipe();
int32_t JConfSocketUpdate();

SmsDelegate mapSms;
SmsDelegate servSms;

/* -------------- +++++++++++++++++++++++++++++++++++++++ ------------------ */

ISwitchable& en4v = SwitchMaker::GetGsmEn();
ISwitchable& pwk = SwitchMaker::GetPwrKey();
const ISwitchable& dcd = SwitchMaker::GetDCDSignal();

static bool dcd_state = true;

static GeneralSett memconf = GeneralSett();
static const GprsParam* servP;

const char* netConf[3];

GsmModem& m66 = GetGsmModem();
Rig::RigRouter rigRouter;

InfoHandler infoHand(binPipe);
TrekListHandler trackHand(binPipe);
SoleTrekHandler soleHand(binPipe);
FirmHandler firmHand(binPipe);
EchoHandler echoHand(binPipe);

const int32_t kRxMaxLen = 1024;
uint8_t GSM_TX_Buff[1024];
uint8_t GSM_RX_Buff[kRxMaxLen];

uint32_t time_register_wait = 0;
/* ------------------------------------------------------------------------- */
const int32_t kTempBuffLen = 256;
char cTempBuff[kTempBuffLen];
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
static volatile enum
{
  eG_Start,
  eG_Init,
  eG_Idle,
  eG_TryConnect,
  eG_ConnIdle,
  eG_Disconnect,
  eG_HardDie,
  eG_Wait
}
g_GsmMainState;

typedef struct
{
  uint32_t offset;
  uint32_t len;
  void ClrBlk(void) {
    offset = len = 0;
  }
} sMemManager_t;

sMemManager_t prdat;

/* ------------------------------------------------------------------------ */
struct
{
  bool res;
  bool load;
  bool accept;
} firm_inst = {false, false, false};

#define INFO_STRING_MASK "IMEI=%s,VERSION=%d.%d.%d,NAME=%s"

void GsmSwitchOn(void)
{
  en4v.off();
  osDelay(2000);
  en4v.on();
  pwk.off();
  osDelay(1000);
  pwk.on();
  osDelay(9000);
  return;
}


int32_t JConfSocketUpdate()
{
  int32_t socket_id = memconf.sockbit.Socket();
  servP = (socket_id != 0) ? (&(memconf.gprs[socket_id - 1])) : (0);
  netConf[0] = servP->serv.GetParametr(kServer);
  netConf[1] = servP->serv.GetParametr(kPort);
  netConf[2] = servP->serv.GetParametr(kAPN);
  sessTim.Start(1000 * 10);
  return 0;
}


int32_t GprsHandler(const SmsChunkDescriptor& smsdsc, char* ans)
{
  if (smsdsc.desc[0].len < 2 || smsdsc.desc[0].text[0] != 'G')
  {
    return 0;
  }

  int32_t paramId = smsdsc.desc[0].text[2] - 0x30;

  if (paramId > 0 && paramId < 4)
  {
    memconf.gprs[paramId - 1].Parse(smsdsc);
    memconf.gprs[paramId - 1].Print(ans);
    sprintf((ans + strlen(ans)), " S:%d", paramId);
  }
  else if (paramId == 0)
  {
    memconf.SocketBits(smsdsc.desc[1].text);
    int32_t socket = memconf.sockbit.Socket();

    if (socket != 0)
    {
      memconf.gprs[socket - 1].Print(ans);
      sprintf((ans + strlen(ans)), " S:%d", socket);
    }
    else
    {
      sprintf(ans, " GPRS vyklychen.");
    }
  }
  else
  {
    strcpy(ans, "Nevernye dannye. Poprobuite eshe raz");
    return strlen(ans);
  }

  JConfSocketUpdate();
  JConfSave((uint8_t*)&memconf, memconf.Lenght());
  return strlen(ans);
}

/* ------------------------------------------------------------------------- */
void JConfInit()
{
  extern int32_t MapHandler(const SmsChunkDescriptor&, char*);
  int32_t conf_len = memconf.Lenght();
  JConfUpload((uint8_t*)&memconf, conf_len);

  if (!memconf.JCIsSign())
  {
    memconf.JCDefault();
    JConfSave((uint8_t*)&memconf, conf_len);
  }

  servSms.SetProcessor(GprsHandler);
  servSms.SetSuccessor(&mapSms);
  mapSms.SetProcessor(MapHandler);
}


int32_t SmsHandler(char* sms, int32_t len, char* ans)
{
  static SmsChunkDescriptor smsdsc;
  bool is_answer = (sms[0] == '*');
  smsdsc.Parse(sms + 1, len);
  int32_t ret = servSms.HandleIt(smsdsc, ans);

  if (is_answer && ret > 0)
    return ret;

  return 0;
}


/* ------------------------------------------------------------------------- */
void m66DCDHandle()
{
  if (dcd.ison() != dcd_state)
  {
    dcd_state = (dcd.ison());
    DBG_Gsm("[DCD]Changed = %d\n", (int)dcd_state);
  }
}

void IncomeSmsHandle()
{
  int32_t sms_length = 0;
  int32_t sms_answer;
  sms_length = m66.ListSMS(cTempBuff, kTempBuffLen);

  if (sms_length > 1)
  {
    // MainSmsParsing(cTempBuff, sms_length);
    sms_answer = SmsHandler(cTempBuff, sms_length, (char*)GSM_TX_Buff);
  }

  if (sms_answer > 0)
    m66.SendSMS((const char*)GSM_TX_Buff, sms_answer);
}


bool IsReadyForConnect()
{
  bool ret = false;

  if (sessTim.Elapsed())
  {
    sessTim.Start(1000 * 40);

    if (servP == 0)
      ret = false;
    else if (m66.State == kRegOk && servP->serv.ServerValid())
      ret = true;
  }

  return ret;
}

static void OnFirmwareFinished(int32_t firm_size)
{
  if (firm_size > 0)
  {
    firm_inst.load = true;
    firm_inst.accept = true;
    firm_inst.res = true;
    sessTim.Start(1000);
  }
}

void RigStuffInit()
{
  rigRouter.RegisterRigHandler(&infoHand);
  rigRouter.RegisterRigHandler(&trackHand);
  rigRouter.RegisterRigHandler(&soleHand);
  rigRouter.RegisterRigHandler(&firmHand);
  rigRouter.RegisterRigHandler(&echoHand);
  firmHand.SetCallback(OnFirmwareFinished);
  ServiceWorker().Register(&rigRouter);
}

/* ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- *
 *                            TASK GSM START
 * ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- */
void tskGsm(void*)
{
  osDelay(500);
  DBG_Gsm("*<* Gsm task start complete *>*\n\n");
  g_GsmMainState = eG_Start;
  JConfInit();
  RigStuffInit();

  /*------------------------------------------*/
  while (1)     /* deadloop for GSM task */
  {
    m66DCDHandle();
    osPass(); /* main task pass */

    switch (g_GsmMainState)
    {
      /* -------------------------------------------------------- */
      case (eG_Wait):
      {
        g_GsmMainState = eG_Start; // *---> exit sleep
        break;
      }

      /* -++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      case (eG_Start):
      {
        /* start modem init */
        GsmSwitchOn();
        m66.SyncPipe();
        /* all OK state to   */
        g_GsmMainState = eG_Init;
        JConfSocketUpdate();
        break;
      }

      /* -++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      case (eG_Init):
      {
        igsmTim.Start(1000);
        time_register_wait = (time_register_wait < 360) ?
                             (time_register_wait + (NET_REG_SEC_TIMEOUT * 2)) : (360);
        gregTim.Start(1000 * time_register_wait);
        g_GsmMainState = (firm_inst.res) ? eG_HardDie : eG_Idle;
        break;
      }

      /* -++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      case (eG_Idle):
      {
        if (IsReadyForConnect())
        {
          g_GsmMainState = eG_TryConnect;
          break;
        }

        if (igsmTim.Elapsed())
        {
          igsmTim.Start(9000);
          m66.CheckNetwork();

          if (m66.State == kNoecho)
          {
            g_GsmMainState = eG_Start;
            break;
          }

          IncomeSmsHandle();

          if (m66.State == kRegOk)
          {
            gregTim.Start(1000 * NET_REG_SEC_TIMEOUT);
          }
        }

        if (gregTim.Elapsed())
        {
          DBG_Gsm("[gsm] register time out\n");
          g_GsmMainState = eG_Start;
        }

        break;
      }

      /* -++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      case (eG_TryConnect):
      {
        m66.Connect(netConf[0], netConf[1], netConf[2]);

        if (m66.State == kConnected)
        {
          /// connected successful
          sessTim.Start(1000 * servP->tim.Big);
          igsmTim.Start(1000 * 20);
          failTim.Start(1000 * servP->tim.Silent);
          g_GsmMainState = eG_ConnIdle;
          break;
        }

        g_GsmMainState = eG_Disconnect;
        break;
      }

      /* -++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      case (eG_ConnIdle):
      {
        osPass();

        if (dcd_state || sessTim.Elapsed())
        {
          g_GsmMainState = eG_Disconnect;
          break;
        }

        if (igsmTim.Elapsed())
        {
          rigRouter.SetActive(false);
          igsmTim.Start(GPRS_ACTIVE_TIMEOUT);
          int32_t session = m66.IsConnectionAlive();
          rigRouter.SetActive(true);

          if (session != 0 || failTim.Elapsed())
          {
            g_GsmMainState = eG_Disconnect;
            break;
          }
        }

        int32_t ret = binPipe.Read(GSM_RX_Buff, 0, kRxMaxLen);

        if (ret >= 6)
        {
          if (igsmTim.Ticks() < 5000)
            igsmTim.Start(1000 * 5);

          failTim.Start(1000 * servP->tim.Silent);
          rigRouter.PassRigFrame((const RigFrame*)GSM_RX_Buff, ret);
        }

        break;
      }

      /* -+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++- */
      case (eG_Disconnect):
      {
        m66.CloseConnection();
        sessTim.Start(1000 * servP->tim.ReConnect);
        g_GsmMainState = (eG_Init);
        break;
      }

      /* -+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++- */
      case (eG_HardDie):
      {
        if (firm_inst.load && firm_inst.accept)
        {
          DBG_Common("\n=[SYS]HELLO ARMAGEDON!!!\n");
          osDelay(1);
        }

        HARDRESRET;
      }

      default:
        break;
    } // switch
  }//while (1)  dead loop
} // task


void Start_GsmThread()
{
  xTaskCreate(tskGsm, "tskGsm", kLowStackSz, (void*)NULL, kPrio_IDLE, NULL);
}


uint8_t GetIntegerModemState()
{
  return static_cast<uint8_t>(m66.State);
}

const char* BuildInfoString()
{
  static char arr[64] = { 0 };
  static int32_t len = 0;

  if (arr[0] == '\0' || len == 0)
  {
    sprintf(arr, INFO_STRING_MASK, m66.Imei, MAJOR_VERSION,
            MINOR_VERSION, PATCH_VERSION, memconf.ID);
    len = strlen(arr);
  }

  return arr;
}
