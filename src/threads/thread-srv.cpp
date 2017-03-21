#include "thread-srv.h"
#include "configs/free-rtos-def.h"
#include "factory/services-factory.h"

Worker& servWorker = ServiceWorker();

void ServiceThread(void* args)
{
  while (1)
  {
    servWorker.DoWork();
    osPass();
  }
}

void Start_ServiceThread()
{
  xTaskCreate(ServiceThread, NULL, kLowStackSz, (void*)NULL, kPrio_IDLE, NULL);
}

