#include "thread-led.h"
#include "configs/free-rtos-def.h"

void tskLed(void*);


void Start_LedThread()
{
  xTaskCreate(tskLed, "0", kLowStackSz, (void*)NULL, kPrio_IDLE, NULL);
}


void tskLed(void*)
{
  static volatile int32_t ticks_from_timer = 0;
  
  while (1)
  {
    __nop();
		osDelay(10);
  }
}

