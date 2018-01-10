#include "firm-writer.h"
#include "stm32f4xx_flash.h"

#define PENDING_FLASH_FLAGS (FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |\
                             FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR)

static const uint32_t kMemStart = 0x08010000;
static const uint32_t kMemEnd = 0x08020000 - 128;
static const int32_t kSecCnt = 1;
static const uint32_t erase_sectors[kSecCnt] = { FLASH_Sector_4 };
static const uint8_t voltRange = VoltageRange_3;

FirmWriter::FirmWriter()
{
  flashfailed = true;
}

// mem must be align to word, so @len must be aligned also
int32_t FirmWriter::Program(const uint8_t* mem, uint32_t len)
{
  if (flashfailed)
    return -3;

  if (memaddr + len >= kMemEnd)
    return -2;

  int32_t ret = ProgramTo(mem, memaddr, len);

  if (ret >= 0)
  {
    // there are no errors shift address
    memaddr += ret;
  }

  return ret;
}


int32_t FirmWriter::Program(const uint8_t* mem, uint32_t shift, uint32_t len)
{
  return -1;
}


int32_t FirmWriter::ProgramTo(const uint8_t* mem, uint32_t addr, uint32_t len)
{
  int32_t retwrote = len;
  uint32_t dataword;
  FLASH_Unlock();

  for (int32_t i = 0; i < len; i += 4)
  {
    dataword = *((uint32_t*)(mem + i));

    if (ProgramAndCheck(addr + i, dataword) != 0)
    {
      // error occured
      retwrote = -1;
      break;
    }
  }

  FLASH_Lock();
  return retwrote;
}


bool FirmWriter::IsBusy()
{
  return false;
}


bool FirmWriter::Erase()
{
  FLASH_Status retflash = FLASH_BUSY;
  bool ret = true;
  FLASH_Unlock();

  for (int32_t secid = 0; secid < kSecCnt; secid++)
  {
    FLASH_ClearFlag(PENDING_FLASH_FLAGS);
    retflash = FLASH_EraseSector(erase_sectors[secid], voltRange);

    if (retflash != FLASH_COMPLETE)
    {
      ret = false;
      break;
    }
  }

  FLASH_Lock();
  // memory was successfully erased
  memaddr = kMemStart;
  flashfailed = false;
  return ret;
}

int32_t FirmWriter::ProgramAndCheck(uint32_t addr, uint32_t data)
{
  FLASH_ClearFlag(PENDING_FLASH_FLAGS);
  FLASH_Status retflash = FLASH_ProgramWord(addr, data);

  if (retflash != FLASH_COMPLETE)
  {
    flashfailed = true;
  }

  return 0;
}
