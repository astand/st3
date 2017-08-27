/*--------------------------------------------*/
#include "Configs/MainConfig.h"
/*-------------------------------------------*/
#include "stm32f4xx_flash.h"

#define SECTOR_MASK               ((uint32_t)0xFFFFFF07)
#define FLASH_Parralelism         FLASH_PSIZE_WORD
#define FLASH_STATUS_FLAGS        (FLASH_FLAG_BSY |\
                                   FLASH_FLAG_WRPERR |\
                                   FLASH_FLAG_RDERR |\
                                   (uint32_t)0xE0 |\
                                   FLASH_FLAG_OPERR)

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

inline static uint32_t GetFlashStatus()
{
  return FLASH->SR & FLASH_STATUS_FLAGS;
}

inline static void WAIT_BSY()
{
  while ((GetFlashStatus() & FLASH_SR_BSY) != 0)
    ;

  dummy(3);
  CLEAR_SR();
}


void BootFlash()
{
  volatile uint32_t addr;
  uint32_t wrword;

  if (HWREG(BOOT_KEY_ADDRESS) == 0xFFFFffff)
  {
    // there is no firmware for updating
    return;
  }

  /* check unlock state */
  if (FLASH->CR & FLASH_CR_LOCK)
  {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }

  /* target firmware memory */
  for (int sec = 0; sec < 4; sec++)
  {
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

  for (addr = 0; addr < (BOOT_FIRMWARE_SIZE); addr += 4)
  {
    while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
      ;

    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_WORD;
    FLASH->CR |= FLASH_CR_PG;
    // read boot firmware word
    wrword = (HWREG(BOOT_FIRMWARE_ADDRESS + addr));
    // and write it in the target memory
    HWREG(MCU_MEMORY_START + addr) = wrword;

    while ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
      ;

    FLASH->CR &= (~FLASH_CR_PG);
  }

  // boot programming ends successfully - clear boot firmware region
  WAIT_BSY();
  FLASH->CR &= CR_PSIZE_MASK;
  FLASH->CR |= FLASH_Parralelism;
  FLASH->CR &= SECTOR_MASK;
  FLASH->CR |= FLASH_CR_SER | (4 << 3);
  FLASH->CR |= FLASH_CR_STRT;
  WAIT_BSY();
  FLASH->CR &= (~FLASH_CR_SER);
  FLASH->CR &= SECTOR_MASK;
  FLASH->CR |= FLASH_CR_LOCK;

  for (volatile int i = 0; i < 1000; i++);

  HARDRESRET;
}

