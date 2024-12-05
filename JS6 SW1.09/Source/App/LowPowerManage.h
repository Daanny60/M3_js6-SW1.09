#ifndef _LOW_POWER_MANAGE_H
#define _LOW_POWER_MANAGE_H






/* 网络管理10S计时*/
#define    CYCLE_NM_10S          (10000/CYCLE_BUSOFFTASK)



void    LowPowerManageTask(void);
void    LowPowerManageInit(void);
/*
void    vSetbusoffFlag(void);
void    vRestBusoffFlag(void);
void    vResetBusoffCnt(void);
void    vResetBusoffErrFlag(void);
void    vBusoffTimeFun(void);
void    vBusoffCheck(void);
void    vSetBusOk(void);
*/
void    vSetTImeOutCycle(void);

void    vWdtInit(void);
#endif