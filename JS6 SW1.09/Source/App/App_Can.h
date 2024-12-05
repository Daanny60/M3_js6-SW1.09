#ifndef __APP_CAN_H
#define __APP_CAN_H


#include "stdint.h"


#define   BCMPOWERGEAR_INVALID    0X00
#define   BCMPOWERGEAR_OFF        0X01
#define   BCMPOWERGEAR_ACC        0X02
#define   BCMPOWERGEAR_ON         0x03

/*
KeyState Enum  
0x00:OFF  0x01:ACC  0x02:IGN  0x03:CRANK+IG 
0x04:Not Used  0x05:Not Used  0x06:Not Used  0x07:Error
*/
typedef enum _EKeyState
{
    KEYSTATE_OFF = 0x00,
    KEYSTATE_ACC = 0x01,
    KEYSTATE_IGN = 0x02,
    KEYSTATE_CRANK_IG = 0x03,
    KEYSTATE_NOT_USED1 = 0x04,
    KEYSTATE_NOT_USED2 = 0x05,
    KEYSTATE_NOT_USED3 = 0x06,
    KEYSTATE_ERROR = 0x07
} EKeyState;


typedef struct
{
    uint8_t    u8NMbusoffFlag;
    uint8_t    u8NMbuserrFlag;
    uint16_t   u16NMbusoffcnt;
    uint16_t   u16NMbusokcnt;
    uint8_t    u8NMbusokFlag;
    uint8_t    u8NMbusRecoverFlag;
    uint16_t   u16nmcyclecnt;
    uint8_t    u8statusslowrecovery;
}spbusoffdef;

/*  执行周期*/
#define     CYCLE_BUSOFFTASK        10  
/* 快恢复功能执行的次数*/       
#define      FASTRECVOVERYNUM     0x6
/* 慢恢复功能执行的次数*/  
#define      SLOWRECOVERYNUM     0x05
/*快恢复执行的周期 */  
#define    FASTREVCOVERYCYCLE    (10/ CYCLE_BUSOFFTASK)
/*慢恢复执行的周期 */  
#define    SLOWRECOVERYCYLE      (1000/ CYCLE_BUSOFFTASK)
/*总线恢复周期*/
#define    RECOVERYCYLE      (5000/ CYCLE_BUSOFFTASK)

/*
EngineState Enum
0x00:No Running 0x01:Running 0x02:Crank 0x03:Fault
*/
typedef enum _EEngineState
{
    ENGINESTATE_NO_RUNNING = 0x00,
    ENGINESTATE_RUNNING = 0x01,
    ENGINESTATE_CRANK = 0x02,
    ENGINESTATE_FAULT = 0x03
} EEngineState;

void AppIpduSWS_4A8_TOProcess(void);
void AppIpduSWS_3B0_TOProcess(void);
void AppIpduSWS_429_TOProcess(void);

void AppIpduSWS_36A_ConfProcess(void);


void AppIpduADS_29C_Ind_Process(void);
void AppIpduBCM_12D_Ind_Process(void);
void AppIpduLBC_4BF_Ind_Process(void);
void AppIpduMPC_316_Ind_Process(void);
void AppIpduMED_385_Ind_Process(void);
void AppIpduFGM_49A_Ind_Process(void);
void AppIpduCSM_0A2_Ind_Process(void);
void AppIpduRBC_4E6_Ind_Process(void);



void AppIpduADS_2CD_Ind_Process(void);
void AppRxIpdu_117_TimeOutInd(void);
void AppRxIpdu_193_TimeOutInd(void);

void AppIpdu_117_Ind_Process(void);
void AppIpdu_193_Ind_Process(void);
void AppIpdu_2AB_Ind_Process(void);
void AppIpdu_23C_Ind_Process(void);


void AppRxIpdu_117_TimeOutProcess(void);
void AppRxIpdu_193_TimeOutProcess(void);
void AppRxIpdu_2AB_TimeOutProcess(void);
void AppRxIpdu_23C_TimeOutProcess(void);

void AppRxIpduADS_29C_TimeOutProcess(void);
void AppRxIpduBCM_12D_TimeOutProcess(void);
void AppRxIpduLBC_4BF_TimeOutProcess(void);
void AppRxIpduMPC_316_TimeOutProcess(void);
void AppRxIpduMED_385_TimeOutProcess(void);
void AppRxIpduFGM_49A_TimeOutProcess(void);
void AppRxIpduCSM_0A2_TimeOutProcess(void);
void AppRxIpduRBC_4E6_TimeOutProcess(void);



void vAppCanInit(void);

uint8_t u8ReadHeatCmd(void);




void AppIpduAds_560_TimeoutCheck(void);
uint8_t u8ReadRemoteHeatSwitchStatus(void);
uint8_t u8ReadHeatEnableCondition(void);
uint8_t App_PEPS_PwrMod_Get(void);
uint16_t App_VehicleSpeed_Get(void);
void vAppCanTask(void);
EKeyState eReadKeyState(void);
EEngineState eReadEngineState(void);
void vAppCanRxTxIndication(void);
void vAppCanBusOffIndication(void);

void    vSetbusoffFlag(void);
void    vRestBusoffFlag(void);
void    vResetBusoffCnt(void);
void    vResetBusoffErrFlag(void);
void    vBusoffTimeFun(void);
void    vBusoffCheck(void);
void    vSetBusOk(void);

#endif