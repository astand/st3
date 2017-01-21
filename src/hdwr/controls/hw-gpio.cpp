#include "hw-gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "port-init.h"

static void EnableGpioRcc()
{
  RCC_AHB1PeriphClockCmd	 (RCC_AHB1Periph_GPIOA |
                            RCC_AHB1Periph_GPIOB |
                            /* */
                            RCC_AHB1Periph_GPIOC, ENABLE);
}


void HwInit_Gpio()
{
  EnableGpioRcc();
  /// Pin definitions here
  InitOnBoardGpio();
}

