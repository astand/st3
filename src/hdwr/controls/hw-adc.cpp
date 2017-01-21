#include "hw-adc.h"
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define ADC_CHANNEL_COUNT             3
#define ADC_COUNT_OF_SAMPLES          64 /* count of samples get */
#define DMA_ADC_BUFFER_SIZE           (ADC_CHANNEL_COUNT*ADC_COUNT_OF_SAMPLES)

__IO uint16_t dma_adc_raw[DMA_ADC_BUFFER_SIZE];
uint32_t adc_average[ADC_CHANNEL_COUNT];

extern "C" void DMA2_Stream0_IRQHandler(void)
{
  uint32_t reg0;
  reg0 = DMA2->LISR;

  if (reg0 & DMA_LISR_TCIF0)
    ;
  else
  {
    while (1)
      __nop();
  }

  DMA2->LIFCR = reg0;
}


void AverageData()
{
  for (int ch_num = 0; ch_num < ADC_CHANNEL_COUNT; ch_num++)
  {
    adc_average[ch_num] = 0;

    for (int shoot_cnt = 0; shoot_cnt < ADC_COUNT_OF_SAMPLES; shoot_cnt++)
    {
      adc_average[ch_num] += dma_adc_raw[shoot_cnt * ADC_CHANNEL_COUNT + ch_num];
    }

    adc_average[ch_num] /= ADC_COUNT_OF_SAMPLES;
  }
}


static void ADCInit()
{
#define ADC_SAMPLE_TIME   ADC_SampleTime_480Cycles
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
  NVIC_InitTypeDef      nvconf;
  /* Enable ADCx, DMA and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&dma_adc_raw;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = DMA_ADC_BUFFER_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
  DMA_Cmd(DMA2_Stream0, ENABLE);
  /* interrupt config */
  nvconf.NVIC_IRQChannel = DMA2_Stream0_IRQn ;
  nvconf.NVIC_IRQChannelPreemptionPriority = 0;
  nvconf.NVIC_IRQChannelSubPriority = 0;
  nvconf.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvconf);
  /* Configure ADC1 Channel7 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; /* 10.5 Mhz */
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 3;
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channel7 configuration **************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SAMPLE_TIME);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SAMPLE_TIME);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 3, ADC_SAMPLE_TIME);
  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
//  ADC_ContinuousModeCmd(ADC1, ENABLE);
//  ADC_SoftwareStartConv(ADC1);
  ADC1->CR2 |= ADC_CR2_SWSTART;
}

void HwInit_Adc()
{
  ADCInit();
}



