#include "pins-definition.h"
#include "stm32f4xx_gpio.h"


const PinDescriptor* GetDescriptor(DescriptorName index)
{
  return &(GetConfiguration(index)->decsriptor);
}


const PinConfiguraion* GetConfiguration(DescriptorName index)
{
  static const PinConfiguraion pin_collection[kMaxPinsCount] =
  {
    { { GPIOC, 0,   true }, { kPushPull, kNone },  GPIO_Speed_25MHz }, /* Led 1 */
    { { GPIOA, 5,   true }, { kPushPull, kNone },  GPIO_Speed_25MHz }, /* Led 2 */
    { { GPIOC, 13,  true  }, { kPushPull, kNone },  GPIO_Speed_25MHz }, /* Enable GSM */
    { { GPIOB, 1,   true  }, { kPushPull, kNone },  GPIO_Speed_25MHz }, /* Enable GPS */
    { { GPIOC, 14,  true  }, { kPushPull, kNone },  GPIO_Speed_25MHz }, /* PwrKey */
    { { GPIOB, 6,   true  }, { kPushPull, kNone },  GPIO_Speed_25MHz }, /* ChipSelect for mx25 spi */
    { { GPIOC, 1,   false }, { kInput, kPullUp  },  GPIO_Speed_25MHz }  /* DCD Signal */
  };

  if (index >= kMaxPinsCount)
  {
    while (1) {}
  }

  return &(pin_collection[index]);
}

