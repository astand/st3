/* ------------------------------------------------------------------------- *
 *  MAIN SOURCE FILE
 * ------------------------------------------------------------------------- */
#include <stdint.h>
#include "startup/systick_ll.h"
#include "threads/rtos-init.h"
#include "hdwr/hw-init.h"

//#include "Configs/MainConfig.h"
//#include "peripheralInit.h"
///* for call SysClkUpdate()*/
//#include "commontaskfuncs.h"
//#include "timers.h"
//#include "stm32hw/stm32hw.h"

//#define TimerTickHandler          TIM3_IRQHandler

//OS_TID id1, id2, id3;



//uint32_t ledgpsmask[] =
//{
//  0x0000FCE7,
//  0x00FFFFFF,
//  0x55555555,
//  0x03030303,
//};


//uint32_t say_hello[2] =
//{
//  0xFFFFe222,
//  0xFFFFe222
//};

//PortHal prt[] =
//{
//  PortHal(GPIOC, 0, true), /* GSM */
//  PortHal(GPIOA, 5, true)  /* gps */
//};


////PortHal pd (GPIOA, 0, false);

///* ------------------------------------------------------------------------- *
// *  Timer sheduler interrupt handler
// * ------------------------------------------------------------------------- */
//extern "C"
//{
//  void TimerTickHandler (void)
//  {
//    uint32_t ulStatus;
//    ulStatus = TIM3->SR;

//    if (ulStatus & ((uint32_t)TIM_SR_UIF))
//      TimersTick();
//    else
//    {
//      /* not waiting interrupt flag*/
//    }

//    TIM3->SR &= ~((uint32_t)ulStatus);
//  }

//  void HardFault_Handler(void)
//  {
//    while (1) {}
//  }
//} // extern "C"


/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
//__task void tskledBlinkThread(void)
//{
//  os_itv_set (BLINK_TASK_ITV_VALUE);

//  while (1)   /* deadloop for LED and watch dog */
//  {
//    os_itv_wait ();
//    IWDT_RESET_COUNTER;
//    uint8_t idx = GpsLedHand();
//    ledgpsmask[idx] = __ror(ledgpsmask[idx], 1);
//    prt[1].Mask(ledgpsmask[idx] & 1);
//    prt[0].Mask(LedBlinkGsm());
//  }
//}

/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
//__task void tskCreator(void)
//{
//  id1 = os_tsk_create(tsk_gsmthrd, 1);
//  id2 = os_tsk_create(tskledBlinkThread, 1);
//  id3 = os_tsk_create(GhauntThread, 1);
////     fault();
//  os_tsk_delete_self();
//}


/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
//void SayHello()
//{
//  uint32_t mask = 0x00000001;

//  for (volatile int32_t i = 0 ; i < 26; i++)
//  {
//    prt[0].Mask((mask << i) & say_hello[0]);
//    prt[1].Mask((mask << i) & say_hello[1]);
//    WaitTim(dbg_to, 150);
//  }
//}
/* ------------------------------------------------------------------------- *
 *
 * ------------------------------------------------------------------------- */
int main (void)
{
  SysTickInit();
  AllHwInit();
//  SystemCoreClockUpdate();
//  GlobalPeripheralInit();
//  en3gv.Off();
//	DBG_Common("\n----------------------------------------------------\n");
//	DBG_Common("Start spy tracker");
//	DBG_Common("\n----------------------------------------------------\n");
//  SayHello();
//  os_sys_init (tskCreator);
  StartUserThreads();

  while (1)
  {
    __nop();
  }
}
