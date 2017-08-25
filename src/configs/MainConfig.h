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

#define MCU_MEMORY_START            0x08000000U
#define MCU_MEMORY_SIZE		          0x20000U   /* 128 kB */
#define __MCU_MEMORY_END            (MCU_MEMORY_START + MCU_MEMORY_SIZE)

/* ------------------------------------------------------------------------- *
 *   Full MCU memory size select
 * ------------------------------------------------------------------------- */
#define BOOT_FIRMWARE_ADDRESS       (MCU_MEMORY_START + 0x10000U)
#define BOOT_FIRMWARE_SIZE          (0xFFE0U)
#define BOOT_KEY_ADDRESS            (__MCU_MEMORY_END - 32U)

#ifdef __cplusplus
}
#endif
