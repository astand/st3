#pragma once

#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"
#include "FreeRTOS/Source/include/timers.h"
#include "FreeRTOS/Source/include/semphr.h"
#include "FreeRTOS/Source/include/event_groups.h"

const UBaseType_t kPrio_IDLE = tskIDLE_PRIORITY + 5;
const uint16_t kLowStackSz = 128;

static inline void osDelay(const TickType_t ticks)
{
  vTaskDelay(ticks);
}

static inline void osPass()
{
  vTaskDelay(1);
  /// or
  /// vTaskDelayUntil(?)
}
