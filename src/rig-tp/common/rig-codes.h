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
  NONE = 0,
  TRACKLIST = 1,
  TRACK = 2,
  INFO = 4,
  ECHO = 5,
  FIRMWARE = 6,
} ID;
// close namespace
}
