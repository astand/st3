#include "navinote.h"
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
Navi::Navi()
{
  rmcvalid = false;
}


void Navi::InitFromRestored()
{
  intermediate_dist = accum_dist / 1000.0;
}

void Navi::HandleGpsData(GpsPositionData_t* data)
{
  if (data->update != kNoUpdate)
  {
    if (data->update == kSpeed)
    {
      if (mvdetector.HandleSpeed(data->pos->spd))
        // spd in 1 kmh / 3600 = distance in meters
        intermediate_dist += (((double)spd / 100.0) / 3600.0);

      liveKurs = data->sup.kurs;
    }
  }
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

  if (diff_kurs > (10 * 100))
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
