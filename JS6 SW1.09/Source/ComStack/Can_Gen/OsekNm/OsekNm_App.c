#include <stdint.h>
#include "OsekNm.h"
#include "Com.h"
#include "Can.h"
#include "CanIf.h"

#include "CanTrcv.h"
#include "demm.h"
#include "Event.h"

/*************************************************************
 * Call back functions for OsekNm Module
 ************************************************************/
///static uint8 NmSleepFlag;/*change to static for QAC*/

static bool nm_can_sleep_flag = FALSE;
static bool nm_can_wake_up_flag = FALSE;
static bool nm_wait_bus_sleep_cancel_flag = FALSE;

/*Called only when NM started*/
void ApplNmCanStart(const nmIndexType nmChannelHandle)
{
    /*Com_IpduGroupControl(COM_IPDUGROUP1 | COM_IPDUGROUP2, COM_YES);*/
    Com_TxStart();
    Com_RxStart();
    
}
/*************************************************************/
/*Perform when enter limphome mode*/
void ApplNmCanLimpHomeStart(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    /*StopCom?*/
    
}
/*Perform when Terror TimeOut.*/
//extern uint8_t u8NM_LimpHome_Flag;
void ApplNmLimpHomeState(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    //if(u8NM_LimpHome_Flag == 0)
    {
        //u8NM_LimpHome_Flag = 1;
        //AppDemmNmLimphomeFaultEventSet(DTC_TEST_EVENT_FAILED);
    }
}
/*Perform when Exit LimpHome State*/
void ApplNmCanLimpHomeEnd(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    //AppDemmNmLimphomeFaultEventSet(DTC_TEST_EVENT_PASSED);

}
/*************************************************************/
void ApplNmBusOff(const nmIndexType nmChannelHandle)/*indicates bus off error occured*/
{
    /*Com_IpduGroupControl(COM_IPDUGROUP2, COM_YES);*/
    Com_TxStop();
    Com_RxStop();
    nm_can_sleep_flag = TRUE;
//	dem_set_test_result(DTC_CAN_BUS_OFF_ID, DTC_TEST_STATE_FAILED);
}

void ApplNmBusOffEnd(const nmIndexType nmChannelHandle)/*indicate end of bus off error */
{
    Com_TxStart();
    Com_RxStart();
    /*Com_IpduGroupControl(COM_IPDUGROUP1 | COM_IPDUGROUP2, COM_YES);*/
    
}
/*This callback function is performed when busOff recovery successed.*/
void ApplNmBusOffRecoverySuccess(const nmIndexType nmChannelHandle)
{
    
}

void ApplNmCanBusOffRecoveryFail(const nmIndexType nmChannelHandle)
{
    /*Bus Off recovery failed */
    

}
/*************************************************************/
/*This callback sets the communication hardware (CAN controller, bus transceiver)
to a state from which they can be woken up by the CAN bus.*/
void ApplNmCanSleep(const nmIndexType nmChannelHandle)
{
    Com_RxStop();
    (void)CanIf_SetControllerMode(nmChannelHandle,CANIF_CS_SLEEP);
    /*CanTrc_Ctrl(ConvToCanIfID(nmChannelHandle),SleepMode);*/
	CanTrcv_Disable();
	nm_can_sleep_flag = TRUE;
	//SetEvent(POWER_MANAGE_TASK, EVENT_NM_CAN_SLEEP);
	//PTP_PTP0 = 1;
}

void ApplNmCanWakeUp(const nmIndexType nmChannelHandle)/*indicates can bus wake up*/
{
    /*Do not need CanIf_SetControllerMode(nmChannelHandle,CANIF_CS_STARTED)
    will be called by NM when performance WAKEUP*/
    /*(void)CanIf_SetControllerMode(nmChannelHandle,CANIF_CS_STARTED);*/
    /*CanTrc_Ctrl(ConvToCanIfID(nmChannelHandle),NormalMode);*/
	CanTrcv_Init();
	CanNm_GotoMode(nmChannelHandle,GOTO_AWAKE);
	Com_TxStart();
	Com_RxStart();
	nm_can_wake_up_flag = TRUE;
	//SetEvent(POWER_MANAGE_TASK, EVENT_NM_CAN_WAKE_UP);

}
/*************************************************************/
void ApplNmWaitBusSleepCancel(const nmIndexType nmChannelHandle)
{
    /*Com_IpduGroupControl(COM_IPDUGROUP1 | COM_IPDUGROUP2, COM_YES);*/
    Com_TxStart();
	nm_wait_bus_sleep_cancel_flag = TRUE;
	//SetEvent(POWER_MANAGE_TASK, EVENT_WAIT_BUS_SLEEP_CANCEL);
}
    
void ApplNmWaitBusSleep(const nmIndexType nmChannelHandle)
{
    /*Com_IpduGroupControl(COM_IPDUGROUP2, COM_YES);*/
    Com_TxStop();
}

void ApplNmShutDown(const nmIndexType nmChannelHandle)
{
 /*diable bus HW*/
    
}

void ApplNmRingIndication(const nmIndexType nmChannelHandle,const nmNetIdType SourceID)
{
    
}

void ApplNmLimpHIndication(const nmIndexType nmChannelHandle,const nmNetIdType SourceID)
{
    
}

bool read_nm_can_sleep_flag(void)
{
	return nm_can_sleep_flag;
}

bool read_nm_can_wake_up_flag(void)
{
	return nm_can_wake_up_flag;
}
//bool clear_nm_can_wake_up_flag(void)
//{
//	nm_can_wake_up_flag = FALSE;
//}

void clear_nm_can_sleep_flag(void)
{
	nm_can_sleep_flag = FALSE;
}

bool read_nm_wait_bus_sleep_cancel_flag(void)
{
	return nm_wait_bus_sleep_cancel_flag;
}

void clear_nm_wait_bus_sleep_cancel_flag(void)
{	
	nm_wait_bus_sleep_cancel_flag = FALSE;
}
/*************************************************************/
