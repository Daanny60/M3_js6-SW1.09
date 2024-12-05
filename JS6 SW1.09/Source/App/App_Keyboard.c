#include "App_Keyboard.h"
#include "adc_keyboard_driver.h"
#include "keyboard_config.h"
#include "com.h"
#include "Com_Cfg.h"
#include "keyboard_io_driver.h"
#include "system_voltage_manage.h"
#include "Event.h"
#include "string.h"
#include "App_Can.h"


/////////////////////////////////////////////////////////////////////////////////////////config  start
typedef void (*KeyboardStructDtcEventFunType)(DemmDtcEvent_e event);
struct _tagAppKeyTx   stAppKeyTx;
/*
周期信号都不�?持长�?按；
事件信息都按照支持长�?按�?�理，长�?按发送的信号相同�? 
*/
const strkeyconfig  skeycfg[APP_KEYFUN_NUM]= 
{    //strucktime           long time                  long enable                 txid                               shortkey       longkey          txnum
{STUCK_TIME_20S,       LONG_PRESSED_TIME_600MS,  APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0x13,           0x14,            0x01},       /////0 SWS_KEY_RIGHT_VOICE_INDEX
{STUCK_TIME_20S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0x07,           0x08,            0x01},       /////1 SWS_KEY_RIGHT_BACK_INDEX
{STUCK_TIME_20S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0x24,           0xf7,            0x01},       /////2 SWS_KEY_RIGHT_SILENCE_INDEX
{STUCK_TIME_20S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0xf8,           0xf9,            0x01},       /////3 SWS_KEY_RIGHT_STAR_INDEX
{STUCK_TIME_20S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_CROSSCOUNTRY,         0x01,           0x02,            0x01},       /////4 SWS_KEY_LEFT_CROSSCOUNTRY_INDEX
{STUCK_TIME_20S,       LONG_PRESSED_TIME_400MS,  APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0x15,           0x15,            0x01},       /////5 SWS_KEY_LEFT_MEDIA_INDEX    tx 1ci
{STUCK_TIME_300S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_DISABLE, COM_TXSIGSWS_SPEEDSET,             0x03,           0x03,            0x01},       /////6 SWS_KEY_LEFT_SPD_ADD_INDEX
{STUCK_TIME_300S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_DISABLE, COM_TXSIGSWS_DISANCEADD,           0x01,           0x01,            0x01},       /////7 SWS_KEY_LEFT_DIS_ADD_INDEX
{STUCK_TIME_300S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_DISABLE,  COM_TXSIGSWS_DISANCEDEC,          0x01,           0x01,            0x01},       /////8 SWS_KEY_LEFT_DIS_DEC_INDEX
{STUCK_TIME_300S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_DISABLE, COM_TXSIGSWS_SPEEDSET,             0x01,           0x01,            0x01},       /////9 SWS_KEY_LEFT_SPD_DEC_INDEX
{STUCK_TIME_20S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_DRIVERINFORMATION,    0x01,           0x02,            0x01},       /////10 SWS_KEY_LEFT_INFORMATION_INDEX
#ifdef SWSCONFIG_HIGH 
{STUCK_TIME_20S,      LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0x25,           0x26,            0x01},       /////11  SWS_KEY_LEFT_STAR
#else 
{STUCK_TIME_300S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_DISABLE,  COM_TXSIGSWS_DIPILOT,             0x01,           0x01,            0x01},       /////11 SWS_KEY_LEFT_ACC_INDEX
#endif
{STUCK_TIME_20S,       LONG_PRESSED_TIME_400MS,  APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0x0c,           0x0c,            0x01},       /////12 SWS_KEY_RIGHT_PHONE_INDEX    tx1ci
{STUCK_TIME_20S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0x0a,           0x0b,            0x01},       /////13 SWS_KEY_RIHT_NEXT_INDEX
{STUCK_TIME_20S,       LONG_PRESSED_TIME_400MS,  APP_KEY_LONGPRESSED_ENABLE,  COM_TXSIGSWS_MEDIAKEY,             0x23,           0x23,            0x01},        /////14 SWS_KEY_RIGHT_MENU_INDEX
#ifdef SWSCONFIG_HIGH
{STUCK_TIME_300S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_DISABLE,  COM_TXSIGSWS_ACCLEFT,             0x01,           0x01,            0x01},       /////15 SWS_KEY_LEFT_PICK_INDEX
{STUCK_TIME_300S,       LONG_PRESSED_TIME_1S,     APP_KEY_LONGPRESSED_DISABLE,  COM_TXSIGSWS_ACCRIGHT,            0x01,           0x01,            0x01},       /////16 SWS_KEY_RIGHT_PICK_INDEX
#endif
};
uint8_t  u8KeyStaMachine[APP_KEYFUN_NUM];
/*
SWS_KEY_LEFT_CROSSCOUNTRY_INDEX
SWS_KEY_LEFT_INFORMATION_INDEX
SWS_KEY_LEFT_ACC_INDEX
*/
const uint8_t u8DidFFtable[4][8]={
   {SWS_KEY_UNUSED,                      SWS_KEY_UNUSED,                SWS_KEY_RIGHT_BACK_INDEX,        SWS_KEY_RIHT_NEXT_INDEX,     SWS_KEY_UNUSED,                     SWS_KEY_RIGHT_PHONE_INDEX,  SWS_KEY_UNUSED,          SWS_KEY_UNUSED            },
   {SWS_KEY_RIGHT_STAR_INDEX,            SWS_KEY_RIGHT_VOICE_INDEX,     SWS_KEY_UNUSED,                  SWS_KEY_UNUSED,              SWS_KEY_UNUSED,                     SWS_KEY_UNUSED,             SWS_KEY_LEFT_MEDIA_INDEX,SWS_KEY_UNUSED            },
   {SWS_KEY_UNUSED,                      SWS_KEY_RIGHT_SILENCE_INDEX,   SWS_KEY_UNUSED,                  SWS_KEY_RIGHT_MENU_INDEX,    SWS_KEY_UNUSED,                      SWS_KEY_LEFT_ACC_INDEX,    SWS_KEY_UNUSED,          SWS_KEY_LEFT_SPD_ADD_INDEX},
   {SWS_KEY_LEFT_SPD_DEC_INDEX,          SWS_KEY_UNUSED,                SWS_KEY_UNUSED,                  SWS_KEY_LEFT_DIS_DEC_INDEX,  SWS_KEY_LEFT_DIS_ADD_INDEX,         SWS_KEY_UNUSED,             SWS_KEY_UNUSED,          SWS_KEY_UNUSED            }
};
#if(DIA_KEYSTUCK==1)
const KeyboardStructDtcEventFunType vAppKeyboardStructDtcEventFun[APP_KEYFUN_NUM] = 
{
 AppDemmRightVoiceSwitchStuckFailureEventSet,
 AppDemmRightBackSwitchStuckFailureEventSet,
 AppDemmRightSilenceSwitchStuckFailureEventSet,
 AppDemmRightStarSwitchStuckFailureEventSet,
 AppDemmLeftCrossCountrySwitchStuckFailureEventSet,
 
 AppDemmLeftMediaSwitchStuckFailureEventSet,
 AppDemmLeftSpeedAddSwitchStuckFailureEventSet,
 AppDemmLeftDisanceAddSwitchStuckFailureEventSet,
 AppDemmLeftDisanceDecSwitchStuckFailureEventSet,
 AppDemmLeftSpeedDecSwitchStuckFailureEventSet,
 
 AppDemmLeftInformationSwitchStuckFailureEventSet,
 /*  used the same funptr,but  not same  evnet*/
 AppDemmLeftAccSwitchStuckFailureEventSet,
 AppDemmRightPhoneSwitchStuckFailureEventSet,        
 AppDemmRightNextSwitchStuckFailureEventSet,
 AppDemmRightMenuSwitchStuckFailureEventSet,
#ifdef  SWSCONFIG_HIGH
 AppDemmLeftPickSwitchStuckFailureEventSet,
 AppDemmRightPickSwitchStuckFailureEventSet,
#endif 
};
#endif
//////
uint8_t     u8didfourfive[4];/////did 0004 0005 
uint16_t        u16silence=0;
uint8_t         u8ModeOutFlag=0;
uint8_t         u8ModeOutCnt=0;
uint8_t         u8ModeInFlag=0;
uint8_t         u8ModeInCnt=0;
uint8_t         u8Modetxflag=0;   ////
///////////////////////////////
/*  save key status for did 22 0004/0005   */
void   vCheckDidTask(void)
       {
        uint8_t i,j=0;
        uint8_t  u8value;
        for(i=0;i<4;i++)
           {
            u8didfourfive[i]=0;
            for(j=0;j<8;j++)
               { 
                 u8value=u8DidFFtable[i][j];
                 if(u8value>APP_KEYFUN_NUM)   continue;
                 else 
                     {
                         if(APP_KEY_VALUE_PRESSED==u8ReadAdcKeyFlag(u8value))   u8didfourfive[i]|=(uint8_t)(1<<j);
                         else                                                   u8didfourfive[i]&=(uint8_t)~(1<<j);
       
                     }
               }
           }
       }
void vReadDidfour(uint8_t *hbyte,uint8_t *lbyte)
     {
        *hbyte=~u8didfourfive[0];
        *lbyte=~u8didfourfive[1];
     }
void vReadDidfive(uint8_t *hbyte,uint8_t *lbyte)
     {
        *hbyte=~u8didfourfive[2];
        *lbyte=~u8didfourfive[3];
     }
//////////////////////////////////////////////////
/* set key cycle press tx*/
void vSetKeyTxCycle(uint8_t j)
{
  uint8_t u8txvalue;
  uint8_t u8txid;
   if(BCMPOWERGEAR_ON!=u8ReadPowGear())  return;
  u8txvalue=skeycfg[j].u8KeyShortPressedTable;
  u8txid=skeycfg[j].u8KeyTxidTable;
  Com_SendSignal(u8txid,&u8txvalue);
}
/* set key sycle press long tx */
void vSetKeyTxLongCycle(uint8_t j)
{
  uint8_t u8txvalue;
  uint8_t u8txid;
   if(BCMPOWERGEAR_ON!=u8ReadPowGear())  return;
  u8txvalue=skeycfg[j].u8KeyLongtPressedTable;
  u8txid=skeycfg[j].u8KeyTxidTable;
  Com_SendSignal(u8txid,&u8txvalue);
}
/* set data to buf */
void vSetTxBuf(uint8_t index,uint8_t num,uint8_t value)
     {
       uint8_t i;
       for(i=0;i<num;i++)
         {
            stAppKeyTx.u8buf[i+index]=value;
         }
      }
/*set  key short press tx */
void vSetKeyTxShort(uint8_t j)
{
     if(BCMPOWERGEAR_ON!=u8ReadPowGear())  return;
     stAppKeyTx.u8Wptr=skeycfg[j].u8KeyTxNumTable;
     stAppKeyTx.u8rptr=0;
     if(stAppKeyTx.u8Wptr>KEYTXBUFNUM) stAppKeyTx.u8Wptr=KEYTXBUFNUM;
     vSetTxBuf(0,stAppKeyTx.u8Wptr,skeycfg[j].u8KeyShortPressedTable);
     stAppKeyTx.u8TxId =skeycfg[j].u8KeyTxidTable;
}
/*set  key long press tx */
void vSetKeyTxLong(uint8_t j)
{ 
     if(BCMPOWERGEAR_ON!=u8ReadPowGear())  return;
     stAppKeyTx.u8Wptr=skeycfg[j].u8KeyTxNumTable;
     stAppKeyTx.u8rptr=0;
     if(stAppKeyTx.u8Wptr>KEYTXBUFNUM) stAppKeyTx.u8Wptr=KEYTXBUFNUM;
     vSetTxBuf(0,stAppKeyTx.u8Wptr,skeycfg[j].u8KeyLongtPressedTable);
     stAppKeyTx.u8TxId =skeycfg[j].u8KeyTxidTable;
}
/*set  key release tx */
void vSetKeyTxRelease(uint8_t j)
{    
     if(BCMPOWERGEAR_ON!=u8ReadPowGear())  return;   
     stAppKeyTx.u8Wptr=KEY_RELEASE_TX_NUM;
     stAppKeyTx.u8rptr=0; 
     vSetTxBuf(0,stAppKeyTx.u8Wptr,KEY_RELEASE_TX_VALUE);
     stAppKeyTx.u8TxId =skeycfg[j].u8KeyTxidTable;
}
/*  SET SHORT_LONG KEY */
void vSetKey_ShortLong_Short(uint8_t j)
     {
     if(BCMPOWERGEAR_ON!=u8ReadPowGear())  return;
     stAppKeyTx.u8Wptr=skeycfg[j].u8KeyTxNumTable;
     stAppKeyTx.u8rptr=0;    
     vSetTxBuf(0,stAppKeyTx.u8Wptr,skeycfg[j].u8KeyShortPressedTable);
     vSetTxBuf(stAppKeyTx.u8Wptr,3,KEY_RELEASE_TX_VALUE);
     stAppKeyTx.u8Wptr=stAppKeyTx.u8Wptr+3;    
     stAppKeyTx.u8TxId =skeycfg[j].u8KeyTxidTable;
     }
/* set state machine*/
void  vSetKeyStatue(uint8_t i,uint8_t u8statue)
{
    u8KeyStaMachine[i]= u8statue;
}

////////////////////////////////////////////////////////////////////////////////////config end
/* app key ram  init */
void vAppKeyBoardInit(void)
{
    vAdcKeyInit();   
    memset(&u8KeyStaMachine[0],0,APP_KEYFUN_NUM);
    memset(&stAppKeyTx,0,sizeof(stAppKeyTx));
    u16silence=0;
    u8ModeOutFlag=0;
    u8ModeOutCnt=0;
    u8ModeInFlag=0;
    u8ModeInCnt=0;
    u8Modetxflag=0;
}
/* app key main task */
void vAppStateCheck(void)
{
    uint8_t i=0;
    uint8_t j=0;
    uint8_t num_of_key=0; 
    /*     */
    for(i=0; i<APP_KEYFUN_NUM; i++)
    {
        if(APP_KEY_VALUE_PRESSED==u8ReadAdcKeyFlag(i))
        {
            num_of_key++;
        }
    }
    if(u8Modetxflag)  return ;  ///////
    /* key of adc  state machine run*/
    for(j = 0; j < APP_KEYFUN_NUM; j ++)
    {
        switch(u8KeyStaMachine[j])
        {
            /////////////////////////////////key  release fun
        case APP_KEYBOARD_STATE_RELEASE:
            if(num_of_key<2)  
            {
                if(APP_KEY_VALUE_PRESSED==u8ReadAdcKeyFlag(j))
                {
                    if(APP_KEY_LONGPRESSED_DISABLE ==skeycfg[j].u8KeyLongpressedfun)  /////if normal key
                    {                       
                        vSetKeyTxCycle(j);
                        vSetKeyStatue(j,APP_KEYBOARD_STATE_PRESSED);
                    }
                    else                                               /////// if short_long key
                    {   
                       vSetKeyStatue(j,APP_KEYBOARD_STATE_PRESSED);
                    }
                }
                else  ////key release
                {
                    //// do nothing
                }
            }
            else    ///////multi key
            {
                //////do nothing
            }
            break;
////////////////////////////////////////////////////////////////////////////////////////////////key pressed fun
        case APP_KEYBOARD_STATE_PRESSED:
            if(num_of_key>1)              /////multi key
            {
                vSetKeyTxRelease(j);
                vSetKeyStatue(j,APP_KEYBOARD_STATE_MULTIKEY);
            }
            else                       ////////////less 2key
            {
                if(APP_KEY_VALUE_RELEASE == u8ReadAdcKeyFlag(j))         /////if  released
                {
                    if(APP_KEY_LONGPRESSED_DISABLE ==skeycfg[j].u8KeyLongpressedfun)  /////normal key
                    {
                        vSetKeyTxRelease(j);
                        vSetKeyStatue(j,APP_KEYBOARD_STATE_RELEASE);
                    }
                    else                                                      /////short_long key
                    {////////tx1+3
                      vSetKey_ShortLong_Short(j);
                        vSetKeyStatue(j,APP_KEYBOARD_STATE_RELEASE);
                    }
                }
                else                                                   /////if pressed
                {
                    if(APP_KEY_LONGPRESSED_DISABLE ==skeycfg[j].u8KeyLongpressedfun)          /////normal key
                    {
                        if(u32ReadAdcKeyContinue(j)>skeycfg[j].u32KeyStruckTimeTable)          ////struck
                        {
                            vSetKeyTxRelease(j);
                            vSetKeyStatue(j,APP_KEYBOARD_STATE_STUCK);
                        }
                        else    ////////continue pressed
                        {
                        vSetKeyTxCycle(j);
                        }
                    }
                    else/////short_long
                    {
                        if(u32ReadAdcKeyContinue(j) >= skeycfg[j].u32KeyLongTimeTable)
                        {
                            vSetKeyTxLong(j);
                            vSetKeyStatue(j,APP_KEYBOARD_STATE_LONG_PRESSED);
                        }
                        else
                        {
                          if((SWS_KEY_LEFT_PICK_INDEX==j)||(SWS_KEY_RIGHT_PICK_INDEX==j))    vSetKeyTxCycle(j);
                        }
                    }
                }
            }
            break;
///////////////////////////////////////////////////////////////////////////////////////////////key pressed long fun
        case APP_KEYBOARD_STATE_LONG_PRESSED:
            if(num_of_key>1)
            {
                vSetKeyTxRelease(j);
                vSetKeyStatue(j,APP_KEYBOARD_STATE_MULTIKEY);
            }
            else
            {
                if(APP_KEY_LONGPRESSED_DISABLE ==skeycfg[j].u8KeyLongpressedfun)  /////normal key
                {
                    if(APP_KEY_VALUE_RELEASE == u8ReadAdcKeyFlag(j))         /////if  released
                    {
                        vSetKeyStatue(j,APP_KEYBOARD_STATE_RELEASE);
                    }
                    else
                    {
                        vSetKeyStatue(j,APP_KEYBOARD_STATE_PRESSED);
                    }
                }
                else /////////short_long
                {
                    if(APP_KEY_VALUE_RELEASE == u8ReadAdcKeyFlag(j))         /////if  released
                    {
                        vSetKeyTxRelease(j);
                        vSetKeyStatue(j,APP_KEYBOARD_STATE_RELEASE);
                    }
                    else
                    {
                        if(u32ReadAdcKeyContinue(j) >= skeycfg[j].u32KeyStruckTimeTable)
                        {
                            vSetKeyTxRelease(j);
                            vSetKeyStatue(j,APP_KEYBOARD_STATE_STUCK);
                        }
                        else
                        {
                          if((SWS_KEY_LEFT_PICK_INDEX==j)||(SWS_KEY_RIGHT_PICK_INDEX==j))    vSetKeyTxLongCycle(j); 
                        }
                    }
                }
            }
            break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////key struck fun

        case APP_KEYBOARD_STATE_STUCK:
            if(APP_KEY_VALUE_RELEASE == u8ReadAdcKeyFlag(j))         /////if  released
            {
             vSetKeyStatue(j,APP_KEYBOARD_STATE_RELEASE);
             #if(DIA_KEYSTUCK==1)
             vAppKeyboardStructDtcEventFun[j](DTC_TEST_EVENT_PASSED);
             #endif
            }
            else
            {
              #if(DIA_KEYSTUCK==1)
              vAppKeyboardStructDtcEventFun[j](DTC_TEST_EVENT_FAILED);
              #endif
            }
            break;
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////key more fun
        case APP_KEYBOARD_STATE_MULTIKEY:
            if(APP_KEY_VALUE_RELEASE == u8ReadAdcKeyFlag(j))         /////if  released
            {
             vSetKeyStatue(j,APP_KEYBOARD_STATE_RELEASE);
            }
            break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////default fun
        default:
            vSetKeyStatue(j,APP_KEYBOARD_STATE_RELEASE);
        }
    }
}
/* CHE ZHAN IN CHECK*/
void  vModeCheZhanInCheck(void)
      {     
           if((u32ReadAdcKeyContinue(SWS_KEY_RIGHT_MENU_INDEX) >=MULTI_KEY_TIME_10S)&&(u32ReadAdcKeyContinue(SWS_KEY_RIHT_NEXT_INDEX) >=MULTI_KEY_TIME_10S))
              {       
                if(0==u8ModeInFlag)
                {
                u8ModeInFlag=1;
                stAppKeyTx.u8Wptr=3;                             
               stAppKeyTx.u8rptr=0;               
               vSetTxBuf(0,3,1); //////3 valid              
               stAppKeyTx.u8TxId =COM_TXSIGSWS_CVSMODEIN;
               u8Modetxflag=1;
                }
              }
            else if((0==u32ReadAdcKeyContinue(SWS_KEY_RIGHT_MENU_INDEX))||(0==u32ReadAdcKeyContinue(SWS_KEY_RIHT_NEXT_INDEX)))
            {
              if(u8ModeInFlag)
                 {
                  u8ModeInFlag=0;
                  stAppKeyTx.u8Wptr=3;                             
                  stAppKeyTx.u8rptr=0;               
                  vSetTxBuf(0,3,0); //////3 valid              
                 stAppKeyTx.u8TxId =COM_TXSIGSWS_CVSMODEIN;
                 u8Modetxflag=1;
                 }
            }
         

      }
/*  CHE ZHAN  OUT  CHECK*/
void  vModeCheZhanOutCheck(void)
      {      
         
            if((u32ReadAdcKeyContinue(SWS_KEY_RIGHT_MENU_INDEX) >=MULTI_KEY_TIME_10S)&&(u32ReadAdcKeyContinue(SWS_KEY_RIGHT_BACK_INDEX) >=MULTI_KEY_TIME_10S))
              {   
                if(0==u8ModeOutFlag)
                {
                  u8ModeOutFlag=1;                           
                  stAppKeyTx.u8Wptr=3;
                 stAppKeyTx.u8rptr=0;               
                 vSetTxBuf(0,3,1); //////3 valid                
                 stAppKeyTx.u8TxId =COM_TXSIGSWS_CVSMODEOUT;
                 u8Modetxflag=1;
                }
              }
            else  if(0==(u32ReadAdcKeyContinue(SWS_KEY_RIGHT_MENU_INDEX))||(0==u32ReadAdcKeyContinue(SWS_KEY_RIGHT_BACK_INDEX)))
            { 
              if(u8ModeOutFlag)
                 {
                    u8ModeOutFlag=0;                           
                  stAppKeyTx.u8Wptr=3;
                 stAppKeyTx.u8rptr=0;               
                 vSetTxBuf(0,3,0); //////3 valid                
                 stAppKeyTx.u8TxId =COM_TXSIGSWS_CVSMODEOUT;
                 u8Modetxflag=1;
                 }
            }
         

      }
/* check  signal tx*/
void   vCheckCanTxFun(void)
        {
            uint8_t  u8txvalue;
            if(stAppKeyTx.u8TimerCnt)   stAppKeyTx.u8TimerCnt--;
            if((0==stAppKeyTx.u8TimerCnt))    
              {
                if(stAppKeyTx.u8rptr<stAppKeyTx.u8Wptr)
                {
                  stAppKeyTx.u8TimerCnt=TXCYCLE100MS;              
                 u8txvalue= stAppKeyTx.u8buf[stAppKeyTx.u8rptr];
                 stAppKeyTx.u8rptr++;
                 Com_SendSignal(stAppKeyTx.u8TxId,&u8txvalue);
                }
                else 
                {
                  u8Modetxflag=0;
                }
              }
        } 
/* return chezhanmodein status*/
uint8_t u8ReadCheZhanModeIn(void)
{
  return u8ModeInCnt;
}
/* return chezhanmodeout status */
uint8_t u8ReadCheZhanModeOut(void)
{
  return u8ModeOutCnt;
}
/*check silence pressed,if pressed donot voice+/- */
void vCheckSilence(void)
     {
      if(u8ReadAdcKeyFlag(SWS_KEY_RIGHT_SILENCE_INDEX))  u16silence=SILENCE_TIME_300MS;
      if(u16silence)                                     u16silence--;
     }
/* read silence :if silence 300ms,do not tx voice+ voice-*/
uint16_t u16ReadSilence(void)
     { 
       return(u16silence);
     }
/* check if accleft and accright,if struck */
void  vCheckAccFfault(void)
     {  uint8_t u8txvalue=0x1; //////normal
        uint8_t u8speedstruck=0;
        if((u32ReadAdcKeyContinue(SWS_KEY_LEFT_SPD_ADD_INDEX)>STUCK_TIME_300S)&&(u32ReadAdcKeyContinue(SWS_KEY_LEFT_SPD_DEC_INDEX)>STUCK_TIME_300S))  u8speedstruck=0x01;
        if((u32ReadAdcKeyContinue(SWS_KEY_RIGHT_PICK_INDEX)>STUCK_TIME_300S)||(u32ReadAdcKeyContinue(SWS_KEY_LEFT_PICK_INDEX)>STUCK_TIME_300S)||(u8speedstruck))
         {
           u8txvalue=0x00;       //struck
         }
          Com_SendSignal(COM_TXSIGSWS_CRUISEFAULTSTATUS,&u8txvalue);
     }
/* key task  fun */
void AppKeyboard_Task(void)
      {        
        vAdcKeyCheck();
        vGpioKeyCheck();
        vAppStateCheck();
        vModeCheZhanInCheck();
        vModeCheZhanOutCheck();
        vCheckAccFfault();
        vCheckCanTxFun();
        vCheckSilence();       
      }



