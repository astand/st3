#include "speedsnaper.hpp"

/* ------------------------------------------------------------------------- */
SpeedSnaper::SpeedSnaper()
{
  m_move_status = STOP;
  accumspd = 0;
  speed_real = 0;
  count = SPEED_STOP_COUNT;
}

/* ------------------------------------------------------------------------- */
bool SpeedSnaper::HandleSpeed(uint16_t inspd)
{
  speed_real = inspd;
  bool resetneed = false;
  resetneed = (m_move_status == STOP) ? (!IsSpeedUp()) : (IsSpeedUp());
  NextDetect();

  if (resetneed)
    ResetDetection();

  if (CountOverflow())
  {
    ResetDetection();
    m_move_status = (m_move_status == STOP) ? MOVE : STOP;
  }

  return (GetMoveStatus() == MOVE);
}

/* ------------------------------------------------------------------------- */
void SpeedSnaper::ResetDetection()
{
  count = SPEED_STOP_COUNT;
  accumspd = 0;
}

/* ------------------------------------------------------------------------- */
void SpeedSnaper::NextDetect()
{
  accumspd += speed_real;
  --count;
}

/* ------------------------------------------------------------------------- */
bool SpeedSnaper::CountOverflow()
{
  return (count <= 0) ? true : false;
}
