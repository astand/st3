#include "rtos-init.h"
#include "configs/free-rtos-def.h"

#include "threads/thread-led.h"
#include "threads/thread-gsm.h"
#include "threads/thread-gps.h"
#include "threads/thread-srv.h"

void StartUserThreads()
{
  Start_LedThread();
  Start_GsmThread();
  Start_GpsThread();
  Start_ServiceThread();
  vTaskStartScheduler();
}
