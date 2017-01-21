#include "port-init.h"
#include "pins-definition.h"
#include "stm32f4xx_gpio.h"

static void SetPinConfig(const PinConfiguraion* pin)
{
  static GPIO_InitTypeDef itd;
  /// set pin
  itd.GPIO_Pin = 0;
  itd.GPIO_Pin |= (1 << pin->decsriptor.pin_num);
  /// set speed
  itd.GPIO_Speed = (GPIOSpeed_TypeDef)pin->speed;
  /// set default - OD, no pull
  itd.GPIO_Mode = GPIO_Mode_OUT;
  itd.GPIO_OType = GPIO_OType_OD;
  itd.GPIO_PuPd = GPIO_PuPd_NOPULL;

  if (pin->type.pull == kPullUp)
    itd.GPIO_PuPd = GPIO_PuPd_UP;
  else if (pin->type.pull == kPullDown)
    itd.GPIO_PuPd = GPIO_PuPd_DOWN;

  if (pin->type.dir == kInput)
    itd.GPIO_Mode = GPIO_Mode_IN;
  else if (pin->type.dir == kPushPull)
    itd.GPIO_OType = GPIO_OType_PP;

  GPIO_Init(pin->decsriptor.port, &itd);
}

static void SetUpAllPins()
{
  for (int16_t i = 0; i < kMaxPinsCount; i++)
    SetPinConfig(GetConfiguration(static_cast<DescriptorName>(i)));
}

void InitOnBoardGpio()
{
  SetUpAllPins();
}
