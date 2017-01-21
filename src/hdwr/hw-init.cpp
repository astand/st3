#include "hw-init.h"
#include "hdwr/pipes/hw-spi.h"
#include "hdwr/pipes/hw-dbg-serial.h"
#include "hdwr/pipes/hw-gps-serial.h"
#include "hdwr/pipes/hw-gsm-serial.h"
#include "hdwr/controls/hw-gpio.h"
#include "hdwr/controls/hw-common.h"
#include "hdwr/controls/hw-tick-timer.h"
#include "hdwr/controls/hw-adc.h"



void AllHwInit()
{
  HwInit_Gpio();
//  HwInit_Common();
//  HwInit_Adc();
  HwInit_UserTimer();
  HwInit_Spi();
  HwInit_DbgSerial();
  HwInit_GpsSerial();
  HwInit_GsmSerial();
}
