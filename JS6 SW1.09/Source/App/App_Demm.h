#ifndef __APP_DEMM_H
#define __APP_DEMM_H

#include "fm33lg0xx_fl.h"
#define DTC_CONDITION_PERIOD_TIME    10
#define DTC_CONDITION_TIMER_3S       (3*1000/DTC_CONDITION_PERIOD_TIME)
void App_Demm_Task(void);
void App_Demm_Init(void);
uint8_t u8GetMonitorDtcCondition(void);
#endif