#include "DMA.h"
#include "Adc_Drv.h"
#include "fm33lg0xx_fl_conf.h"





void DMA_Init(void)
{
  /* ADC DMA */
  FL_DMA_InitTypeDef DMA_InitStruct = {0};
  FL_DMA_ConfigTypeDef DMA_ConfigStruct = {0};
  DMA_InitStruct.periphAddress = FL_DMA_PERIPHERAL_FUNCTION1;
  DMA_InitStruct.direction = FL_DMA_DIR_PERIPHERAL_TO_RAM;
  DMA_InitStruct.memoryAddressIncMode = FL_DMA_MEMORY_INC_MODE_INCREASE;
  DMA_InitStruct.dataSize = FL_DMA_BANDWIDTH_16B;
  DMA_InitStruct.priority = FL_DMA_PRIORITY_HIGH;
  DMA_InitStruct.circMode = FL_ENABLE;
  FL_DMA_Init(DMA, &DMA_InitStruct, FL_DMA_CHANNEL_4);

  FL_DMA_Enable(DMA);

  DMA_ConfigStruct.memoryAddress = (uint32_t)&ADC_Drv.Buf;
  DMA_ConfigStruct.transmissionCount = sizeof(ADC_Drv.Buf) /  sizeof(adc_result_type) - 1;
  FL_DMA_StartTransmission(DMA, &DMA_ConfigStruct, FL_DMA_CHANNEL_4);

}