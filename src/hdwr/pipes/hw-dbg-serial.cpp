#include "hw-dbg-serial.h"

#include "configs/dbgconf.h"

#include "utility/streams/uart-stream.h"
#include <misc.h>
#include <stm32f4xx.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

#define USARTx_BAUDRATE   921600

/* Definition for USARTx clock resources */
#define USARTx                          USART1
#define USARTx_IRQn                     USART1_IRQn
#define USARTx_IRQHandler               USART1_IRQHandler
#define USARTx_CLK_ENABLE()             RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

#define USARTx_FORCE_RESET()            RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
#define USARTx_RELEASE_RESET()          RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                   GPIO_Pin_9
#define USARTx_TX_GPIO_PORT             GPIOA
#define USARTx_TX_AF                    GPIO_AF_USART1
#define USARTx_TX_PinSource             GPIO_PinSource9

#define USARTx_RX_PIN                   GPIO_Pin_10
#define USARTx_RX_GPIO_PORT             GPIOA
#define USARTx_RX_AF                    GPIO_AF_USART1
#define USARTx_RX_PinSource             GPIO_PinSource10

/// Define TASKDBG it if you need debug
#if defined (TASKDBG)

static RingBuffer dbg_tx(512);
static RingBuffer dbg_rx(64);
static UartHal dbguart(USARTx);

static UartStream dbgstream(dbguart, dbg_rx, dbg_tx);


extern "C" void USARTx_IRQHandler(void)
{
  dbgstream.Process();
}


int DbgPutc(int c)
{
  return dbgstream.Write(static_cast<uint8_t>(c));
}


/// Next two functions will called from Retarget.c driver
extern "C" int SER_PutChar (int c)
{
  return (DbgPutc(c));
}


extern "C" int SER_GetChar (void)
{
  return dbgstream.Read();
}


/* Peripheril Initialize -------------------------------------------------- */
static void PinsInit()
{
  GPIO_InitTypeDef gpitf;
  /* Connect USART pins to AF7 */
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_PinSource,  USARTx_TX_AF);
  GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_PinSource, USARTx_RX_AF);
  /* */
  gpitf.GPIO_Mode = GPIO_Mode_AF;
  gpitf.GPIO_Speed = GPIO_Speed_25MHz;
  gpitf.GPIO_OType = GPIO_OType_PP;
  gpitf.GPIO_PuPd = GPIO_PuPd_UP;
  gpitf.GPIO_Pin = (USARTx_TX_PIN |  USARTx_RX_PIN);
  GPIO_Init(USARTx_TX_GPIO_PORT, &gpitf);
  GPIO_Init(USARTx_RX_GPIO_PORT, &gpitf);
}

static void UartInit()
{
  USART_InitTypeDef uitd;
  NVIC_InitTypeDef  nvitd;
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();
  USARTx_CLK_ENABLE();
  uitd.USART_BaudRate =  USARTx_BAUDRATE;
  uitd.USART_WordLength = USART_WordLength_8b;
  uitd.USART_StopBits = USART_StopBits_1;
  uitd.USART_Parity = USART_Parity_No;
  uitd.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  uitd.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &uitd);
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  /* Enable the USARTx Interrupt */
  nvitd.NVIC_IRQChannel = USARTx_IRQn;
  nvitd.NVIC_IRQChannelPreemptionPriority = 3;
  nvitd.NVIC_IRQChannelSubPriority = 0;
  nvitd.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvitd);
  USART_Cmd(USARTx, ENABLE);
}

void DbgInit()
{
  PinsInit();
  UartInit();
}

/* -------------------- NO TASKDBG ---------------------- */
#else
extern "C" int SER_PutChar (int c)
{
  return (c);
}

extern "C" int SER_GetChar (void)
{
  return -1;
}

void DbgInit()
{
  /// Dummy func
}

#endif
/* -------------------- END NO TASKDBG ------------------- */


void HwInit_DbgSerial()
{
  DbgInit();
}



