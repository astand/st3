#pragma once

#include <stdint.h>
#include "stm32f4xx.h"

typedef enum
{
  kLed1 = 0,
  kLed2,
  kEnGSM,
  kEnGPS,
  kPwrKey,
  kChipSelect,
  kDCDInput,
  // Max value
  kMaxPinsCount
} DescriptorName;

// typedef enum { kInput, kInputPU, kOutOD, kOutODPU, kOutPP } PinType;
typedef enum { kInput, kOpedDrain, kPushPull } Direction;
typedef enum { kNone, kPullUp, kPullDown } PullingLine;

typedef struct
{
  GPIO_TypeDef* port;
  uint32_t pin_num;
  bool reverse;
} PinDescriptor;

typedef struct
{
  Direction dir;
  PullingLine pull;
} PinType;

typedef struct
{
  PinDescriptor decsriptor;
  PinType type;
  /// GPIO_speed_define  GPIO speed define
  uint32_t speed;
} PinConfiguraion;


// Descriptor holds fields for PortHal initialisation
extern const PinDescriptor* GetDescriptor(DescriptorName index);
// Configuration holds fields for hardware intialisation by SPL
extern const PinConfiguraion* GetConfiguration(DescriptorName index);

