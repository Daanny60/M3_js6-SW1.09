#include "Tim.h"
#include "fm33lg0xx_fl_conf.h"

SystemTimeType SystemTime;


void BSTIM16_Init(void)
{
  FL_BSTIM16_InitTypeDef    TimerBase_InitStruct;

  TimerBase_InitStruct.prescaler = 0;
  TimerBase_InitStruct.autoReload = 48000;
  TimerBase_InitStruct.autoReloadState = FL_ENABLE;
  TimerBase_InitStruct.clockSource = FL_CMU_BSTIM16_CLK_SOURCE_APBCLK;

  FL_BSTIM16_Init(BSTIM16, &TimerBase_InitStruct);


  FL_BSTIM16_ClearFlag_Update(BSTIM16);    /* 清除计数器中断标志位 */
  FL_BSTIM16_EnableIT_Update(BSTIM16);     /* 开启计数器中断 */


  NVIC_DisableIRQ(BSTIM_IRQn);
  NVIC_SetPriority(BSTIM_IRQn, 2);        /* 中断优先级配置 */
  NVIC_EnableIRQ(BSTIM_IRQn);


  FL_BSTIM16_Enable(BSTIM16);         /* 使能定时器 */

}


static inline void SystemTime_Tick(void)
{
  SystemTime ++;
}


SystemTimeType Get_SystemTime(void)
{
  SystemTimeType system_time;
  uint32_t primask;

  primask = __get_PRIMASK();
  __disable_irq();
  system_time = SystemTime;
  __set_PRIMASK(primask);

  return (system_time);
}

/**
  * @brief  BSTIM中断服务函数
  * @param  void
  * @retval void
  */

void BSTIM_IRQHandler(void)
{
  if(FL_BSTIM16_IsEnabledIT_Update(BSTIM16) && FL_BSTIM16_IsActiveFlag_Update(BSTIM16))
  {
      FL_BSTIM16_ClearFlag_Update(BSTIM16);
      SystemTime_Tick();
      /* 用户代码，处理更新事件 */
      /* ... */
  }
}
