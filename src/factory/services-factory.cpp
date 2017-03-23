#include "services-factory.h"

Worker& ServiceWorker()
{
  static Worker worker_;
  return worker_;
}


uint8_t* WorkBuffer()
{
  static const int32_t workLength = 1024;
  static uint8_t buff[workLength];
  return buff;
}

