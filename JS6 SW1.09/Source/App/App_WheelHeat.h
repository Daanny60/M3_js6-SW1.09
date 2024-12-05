#ifndef __APP_WHEEL_HEAT_H
#define __APP_WHEEL_HEAT_H
#include "fm33lg0xx_fl_conf.h"
#include "types.h"
#define WHEEL_HAET_PERIOD_TIME    10
#define WHEEL_OVERHEAT_PERIOD_TIME  1000 //1s
#define WHEEL_HEAT_TIMER_30MIN   (30*60*1000/WHEEL_HAET_PERIOD_TIME)
#define WHELL_HEAT_TIME_22MIN    (22*60*1000/WHEEL_HAET_PERIOD_TIME) 
#define WHELL_HEAT_TIME_10MIN    (10*60*1000/WHEEL_HAET_PERIOD_TIME) 
#define  WHEEL_HEAT_TIMER_20S    (20*1000/WHEEL_HAET_PERIOD_TIME) 
#define  WHEEL_HEAT_TIMER_2S     (2*1000/WHEEL_HAET_PERIOD_TIME)

#define  WHEEL_HEAT_TIMER_4S     2*WHEEL_HEAT_TIMER_2S
#define  WHEEL_HEAT_TIMER_6S     2*WHEEL_HEAT_PID_TIMER3S
#define WHEEL_HEAT_PID_TIMER3S   (3*1000/WHEEL_HAET_PERIOD_TIME)

#define WHEEL_OVERHEAT_PERIOD_1S (1*1000/WHEEL_HAET_PERIOD_TIME)
#define WHEEL_OVERHEAT_FAULT_15S (15*1000/WHEEL_OVERHEAT_PERIOD_TIME)   //danny
#define WHEEL_OVERHEAT_RECOVER_10S (10*1000/WHEEL_OVERHEAT_PERIOD_TIME) //danny

#define WHEEL_OVERHEAT_TEMP_37_5     940 //600 danny

#define WHEEL_HEAT_STATE_OFF        0
#define WHEEL_HEAT_STATE_ON         1
#define WHEEL_HEAT_STATE_OVERLOAD   2
#define OVER_LOAD_CHECK_NUM         100        ////check num
#define CURRENT_13A_VALUE           800       ///////13A保护电流对应的数值 原先值：1076  danny
#define VALUE_THRESD_OVER_LOAD      800        ////// overload map xxmv   13A*1200/14500=1.076v   10A*1200/14500=0.824V 824
//1680
//1700 - 39
//1750 - 23
//1190---35
#define ADC_WHL_HAET_ALL_ON_ADC     1190 // 1422   //   1270-35℃     ////41du -1386////////43du  1315  ///46-1212  45--1244

#define ADC_WHL_HEAT_NTC_OPEN_THRESD       3978   ////   -40 :4796mv-adc3978   NTC_OPEN;low  -40C,not make sure;
#define ADC_WHL_HEAT_NTC_SHORT_THRESD      260    ////                NTC_SHORT
#define ADC_WHL_HEAT_SHORT_THRESD          3500      /////HEAT SHORT   FULL VOLATAGE
#define ADC_WHL_HEAT_OPEN_THRESD           82      //////HEAT OPEN   ZERO  VOLATAGE   1a*1200/14500=82ma
#define   CMDHEAT_INVALID            0
#define   CMDHEAT_DISABLE_CMD        1
#define   CMDHEAT_ENABLE             2
#define   CMDHEAT_DISABLE_NTC_OPEN   3
#define   CMDHEAT_DISABLE_NTC_SHORT  4
#define   CMDHEAT_DISABLE_VOLT_MODE  5
#define   CMDHEAT_DISABLE_TIME_OUT   6
#define   CMDHEAT_DISABLE_IS_OUT     7
#define   CMDHEAT_DISABLE_GEAR       8
#define   CMDHEAT_DISABLE_CURRENT    9
#define   CMDHEAT_DISABLE_CONDITON   10

#define   CANRXHEATCMD_INVALID      0
#define   CANRXHEATCMD_DISABLE      1
#define   CANRXHEATCMD_ENABLE       2
#define   CANTXHEATCMD_NOUSED       3

#define    VEHREMOTESTART           0X01
#define    COLOR_CRIMSON            0X01

typedef struct {
  uint8_t flag_ntc_open           : 1;    //1.0 
  uint8_t flag_ntc_short          : 1;    //1.1
  uint8_t flag_heat_open          : 1;    //1.2
  uint8_t flag_heat_short         : 1;    //1.3
  uint8_t Unused0                 : 4;    //1.4-1.7 
}sflagheat_type;

typedef union {
  sflagheat_type   heatflag;
  uint8_t          flag;
}suion_flag_type;

typedef struct
{
    uint32_t  u32HeatTimer;         ////30 min timer
    
    uint32_t  u32unHeatTimer;         ////30 min timer
    
    uint16_t  u16PwmCnt;
    uint16_t  u16PwmThresd;
    uint8_t   Machine;
    uint8_t   u8Index;               ///////save point
    uint8_t   u8OnFlag;              //////save   heat on
    uint8_t   u8OnIndex;             //////save cnt
    uint16_t  u16NtcValue;           ////////ntcvalule
    uint16_t  u16ISValue;            ///////10A CHECK value
    uint16_t  u16OnValue;            ///////CHECK OPEN value
    uint16_t  u16ShortValue;         ///////CHECK short value
    uint16_t  u16NtcShortCnt;       ///////////
    uint16_t  u16NtcOpenCnt;         //////////
    uint16_t  u16HeatShortCnt;       //////////
    uint16_t  u16HeatOpenCnt;        ////////
    uint16_t  u16ISBuf[16];            //////
    uint16_t  u16NtcBuf[16];
    uint16_t  u16OnBuf[16];
    uint16_t  u16HeatSwitchCnt;
    uint8_t   u8HeatSwitchStauts;
    uint8_t   u8RemoteSwitchStauts;
    uint8_t   u8Ntcindex;
    uint8_t   u8HeatEnable;
    uint16_t  u16pidcntr;
    uint8_t   u8heatcntr;
    uint16_t   u8overheatperiodcntr;//danny
    uint16_t  u16overheatfaultcntr;
    uint16_t  u16overheatrecovercntr;
    uint8_t   u8OverloadDetected; //danny
	uint8_t   flag;
} App_WheelHeatType;
void App_wheelHeatInit(void);
void App_WheelHeat_Task(void);
uint16_t u16readntcvalue(void);
void   vAppHeatInit(void);
uint8_t u8ReadAdjustCmd(void);
void  vResetHeatTime(void);
extern void App_HeatSignal_Task(void);
extern uint8_t u8HeatGetStatus(void);
#endif
