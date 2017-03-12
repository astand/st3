#include "navinote.hpp"
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
int32_t NaviNote::SetL(const char* b, uint8_t l)
{
  uint32_t grd = str0::Atoi(b, l);
  uint32_t gmin = str0::Atoi((b + l), 2);
  uint32_t gsec = str0::Atoi((b + l + 3), 4);
  uint32_t gdectail = ((((10000 * gmin) + gsec) * 10 ) / 6);
  return grd * 1000000 + gdectail;
}

/* ------------------------------------------------------------------------- */
void NaviNote::PrsDate(const char* buf)
{
  clnd.day = str0::Atoi(buf, 2);
  clnd.month = str0::Atoi(buf + 2, 2);
  clnd.year = str0::Atoi(buf + 4, 2);
}

/* ------------------------------------------------------------------------- */
void NaviNote::PrsTime(const char* buf)
{
  clnd.hr = str0::Atoi(buf, 2);
  clnd.min = str0::Atoi(buf + 2, 2);
  clnd.sec = str0::Atoi(buf + 4, 2);
}

/* ------------------------------------------------------------------------- */
Navi::Navi()
{
  rmcvalid = false;
}


void Navi::InitFromRestored()
{
  intermediate_dist = accum_dist / 1000.0;
}

/* ------------------------------------------------------------------------- */
int16_t Navi::SetLatitude(const char* buf)
{
  lafull = SetL(str0::ToSym(buf, P_RMC_LAT), 2);

  if (*str0::ToSym(buf, P_RMC_LATNS) != 'N')
    lafull = -lafull;

  return 0;
}

/* ------------------------------------------------------------------------- */
int16_t Navi::SetLongitude(const char* buf)
{
  lofull = SetL(str0::ToSym(buf, P_RMC_LONG), 3);

  if (*str0::ToSym(buf, P_RMC_LONGEW) != 'E')
    lofull = -lofull;

  return 0;
}

/* ------------------------------------------------------------------------- */
bool Navi::RMCParse(const char* buf)
{
  rmcvalid = (*(str0::ToSym(buf, P_RMC_VALID)) == 'A') ? (true) : (false);

  if (rmcvalid == false)
    return false;

  PrsDate(str0::ToSym(buf, P_RMC_DATE));
  PrsTime(str0::ToSym(buf, P_RMC_TIME));

  if (str0::LenToSym(str0::ToSym(buf, P_RMC_LAT)) == 9 &&
      (str0::LenToSym(str0::ToSym(buf, P_RMC_TIME)) == 10))
  {
    SetLatitude(buf);
    SetLongitude(buf);
  }

  return true;
}

/* ------------------------------------------------------------------------- */
bool Navi::VTGParse(const char* vbuf)
{
  /* get ground speed in 10 meters/h (xxx.xx kmh * 100) */
  spd = str0::Atod((str0::ToSym(vbuf, P_VTG_SPD)), 3, 2);
  liveKurs = str0::Atod((str0::ToSym(vbuf, P_VTG_COERSE)), 3, 2);

  if (mvdetector.HandleSpeed(spd))
    // spd in 1 kmh / 3600 = distance in meters
    intermediate_dist += (((double)spd / 100.0) / 3600.0);

  return true;
}

/* ------------------------------------------------------------------------- */
bool Navi::GGAParse(const char* gbuf)
{
  const char* alt;
  alt = str0::ToSym(gbuf, P_GGA_ALT);

  if (*alt == ',')
    return false;

  if (*alt == '-')
  {
    altitude = str0::Atod(alt + 1, 4, 1);
    altitude = -altitude;
  }
  else
    altitude = str0::Atod(alt, 4, 1);

  return true;
}

/* ------------------------------------------------------------------------- */
void Navi::FreezeFixSpd()
{
  fixspd = spd;
}

/* ------------------------------------------------------------------------- */
void Navi::FreezeDistance()
{
  /// distance for saving to memory LSB 1m
  accum_dist = (uint32_t)(intermediate_dist * 1000);
}

/* ------------------------------------------------------------------------- */
bool Navi::Valid()
{
  return rmcvalid;
}
/* ------------------------------------------------------------------------- *
 * calc sufficient timeout value for next position fixing func (speed)
 * @fto - global to for fixing = 2 min
 * Actual speed determine actual fixing period, every time new period calc
 * return value = @fto - calculated period. It's leaving time in timer,
 * if this difference more than timer container => fix action
 *
 * if last fixspd value different from current value more than FIX_SPD_VAL
 * return @fto => invoke instantaneous fixing
 * ------------------------------------------------------------------------- */
uint32_t Navi::MathTo(uint32_t fto)
{
  uint32_t among = 0;
  /* check speed difference if big return max time and execute fixing */
  among = (spd > fixspd) ? (spd - fixspd) : (fixspd - spd);

  if (among > FIX_SPD_VAL)
    return fto;

  /* return time (s) = (fto - to) */
  if (spd < 200)
  {
    /* low speed start */
    if (fixspd > 200)
      /* need fix if not yet */
      return fto;

    return 0;
  }

  uint32_t newspd = (spd / 100);
  /* calc fix period with according to actual speed */
  among = ((newspd * newspd) >> 3) - (newspd * 5) + 140;
  among = among * 3600 / newspd;
  return (fto >= among) ? ((fto - among)) : (0);
}

int32_t Navi::CoerseChanged()
{
  int32_t diff_kurs = labs(fixkurs - liveKurs);

  if (diff_kurs > 500)
  {
    fixkurs = liveKurs;
    return 1;
  }
  
  return 0;
}

/* ------------------------------------------------------------------------- */
void Navi::Copy(Navi* cp)
{
  this->clnd.year = cp->clnd.year;
  this->clnd.month = cp->clnd.month;
  this->clnd.day = cp->clnd.day;
  this->clnd.hr = cp->clnd.hr;
  this->clnd.min = cp->clnd.min;
  this->clnd.sec = cp->clnd.sec;
  this->altitude = cp->altitude;
  this->accum_dist = cp->accum_dist;
  this->spd = cp->spd;
  this->lafull = cp->lafull;
  this->lofull = cp->lofull;
}
