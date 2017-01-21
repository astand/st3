#pragma once

#include <stdint.h>

/* -------------------------------------------------------------------------- */
class SpeedSnaper {
 public:
  SpeedSnaper();
  /* Call in speed parse function and return true if STATE is moving */
  bool HandleSpeed(uint16_t);
  bool IsMovement() {
    return (m_move_status == MOVE);
  }

 private:
  static const uint16_t SPEED_STOP_EDGE = 350;
  static const uint16_t SPEED_STOP_COUNT = 5;
  int16_t count;

  enum {STOP = 0, MOVE} m_move_status;
  uint16_t accumspd;
  uint16_t speed_real;

  void ResetDetection();

  void NextDetect();
  bool CountOverflow();

  bool IsSpeedUp() {
    return speed_real >= SPEED_STOP_EDGE;
  }
  int32_t GetMoveStatus() {
    return (int32_t)m_move_status;
  }
};
