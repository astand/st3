#pragma once

typedef struct
{
  uint16_t Opc;
  uint16_t RigId;
  uint16_t BlockNum;
  uint8_t* Data;
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
