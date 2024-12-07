#include "App_Demm.h"
#include "Demm.h"
#include "Dem_Cfg.h"
#include "Event.h"
#include "App_Can.h"
#include "system_voltage_manage.h"
#include "Adc_Drv.h"

static uint8_t u8DTCMonitorCond = 0;
static uint16_t u16IGNTimer = 0;

void ClearDTCCondition(void)
{
    u8DTCMonitorCond = 0;
    u16IGNTimer = 0;
}

#if(DIA_KEYSTUCK==1)
/* 0-4 */
static inline void AppDemmRightPhoneSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmRightPhoneSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_RIGHTPHONE_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_RIGHTPHONE_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmRightBackSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmRightBackSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_RIGHTBACK_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_RIGHTBACK_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmRightSilenceSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmRightSilenceSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_RIGHTSILENCE_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_RIGHTSILENCE_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmRightStarSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmRightStarSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_RIGHTSTAR_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_RIGHTSTAR_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmLeftCrossCountrySwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftCrossCountrySwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTCROSSCOUNTRY_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTCROSSCOUNTRY_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
/*5-9*/
static inline void AppDemmLeftMediaSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftMediaSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTMEDIA_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTMEDIA_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmLeftSpeedAddSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftSpeedAddSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTSPEEDADD_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTSPEEDADD_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmLeftDisanceAddSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftDisanceAddSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTDISTANCEADD_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTDISTANCEADD_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmLeftDisanceDecSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftDisanceDecSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTDISTANCEDEC_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTDISTANCEDEC_SWITCH_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmLeftSpeedDecSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftSpeedDecSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTSPEEDDEC_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTSPEEDDEC_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
/* 10-14 */
static inline void AppDemmLeftInformationSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftInformationSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTINFORMATION_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTINFORMATION_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmLeftAccSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftAccSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTACC_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTACC_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmRightVoiceSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmRightVoiceSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_RIGHTVOICE_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_RIGHTVOICE_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmRightNextSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmRightNextSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_RIGHTNEXT_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_RIGHTNEXT_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmRightMenuSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmRightMenuSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_RIGHTMENU_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_RIGHTMENU_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
#ifdef   SWSCONFIG_HIGH
/*15-16*/
static inline void AppDemmLeftPickSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmLeftPickSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_LEFTPICK_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_LEFTPICK_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmRightPickSwitchStuckFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmRightPickSwitchStuckFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_RIGHTPICK_STUCK_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_RIGHTPICK_STUCK_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
#endif
#ifndef   SWSCONFIG_LOW
/*
*/
static inline void AppDemmHeatShortFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmHeatShortFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_HEAT_SHORT_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_HEAT_SHORT_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}

static inline void AppDemmHeatOpenFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmHeatOpenFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_HEAT_OPEN_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_HEAT_OPEN_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}

static inline void AppDemmNtcShortFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmNtcShortFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_NTC_SHORT_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_NTC_SHORT_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}

static inline void AppDemmNtcOpenFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmNtcOpenFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_NTC_OPEN_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_NTC_OPEN_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
#endif
#else
static inline void AppDemmSystemVoltageLowFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmSystemVoltageLowFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_SYSTEMVOLTAGELOW_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result( DTC_SYSTEMVOLTAGELOW_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmSystemVoltageHighFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmSystemVoltageHighFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result( DTC_SYSTEMVOLTAGEHIGH_FAILURE_ID, DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result( DTC_SYSTEMVOLTAGEHIGH_FAILURE_ID, DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmHarnessFractureOrShortFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmHarnessFractureOrShortFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_HARNESSFRACTUREORSHORT_FAILURE_ID , DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_HARNESSFRACTUREORSHORT_FAILURE_ID , DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmCanDisconnectedFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmCanDisconnectedFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_CANDISCONNECTED_FAILURE_ID , DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_CANDISCONNECTED_FAILURE_ID , DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmHeatOpenFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmHeatOpenFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_HEATOPEN_FAILURE_ID , DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_HEATOPEN_FAILURE_ID , DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmHeatShortFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmHeatShortFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_HEATSHORT_FAILURE_ID , DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_HEATSHORT_FAILURE_ID , DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmNtcShortOrOpenFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmNtcShortOrOpenFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_NTCSHORTOROPEN_FAILURE_ID  , DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_NTCSHORTOROPEN_FAILURE_ID  , DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void AppDemmTemOver37_5FailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmTemOver37_5FailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_TEMPOVER37_5_FAILURE_ID  , DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_TEMPOVER37_5_FAILURE_ID  , DTC_TEST_STATE_PASSED);
    }
    else
    {

    }
}
static inline void  AppDemmCanDisconnected_EMSFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmCanDisconnected_EMSFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_CANDISCONNECTED_EMS_FAILURE_ID  , DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_CANDISCONNECTED_EMS_FAILURE_ID  , DTC_TEST_STATE_PASSED);
    }
    else
    {

    }

}
static inline void  AppDemmTemPinISCurrentFailureProcess(void)
{
    DemmDtcEvent_e  event;
    event = AppDemmTemPinISCurrentFailureEventGet();
    if (event == DTC_TEST_EVENT_FAILED)
    {
        dem_set_test_result(DTC_PinISCurrent_FAILURE_ID  , DTC_TEST_STATE_FAILED);
    }
    else if (event == DTC_TEST_EVENT_PASSED)
    {
        dem_set_test_result(DTC_PinISCurrent_FAILURE_ID  , DTC_TEST_STATE_PASSED);
    }
    else
    {

    }

}


#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (*AppDemmProcessFunType) (void);
/*DTC 处理函数表*/
const AppDemmProcessFunType AppDemmProcessFun[TOTAL_DTC_NUM] = 
{
#if(0)
   AppDemmRightVoiceSwitchStuckFailureProcess,
   AppDemmRightBackSwitchStuckFailureProcess,
   AppDemmRightSilenceSwitchStuckFailureProcess,
   AppDemmRightStarSwitchStuckFailureProcess,
   AppDemmLeftCrossCountrySwitchStuckFailureProcess,

   AppDemmLeftMediaSwitchStuckFailureProcess,
   AppDemmLeftSpeedAddSwitchStuckFailureProcess,
   AppDemmLeftDisanceAddSwitchStuckFailureProcess,
   AppDemmLeftDisanceDecSwitchStuckFailureProcess,
   AppDemmLeftSpeedDecSwitchStuckFailureProcess,

   AppDemmLeftInformationSwitchStuckFailureProcess,
   AppDemmLeftAccSwitchStuckFailureProcess,
   AppDemmRightPhoneSwitchStuckFailureProcess,   
   AppDemmRightNextSwitchStuckFailureProcess,
   AppDemmRightMenuSwitchStuckFailureProcess,
#ifdef SWSCONFIG_HIGH
   AppDemmLeftPickSwitchStuckFailureProcess,
   AppDemmRightPickSwitchStuckFailureProcess,
   AppDemmHeatShortFailureProcess,
   AppDemmHeatOpenFailureProcess,
   AppDemmNtcShortFailureProcess,
   AppDemmNtcOpenFailureProcess,
#endif
#ifdef SWSCONFIG_MID  
   AppDemmHeatShortFailureProcess,
   AppDemmHeatOpenFailureProcess,
   AppDemmNtcShortFailureProcess,
   AppDemmNtcOpenFailureProcess,
#endif
#else
   AppDemmSystemVoltageLowFailureProcess,
   AppDemmSystemVoltageHighFailureProcess,
   AppDemmHarnessFractureOrShortFailureProcess,
   AppDemmCanDisconnectedFailureProcess,
   AppDemmHeatOpenFailureProcess,
   AppDemmHeatShortFailureProcess,
   AppDemmNtcShortOrOpenFailureProcess,
   AppDemmTemOver37_5FailureProcess,
   AppDemmCanDisconnected_EMSFailureProcess,
   AppDemmTemPinISCurrentFailureProcess,

#endif


};

uint8_t u8GetMonitorDtcCondition(void)
{        
    return u8DTCMonitorCond;
}

static void vMonitorDtcConditionTask(void)
{
    EKeyState ekeystate;
    adc_result_mv_type mvsystemvoltage;
    uint8_t u8conditionIGN = 0;
    uint8_t u8conditionVolt = 0; 
    ekeystate = eReadKeyState();
    if(ekeystate == KEYSTATE_IGN)
    {
        u8conditionIGN = 1;
    }
    else
    {
        u8conditionIGN = 0;
    }
    mvsystemvoltage = System_VoltageMv_Get();
    //Voltage=Normal(9-16V) 
    if(mvsystemvoltage >= 9000 && mvsystemvoltage <= 16000)
    {
        u8conditionVolt = 1; // 点火状态满足条件
    }
    else
    {
        u8conditionVolt = 0;
    }
    //Condition1 KeyState=IGN,Condition2 Voltage=Normal(9-16V) Start Timer
    if(u8conditionIGN && u8conditionVolt)
    {
        u16IGNTimer ++;
    }
    else
    {
        u16IGNTimer = 0;
    }
    if(u16IGNTimer >= DTC_CONDITION_TIMER_3S)
    {
        u8DTCMonitorCond = 1;
    }

}

static void vSelfHealingProcess(void)
{
    static EKeyState LastkeyState = KEYSTATE_OFF;

    EKeyState CurrentkeyState;
    CurrentkeyState = eReadKeyState();
    if(CurrentkeyState != LastkeyState)
    {
        if(CurrentkeyState == KEYSTATE_IGN)
        {
           demm_operation_cycle_chaned_event_proc();
        }
    }

    LastkeyState = CurrentkeyState;
}


void App_Demm_Init(void)
{
    ClearDTCCondition();
}


void App_Demm_Task(void)
{
    uint8_t i;

    for(i = 0; i < TOTAL_DTC_NUM; i ++)
    {
        AppDemmProcessFun[i]();
    }
    vSelfHealingProcess();
    vMonitorDtcConditionTask();
}