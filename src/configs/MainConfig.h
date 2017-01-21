#pragma once

#ifdef __cplusplus
extern "C" {
#endif
/* ------------------------------------------------------------------------- *
 * Common  include system files
 * ------------------------------------------------------------------------- */
//#include <RTL.H>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx.h"



/* ------------------------------------------------------------------------- *
 * Main clock value for calculate timing values
 * ------------------------------------------------------------------------- */
#define SYSCLK_MAIN 84000000
/* ------------------------------------------------------------------------- *
 *  Timers base koeff for timeout handler
 * ------------------------------------------------------------------------- */
#define BLINK_TASK_ITV_VALUE     100

/* ------------------------------------------------------------------------- *
 *  Macros for hardware access, both direct and via the bit-band region.
 * ------------------------------------------------------------------------- */
#define HWREG(x)    \
  (*((volatile unsigned long *)(x)))
#define HWREGH(x)   \
  (*((volatile unsigned short *)(x)))
/* ------------------------------------------------------------------------- *
 *  SYS reset and tick disable (to any Cortex-M3 proc)
 * ------------------------------------------------------------------------- */
#define HARDRESRET       \
  HWREG(0xE000ED0C) = 0x05FA0001; while (1)

#define SYSTICKDISABLE   \
  HWREG(0xE000E010) = 0x00000000

/* ------------------------------------------------------------------------- *
 *  DMA buffer defines (chargeControl.c/peripheral.c)
 * ------------------------------------------------------------------------- */
#define ADC_CHANNEL_COUNT             3
#define ADC_COUNT_OF_SAMPLES          64 /* count of samples get */
#define DMA_ADC_BUFFER_SIZE           (ADC_CHANNEL_COUNT*ADC_COUNT_OF_SAMPLES)

/* extern for using raw adc buffer in charge control thread */
extern __IO uint16_t dma_adc_raw[];
extern uint32_t pindrlink;
/* ------------------------------------------------------------------------- *
 *
 *
 *                           Flash memory MAP
 *
 *
 * ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- *
 *  Start main Flash Memory
 * ------------------------------------------------------------------------- */
#define MCU_FULL_MEMORY_START      0x08000000

/* ------------------------------------------------------------------------- *
 *   Full MCU memory size select
 * ------------------------------------------------------------------------- */
#define MCU_FULL_MEMORY_SIZE		    0x80000   /* 512 kB */


/* ------------------------------------------------------------------------- *
 *   Full size of function that will be kill main flash sector
 * ------------------------------------------------------------------------- */
#define KILL_FLASH_MEMORY_SIZE	    0x800   /* 2kB*/

/* ------------------------------------------------------------------------- *
 * Full size of memory sector that will be contain
 * all system non volatile settings
 * ------------------------------------------------------------------------- */
#define CONFIG_MEMORY_SIZE			    0x1000	/* 4 kB*/

/* ------------------------------------------------------------------------- *
 *   Calculation BASE adrress for killflash function (see in *.sct file)
 * ------------------------------------------------------------------------- */
#define KILL_FLASH_BASE_ADDR		    \
  ((MCU_FULL_MEMORY_START + MCU_FULL_MEMORY_SIZE) - KILL_FLASH_MEMORY_SIZE)

/* ------------------------------------------------------------------------- *
 *   Calculation BASE adrress for CONFIG
 * ------------------------------------------------------------------------- */
#define CONFIG_START_ADRESS			  \
  (KILL_FLASH_BASE_ADDR -	CONFIG_MEMORY_SIZE)

/* ------------------------------------------------------------------------- *
 *   Page for btn status saving
 * ------------------------------------------------------------------------- */
#define BTN_START_ADDRESS          \
  (CONFIG_START_ADRESS - MCU_FLASH_PAGE_SIZE)

/* ------------------------------------------------------------------------- *
 *   Base address for saving new firmware kill main flash sector
 * ------------------------------------------------------------------------- */
#define START_SAVE_ADDR 	          \
  ((MCU_FULL_MEMORY_SIZE/2)+MCU_FULL_MEMORY_START)

/* ------------------------------------------------------------------------- *
 *   Base address for saving new firmware kill main flash sector
 * ------------------------------------------------------------------------- */
#define FIRM_MAX_SIZE							0x10000 /* 64 kB */

/* ------------------------------------------------------------------------- *
 *   Page size for erase operation
 * ------------------------------------------------------------------------- */
#define MCU_FLASH_PAGE_SIZE        0x800 /* 2 kB */

/* ------------------------------------------------------------------------- *
 *
 *
 *                          End flash memory MAP
 *
 *
 * ------------------------------------------------------------------------- */
#define 	RAMGCONF 	0x20004000
#define  	RAMBTN 		(RAMGCONF + 4096)
/* ------------------------------------------------------------------------- *
 *  ror operation
 * ------------------------------------------------------------------------- */
#define ROR32(x)  (x = __ror(x,1))
#define DEADLOOP    while(1)
extern void KillFlash(void);
extern void dummy(void);
#ifdef __cplusplus
}
#endif
