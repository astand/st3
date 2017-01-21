#include "switch-maker.h"
#include "utility/bridge/inc/gpio-hal.h"
#include "hdwr/controls/pins-definition.h"

ISwitchable& SwitchMaker::GetLed1()
{
  static PinDescriptor const* dsc_ = GetDescriptor(kLed1);
  static PortHal ph_(dsc_->port, dsc_->pin_num, dsc_->reverse);
  return ph_;
}


ISwitchable& SwitchMaker::GetLed2()
{
  static PinDescriptor const* dsc_ = GetDescriptor(kLed2);
  static PortHal ph_(dsc_->port, dsc_->pin_num, dsc_->reverse);
  return ph_;
}


ISwitchable& SwitchMaker::GetGsmEn()
{
  static PinDescriptor const* dsc_ = GetDescriptor(kEnGSM);
  static PortHal ph_(dsc_->port, dsc_->pin_num, dsc_->reverse);
  return ph_;
}


ISwitchable& SwitchMaker::GetGpsEn()
{
  static PinDescriptor const* dsc_ = GetDescriptor(kEnGPS);
  static PortHal ph_(dsc_->port, dsc_->pin_num, dsc_->reverse);
  return ph_;
}


ISwitchable& SwitchMaker::GetPwrKey()
{
  static PinDescriptor const* dsc_ = GetDescriptor(kPwrKey);
  static PortHal ph_(dsc_->port, dsc_->pin_num, dsc_->reverse);
  return ph_;
}


ISwitchable& SwitchMaker::GetChipSelect()
{
  static PinDescriptor const* dsc_ = GetDescriptor(kChipSelect);
  static PortHal ph_(dsc_->port, dsc_->pin_num, dsc_->reverse);
  return ph_;
}


ISwitchable& SwitchMaker::GetDCDSignal()
{
  static PinDescriptor const* dsc_ = GetDescriptor(kDCDInput);
  static PortHal ph_(dsc_->port, dsc_->pin_num, dsc_->reverse);
  return ph_;
}


// ISwitchable& SwitchMaker::GetLed1()
// {
//   static PinDescriptor const* dsc_ = GetDescriptor(kLed1);
//   static PortHal ph_(dsc_->port, dsc_->pin_num, dsc_->reverse);

//   return ph_;
// }

