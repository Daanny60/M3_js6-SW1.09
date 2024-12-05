#include "App_Encoder.h"
#include "system_voltage_manage.h"
#include "EncoderDrv.h"
#include "com.h"
#include "Com_Cfg.h"
#include "App_Keyboard.h"
#include "keyboard_config.h"
#include "App_Can.h"
#define ENCODER_MAX_ROLL_ONE_CYCLE 7


typedef struct {
  unsigned char Up;
  unsigned char Down;
}EncoderRollType;

struct  _tagAppEncoderTx    stEncoderTx;

EncoderRollType RiEncoderRoll;
EncoderRollType LeEncoderRoll;
EncoderRollType MiEncoderRoll;
/*  */
void App_EncoderRollClear(void)
{
  RiEncoderRoll.Up   = 0;
  RiEncoderRoll.Down = 0;
  LeEncoderRoll.Down = 0;
  LeEncoderRoll.Up   = 0;
  MiEncoderRoll.Down = 0;
  MiEncoderRoll.Up   = 0;
}
/* set data to buf */
void vSetencoderTxBuf(uint8_t index,uint8_t value,uint8_t u8txid)
     {     
            stEncoderTx.u8Ecbuf[index]=value;
            stEncoderTx.u8EcTxId[index]=u8txid;
      }

/* set encoder*/
void vSetEncoder(uint8_t txid,uint8_t value)
     {  
        if(BCMPOWERGEAR_ON!=u8ReadPowGear())  return;
        if(stEncoderTx.u8EcWptr!=stEncoderTx.u8EcRptr)     return;                                       ////not equal means in tx
        else                                               stEncoderTx.u8EcTimerCnt=0;                   ////equal cycletx.not eventtx立刻发送
        stEncoderTx.u8EcWptr=0;
        stEncoderTx.u8EcRptr=0;       
        vSetencoderTxBuf(stEncoderTx.u8EcWptr,value,txid);
        stEncoderTx.u8EcWptr++;        
        vSetencoderTxBuf(stEncoderTx.u8EcWptr,0,txid);
        stEncoderTx.u8EcWptr++;    
     }
/* tx 0x429 frame */
 void   vCheckCanTxEncoderFun(void)
        {
            uint8_t  u8txvalue;
            uint8_t  u8txid;
            if(stEncoderTx.u8EcTimerCnt)   stEncoderTx.u8EcTimerCnt--;
            if((0==stEncoderTx.u8EcTimerCnt)&&(stEncoderTx.u8EcRptr!=stEncoderTx.u8EcWptr))    
              {
               stEncoderTx.u8EcTimerCnt=stEncoderTx.u8EcValue;  
               stEncoderTx.u8EcRptr%=NUM_ENCODER_BUF;
               u8txvalue= stEncoderTx.u8Ecbuf[stEncoderTx.u8EcRptr];
               u8txid=    stEncoderTx.u8EcTxId[stEncoderTx.u8EcRptr];
               stEncoderTx.u8EcRptr++;                
               Com_SendSignal(u8txid,&u8txvalue);
              }
            else   if((0==stEncoderTx.u8EcTimerCnt)&&(stEncoderTx.u8EcRptr==stEncoderTx.u8EcWptr)) 
             {
               stEncoderTx.u8EcRptr=0;
               stEncoderTx.u8EcWptr=0; 
               stEncoderTx.u8EcTimerCnt=APP_ENCODER_1000MS;  
               stEncoderTx.u8EcValue =APP_ENCODER_100MS;
               u8txvalue=0;
               Com_SendSignal(COM_TXSIGSWS_HeatOrNot,&u8txvalue);
             }
        } 
/* */
void App_EncoderInit(void)
 {
   memset(&stEncoderTx,0,sizeof(stEncoderTx));
    stEncoderTx.u8EcTimerCnt=APP_ENCODER_1000MS;
    stEncoderTx.u8EcValue =APP_ENCODER_100MS;
 }
/**/
void App_EncoderTask(void)
{
  uint8_t LeEncoderEvent;
  uint8_t RitEncoderEvent;
  uint8_t MiEncoderEvent;
  uint8_t SystemVoltageMod;  

  LeEncoderEvent   = LeftEncoderDrvEventGet();
  RitEncoderEvent  = RightEncoderDrvEventGet();
  MiEncoderEvent   = MiddleEncoderDrvEventGet();
  SystemVoltageMod = system_voltage_mode_get();

  if(SYSTEM_VOLTAGE_MODE_NORMAL == SystemVoltageMod)
  {
    ////////////////////////////////////////////////////////////////////////////
    if(ENCODER_EVENT_ROLL_UP == LeEncoderEvent)
    {
       LeEncoderRoll.Up = 0;
      LeEncoderRoll.Down=2;     
      vSetEncoder(COM_TXSIGSWS_LEFTMODEENCODER, LeEncoderRoll.Down);     
    }
    else if(ENCODER_EVENT_ROLL_DOWN == LeEncoderEvent)
    {
       LeEncoderRoll.Down = 0;
      LeEncoderRoll.Up=1;    
      vSetEncoder(COM_TXSIGSWS_LEFTMODEENCODER, LeEncoderRoll.Up);       
    }
    else
    {    }
/////////////////////////////////////////////////////////////////////////////////
    if(ENCODER_EVENT_ROLL_UP == RitEncoderEvent)
    {
         
      RiEncoderRoll.Down = 0;    
      RiEncoderRoll.Up =1;    
      vSetEncoder(COM_TXSIGSWS_RIGHTMODEENCODER, RiEncoderRoll.Up);
     
    }
    else if(ENCODER_EVENT_ROLL_DOWN == RitEncoderEvent)
    {
    
      RiEncoderRoll.Up = 0;     
      RiEncoderRoll.Down =2;     
      vSetEncoder(COM_TXSIGSWS_RIGHTMODEENCODER, RiEncoderRoll.Down);
     
    }
    else
    {    }
/////////////////////////////////////////////////////////////////////////////////
    if(ENCODER_EVENT_ROLL_UP == MiEncoderEvent)
    {
       MiEncoderRoll.Up = 0;     
       MiEncoderRoll.Down =4;     
       if((0==u16ReadSilence()) && (BCMPOWERGEAR_ON==u8ReadPowGear()) ) Com_SendSignal(COM_TXSIGSWS_MEDIAKEY, &MiEncoderRoll.Down);    
    }
    else if(ENCODER_EVENT_ROLL_DOWN == MiEncoderEvent)
    {
      MiEncoderRoll.Down = 0;    
      MiEncoderRoll.Up =1;     
      if( (0==u16ReadSilence()) && (BCMPOWERGEAR_ON==u8ReadPowGear()) ) Com_SendSignal(COM_TXSIGSWS_MEDIAKEY, &MiEncoderRoll.Up);     
    }
    else
    { }   
    
  }
  else
  {

  }
  vCheckCanTxEncoderFun();
}



