#include "thread-led.h"
#include "configs/free-rtos-def.h"
#include "factory/switch-maker.h"

extern uint8_t GetIntegerModemState();

/* ------------------------------------------------------------------------- */
static uint32_t led1_pallete[7] =
{
  0x311,      /* no echo  */
  0x031,      /* no sim */
  0x10001,    /* no reg */
  0xFFF,      /* reg */
  0x33333333, /* connect */
  0xCCCCCCCC, /* off */
  0
};

void Led1SchedulingProcess()
{
  static ISwitchable& led1 = SwitchMaker::GetLed1();
  int32_t id = GetIntegerModemState();
  led1_pallete[id] = __ror(led1_pallete[id], 1);
  led1.off();

  if ((led1_pallete[id]) & 1)
    led1.on();
}

void Led2SchedulingProcess()
{
}

void tskLed(void*)
{
  static volatile int32_t ticks_from_timer = 0;

  while (1)
  {
    /// 32 times in 4 secs
    osDelay(125);
    Led1SchedulingProcess();
    Led2SchedulingProcess();
  }
}

void Start_LedThread()
{
  xTaskCreate(tskLed, "0", kLowStackSz, (void*)NULL, kPrio_IDLE, NULL);
}

