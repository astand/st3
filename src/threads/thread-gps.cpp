/* ------------------------------------------------------------------------- */
#include <string.h>
#include "configs/dbgconf.h"
#include "thread-gps.h"
#include "nmeautil/nmeautil.hpp"
#include "trekrep/trek-location-descriptor.h"
#include "trekrep/cached-trek.h"
#include "equip/flash/mx-25-driver.h"
#include "utility/timers/timer.h"
#include "utility/timers/d-timer.h"
#include "oldcommon/magic/magic.h"
#include "trekrep/treksaver.h"
#include "trekrep/treklist.h"
#include "rtos-init.h"
#include "configs/free-rtos-def.h"
#include "factory/switch-maker.h"
#include "factory/pipes-maker.h"
#include "gsm/sms-chunks.h"

using namespace Timers;
using namespace MAGIC;
static const int32_t CACHE_STOP_TIMEOUT = 10 * 1000;

Navi scoor;
GpsPositionData_t gpsdata =
{
  &scoor,
  { 0 },
  kNoUpdate,
  kNoError,
  kNoSensor
};
// NaviNote trekNote;
IFlashStorable& storechunk = (IFlashStorable&)scoor;
ISwitchable& gps_en = SwitchMaker::GetGpsEn();
CachedTrek cachedTrek;
char gpssens[80];
static uint8_t spbuf[64];
static uint16_t* const track_num = (uint16_t*)(spbuf + 2);

// time out for stop point update action
static DTimers::Timer cacheStopTim;
static Timer dbgTim(1000, true);
static Timer waitmovTim(1000, false);
static const uint32_t DEDUG_TO = (120 * 1000);
static char dbgbuf[256];

typedef enum
{
  kPositionNotValid = 0,
  kWaitMove = 1,
  kMoving = 2,
  kMovePaused = 3,
  kTestWriting = 4,
  kNoGpsSensor = 5
} TrackState_e;

struct
{
  TrackState_e st0;
  uint8_t Get() {
    return (uint8_t)st0;
  }
} trackinst = {kPositionNotValid};


IFlashMemory& fiend = PipesMaker::GetFlashMemory();
TrekSaver treksaver(fiend);
TrekList treklist(fiend);

static GpsPositioner& gpspos = PipesMaker::GetGpsPositioner();

/* ------------------------------------------------------------------------- */
void ANaviPrint(char* bfu, const  Navi& inst)
{
  sprintf (bfu, "{ lat:%d.%06d,lon:%d.%06d,", inst.lafull / 1000000,
           inst.lafull % 1000000, inst.lofull / 1000000, inst.lofull % 1000000);
  sprintf(bfu + strlen(bfu),
          "titl:\"%04d-%02d-%02dT%02d:%02d:%02d\",spd:%d, dist:%d, kurs:%d },",
          inst.clnd.year + 2000, inst.clnd.month, inst.clnd.day, inst.clnd.hr,
          inst.clnd.min, inst.clnd.sec, inst.spd / 100, inst.accum_dist,
          inst.liveKurs / 100);
}

/* ------------------------------------------------------------------------- */
bool IsNewTrack(uint8_t const* buf)
{
  return (*track_num == 0);
}

/* ------------------------------------------------------------------------- */
void DummyWait(uint32_t ii)
{
  volatile uint32_t inner = ii;

  while (inner)
  {
    __NOP();
    --inner;
  }
}

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
void TrackProcess()
{
  switch (trackinst.st0)
  {
    case (kPositionNotValid):
      if (scoor.Valid())
      {
        DBG_2Gps("[GPS2]Valid RMC detect --> goto waitmove\n");
        trackinst.st0 = kWaitMove;
      }

      break;

    case (kWaitMove):
      if (scoor.mvdetector.IsMovement())
      {
        DBG_2Gps("[GPS2]move is detected --> goto MOVE\n");
        treksaver.StartNewNote();
        trackinst.st0 = kMoving;
      }

      break;

    case (kMoving):
      if (scoor.CoerseChanged() || dbgTim.Ticks() <= scoor.MathTo(DEDUG_TO))
      {
        scoor.FreezeFixSpd();
        scoor.FreezeDistance();
        dbgTim.Start(DEDUG_TO);
        ANaviPrint(dbgbuf, scoor);
        DBG_2Gps("%s\n", dbgbuf);
        treksaver.SaveNote(storechunk);
        cacheStopTim.Start(CACHE_STOP_TIMEOUT);
        cachedTrek.Add(scoor);
      }

      if (!scoor.mvdetector.IsMovement())
      {
        DBG_2Gps("[GPS2]suspend moving --> goto movesuspend\n");
        waitmovTim.Start(15 * 60 * 1000);
        trackinst.st0 = kMovePaused;
        dbgTim.Start(10000);
      }

      break;

    case (kMovePaused):
      if (scoor.mvdetector.IsMovement())
      {
        DBG_2Gps("[GPS2]move is restored --> goto MOVE\n");
        trackinst.st0 = kMovePaused;
      }

      if (waitmovTim.Elapsed())
      {
        /* new ID */
        DBG_2Gps("[GPS2]time is run out --> Start full\n");
        trackinst.st0 = kPositionNotValid;
      }

      break;

    case (kTestWriting):
      if (dbgTim.Elapsed())
      {
        scoor.spd = (scoor.spd + 1) & 0x7f;
        scoor.lofull += 100;
        scoor.lafull += 20;
        scoor.accum_dist += 1000;
        ANaviPrint(dbgbuf, scoor);
        treksaver.SaveNote(storechunk);
        ANaviPrint(dbgbuf, scoor);
        DBG_2Gps(dbgbuf);
      }

      break;

    default:
      break;
  }
}


/* Jconf read from memory -------------------------------------------------- */
void JConfUpload(uint8_t* mem, uint32_t len)
{
  fiend.Read(Mx25drv::MX_JCONF_A, mem, len);
}



/* Jconf save to memory ---------------------------------------------------- */
void JConfSave(uint8_t* mem, uint32_t len)
{
  fiend.Erase(Mx25drv::MX_JCONF_A);
  fiend.Write(Mx25drv::MX_JCONF_A, mem, len );
}



/*
 * MEMory read to static @spbuf from aligned address (64)
 * and return res through Navi pointer
 */
Navi* MEM_OutNavi(uint32_t addr)
{
  fiend.Read((addr + 4), spbuf, NaviNote::Lenght());
  return ((Navi*)(spbuf));
}

/* ------------------------------------------------------------------------- *
 *   Tasks implementation
 * ------------------------------------------------------------------------- */
void tskGps(void*)
{
  // uint32_t addr;
  // track_saver.Init();
  // gps_en.on();
  // while (1)
  //   os_tsk_pass();
  // for (uint32_t addr = 0; addr < 500; addr ++)
  // {
  //   os_tsk_pass();
  //   fiend.EraseSec(addr * 4096);
  // }
  // track_saver.id = 0xa4;
  // fiend.Write(track_saver.address, (uint8_t*) & (track_saver), 4);
  // fiend.Read(track_saver.address, spbuf, mxmap::step);
  // DBG_Gps("start read\n");
  // for (int i = 0; i < 4096 * 500; i += mxmap::step)
  // {
  //   fiend.Read(i, spbuf, mxmap::step);
  // }
  // DBG_Gps("stop read\n");
  // PrintAllIDs();
  // start cache timer in continious mode
  cacheStopTim.Start(CACHE_STOP_TIMEOUT, true);
  dbgTim.Start(1000);
  // MEM_SaveAddress();
  treksaver.Init();
  treklist.RefreshTrekList();
  treklist.ReadLastNote(storechunk);
  scoor.InitFromRestored();
  // FileListNotification();
  // DBG_Common("end %d\n", track_saver.address);

  /* test for flash writing */
  if (false)
  {
    dbgTim.Start(1000);
    treksaver.SetNextTrek();
    scoor.clnd.min += 1;

    if (scoor.clnd.min == 60)
    {
      scoor.clnd.min = 0;
      scoor.clnd.hr += 1;
    }

    trackinst.st0 = kTestWriting;
  }

  cachedTrek.Add(scoor);
  trackinst.st0 = kNoGpsSensor;
  osDelay(1000);

  while (1)
  {
    // if (GetTim(agps_to) == 0)
    if (cacheStopTim.Elapsed() && trackinst.st0 != kMovePaused)
      cachedTrek.Add(scoor);

    gpspos.UpdatePosition(&gpsdata);

    if (gpsdata.update != kNoUpdate)
    {
      scoor.HandleGpsData(&gpsdata);

      if (trackinst.st0 == kNoGpsSensor)
      {
        // change NoSensor status only one time
        trackinst.st0 = kPositionNotValid;
      }
    }

    /* ??? need new timing base algoritm */
    // OutProcess();
    TrackProcess();
    osPass();
  }
} // task close
/* */


ITrekList* FileLink()
{
  return (ITrekList*)&treklist;
}


void Start_GpsThread()
{
  xTaskCreate(tskGps, "tskGps", kLowStackSz, (void*)NULL, kPrio_IDLE, NULL);
}


uint8_t GetIntegerGPSState()
{
  return trackinst.Get();
}

/* ---------------------------------------------------------------------------- */
int32_t PrintYandexLink(char* inb)
{
//  if (scoor.Valid())
  if (true)
  {
    sprintf(inb,
            "http://static-maps.yandex.ru/1.x/?ll=%d.%06d,%d.%06d&z=10&pt=%d.%06d,%d.%06d,flag&l=map",
            scoor.lofull / 1000000,
            scoor.lofull % 1000000,
            scoor.lafull / 1000000,
            scoor.lafull % 1000000,
            scoor.lofull / 1000000,
            scoor.lofull % 1000000,
            scoor.lafull / 1000000,
            scoor.lafull % 1000000);
  }
  else
    strcpy(inb, "no fix point\n");

  return strlen(inb);
}
/* */
int32_t PrintGoogleLink(char* inb)
{
  sprintf(inb,
          "https://www.google.ru/maps/place//@%d.%06d,%d.%06d,16z/data=!4m2!3m1!1s0x0:0x0?hl=ru",
          scoor.lafull / 1000000,
          scoor.lafull % 1000000,
          scoor.lofull / 1000000,
          scoor.lofull % 1000000);
  return strlen(inb);
}

/*   */
int32_t PrintTextLink(char* inb)
{
  if (true)
  {
    sprintf(inb,
            "fix:%d.%06d,%d.%06d;dist:%d,time: %02d/%02d/%02d %02d:%02d\n",
            scoor.lafull / 1000000,
            scoor.lafull % 1000000,
            scoor.lofull / 1000000,
            scoor.lofull % 1000000,
            scoor.accum_dist / 10000,
            scoor.clnd.day,
            scoor.clnd.month,
            scoor.clnd.year,
            scoor.clnd.hr,
            scoor.clnd.min);
  }

  return strlen(inb);
}


int32_t MapHandler(const SmsChunkDescriptor& smsdsc, char* ans)
{
  if ((smsdsc.desc[0].len < 2) && (smsdsc.desc[0].text[0] != '0'))
  {
    // not handling
    return 0;
  }

  int32_t map_selector = smsdsc.desc[0].text[2] - '0';
  int32_t ret;

  switch (map_selector)
  {
    case (1):
      ret = PrintYandexLink(ans);
      break;

    case (2):
      ret = PrintGoogleLink(ans);
      break;

    case (3):
      ret = PrintTextLink(ans);
      break;

    default:
      sprintf(ans, "Nevernyi zapros pozicii");
      ret = strlen(ans);
      break;
  }

  return ret;
}
