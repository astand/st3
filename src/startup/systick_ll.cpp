#include <misc.h>
#include <stm32f4xx.h>
#include "system_stm32f4xx.h"
#include "configs/free-rtos-def.h"
#include "systick_ll.h"


/// Import FRtos scheduler handler here
extern "C" void xPortSysTickHandler(void);


void SysTickInit()
{
  /// 4 bits for preemption priority
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//  NVIC_SetPriorityGrouping(__NVIC_PRIO_BITS);
// TODO: is it useful to change SysTick interrupt priority level
// ti seems that this priority always = -1
  NVIC_SetPriority(SysTick_IRQn, 10);
  SystemCoreClockUpdate();
  /// SysTick frequency is 1kHz
  SysTick_Config(SystemCoreClock / 1000);
}

extern "C" void SysTick_Handler(void)
{
  /// Call free rtos scheduler
  xPortSysTickHandler();
}
