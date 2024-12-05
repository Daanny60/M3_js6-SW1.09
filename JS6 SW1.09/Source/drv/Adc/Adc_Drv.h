#ifndef __ADC_DRV_H
#define __ADC_DRV_H


#include "types.h"
#define NUM_OF_ADC_CH 4


#define  ADC_POWER_12V_RESULT_INDEX  1          //ADIN11

#define  ADC_WHL_HEAT_IS_INDEX       2          //ADIN13

#define  ADC_WHL_HEAT_NTC_INDEX      0          //ADIN6

#define  ADC_VREF1P2_RESULT_INDEX    3          //ADVREF

#define ADC_DRV_FILTER_SIZE 5


typedef uint16_t adc_result_type;
typedef uint32_t adc_result_mv_type;

typedef struct 
{
  adc_result_type Result[NUM_OF_ADC_CH];
  adc_result_type ResultSample[NUM_OF_ADC_CH][ADC_DRV_FILTER_SIZE];
  adc_result_type ResultFilter[NUM_OF_ADC_CH];
  adc_result_type Buf[NUM_OF_ADC_CH];
  uint8_t FilterCnt;
  uint8_t Timeout;
}ADC_Drv_t;

extern ADC_Drv_t ADC_Drv;

void ADC_Init(void);
void ADC_Start(void);
void ADC_Task(void);
adc_result_type ADC_Result_Get(uint8_t index);
adc_result_mv_type ADC_ResultMv_Get(uint8_t index);
adc_result_mv_type ADC_ResulFiltertMv_Get(uint8_t index);


#endif