#include "hw-gsm-serial.h"

#include "utility/streams/uart-stream.h"
#include <misc.h>
#include <stm32f4xx.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

#define USARTx_BAUDRATE   115200

/* Definition for USARTx clock resources */
#define USARTx                          USART2
#define USARTx_IRQn                     USART2_IRQn
#define USARTx_IRQHandler               USART2_IRQHandler
#define USARTx_CLK_ENABLE()             RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE)

#define USARTx_FORCE_RESET()            RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE)
#define USARTx_RELEASE_RESET()          RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE)

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                   GPIO_Pin_2
#define USARTx_TX_GPIO_PORT             GPIOA
#define USARTx_TX_AF                    GPIO_AF_USART2
#define USARTx_TX_PinSource             GPIO_PinSource2

#define USARTx_RX_PIN                   GPIO_Pin_3
#define USARTx_RX_GPIO_PORT             GPIOA
#define USARTx_RX_AF                    GPIO_AF_USART2
#define USARTx_RX_PinSource             GPIO_PinSource3

static RingBuffer gsm_tx(4096);
static RingBuffer gsm_rx(1024);
static UartHal gsmuart(USARTx);

static UartStream gsmstream(gsmuart, gsm_rx, gsm_tx);

extern "C" void USARTx_IRQHandler(void)
{
  gsmstream.Process();
}

/* Peripheril Initialize -------------------------------------------------- */
static void PinsInit()
{
  GPIO_InitTypeDef gpitf;
  /* Connect USART pins to AF7 */
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_PinSource, USARTx_TX_AF);
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


void HwInit_GsmSerial()
{
  PinsInit();
  UartInit();
}

IStreamable& GetGsmPipe()
{
  return gsmstream;
}


