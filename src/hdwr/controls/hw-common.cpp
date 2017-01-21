#include "hw-common.h"
#include "stm32f4xx_rcc.h"




#if !defined (WATCH_DOG_ENABLE)
void ResetWatchDogCounter()
{
  IWDG->KR = ((uint16_t)0xAAAA);
}

static void IWDGInit(void)
{
  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* IWDG timeout equal to 2000 ms (the timeout may varies due to LSI
   * frequency dispersion). Enable write access to IWDG_PR and IWDG_RLR
   * registers    																												*/
  IWDG->KR = 0x5555;
  /* IWDG counter clock: LSI/32 (1250Hz)*/
  IWDG->PR = ((uint8_t)0x05);
  /* set reload value */
  IWDG->RLR = 4095;
  ResetWatchDogCounter();
  IWDG->KR = ((uint16_t)0xCCCC);
}
#else
static void IWDGInit() {}
void ResetWatchDogCounter() {}
#endif

void HwInit_Common()
{
  IWDGInit();
}

