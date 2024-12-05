
#include "App_Can.h"
#include <stdint.h>
#include "Com_Cfg.h"
#include "Com.h"
///#include "App_Mfs.h"
#include <string.h>
#include "App_Encoder.h"
#include "Event.h"
#include "App_Led.h"
#include "LowPowerManage.h"
#include "system_voltage_manage.h"
#include "CanTrcv.h"
#include "OsekNm.h"
#include "App_demm.h"
#include "eeprom_cfg.h"

//#define HCAN_BUSOFF_MAX_CNT  255UL
#define HCAN_BUSOFF_DTC_CNT  6UL

spbusoffdef   spbusoff;

static uint8_t u8BusOffFlag;
static uint16_t u16Frame2ABTimeoutCntr;
static uint16_t u16Frame2ABRecovCntr;
static uint8_t u8Frame2ABTimeoutFlag;



static uint8_t u8SystemVoltFaultFlag;


uint8_t  u8HeatCommand[5];//远程加热命令
uint8_t  u8RemoteHeatSwitchStatus;//远程加热开关状态
uint8_t u8KeyState;//钥匙状态
volatile uint8_t u8Frame560TimeoutCntr;
volatile uint8_t u8Frame560RecvCntr;
uint8_t u8HeatEnableCondition;//加热使能条件

uint8_t u8EngineState;//发动机状态
uint8_t u8HvtSts;//高压上电状态
uint16_t u16VehicleSpeed;//车速

uint16_t u16BusOffRecoverCntr;

//0x2CD 按键的信号的前后状态定义
static uint8_t u8PreviousSteerWhllHeatSW;
static uint8_t u8CurrentSteerWhllHeatSW ;
// 标志位用于检测从0到1的跳变
static uint8_t b0To1Detected = 0;


void vAppCanInit(void)
{    
    u8Frame560TimeoutCntr = 0;
    u8Frame560RecvCntr = 0;
    u8RemoteHeatSwitchStatus = 0;
    //u8HCANBusOffCntr = 0;
    u8BusOffFlag = 0;
    u16BusOffRecoverCntr = 0;
    u16Frame2ABTimeoutCntr = 0;
    u8Frame2ABTimeoutFlag = 0;
    /*
    Eeprom_RamRead(&u8HCANBusOffDtcCntr, DTC_APP_BUSOFF_CNTR_EE_FLASH_RAM_ADDRESS, 1);
    if(u8HCANBusOffDtcCntr > 6)
    {
        u8HCANBusOffDtcCntr = 0;
        Eeprom_RamWrite(&u8HCANBusOffDtcCntr, DTC_APP_BUSOFF_CNTR_EE_FLASH_RAM_ADDRESS, 1);
    }
    */        
    memset(&spbusoff, 0, sizeof(spbusoff));
}



void vAppCanRxTxIndication(void)
{
    //u8HCANBusOffCntr = 0;
    //u8HCANBusOffDtcCntr = 0;
}

void vAppCanBusOffIndication(void)
{
    u8BusOffFlag = 1;
}

void vCheckBusOffDTC(void)
{
    uint8_t u8BusOffDTCcondition;
    u8BusOffDTCcondition = u8GetMonitorDtcCondition();        
    if(1 == u8BusOffDTCcondition)
    {
        if(spbusoff.u8statusslowrecovery)
        {                
        
            AppDemmHarnessFractureOrShortFailureEvent(DTC_TEST_EVENT_FAILED);
        }
        else if(spbusoff.u8NMbusRecoverFlag)
        {
            AppDemmHarnessFractureOrShortFailureEvent(DTC_TEST_EVENT_PASSED);
        }
    }
}

void vCheckFrame2AB(void)
{
    uint8_t u8BusOffDTCcondition;
    u8BusOffDTCcondition = u8GetMonitorDtcCondition();   
    if(u8Frame2ABTimeoutFlag && 1 == u8BusOffDTCcondition && 0 == u8BusOffFlag)
    {
        u16Frame2ABTimeoutCntr++;
        u16Frame2ABRecovCntr = 0;
        if(u16Frame2ABTimeoutCntr > 50)//500ms
        {
            u16Frame2ABTimeoutCntr = 50;
            AppDemmCanDisconnectedFailureEvent(DTC_TEST_EVENT_FAILED);
        }
    }
    else
    {
        u16Frame2ABTimeoutCntr = 0;
        if(u16Frame2ABRecovCntr>=10)
        {
            AppDemmCanDisconnectedFailureEvent(DTC_TEST_EVENT_PASSED);
        }
    }
}
void vAppCanTask(void)
{
  uint8_t u8systemstate;
  vBusoffCheck();
  vBusoffTimeFun();
  u8systemstate = system_voltage_mode_get();
  if(SYSTEM_VOLTAGE_MODE_HIGH == u8systemstate || SYSTEM_VOLTAGE_MODE_LOW == u8systemstate)
  {
    if(u8SystemVoltFaultFlag == 0)
    {
      //CanTrcv_Disable();
      CanIf_SetControllerMode(0,CANIF_CS_STOPPED);
      CanNm_Stop(0);
      Com_TxStop();
      Com_RxStop();
      u8SystemVoltFaultFlag = 1;    
      
    }
  }
  else
  {
    if(u8SystemVoltFaultFlag == 1)
    {
      Can_Init();
      (void)CanIf_SetControllerMode(0,CANIF_CS_STARTED);
      //CanTrcv_Enable();
      Com_TxStart();
      Com_RxStart();
      u8SystemVoltFaultFlag = 0;
      CanNm_Start(0);
      //u8HCANBusOffCntr = 0;
    }
  }
  vCheckBusOffDTC();
  vCheckFrame2AB();
}
////////////////////////////////////////////////////////////////////////////////////////
void AppIpduSWS_36A_TOProcess(void)
{
  //memcpy(TxIpduSWS_4A8._c, TxIpduSWS_4A8DefualtValue._c, sizeof(TxIpduSWS_4A8));
}


void AppIpduSWS_36A_ConfProcess(void)
{

}

void AppIpdu_117_Ind_Process(void)
{
     (void)Com_ReceiveSignal(COM_RXSIG_ENGINESTATE, &u8EngineState);
     
}

void AppIpdu_193_Ind_Process(void)
{
    (void)Com_ReceiveSignal(COM_RXSIG_HVTSTS, &u8HvtSts);
}

void AppIpdu_23C_Ind_Process()
{
    (void)Com_ReceiveSignal(COM_RXSIG_VEHSPD, &u16VehicleSpeed);
}

void AppIpdu_2AB_Ind_Process(void)
{
    (void)Com_ReceiveSignal(COM_RXSIG_KEYSTATE, &u8KeyState);
    u8Frame2ABTimeoutFlag = 0;
    u16Frame2ABRecovCntr++;
    if(u16Frame2ABRecovCntr > 10)
    {
        u16Frame2ABRecovCntr = 10;
    }

}
//////////////////////////////////////////////////////////////////////////////////////////

void AppIpduADS_2CD_Ind_Process(void)
{
    uint8_t u8heatcmddata = 0;

    (void)Com_ReceiveSignal(COM_RX_SIG1, &u8heatcmddata);
    u8CurrentSteerWhllHeatSW = u8heatcmddata;
    // 检测从0到1的跳变
    if (u8PreviousSteerWhllHeatSW == 0 && u8CurrentSteerWhllHeatSW == 1)
    {
        b0To1Detected = 1;
    }
    if (u8PreviousSteerWhllHeatSW == 1 && u8CurrentSteerWhllHeatSW == 0)
    {
        if (b0To1Detected)
        {
            // 从0到1再到0的完整跳变
            if (u8RemoteHeatSwitchStatus == 0)
            {
                // 加热关闭状态下检测到跳变，认为是加热开启信号
                u8RemoteHeatSwitchStatus = 1;
            }
            else
            {
                u8RemoteHeatSwitchStatus = 0;
            }
            // 重置0到1的检测标志
            b0To1Detected = 0;
        }
    }
    // 更新上一次的信号值
    u8PreviousSteerWhllHeatSW = u8CurrentSteerWhllHeatSW;
}


///////////////////////////////////////////////////////////////////////////////////////////
void AppRxIpduADS_560_TimeOutProcess(void)
{ 
   

}

void AppRxIpdu_117_TimeOutProcess(void)
{
    u8EngineState = 0;
}

void AppRxIpdu_193_TimeOutProcess(void)
{
    u8HvtSts = 0;
}

void AppRxIpdu_2AB_TimeOutProcess(void)
{
    u8Frame2ABTimeoutFlag = 1;
    u8KeyState = 0;
}

void AppRxIpdu_23C_TimeOutProcess(void)
{
    u16VehicleSpeed = 0;
}

void AppIpduAds_560_TimeoutCheck(void)
{
    u8Frame560TimeoutCntr++;
    if(u8Frame560TimeoutCntr>10)
    {
        u8Frame560TimeoutCntr = 10;
        u8Frame560RecvCntr = 0;
       
    }
}

uint8_t u8ReadRemoteHeatSwitchStatus(void)
{
    uint8_t u8readrtn;
    u8readrtn = u8RemoteHeatSwitchStatus;
    if(u8RemoteHeatSwitchStatus !=0)
    {
        u8RemoteHeatSwitchStatus = 0;
    }
  return u8readrtn;
}

uint8_t u8ReadHeatEnableCondition(void)
{
    //如果u8EngineState== 0x1 或者 u8HvtSts == 0x1 则u8HeatEnableCondition为1否则为0
    if(u8EngineState == 0x1 || u8HvtSts == 0x1)
    {
        u8HeatEnableCondition = 1;
    }
    else
    {
        u8HeatEnableCondition = 0;
    }
    return u8HeatEnableCondition;
}

EKeyState eReadKeyState(void)
{
    EKeyState ekeystate;
    ekeystate = (EKeyState)u8KeyState;
    return ekeystate;
}

EEngineState eReadEngineState(void)
{
    EEngineState eengineState;
    eengineState = (EEngineState)u8EngineState;
    return eengineState;
}

uint8_t App_PEPS_PwrMod_Get(void)
{
    uint8_t powermod=0;
    powermod = u8KeyState;
    return powermod;
}

uint16_t App_VehicleSpeed_Get(void)
{
    uint8_t vehiclespeed=0;
    vehiclespeed = u16VehicleSpeed;
    return vehiclespeed;    
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////busoff
void   vResetBusoffErrFlag(void)
{
    spbusoff.u8NMbuserrFlag = 0;
}
void   vResetBusoffCnt(void)
{
    spbusoff.u16NMbusoffcnt = 0;
}
void    vRestBusoffFlag(void)
{
    spbusoff.u8NMbusoffFlag = 0;
}
void   vSetbusoffFlag(void)
{
    spbusoff.u8NMbusoffFlag = 1;
}
void vSetBusOk(void)
{
    spbusoff.u8statusslowrecovery = 0;
    spbusoff.u16NMbusoffcnt = 0;
    spbusoff.u8NMbusokFlag = 1;
}
/*
如果出现busoff故障，开始进行判断是快恢复还是慢恢复；

*/
void   vBusoffCheck(void)
{
    if(spbusoff.u8NMbusoffFlag)
    {
        spbusoff.u8NMbusoffFlag = 0;
        spbusoff.u16NMbusokcnt = 0;
        spbusoff.u8NMbusokFlag=0;
        spbusoff.u8NMbusRecoverFlag = 0;
        spbusoff.u8NMbuserrFlag = 1;
        spbusoff.u16NMbusoffcnt++;
        Com_TxStop();
        Com_RxStop();

        if(0 == spbusoff.u8statusslowrecovery) ////////fastrecovery
        {
            if(spbusoff.u16NMbusoffcnt < FASTRECVOVERYNUM)
            {
                spbusoff.u16nmcyclecnt = FASTREVCOVERYCYCLE;
            }
            else
            {
                spbusoff.u16NMbusoffcnt = 0;
                spbusoff.u16nmcyclecnt = SLOWRECOVERYCYLE;
                spbusoff.u8statusslowrecovery = 1;
            }
        }
        else                                /////////////slowrecovery
        {
            if((spbusoff.u16NMbusoffcnt < SLOWRECOVERYNUM))
            {
                spbusoff.u16nmcyclecnt = SLOWRECOVERYCYLE;
                
            }
            else
            {
                CanNm_BusOff(0);
                //spbusoff.u16NMbusoffcnt = 0;
                //spbusoff.u16nmcyclecnt = FASTREVCOVERYCYCLE;
                //spbusoff.u8statusslowrecovery = 0;
            }
        }
    }
    else
    {
        if(spbusoff.u8NMbusokFlag)
        {
            
            if(spbusoff.u16NMbusokcnt < RECOVERYCYLE)
            {
                spbusoff.u16NMbusokcnt++;
            }
            else 
            {
                spbusoff.u8NMbusRecoverFlag = 1;
            }
        }
    }
}
/*
开始计时判断是否时间到，如果时间到并且处于故障状态就恢复一次；
*/
void  vBusoffTimeFun(void)
{
    if(spbusoff.u16nmcyclecnt)
    {
        spbusoff.u16nmcyclecnt--;
    }

    if(0 == spbusoff.u16nmcyclecnt)
    {
        if(spbusoff.u8NMbuserrFlag)
        {
            spbusoff.u8NMbuserrFlag = 0;
            Can_Init();
            (void)CanIf_SetControllerMode(0, CANIF_CS_STARTED);
            Com_TxStart();
            Com_RxStart();
        }
        else
        {
        }
    }
    else
    {
    }
}