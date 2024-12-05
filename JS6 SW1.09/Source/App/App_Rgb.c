#include "App_Rgb.h"
#include "Event.h"
#include "App_Can.h"
#include "Demm.h"
#include "keyboard_driver.h"
#include "Pwm.h"
#include "string.h"
#include  "App_Led.h"
#include "gpio.h"

strRgb   stRgb;
    uint8_t  u8accmode;
    uint8_t  u8dnpstatus;  
    uint8_t  u8dnptakeoverreq; 
    uint16_t  u16LedTimeConfigTable[5][2]=
    {                                         
                        {   0,750}, //not activation   ---- off
                        { 750,750}, //activation       ---- on
                        { 410, 750}, //level2          ---- low  light
                        { 210, 380}, //level3          ---- high light
                        {   0, 750}  //timeout         ---- off  
    };
//////////////////////////////////////////////////////////////////
/*set  rgb duty from color */
void vAppRgbLightOn(void)
     {
       vLedRightD1On();
       vLedLeftD1On();
        if(APP_MODE_DAY==u8ReadDayNight())         {  vLedRightD2On();  vLedLeftD2On(); }
        else                                       {  vLedRightD2Off(); vLedLeftD2Off();}
     }
/* reset rgb*/
void vAppRgbLightOff(void)
     {     
       vLedRightD1Off();
       vLedLeftD1Off();
       vLedRightD2Off();
       vLedLeftD2Off();
     }
/* init struct*/
void AppRgbInit(void)
{
  memset(&stRgb,0,sizeof(stRgb));
}
uint8_t u8ReadRGBMode(void)
     {
       return stRgb.u8Mode;
     }
/*read all data */
void vReadPara(void)
  {   
   u8accmode=u8ReadADS_AccMode();
   u8dnpstatus=u8ReadADS_DnpStatus();   
   u8dnptakeoverreq=u8ReadADS_DnpTakeOverReq();   
  }
/*
 check if acc not activation
 if not equal 34567  
 else not ok
*/
uint8_t u8IfAccNotActivation(void)
        {
          uint8_t result=RETURN_NOTOK;
          if((u8accmode<ACCMODE_ACTIVECONTROL)||(u8accmode>ACCMODE_STANDSTILLWAIT)) result=RETURN_OK;
         return result;
        }
/* 
check LCC is not ok
u8dnpstauts euqal 0 1
*/
uint8_t u8IfLccNotOk(void)
        {
         uint8_t result=RETURN_NOTOK;
         if(u8dnpstatus<DNPSTATUS_LCCREADY) result=RETURN_OK;
         return result;
        }
/*
check  NOA is not ok
u8dnpstauts euqal 0 1 7 9
*/
uint8_t u8IfNoaNotOk(void)
        {
         uint8_t result=RETURN_NOTOK;
         if((u8dnpstatus<DNPSTATUS_LCCREADY)||(DNPSTATUS_FAULT==u8dnpstatus)||(DNPSTATUS_FORBIDDEN==u8dnpstatus)) result=RETURN_OK;
         return result;
        }
/*
check acc is activation
accmode euqal 34567
*/
uint8_t u8IfAccActivation(void)
        {
          uint8_t result=RETURN_NOTOK;
          if((u8accmode>ACCMODE_STANDEBY)&&(u8accmode<ACCMODE_TMPFAILURE)) result=RETURN_OK;
         return result;
        }
/* 
check lcc is activation
dnpstatua euqal 4 5 a
*/
uint8_t u8IfLccActivation_45a(void)
        {
          uint8_t result=RETURN_NOTOK;
          if((DNPSTATUS_LCCRACTIVE2==u8dnpstatus)||(DNPSTATUS_ILCAACTIVE==u8dnpstatus)||(DNPSTATUS_LCCACTIVE1==u8dnpstatus)) result=RETURN_OK;
         return result;
        }
/* 
check noa is activation 
dnpstatus ==6
*/
uint8_t u8IfNoaActivation(void)
        {
         uint8_t result=RETURN_NOTOK;
          if(DNPSTATUS_DNPACTVIE==u8dnpstatus) result=RETURN_OK;
         return result; 
        }
/* 
check lcc is activation
dnpstatua euqal 4 6 a
*/
uint8_t u8IfLccActivation_46a(void)
        {
          uint8_t result=RETURN_NOTOK;
          if((DNPSTATUS_LCCRACTIVE2==u8dnpstatus)||(DNPSTATUS_DNPACTVIE==u8dnpstatus)||(DNPSTATUS_LCCACTIVE1==u8dnpstatus)) result=RETURN_OK;
         return result;
        }
/* 
check if first take over
dnptakeover euqal 1
*/
uint8_t u8IfTakeOverLevel1(void)
        {
            uint8_t result=RETURN_NOTOK;
          if(DNPTAKEOVERREQ_WARNLEVEL1==u8dnptakeoverreq) result=RETURN_OK;
         return result; 
        }
/* 
check if second take over
dnptakeover euqal 2 3
*/
uint8_t u8IfTakeOverLevel2(void)
        {
            uint8_t result=RETURN_NOTOK;
          if(DNPTAKEOVERREQ_WARNLEVEL2==u8dnptakeoverreq) result=RETURN_OK;
         return result; 
        }
/* 
check if third take over
dnpstatus=1 & dnptakeover=5
*/
uint8_t u8IfTakeOverlevel3(void)
        {
           uint8_t result=RETURN_NOTOK;
          if((DNPTAKEOVERREQ_WARNLEVEL3==u8dnptakeoverreq) && u8IfLccActivation_46a()) result=RETURN_OK;
         return result; 
        }
/* 
check if four take over
dnpstatus=8 & dnptakeover=4
*/
uint8_t u8IfTakeOverLevel4(void)
        {
          uint8_t result=RETURN_NOTOK;
          if((DNPSTATUS_MRC==u8dnpstatus)&&(DNPSTATUS_LCCRACTIVE2==u8dnptakeoverreq)) result=RETURN_OK;
          return result; 
        }
///////////////////////////////////////////////////////////////////////////////////////////
/* function not actvation */
uint8_t u8CheckFunctionNotActvation(void)
        {
          uint8_t result=RETURN_NOTOK;
          if(u8IfAccNotActivation() && u8IfLccNotOk() && u8IfNoaNotOk()) result=RETURN_OK;
          return RETURN_OK;
        }
/*
fun actvation  
*/
uint8_t u8CheckFunActivation(void)
      {
         uint8_t result=RETURN_NOTOK;
         uint8_t result1=RETURN_NOTOK;
         uint8_t result2=RETURN_NOTOK;
            if(u8IfAccActivation()||u8IfLccActivation_45a()||u8IfNoaActivation())  result1=RETURN_OK;
            if(u8IfTakeOverLevel1()&&(u8IfLccActivation_46a()))  result2=RETURN_OK;
            if(result1||result2)    result=RETURN_OK;
            return result; 
      }
/* low light */
uint8_t u8CheckAlarmLevel2(void)
        {
            uint8_t result=RETURN_NOTOK;
            if(u8IfTakeOverLevel2()&&(u8IfLccActivation_46a()))  result=RETURN_OK;
            return result;
        }
/* high light*/
uint8_t u8CheckAlarmLevel3(void)
        {
          uint8_t result=RETURN_NOTOK;
          if(u8IfTakeOverlevel3()||u8IfTakeOverLevel4()) result=RETURN_OK;
          return result;
        }
/*
led3528 ledonoff
*/
void   vLed3528Fun(void)
       {
            stRgb.u16TimeCnt+=APP_RGB_PERIOD;
            
            stRgb.u8Mode%=APP_MODE_NUM;
            if(stRgb.u16TimeCnt<u16LedTimeConfigTable[stRgb.u8Mode][0])
              {
                vAppRgbLightOn(); 
              }
            else  if(stRgb.u16TimeCnt<u16LedTimeConfigTable[stRgb.u8Mode][1])
            {
              vAppRgbLightOff(); 
            }    
            else
            {
              stRgb.u16TimeCnt=0;              
            }
           
       }
/*rgb blacklight for time */


void AppRgbTask(void)
{  
#ifdef SWSCONFIG_HIGH
  if(0==u8ReadAdsTimeOut())
      { 
        vReadPara();
              if(u8CheckAlarmLevel3())          {stRgb.u8Mode=APP_MODE_ALARM_LEVEL3;  }
        else  if(u8CheckAlarmLevel2())          {stRgb.u8Mode=APP_MODE_ALARM_LEVEL2;  }
        else  if(u8CheckFunActivation())        {stRgb.u8Mode=APP_MODE_ACTVATION;     }
       ///// else  if(u8CheckFunctionNotActvation()) {stRgb.u8Mode=APP_MODE_NOACTVATIONG;  }
        else                                    {stRgb.u8Mode=APP_MODE_NOACTVATIONG;  }
      }
 else 
      {
         stRgb.u8Mode=APP_MODE_TIMEOUT;
      } 
   vLed3528Fun();
#endif
}