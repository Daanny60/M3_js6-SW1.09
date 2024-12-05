#ifndef __APP_LED_H
#define __APP_LED_H

#include "fm33lg0xx_fl.h"

typedef enum 
{
  APP_LED_STATE_NORMAL = 0,
  APP_LED_STATE_FAIL,
  APP_LED_STATE_DELAY,
  APP_LED_STATE_OTA, 
  APP_LED_STATE_OTA_DLY,
  APP_LED_STATE_OFF,
  APP_LED_STATE_LOWTOHIGH,
  APP_LED_STATE_HIGHTOLOW,
}AppLedState_e;

#define APP_LED_TASK_PERIOD_TIME 30

#define APP_LED_TIME_2S      ((1000 / APP_LED_TASK_PERIOD_TIME) * 2)
#define APP_LED_TIME_3S      ((1000 / APP_LED_TASK_PERIOD_TIME) * 3)
#define APP_LED_TIME_3MIN    ((1000 / APP_LED_TASK_PERIOD_TIME) * 180)
/////////////////////////////////////////////////////////////////////////
#define  BACKLIGHT_NOT_OK       0
#define  BACKLIGHT_LOWLIGHT     1
#define  BACKLIGHT_HIGH_LIGHT   2
//////////////////////////////////////////////////////////////////////////
#define  GEAR_ON   3u
typedef struct 
{
  AppLedState_e State;
  AppLedState_e StateOld;
  uint16_t   u16PwmDuty;          ///// 目标占空比档位置
  uint16_t   u16PwmDutyOld;       ////  上一次占空比档位置
  uint16_t   u16PwmDutyNow;      ////   当前占空比数值
  uint16_t   u16SecondTime;      ////   秒延时
  uint16_t   u16MInuteTime;      ////   分钟延时
  uint16_t   u16TimeCnt;        ////    时间计数器  
}AppLedTypeDef;





uint8_t u8ReadDayNight(void);
void AppLed_Task(void);
void AppLed_Init(void);
uint8_t AppLedState_Get(void);
void vSetMinuteTime(void);
void vMinuteTimeDecCheck(void);
#endif