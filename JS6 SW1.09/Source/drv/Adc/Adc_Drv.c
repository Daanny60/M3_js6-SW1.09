#include <string.h>
#include "Adc_Drv.h"
#include "fm33lg0xx_fl_conf.h"
#include "DMA.h"




#define ADC_TASK_TIMEOUT  100
#define ADC_TASK_PERIOD_TIME  2



ADC_Drv_t ADC_Drv = {0};

void ADC_Init(void)
{

  FL_ADC_CommonInitTypeDef    ADC_CommonInitStruct;
  FL_ADC_InitTypeDef          ADC_InitStruct ;

  /* ADC 时钟设置 */
  ADC_CommonInitStruct.clockSource            = FL_CMU_ADC_CLK_SOURCE_RCHF;
  ADC_CommonInitStruct.clockPrescaler         = FL_ADC_CLK_PSC_DIV8;
  ADC_CommonInitStruct.referenceSource        = FL_ADC_REF_SOURCE_VDDA;
  ADC_CommonInitStruct.bitWidth               = FL_ADC_BIT_WIDTH_12B;
  FL_ADC_CommonInit(&ADC_CommonInitStruct);	

  /* ADC 寄存器设置 */
  ADC_InitStruct.conversionMode                   = FL_ADC_CONV_MODE_SINGLE; /* 连续模式 */
  ADC_InitStruct.autoMode                         = FL_ADC_SINGLE_CONV_MODE_AUTO;/* 自动 */
  ADC_InitStruct.waitMode                         = FL_DISABLE;                   /* 等待 */       ////FL_ENABLE 芯片厂家要求改成 FL_DISABLE
  ADC_InitStruct.overrunMode                      = FL_ENABLE;                   /* 覆盖上次数据 */
  ADC_InitStruct.scanDirection                    = FL_ADC_SEQ_SCAN_DIR_FORWARD; /* 通道正序扫描 */
  ADC_InitStruct.externalTrigConv                 = FL_ADC_TRIGGER_EDGE_NONE;    /* 禁止触发信号 */
  ADC_InitStruct.triggerSource                    = FL_ADC_TRGI_LUT0;
  ADC_InitStruct.fastChannelTime                  = FL_ADC_FAST_CH_SAMPLING_TIME_2_ADCCLK;  /* 快速通道采样时间 */
  ADC_InitStruct.lowChannelTime                   = FL_ADC_SLOW_CH_SAMPLING_TIME_192_ADCCLK;/* 慢速通道采样时间 */
  ADC_InitStruct.oversamplingMode                 = FL_ENABLE;                   /* 过采样打开 */
  ADC_InitStruct.overSampingMultiplier            = FL_ADC_OVERSAMPLING_MUL_8X;  /* 8倍过采样 */
  ADC_InitStruct.oversamplingShift                = FL_ADC_OVERSAMPLING_SHIFT_3B;/* 数据右移, /8 */
  FL_ADC_Init(ADC, &ADC_InitStruct);

  FL_ADC_EnableDMAReq(ADC);
  memset(&ADC_Drv, 0, sizeof(ADC_Drv));
}



void ADC_Start(void)
{
    FL_VREF_EnableVREFBuffer(VREF);                             /* 使能VREF BUFFER */ 
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH6);    /* 通道选择ADC_6 */
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH11);    /* 通道选择ADC_11 */
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH13);    /* 通道选择ADC_13 */

    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);/* 通道选择VREF */
  
    FL_ADC_ClearFlag_EndOfConversion(ADC);                      /* 清标志 */
    FL_ADC_Enable(ADC);                                         /* 启动ADC */
    FL_ADC_EnableSWConversion(ADC);                             /* 开始转换 */ 
}


void ADC_Task(void)
{

  uint8_t i;
  uint8_t j;
  adc_result_type adc_result_sum = 0; 

  if(FL_DMA_IsActiveFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4))
  {
    FL_DMA_ClearFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4);
    FL_ADC_ClearFlag_EndOfConversion(ADC);                      /* 清标志 */

    FL_ADC_Disable(ADC);                                        
     
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH13); 
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH11);
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH6);
 
    

    memcpy(ADC_Drv.Result, ADC_Drv.Buf, sizeof(ADC_Drv.Buf));

    if(ADC_Drv.FilterCnt < ADC_DRV_FILTER_SIZE)
    {
      for(i = 0; i < NUM_OF_ADC_CH; i ++)
      {
        ADC_Drv.ResultSample[i][ADC_Drv.FilterCnt] = ADC_Drv.Result[i];
      }
      ADC_Drv.FilterCnt ++;
    }
    else
    {
      for(i = 0; i < NUM_OF_ADC_CH; i ++)
      {
        for(j = 0; j < (ADC_DRV_FILTER_SIZE - 1); j ++)
        {
          ADC_Drv.ResultSample[i][j] = ADC_Drv.ResultSample[i][j + 1];
        }
      }   
      for(i = 0; i < NUM_OF_ADC_CH; i ++)
      {
        ADC_Drv.ResultSample[i][ADC_DRV_FILTER_SIZE - 1] = ADC_Drv.Result[i];
      }   
    }

    for(i = 0; i < NUM_OF_ADC_CH; i ++)
    {
      adc_result_sum = 0;
      for(j = 0; j < ADC_Drv.FilterCnt; j ++)
      {
        adc_result_sum += ADC_Drv.ResultSample[i][j];
      }
      ADC_Drv.ResultFilter[i] = adc_result_sum / ADC_Drv.FilterCnt;
    }


    ADC_Drv.Timeout = 0;
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH6);    /* 通道选择ADC_6 */
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH11);    /* 通道选择ADC_11 */
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH13);    /* 通道选择ADC_13 */

    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);/* 通道选择VREF */ 
    FL_ADC_Enable(ADC); 
    FL_ADC_EnableSWConversion(ADC);                             /* 开始转换 */ 
  }

  ADC_Drv.Timeout ++;
  if(ADC_Drv.Timeout >= ADC_TASK_TIMEOUT / ADC_TASK_PERIOD_TIME)
  {
    FL_ADC_Disable(ADC);
    ADC_Drv.Timeout = 0;
    memset(&ADC_Drv, 0, sizeof(ADC_Drv));
    (void)ADC_Init();
    (void)DMA_Init();
    (void)ADC_Start();

  }
}


adc_result_type ADC_Result_Get(uint8_t index)
{
  return (ADC_Drv.Result[index]);
}

adc_result_mv_type ADC_ResultMv_Get(uint8_t index)
{
  adc_result_mv_type adc_result_mv;
  uint32_t adc_vref1p2;

  adc_vref1p2 = ADC_Drv.Result[ADC_VREF1P2_RESULT_INDEX];
  adc_result_mv = ((uint64_t)ADC_Drv.Result[index] * 3000ul * (ADC_VREF)) / ((uint32_t)adc_vref1p2 * 4095);

  return (adc_result_mv);
}



adc_result_mv_type ADC_ResulFiltertMv_Get(uint8_t index)
{
  adc_result_mv_type adc_result_mv;
  uint32_t adc_vref1p2;

  adc_vref1p2 = ADC_Drv.ResultFilter[ADC_VREF1P2_RESULT_INDEX];
  adc_result_mv = ((uint64_t)ADC_Drv.ResultFilter[index] * 3000ul * (ADC_VREF)) / ((uint32_t)adc_vref1p2 * 4095);

  return (adc_result_mv);
}