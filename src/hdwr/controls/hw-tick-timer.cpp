#include "hw-tick-timer.h"
#include <misc.h>
#include "utility/timers/timer.h"

using namespace Timers;

extern "C" void TIM3_IRQHandler(void)
{
  uint32_t ulStatus;
  ulStatus = TIM3->SR;

  if (ulStatus & ((uint32_t)TIM_SR_UIF))
  {
    TickerUp::TickProcess();
  }

  TIM3->SR &= ~((uint32_t)ulStatus);
}



void GeneralTimerInit()
{
  RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;
  RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  NVIC_InitTypeDef  nvconf;
  TIM3->CR1 = 0;
  TIM3->CR1 |= (TIM_CR1_DIR);
  TIM3->PSC = (83);   /* 1MHz */
  TIM3->ARR = 999;    /* 1KHz */
  TIM3->DIER = TIM_DIER_UIE; /* interrupt enable */
  /* Enable the USARTx Interrupt */
  nvconf.NVIC_IRQChannel = TIM3_IRQn;
  nvconf.NVIC_IRQChannelPreemptionPriority = 0;
  nvconf.NVIC_IRQChannelSubPriority = 0;
  nvconf.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvconf);
  TIM3->CR1 |= TIM_CR1_CEN;   /* start timer */
}


void HwInit_UserTimer()
{
  GeneralTimerInit();
}

