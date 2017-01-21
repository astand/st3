/*--------------------------------------------*/
#include "Configs/MainConfig.h"
/*-------------------------------------------*/
#include "stm32f4xx_flash.h"

#define SECTOR_MASK               ((uint32_t)0xFFFFFF07)
#define FLASH_Parralelism         FLASH_PSIZE_WORD

void ResetWatchDogCounter();

inline static void CLEAR_SR()
{
  FLASH->SR = (FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
               FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
}

inline void dummy(int32_t delay)
{
  volatile int32_t i = delay;

  for (i = delay; i != 0 ; i--);
}

inline static void WAIT_BSY()
{
  while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY)
    /* empty block */
    __NOP();

  dummy(3);
  CLEAR_SR();
}



void KillFlash(void)
{
  volatile uint32_t addr;
  uint32_t wrword;

  /* check unlock state */
  if (FLASH->CR & FLASH_CR_LOCK)
  {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }

  /* clear first sectors */
  for (int sec = 0; sec < 4; sec++)
  {
    ResetWatchDogCounter();
    WAIT_BSY();
    /* if the previous operation is completed, proceed to erase the sector */
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_Parralelism;
    FLASH->CR &= SECTOR_MASK;
    FLASH->CR |= FLASH_CR_SER | (sec << 3);
    FLASH->CR |= FLASH_CR_STRT;
    WAIT_BSY();
    FLASH->CR &= (~FLASH_CR_SER);
    FLASH->CR &= SECTOR_MASK;
  }

  for (addr = 0; addr < (FIRM_MAX_SIZE); addr += 4)
  {
    while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
      ;

    wrword = (HWREG(0x08010000 + addr));
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_WORD;
    FLASH->CR |= FLASH_CR_PG;
    /* write operation */
    *(__IO uint32_t*)(addr) = wrword;

    while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
      ;

    FLASH->CR &= (~FLASH_CR_PG);
  }

  /* clear flash main mem sector */
  HARDRESRET;
}

