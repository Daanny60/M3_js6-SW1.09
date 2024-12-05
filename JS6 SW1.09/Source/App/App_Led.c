#include <string.h>
#include "App_Led.h"
#include "fm33lg0xx_fl.h"
///////////#include "App_Mfs.h"
#include "GPIO.h"
#include "system_voltage_manage.h"
#include "Event.h"
#include "pwm.h"
#include "App_Can.h"
#include "App_rgb.h"
AppLedTypeDef AppLed;
static uint8_t u8bright;
static uint8_t u8day;
/* light table */
///const uint16_t u16lighttable[12]=        {0,255,405, 560, 715, 840,970,1275,1580,1885,2220,2550};
const uint16_t u16lighttable[12]=    {0,300,480, 660, 840, 990,1140,1500,1860,2220,2610,3000};
///const uint16_t u16LightTimeTable[12]={0,32,17, 17, 17, 14, 14, 35, 35, 35, 38, 38};
/**/
void AppLed_Init(void)
{
    memset(&AppLed, 0, sizeof(AppLed));
    u8bright=0;
     u8day=0;
}
/* get state*/
uint8_t AppLedState_Get(void)
{
    return (AppLed.State);
}
uint8_t u8ReadDayNight(void)
    {
      return u8day;
    }
/*save state and duty*/
void  vAppLedSaveState(void)
{
    AppLed.StateOld=AppLed.State;
    AppLed.u16PwmDutyOld=AppLed.u16PwmDuty;
}
/*set state*/
void vSetBackLightState(AppLedState_e appstate)
{
    vAppLedSaveState();
    AppLed.State=appstate;
}
/* return normal */
void  vAppLedExitState(void)
{
    vSetBackLightState(APP_LED_STATE_NORMAL);
    AppLed.u16PwmDuty=0x0;
}
/* light  1:1-11, 2:12-22,   0:others*/
uint8_t u8CheckBackLightResult(void)
{
    uint8_t  result=BACKLIGHT_NOT_OK;
      u8bright=u8Readlight();
    if((u8bright>0)&&(u8bright<12))
    {
        result=BACKLIGHT_LOWLIGHT;
        u8day=APP_MODE_NIGHT;
    }
    else  if((u8bright>11)&&(u8bright<23))
    {
        result=BACKLIGHT_HIGH_LIGHT;
         u8day=APP_MODE_DAY;
    }
    else
    {
        result=BACKLIGHT_NOT_OK;
    }
    return result;
}
/* set duty*/
void  vSetBackLight(uint16_t duty)
{   
    AppLed.u16PwmDuty=u16lighttable[duty];
    ////////vBackLightPwmDuty(duty);
}

/* refesh pwm duty*/
void vRefreshPwmDuty(void)
    {               
       if(AppLed.u16PwmDuty>AppLed.u16PwmDutyNow)   /////////渐变点亮
          {   
            AppLed.u16PwmDutyNow+=30;
            vBackLightPwmDuty(AppLed.u16PwmDutyNow);               
          } 
    else if(AppLed.u16PwmDuty<AppLed.u16PwmDutyNow)   //////渐变熄灭
          {           
               if(AppLed.u16PwmDutyNow>29)  AppLed.u16PwmDutyNow-=30;
               else                         AppLed.u16PwmDutyNow=0;
               vBackLightPwmDuty(AppLed.u16PwmDutyNow);              
          }
    else 
        { 
         vBackLightPwmDuty(AppLed.u16PwmDutyNow); 
        }
    }
/**/
void  vStateInOta(void)
{   
    AppLed.u16MInuteTime=APP_LED_TIME_3MIN;
    vSetBackLightState(APP_LED_STATE_OTA);
     vSetBackLight(0);
}
/*  dec check if to 0*/
void vMinuteTimeDecCheck(void)
     {
       if(AppLed.u16MInuteTime)   AppLed.u16MInuteTime--;
     }
/* set  3minute*/
void vSetMinuteTime(void)
     {
         AppLed.u16MInuteTime=APP_LED_TIME_3MIN;
     }

/**/
void AppLed_Task(void)
{
    uint8_t system_voltage_mod;
    uint8_t u8backlight;   
    system_voltage_mod = system_voltage_mode_get();
    u8backlight=u8CheckBackLightResult();
   vMinuteTimeDecCheck();
    if(SYSTEM_VOLTAGE_MODE_NORMAL != system_voltage_mod)
    {       
        vSetBackLightState(APP_LED_STATE_OFF);
    }

    switch (AppLed.State)
    {
    case APP_LED_STATE_OFF:
        if(SYSTEM_VOLTAGE_MODE_NORMAL==system_voltage_mod)
        {
            vAppLedExitState();
        }
        else
        {
            vSetBackLight(0);
        }
        break;
    case APP_LED_STATE_NORMAL:
        if(u8ReadOtaReq())
        {
            vStateInOta();
        }
        else if(BACKLIGHT_HIGH_LIGHT==u8backlight)
        {
            vSetBackLight(0);
        }
        else if(BACKLIGHT_LOWLIGHT==u8backlight)
        {
          if(u8bright>12)  u8bright=12;
            vSetBackLight(u8bright);
        }
        else if(BACKLIGHT_NOT_OK==u8backlight)
        {          
            vSetBackLightState(APP_LED_STATE_FAIL);
        }
        else
            {}
        break;
    case APP_LED_STATE_FAIL:
        if(u8ReadOtaReq())
        {
            vStateInOta();
        }
        else if((BACKLIGHT_HIGH_LIGHT==u8backlight)||(BACKLIGHT_LOWLIGHT==u8backlight))
        {
            
            vSetBackLightState(APP_LED_STATE_NORMAL);
        }
        else
        {
            if(GEAR_ON!=u8ReadPowGear())
            {                
                AppLed.u16SecondTime=APP_LED_TIME_2S;
                vSetBackLightState(APP_LED_STATE_DELAY);
            }
        }
        break;
    case APP_LED_STATE_DELAY:
        if(u8ReadOtaReq())
        {          
            vStateInOta();
        }
        else if((BACKLIGHT_HIGH_LIGHT==u8backlight)||(BACKLIGHT_LOWLIGHT==u8backlight))
        {           
            vSetBackLightState(APP_LED_STATE_NORMAL);
        }
        else
        {
            if(AppLed.u16SecondTime)
            {
                AppLed.u16SecondTime--;
            }
            if(0==AppLed.u16SecondTime)
            {
                vSetBackLight(0);
            }
        }
        break;
    case APP_LED_STATE_OTA:
        if(0==u8ReadOtaReq())
        {           
            AppLed.u16SecondTime=APP_LED_TIME_3S;
            vSetBackLightState(APP_LED_STATE_OTA_DLY);
        }
        else
        {
            if(0==AppLed.u16MInuteTime)
            {               
                AppLed.State=AppLed.StateOld;
                AppLed.u16PwmDuty=AppLed.u16PwmDutyOld;
               vResetOtaReq();
            }
        }
        break;
    case      APP_LED_STATE_OTA_DLY:
        if(AppLed.u16SecondTime)
        {
            AppLed.u16SecondTime--;
        }
        if(0==AppLed.u16SecondTime)
        {
            vSetBackLightState(APP_LED_STATE_NORMAL);
        }
        break;
    default:
        AppLed_Init();
        break;
    }
    vRefreshPwmDuty();
}
