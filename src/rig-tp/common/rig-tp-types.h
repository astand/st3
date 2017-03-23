#pragma once

#include <stdint.h>

namespace Rig
{

typedef struct
{
  uint16_t Opc;
  uint16_t RigId;
  uint16_t BlockNum;
  uint8_t Data[1000];
} RigFrame;

typedef enum
{
  Idle,
  CmdAck,
  Data,
  Error
} RigTxState;

typedef enum
{
  Ignored,
  Handled
} HandleResult;
// close namespace
}
