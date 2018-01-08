#include "navkeeper.h"
#include <stdlib.h>

NavKeeper::NavKeeper(NaviNote* noteref) : note_(noteref)
{
}


void NavKeeper::InitFromRestored()
{
  intermediate_dist = note_->accum_dist / 1000.0;
}

void NavKeeper::HandleGpsData(GpsPositionData_t* data)
{
  if (data->update != kNoUpdate)
  {
    if (data->update == kSpeed)
    {
      if (mvdetector.HandleSpeed(data->pos->spd))
        // spd in 1 kmh / 3600 = distance in meters
        intermediate_dist += (((double)(note_->spd) / 100.0) / 3600.0);

      liveKurs = data->sup.kurs;
    }
  }
}

/* ------------------------------------------------------------------------- */
void NavKeeper::FreezeFixSpd()
{
  fixspd = note_->spd;
}

/* ------------------------------------------------------------------------- */
void NavKeeper::FreezeDistance()
{
  /// distance for saving to memory LSB 1m
  note_->accum_dist = (uint32_t)(intermediate_dist * 1000);
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
uint32_t NavKeeper::MathTo(uint32_t fto)
{
  uint32_t among = 0;
  /* check speed difference if big return max time and execute fixing */
  among = (note_->spd > fixspd) ? (note_->spd - fixspd) : (fixspd - note_->spd);

  if (among > FIX_SPD_VAL)
    return fto;

  /* return time (s) = (fto - to) */
  if (note_->spd < 200)
  {
    /* low speed start */
    if (fixspd > 200)
      /* need fix if not yet */
      return fto;

    return 0;
  }

  uint32_t newspd = (note_->spd / 100);
  /* calc fix period with according to actual speed */
  among = ((newspd * newspd) >> 3) - (newspd * 5) + 140;
  among = among * 3600 / newspd;
  return (fto >= among) ? ((fto - among)) : (0);
}

int32_t NavKeeper::CoerseChanged()
{
  int32_t diff_kurs = labs(fixkurs - liveKurs);

  if (diff_kurs > (10 * 100))
  {
    fixkurs = liveKurs;
    return 1;
  }

  return 0;
}
