#include "hw-spi.h"
#include "misc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "utility/buffers/ringbuffer.h"

/* Static defines ---------------------------------------------------------- */
#define SPIx                    SPI1
#define SPIx_InterruptHandler		SPI1_IRQHandler
#define SPIx_AF                 GPIO_AF_SPI1


#define SPIx_CLKx               GPIOB
#define SPIx_CLKx_Pin           GPIO_Pin_3
#define SPIx_CLKx_PinSrc        GPIO_PinSource3

#define SPIx_MISOx              GPIOB
#define SPIx_MISOx_Pin          GPIO_Pin_4
#define SPIx_MISOx_PinSrc       GPIO_PinSource4

#define SPIx_MOSIx              GPIOB
#define SPIx_MOSIx_Pin          GPIO_Pin_5
#define SPIx_MOSIx_PinSrc       GPIO_PinSource5

static RingBuffer rxb(128);
static RingBuffer txb(128);

static SpiHal sphal(SPIx);
static SpiBlockSender spi_sender(sphal, rxb, txb);

extern "C" void SPIx_InterruptHandler (void)
{
  spi_sender.Process();
}



/* Functions definitions --------------------------------------------------- */
static void SpiHWInit(void)
{
  GPIO_InitTypeDef spigp;
  SPI_InitTypeDef spiis;
  NVIC_InitTypeDef nv;
  GPIO_PinAFConfig(SPIx_CLKx, SPIx_CLKx_PinSrc, SPIx_AF);
  GPIO_PinAFConfig(SPIx_MISOx, SPIx_MISOx_PinSrc, SPIx_AF);
  GPIO_PinAFConfig(SPIx_MOSIx, SPIx_MOSIx_PinSrc, SPIx_AF);
  spigp.GPIO_Mode = GPIO_Mode_AF;
  spigp.GPIO_Speed = GPIO_Speed_50MHz;
  spigp.GPIO_OType = GPIO_OType_PP;
  spigp.GPIO_PuPd  = GPIO_PuPd_DOWN;
  spigp.GPIO_Pin = SPIx_CLKx_Pin;
  GPIO_Init(SPIx_CLKx, &spigp);
  spigp.GPIO_Pin = SPIx_MISOx_Pin;
  GPIO_Init(SPIx_MISOx, &spigp);
  spigp.GPIO_Pin = SPIx_MOSIx_Pin;
  GPIO_Init(SPIx_MOSIx, &spigp);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  spiis.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spiis.SPI_Mode = SPI_Mode_Master;
  spiis.SPI_DataSize = SPI_DataSize_8b;
  spiis.SPI_CPOL = SPI_CPOL_Low;
  spiis.SPI_CPHA = SPI_CPHA_1Edge;
  spiis.SPI_NSS = SPI_NSS_Soft;
  spiis.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  spiis.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_Init(SPIx, &spiis);
  SPIx->CR2 = 0;
  SPIx->CR2 |= (SPI_CR2_RXNEIE | SPI_CR2_ERRIE);
  nv.NVIC_IRQChannel = SPI1_IRQn;
  nv.NVIC_IRQChannelPreemptionPriority = 0;
  nv.NVIC_IRQChannelSubPriority = 0;
  nv.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nv);
  SPI_Cmd(SPIx, ENABLE);
}

void HwInit_Spi()
{
  SpiHWInit();
}

SpiBlockSender& GetBlockSender()
{
  return spi_sender;
}
