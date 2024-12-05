#ifndef __SLEEP_H
#define __SLEEP_H


#include "mf_config.h"

__STATIC_INLINE void Sleep(void)
{
    FL_PMU_SleepInitTypeDef LPM_InitStruct;
  
    //  FL_CMU_RCLF_Enable();               /* 暂开启RCLF */
    FL_RMU_PDR_Enable(RMU);                 /* 打开PDR */
    //FL_RMU_BOR_Disable(RMU);                /* 关闭BOR 2uA  */

    FL_VREF_Disable(VREF);                  /* 关闭VREF1p2 */

    LPM_InitStruct.deepSleep            = FL_PMU_SLEEP_MODE_DEEP;
    LPM_InitStruct.powerMode            = FL_PMU_POWER_MODE_SLEEP_OR_DEEPSLEEP;
    LPM_InitStruct.wakeupFrequency      = FL_PMU_RCHF_WAKEUP_FREQ_8MHZ;
    LPM_InitStruct.wakeupDelay          = FL_PMU_WAKEUP_DELAY_2US;
    LPM_InitStruct.LDOLowPowerMode      = FL_PMU_LDO_LPM_DISABLE;
    LPM_InitStruct.coreVoltageScaling   = FL_DISABLE;
    FL_PMU_Sleep_Init(PMU, &LPM_InitStruct);
    /* 使能BOR */
    FL_RMU_BOR_Enable(RMU);              
    /* 睡眠电源模式*/
    FL_PMU_SetLowPowerMode(PMU, FL_PMU_POWER_MODE_SLEEP_OR_DEEPSLEEP);
    /* 休眠 */  
    __WFI();                     
    /* 使能VREF1p2 */
    FL_VREF_Enable(VREF);           
}



#endif
