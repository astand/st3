/* ------------------------------------------------------------------------- *
 *  MAIN SOURCE FILE
 * ------------------------------------------------------------------------- */
#include <stdint.h>
#include "startup/systick_ll.h"
#include "threads/rtos-init.h"
#include "hdwr/hw-init.h"

extern void BootFlash();

int main (void)
{
  SysTickInit();
  AllHwInit();
  StartUserThreads();

  while (1)
  {
    __nop();
  }
}
