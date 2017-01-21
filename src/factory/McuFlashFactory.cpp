#include "McuFlashFactory.hpp"
#include "oldcommon/McuFlash/SaverModel/SectorWriter.hpp"
#include "oldcommon/McuFlash/HardwareDriver/FlashDriver-STM32F4.hpp"


ISectorWriter* FlashFactory::m_sw_instance;

static IFlashDriver* flash_driver;

// Return sector writer for firmware saving
ISectorWriter* FlashFactory::GetSectorWriter()
{
  CreateHardwareDriver();

  if (m_sw_instance == 0)
    m_sw_instance = new SectorWriter(flash_driver, new SectorDescriptor(4, Mapping::List[0]));

  return m_sw_instance;
}


void FlashFactory::CreateHardwareDriver()
{
  if (flash_driver == 0)
    flash_driver = new FlashDriverStm32f4();
}
