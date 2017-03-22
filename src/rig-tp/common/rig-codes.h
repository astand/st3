#pragma once

namespace Rig
{
// OpCodes
typedef enum
{
  RRQ = 1,
  WRQ = 2,
  DATA = 3,
  ACK = 4,
  ERR = 5
} Opc;

// Rig IDs
typedef enum
{
  None = 0,
  TrekList = 1,
  SoleTrek = 2,
  Firmware = 3,
  Info = 4,
  Echo = 128
} RigId;
// close namespace
}
