#include "thread-gsm.h"
#include "threads/thread-gps.h"
#include "configs/free-rtos-def.h"
#include "configs/dbgconf.h"
#include "Configs/MainConfig.h"
#include "Configs/Version.h"
#include "rtos-init.h"
#include "threads/thread-gps.h"
// #include "oldcommon/gsm/at-commands.h"
#include "oldcommon/magic/magic.h"
//#include "equip/gsm/gsmprot.h"
#include "oldcommon/typesdef.h"
#include "utility/abstract/io/ISectorWriter.h"
#include "utility/timers/timer.h"
#include "factory/modem-maker.h"
#include "factory/bin-maker.h"

using namespace M66;

#include "generalsett.h"

#include "nmeautil/nmeautil.hpp"

#include "BoardSide.h"
#include "factory/McuFlashFactory.hpp"

#include "factory/switch-maker.h"
#include "factory/pipes-maker.h"

int32_t PrintYandexLink(char* inb, uint16_t inblen);
int32_t PrintGoogleLink(char* inb, uint16_t inblen);
int32_t PrintTextLink(char* inb, uint16_t inblen);

using namespace MAGIC;
//using namespace ATFORSIM900;
using namespace Timers;

static Timer tftpTim;
static Timer sessTim;
static Timer igsmTim;
static Timer gregTim;
static Timer failTim;


static IStreamable& binPipe = GetBinPipe();

/* ------------------------------------------------------------------------- *
 *    Prototypes of static functions
 * ------------------------------------------------------------------------- */
//uint8_t GSM_Send_SMS(uint8_t* NumberPtr, uint8_t Len, uint8_t Options);
int32_t GsmGetAck (uint16_t rx_to, const char* cmp_s, const char* alt_s);
int32_t JConfSocketUpdate();
uint8_t m66_CheckFailAnswer(char* buf, int32_t len);
/* -------------- +++++++++++++++++++++++++++++++++++++++ ------------------ */
// void osDelayWrapper(uint16_t tim);

/* ------------------------------------------------------------------------- *
 * 	       -----------------Defines sector -----------------
 * ------------------------------------------------------------------------- */
#define GSM_SMS_REPLY_NEED				0x01
#define GSM_SMS_LONG_VALID				0x02
/* ------------------------------------------------------------------------- */
#define GSM_SMS_FORMAT_MASK			  0x30
#define GSM_SMS_FORMAT_7BIT			  0x00
#define GSM_SMS_FORMAT_16BIT			0x10
#define GSM_SMS_FORMAT_HEX				0x20
#define GSM_SMS_FORMAT_BINARY		  0x30

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
#define NET_RESTART_TO		        60000
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

ISwitchable& en4v = SwitchMaker::GetGsmEn();
ISwitchable& pwk = SwitchMaker::GetPwrKey();
const ISwitchable& dcd = SwitchMaker::GetDCDSignal();

static bool dcd_state = true;

static ITrekList* const trList = FileLink();


static GeneralSett memconf = GeneralSett();
static const TcpInstance_c defgprs = TcpInstance_c();
static TcpInstance_c* currgprs = 0;


static char serv_ip[20];
static char serv_prt[10];
char* ssrv[2] = {serv_ip, serv_prt};

ISectorWriter* firmsaver = FlashFactory::GetSectorWriter();

GsmModem& m66 = GetGsmModem();

/* ------------------------------------------------------------------------- */
typedef struct
{
  uint8_t y;
  uint8_t m;
  uint8_t d;
  uint8_t hh;
  uint8_t mm;
  uint8_t ss;
  uint8_t valid;
} DATETIMEFORMAT;
DATETIMEFORMAT dateTime;



/* ------------------------------------------------------------------------- */
const uint8_t daytab [2][13] =
{
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

const int32_t kRxMaxLen = 1024;

uint8_t GSM_TX_Buff[1024];
uint8_t GSM_RX_Buff[kRxMaxLen];

BoardSideInst bsin;

uint8_t smsoptions;

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
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
uint8_t Inc_Phone_In[MAX_PHONE_LEN + 1];

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

/* ------------------------------------------------------------------------- */
enum
{
  /* red */
  eLedIdle = 0,
  eLedNoSim = 1,
  eledNoEcho = 2,
  /* grn */
  eLedReg = 3,
  eLedNoReg = 4,
  eLedConn = 5,
  eledOff = 6,

  eLedSize
} eLedState = eledOff;
/* ------------------------------------------------------------------------- */
static uint32_t led_templates[eLedSize] =
{
  0xFFFFFFFF,
  /* no sim   : -----------+-+ */
  0x00000088,
  /* no echo  : ------++-++-++*/
  0x00000888,
  /* reg      : ---------+++++ */
  0x00000FFF,
  /* no reg   : -------------+ */
  0x00000001,
  /* connect  : -+-+-+-+-+-+-+ */
  0xCCCCCCCC,
  /* off */
  0
};
/* ------------------------------------------------------------------------- */
//62.141.87.178,20011
//178.140.6.233,20201
const char* gpr_debug_str = "GPRS 000,,,,,,178.140.204.50,20201,,,240,2,3,24";
//const char * gpr_debug_str = "GPRS 000,,,,,,62.141.87.178,20012,,,240,2,3,24";

#define INFO_STRING_MASK "IMEI=%s,VERSION=%d.%d.%d,NAME=%s"

char testinfostring[64];
static char imei_str[32] = "111110002220000";

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
void GSM_Switch_Off(void)
{
  return;
}

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
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




/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
uint8_t m66_CheckFailAnswer(char* buf, int32_t len)
{
  if (memcmp(buf, "NO DIALTONE", len) == 0 ||
      memcmp(buf, "ERROR", len) == 0 ||
      memcmp(buf, "NO CARRIER", len) == 0 ||
      memcmp(buf, "+CMS ERROR", 10) == 0)
    return (1);

  return (0);
}

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
//uint8_t m66B

/* ------------------------------------------------------------------------- *
 * led indicate task
 * call from special <tskledBlinkThread> 100 ms task in main.c
 * ------------------------------------------------------------------------- */
uint32_t LedBlinkGsm(void)
{
  uint32_t mmask;
  led_templates[eLedState] = __ror(led_templates[eLedState], 1);
  mmask = ((led_templates[eLedState]) & 1) ? (1) : (0);
  return mmask;
}
/* ------------------------------------------------------------------------ *
 * function for getting day index of year
 * ------------------------------------------------------------------------ */
uint32_t getYearDayIndx (const DATETIMEFORMAT dayIndxDate)
{
//   uint8_t leapyear;
  uint8_t indx;
  uint32_t res = 0;

  for (indx = 1; indx < dayIndxDate.m; res += daytab[1][indx++]);

  return res += dayIndxDate.d;
}


/* ------------------------------------------------------------------------- *
 *  function for getting day of week
 * ------------------------------------------------------------------------- */
uint32_t getDayOfWeek(const DATETIMEFORMAT lcl_DateTime)
{
  int32_t a = (14 - (lcl_DateTime.m)) / 12;
  int32_t y = (lcl_DateTime.y) - a;
  int32_t m = ((lcl_DateTime.m) + 12 * a - 2);
  return ((7000 - 1 +
           (lcl_DateTime.d + y + (y / 4) - (y / 100) + (y / 400) + (31 * m) / 12)) % 7);
}

/* ------------------------------------------------------------------------- *
 * Func for convert dateTime to -12 hours
 * for getting day (12:00) date tick
 * ------------------------------------------------------------------------- */

DATETIMEFORMAT MagicDateTimeConverter(DATETIMEFORMAT lDatTim)
{
  uint8_t leapyear;

  if (lDatTim.hh < 12)
  {
    /* prev day */
    if (lDatTim.d == 1)
    {
      if (lDatTim.m == 1)
      {
        /* roll back  month and year*/
        lDatTim.m = 13;
        lDatTim.y--;
      }

      leapyear = ((lDatTim.y + 2000) % 4 == 0 &&
                  (lDatTim.y + 2000) % 100 != 0 ||
                  (lDatTim.y + 2000) % 400 == 0);
      lDatTim.m--;
      lDatTim.d = daytab[leapyear][lDatTim.m];
    }
    else
      (lDatTim.d)--;
  }

  return lDatTim;
}

/* ------------------------------------------------------------------------- *
 * get actual time for another EVENT
 * format this param - (h*60 + m)
 * ------------------------------------------------------------------------- */
uint32_t GetActualTime (uint8_t* shedBuff, uint32_t onT, uint32_t offT)
{
  int32_t   offsetTable;
  uint32_t  resTime;

  if ((shedBuff[2] & 0xF0) == 0xF0)
  {
    /* time from table */
    if ((offsetTable = (int8_t)(shedBuff[3])) < 0)
      /* negative offset */
      offsetTable = (1440 + offsetTable);

    if (shedBuff[2] & 1)
    {
      /* ontable */
      resTime = ((onT + offsetTable) % 1440);
    }
    else
    {
      /*off table */
      resTime = ((offT + offsetTable) % 1440);
    }
  }
  else
  {
    /* actual time from shed event */
    resTime = (shedBuff[2] * 60) + (shedBuff[3]);
  }

  return resTime;
}

/* ------------------------------------------------------------------------- *
 * function for getting actual state of light system mode
 * call in <GsmThreadTickTask> every one minute
 * ------------------------------------------------------------------------- */
//void GsmThreadTickTask(void)
//{
//	if (dateTimeTick() > 0)
//	{
//		/* minute change */
//		DBG_Common("[clock]<%d><%d><%d>-<%d><%d><%d>\n",
//		           dateTime.hh, dateTime.mm, dateTime.ss,
//		           dateTime.d, dateTime.m, dateTime.y);
////		SheduleLightModeState(dateTime);
//	}
//}



/* ------------------------------------------------------------------------- *

 * ------------------------------------------------------------------------- */
int32_t GTftpSend(uint16_t len)
{
  int32_t blksize = 0;

  if (bsin.SendActionDenied())
    /* fid == 0 or fid are DATA_OUT flag */
    return 0;

  if (bsin.bchief.IsBlockSendPermitted())
  {
    bsin.headDataUp();
    tftpTim.Start(15000);
  }
  else
  {
    if (tftpTim.Elapsed())
    {
      if (!bsin.bchief.RollBackBidsend())
      {
        DBG_2Gsm("ftp-> Resend count fail. Stop sending\n");
        bsin.ResetAll();
      }
    }

    return 0;
  }

  while ((binPipe.CanWrite() < 512))
    osPass();

  switch (bsin.getFid())
  {
    case (ID_GET_FILENOTES):
      blksize = trList->UploadList((bsin.bchief.bidsend - 1) * 25, 25,
                                   bsin.from->msg);
      break;

    case (ID_GET_TRACK):
      blksize = trList->UploadTrek(bsin.NeedTreck(), (bsin.bchief.bidsend - 1) * 25,
                                   (25 * NaviNote::Lenght()),
                                   bsin.from->msg);
//      blksize = FileTrackUpload(bsin.from->msg,
//                                (bsin.bchief.bidsend - 1) * mxmap::count_block,
//                                mxmap::occur_block, bsin.NeedTreck());
//      blksize *= navinote::Lenght();
      break;

    case (ID_GET_INFO):
//			sprintf((char*)bsin.from->msg,INFO_STRING_MASK,imei_str, memconf.ID);
      /* ??? need move filling info string to other function */
      strcpy((char*)bsin.from->msg, testinfostring);
      blksize = strlen((char*)bsin.from->msg) + 1;

      if (bsin.bchief.bidsend != 1)
        blksize = 0;

      break;

    case (ID_GET_ECHO):
    default:
      bsin.ResetAll();
      break;
  } // switch

  if (blksize < 0)
  {
    /* error */
    bsin.ResetAll();
    bsin.headError((uint16_t)blksize);
    blksize = 0;
  }

  DBG_2Gsm("ftp-> OPC:%04x. ID:%04x. Size:%d\n", bsin.from->opc,
           bsin.from->blockid, blksize);
  bsin.bchief.FixLastBid(blksize);
  binPipe.Write((const uint8_t*)(bsin.from), 0, blksize + 4);
  return 0;
}


/* ------------------------------------------------------------------------- *

 * ------------------------------------------------------------------------- */
int32_t GTftpACKParse(uint16_t len)
{
  tftpTim.Start(15000);
  bsin.Validation();

  if (bsin.bchief.Last())
    bsin.ResetAll();

  return 0;
}

/* ------------------------------------------------------------------------- *

 * ------------------------------------------------------------------------- */
int32_t GTftpDATAParse(int32_t len)
{
  /* not correct block ID */
  if (!bsin.Validation())
    return 0;

  /* ??? need check timer for correct exit from Fid() state */
  len -= 4;
  bsin.headAckUp();
  bsin.FInsize.AddPassing((int32_t)len);

  switch (bsin.getFid())
  {
    case (ID_FIRMWARE):
      firmsaver->Program(bsin.come->msg, len);

      if (len == 0)
      {
        firm_inst.load = true;
        firm_inst.accept = true;
        firm_inst.res = true;
        sessTim.Start(1000);
      }

      break;

    default:
      break;
  }

  if (len == 0)
    bsin.ResetAll();

  binPipe.Write((uint8_t*)(bsin.from), 0, 4);
  return 0;
}
/* ------------------------------------------------------------------------- *

 * ------------------------------------------------------------------------- */
int32_t GTftpRRQParse(uint16_t len)
{
  int32_t fsize = -1, ret0;
  bsin.headUp();
  bsin.Options(0x0401);
  bsin.setFid();

  switch (bsin.getFid())
  {
    case ID_GET_FILENOTES:
      fsize = trList->RefreshTrekList();
      break;

    case ID_GET_TRACK:
      ret0 = trList->GetListIndex(bsin.Options());

      if (ret0 >= 0)
        fsize = trList->GetTrekSize(bsin.NeedTreck(ret0));
      else
      {
        fsize = 0;
        bsin.headError(6);
        bsin.ResetAll();
      }

      break;

    case ID_GET_INFO:
      sprintf(testinfostring, INFO_STRING_MASK,
              imei_str, MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, memconf.ID);
      fsize = strlen(testinfostring);
      break;

    case ID_GET_ECHO:
      fsize = 0;
      bsin.ResetAll();
      break;

    default:
      bsin.headError(5);
      fsize = 0;
      break;
  }

  DBG_2Gsm("RRQ> Size:%d\n", fsize);
  bsin.FSize((uint32_t)(fsize));
  binPipe.Write((uint8_t*)(bsin.from), 0, 6 + 4);
  bsin.bchief.Anew();
  return 0;
}

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
int32_t GTftpWRQParse(uint16_t len)
{
  len -= 4;

  if (len < 6) return -1;

  bsin.ResetAll();

  if (bsin.FSize() != 0)
  {
    /* correct file size waiting */
    bsin.setFid();
    firmsaver->Erase();
  }

  bsin.headUp();
  binPipe.Write((uint8_t*)(bsin.from), 0, 4);
  DBG_2Gsm("WRQ> Size:%d\n", bsin.FSize());
  return 0;
}

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */


void GTftp2Process(uint8_t* msg, int32_t len)
{
  bsin.come = (GTftp2_t*)msg;
  bsin.from = (GTftp2_t*)GSM_TX_Buff;

  if (len > 3)
  {
    DBG_2Gsm("ftp <- opc=%d, id=%04x. len=%d\n", bsin.come->opc, bsin.come->blockid,
             len);

    if (bsin.come->opc == OPC_RRQ)
      GTftpRRQParse(len);
    else if (bsin.come->opc == OPC_ACK)
      GTftpACKParse(len);
    else if (bsin.come->opc == OPC_WRQ)
      GTftpWRQParse(len);
    else if (bsin.come->opc == OPC_DATA)
      GTftpDATAParse((int32_t)len);
    else { }
  }

  GTftpSend(0);
}

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void JConfInit()
{
  JConfUpload((uint8_t*)&memconf, memconf.Lenght());

  if (!memconf.JCIsSign())
  {
    memconf.JCDefault();
    JConfSave((uint8_t*)&memconf, memconf.Lenght());
  }
}
/* ------------------------------------------------------------------------- */
uint32_t JConfNetInit(char* s, char* out)
{
  /* ??? why need ret */
  volatile int32_t ret;
  uint32_t preamb = __REV(*(uint32_t*)s);
  uint32_t typepreamb;
  /* valid value */
  typepreamb = ((preamb & 0x0000FF00) >> 8) - 0x30;
  preamb = (preamb & 0x000000FF) - 0x30;

  if (preamb > 5)
  {
    /* error statement */
    strcpy(out, "oshibka: nevernye parametry\n");
    return (strlen(out));
  }

  if (preamb != 0)
  {
    if (typepreamb == 0)
      ret = memconf.IP[preamb - 1].Update(s + 4);
    else if (typepreamb == 1)
      ret = memconf.IP[preamb - 1].UpdateTim(s + 4);
  }
  else
    preamb = memconf.SocketBits(s + 4);

  sprintf(out, "%d: ", preamb);
  memconf.IP[preamb - 1].Print(out + strlen(out));
  JConfSocketUpdate();
  JConfSave((uint8_t*)&memconf, memconf.Lenght());
  return strlen(out);
}
/* ------------------------------------------------------------------------- */
int32_t JConfSocketUpdate()
{
  currgprs = (memconf.sockbit.Socket() != 0) ?
             (&(memconf.IP[memconf.sockbit.Socket() - 1])) : ((TcpInstance_c*)&defgprs);
  currgprs->PrintIP(ssrv[0]);
  currgprs->PrintPort(ssrv[1]);
  sessTim.Start(1000 * 10 * 1);
  return 0;
}
/* ------------------------------------------------------------------------- */
// void osDelayWrapper(uint16_t tim)
// {
//   osDelay(tim);
// }

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
uint8_t MainSmsParsing(char* inSms)
{
  uint32_t preamb;
  DBG_Gsm("%s\n", inSms);
  DBG_Gsm("%x\n", *(uint32_t*)(inSms));
  preamb = swapp::Word(*(uint32_t*)(inSms));
  bool is_answer = true;

  switch (*(uint32_t*)(inSms))
  {
    case (0x31303023):
      prdat.len = PrintYandexLink((char*)GSM_TX_Buff, 256);
      break;

    case (0x32303023):
      prdat.len = PrintTextLink((char*)GSM_TX_Buff, 256);
      break;

    case (0x33303023):
      prdat.len = PrintGoogleLink((char*)GSM_TX_Buff, 256);
      break;

    default:
      is_answer = false;
      break;
  }

  if ((preamb & 0xFF000000) == 0x2A000000)
  {
    // '*' - first symbol
    preamb &= ~0xFF000000;
    preamb |= 0x23000000;
    is_answer = true;
  }

  switch (preamb & 0xFFFF0000)
  {
    case (0x23310000):
      /* GPRS params */
      prdat.len = JConfNetInit(inSms, (char*)GSM_TX_Buff);
      break;
  } // swirch;

//  if (is_answer)
//    GSM_Send_SMS(Inc_Phone_In, prdat.len, GSM_SMS_FORMAT_7BIT);
  DBG_Gsm("%s\n", GSM_TX_Buff);
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
//  int32_t sms_message_length = 0;
  sms_length = m66.ListSMS(cTempBuff, kTempBuffLen);
//  sms_message_length = FromPduToAscii((char*)GSM_RX_Buff, cTempBuf, sms_length);
//  if (sms_message_length > 1)
//    MainSmsParsing((char*)GSM_RX_Buff, sms_message_length);
}

/* ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- *
 *                            TASK GSM START
 * ------------------------------------------------------------------------- *
 * ------------------------------------------------------------------------- */
void tskGsm(void*)
{
  memset((void*) & (dateTime), 0, sizeof(dateTime));
  osDelay(500);
  DBG_Gsm("*<* Gsm task start complete *>*\n\n");
  g_GsmMainState = eG_Start;
  JConfInit();

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
        eLedState = eLedIdle;
        /* start modem init */
        GsmSwitchOn();
        m66.SyncPipe();
//        if (m66.atState == kError || kAlter)
//        {
//
//        }
//
//
//        if (!(m66Syncro(15, 400)))
//        {
//          g_GsmMainState = eG_Start;
//          break;
//        }
//        m66EchoOff();
//        m66FixIpr();
//        m66CommonConf();
//        eLedState = eLedNoSim;
        // if (!m66SimCheck(3600 * 1000))
        // {
        //   g_GsmMainState = eG_Start;
        //   break;
        // }
        /* all OK state to   */
        g_GsmMainState = eG_Init;
        JConfSocketUpdate();
        break;
      }

      /* -++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      case (eG_Init):
      {
//				LoadSecTim(gsm_restart_to, NET_RESTART_TO);
        igsmTim.Start(1000);
        eLedState = eLedNoReg;
        time_register_wait = (time_register_wait < 360) ?
                             (time_register_wait + (NET_REG_SEC_TIMEOUT * 2)) : (360);
        gregTim.Start(1000 * time_register_wait);
        g_GsmMainState = (firm_inst.res) ? eG_HardDie : eG_Idle;
        break;
      }

      /* -++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      case (eG_Idle):
      {
        if (!sessTim.Elapsed() && m66.State == kRegOk)
        {
          g_GsmMainState = eG_TryConnect;
        }

        // if (!igsmTim.Elapsed())
        //   break;

        /* ----------------- BIG block SMS and Net state ------------------- */
        if (igsmTim.Elapsed())
        {
          igsmTim.Start(9000);
          m66.CheckNetwork();

          if (m66.State == kNoecho)
          {
            eLedState = eledNoEcho;
            g_GsmMainState = eG_Start;
            break;
          }

          eLedState = eLedNoReg;

          if (m66.State == kRegOk)
          {
            eLedState = eLedReg;
            gregTim.Start(1000 * NET_REG_SEC_TIMEOUT);
//            if (GsmReceiveSMS(Inc_Phone_In) > 1)
//              MainSmsParsing((char*)GSM_RX_Buff);
          }

          /* end check net state block check incoming SMS */

          if (true)
          {
//            strcpy((char*)GSM_RX_Buff, "#001");
//            MainSmsParsing((char*)GSM_RX_Buff);
//            strcpy((char*)GSM_RX_Buff, "#002");
//            MainSmsParsing((char*)GSM_RX_Buff);
//            strcpy((char*)GSM_RX_Buff, "#003");
//            MainSmsParsing((char*)GSM_RX_Buff);
//            strcpy((char*)GSM_RX_Buff, "#105#64.48.155.8825#20010");
//            MainSmsParsing((char*)GSM_RX_Buff);
//            strcpy((char*)GSM_RX_Buff, "#111#012#5#3");
//            MainSmsParsing((char*)GSM_RX_Buff);
//
//						strcpy((char*)GSM_RX_Buff, "?100");
//            MainSmsParsing((char*)GSM_RX_Buff);
//												strcpy((char*)GSM_RX_Buff, "?102");
//            MainSmsParsing((char*)GSM_RX_Buff);
//												strcpy((char*)GSM_RX_Buff, "?106");
//            MainSmsParsing((char*)GSM_RX_Buff);
//												strcpy((char*)GSM_RX_Buff, "?101");
//            MainSmsParsing((char*)GSM_RX_Buff);
          }
        }

//				/* ------------- BIG block SMS and Net state END --------------------- */
//				if ((!GetTim(gsm_register_to)) || GetTim(gsm_restart_to) == 0)
//				{
//					DBG_Gsm("RegSecTimer: %d | NetRestertSecTimer: %d\n",
//					        GetTim(gsm_register_to), GetTim(gsm_restart_to));
//					g_GsmMainState = eG_Start;
//				}

        /* ------------- BIG block SMS and Net state END --------------------- */
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
//        if (currgprs->TcpValid() == false)
//        {
//          g_GsmMainState = eG_Idle;
//          break;
//        }

        m66.Connect("178.140.8.184", "20201");

        if (m66.State == kConnected)
        {
          /// connected successful
          sessTim.Start(1000 * 10 * currgprs->bigto);
          igsmTim.Start(1000 * 20); /* break time out */
          failTim.Start(1000 * 10 * currgprs->silentto); /* global stop timeout */
          bsin.ResetAll();
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

        if (dcd_state)
        {
          g_GsmMainState = eG_Disconnect;
          break;
        }

        if (igsmTim.Elapsed())
        {
          igsmTim.Start(GPRS_ACTIVE_TIMEOUT);
          int32_t session = m66.IsConnectionAlive();

          if (session != 0 || failTim.Elapsed())
          {
            g_GsmMainState = eG_Disconnect;
            break;
          }
        }

        int32_t ret = binPipe.Read(GSM_RX_Buff, 0, kRxMaxLen);

        if (ret > 0)
        {
          if (igsmTim.Ticks() < 5000)
            igsmTim.Start(1000 * 5);

          failTim.Start(1000 * 10 * currgprs->silentto);
        }

        GTftp2Process(GSM_RX_Buff, ret);
        break;
      }

      /* -+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++- */
      case (eG_Disconnect):
      {
        m66.CloseConnection();
        sessTim.Start(1000 * 10 * currgprs->reconnto);
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
          __disable_irq();
          SYSTICKDISABLE;
          KillFlash();
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
















