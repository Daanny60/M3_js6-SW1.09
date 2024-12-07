#include "fm33lg0xx_fl.h"
#include "Event.h"
#include <string.h>
#include "keyboard_config.h"
#include "App_Keyboard.h"

volatile uint8_t DTCEventFlag=0;
#if(DIA_KEYSTUCK2)
typedef struct 
{ 
    /*0-4*/
    DemmDtcEvent_e  RightPhoneSwitchStuckFailureEvent;
    DemmDtcEvent_e  RightBackSwitchStuckFailureEvent;
    DemmDtcEvent_e  RightSilenceSwitchStuckFailureEvent;
    DemmDtcEvent_e  RightStarSwitchStuckFailureEvent;
    DemmDtcEvent_e  LeftCrossCountrySwitchStuckFailureEvent;
     /*5-9*/
    DemmDtcEvent_e  LeftMediaSwitchStuckFailureEvent;
    DemmDtcEvent_e  LeftSpeedAddSwitchStuckFailureEvent;
    DemmDtcEvent_e  LeftDisanceAddSwitchStuckFailureEvent;
    DemmDtcEvent_e  LeftDisanceDecSwitchStuckFailureEvent;
    DemmDtcEvent_e  LeftSpeedDecSwitchStuckFailureEvent;
     /*10-14*/
    DemmDtcEvent_e  LeftInformationSwitchStuckFailureEvent;
    DemmDtcEvent_e  LeftAccSwitchStuckFailureEvent;
    DemmDtcEvent_e  RightVoiceSwitchStuckFailureEvent;
    DemmDtcEvent_e  RightNextSwitchStuckFailureEvent;
    DemmDtcEvent_e  RightMenuSwitchStuckFailureEvent;
    /*15-16*/
    DemmDtcEvent_e  LeftPickSwitchStuckFailureEvent;
    DemmDtcEvent_e  RightPickSwitchStuckFailureEvent;
#ifndef  SWSCONFIG_LOW
    /*15-18*/
    DemmDtcEvent_e  HeatShortFailureEvent;
    DemmDtcEvent_e  HeatOpenFailureEvent;
    DemmDtcEvent_e  NtcShortFailureEvent;
    DemmDtcEvent_e  NtcOpenFailureEvent;    
#endif  
}AppDemmEvent_TypeDef;  
#else
typedef struct 
{ 
   DemmDtcEvent_e  SystemVoltageLowFailureEvent;
   DemmDtcEvent_e  SystemVoltageHighFailureEvent;   
   DemmDtcEvent_e  HarnessFractureOrShortFailureEvent;
   DemmDtcEvent_e  CanDisconnectedFailureEvent;//BCM节点丢失
   DemmDtcEvent_e  HeatOpenFailureEvent;
   DemmDtcEvent_e  HeatShortFailureEvent;
   DemmDtcEvent_e  NtcShortOrOpenFailureEvent;
   DemmDtcEvent_e  TemOver37_5FailureEvent;
   DemmDtcEvent_e  CanDisconnected_EMSFailureEvent;//EMS节点丢失
   DemmDtcEvent_e  PinISCurrentFailureEvent;//iS电流检测<0
}AppDemmEvent_TypeDef;
 #endif
    


typedef struct
{    
    AppDemmEvent_TypeDef AppDemmEvent;
}EventTypeDef;

EventTypeDef Event;

void EventInit(void)
{
    memset(&Event, 0, sizeof(Event));
}

#if(DIA_KEYSTUCK2==1)
/*0-4*/
void AppDemmRightPhoneSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.RightPhoneSwitchStuckFailureEvent = event;
}
void AppDemmRightBackSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.RightBackSwitchStuckFailureEvent = event;
}
void AppDemmRightSilenceSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.RightSilenceSwitchStuckFailureEvent = event;
}
void AppDemmRightStarSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.RightStarSwitchStuckFailureEvent = event;
}
void AppDemmLeftCrossCountrySwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftCrossCountrySwitchStuckFailureEvent = event;
}
/*5-9*/
void AppDemmLeftMediaSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftMediaSwitchStuckFailureEvent = event;
}
void AppDemmLeftSpeedAddSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftSpeedAddSwitchStuckFailureEvent = event;
}
void AppDemmLeftDisanceAddSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftDisanceAddSwitchStuckFailureEvent = event;
}
void AppDemmLeftDisanceDecSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftDisanceDecSwitchStuckFailureEvent = event;
}
void AppDemmLeftSpeedDecSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftSpeedDecSwitchStuckFailureEvent = event;
}
/*10-14*/
void AppDemmLeftInformationSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftInformationSwitchStuckFailureEvent = event;
}
void AppDemmLeftAccSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftAccSwitchStuckFailureEvent = event;
}
void AppDemmRightVoiceSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.RightVoiceSwitchStuckFailureEvent = event;
}
void AppDemmRightNextSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.RightNextSwitchStuckFailureEvent = event;
}
void AppDemmRightMenuSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.RightMenuSwitchStuckFailureEvent = event;
}
/*15-16*/
void AppDemmLeftPickSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.LeftPickSwitchStuckFailureEvent = event;
}
void AppDemmRightPickSwitchStuckFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.RightPickSwitchStuckFailureEvent = event;
}

#ifndef  SWSCONFIG_LOW
/*15-18*/
void AppDemmHeatShortFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.HeatShortFailureEvent = event;
}
void AppDemmHeatOpenFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.HeatOpenFailureEvent = event;
}
void AppDemmNtcShortFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.NtcShortFailureEvent = event;
}
void AppDemmNtcOpenFailureEventSet(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.NtcOpenFailureEvent = event;
}

#endif
//////////////////////////////////////////////////////////////////////////////
/*0-4*/
DemmDtcEvent_e AppDemmRightPhoneSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.RightPhoneSwitchStuckFailureEvent;
    Event.AppDemmEvent.RightPhoneSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmRightBackSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.RightBackSwitchStuckFailureEvent;
    Event.AppDemmEvent.RightBackSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmRightSilenceSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.RightSilenceSwitchStuckFailureEvent;
    Event.AppDemmEvent.RightSilenceSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmRightStarSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.RightStarSwitchStuckFailureEvent;
    Event.AppDemmEvent.RightStarSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmLeftCrossCountrySwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftCrossCountrySwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftCrossCountrySwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
/*5-9*/
DemmDtcEvent_e AppDemmLeftMediaSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftMediaSwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftMediaSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmLeftSpeedAddSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftSpeedAddSwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftSpeedAddSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmLeftDisanceAddSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftDisanceAddSwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftDisanceAddSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmLeftDisanceDecSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftDisanceDecSwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftDisanceDecSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmLeftSpeedDecSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftSpeedDecSwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftSpeedDecSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
/*10-14*/
DemmDtcEvent_e AppDemmLeftInformationSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftInformationSwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftInformationSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmLeftAccSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftAccSwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftAccSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmRightVoiceSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.RightVoiceSwitchStuckFailureEvent;
    Event.AppDemmEvent.RightVoiceSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmRightNextSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.RightNextSwitchStuckFailureEvent;
    Event.AppDemmEvent.RightNextSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmRightMenuSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.RightMenuSwitchStuckFailureEvent;
    Event.AppDemmEvent.RightMenuSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
/*15-16*/
DemmDtcEvent_e AppDemmLeftPickSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.LeftPickSwitchStuckFailureEvent;
    Event.AppDemmEvent.LeftPickSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmRightPickSwitchStuckFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.RightPickSwitchStuckFailureEvent;
    Event.AppDemmEvent.RightPickSwitchStuckFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
#ifndef SWSCONFIG_LOW
/*15-18*/
DemmDtcEvent_e AppDemmHeatShortFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.HeatShortFailureEvent;
    Event.AppDemmEvent.HeatShortFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmHeatOpenFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.HeatOpenFailureEvent;
    Event.AppDemmEvent.HeatOpenFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmNtcShortFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.NtcShortFailureEvent;
    Event.AppDemmEvent.NtcShortFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmNtcOpenFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.NtcOpenFailureEvent;
    Event.AppDemmEvent.NtcOpenFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}
#endif
#else

/*这些是用来输入dtc是否显示的的参数*/
void AppDemmSystemVoltageLowFailureEvent(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.SystemVoltageLowFailureEvent = event;
}

void AppDemmSystemVoltageHighFailureEvent(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.SystemVoltageHighFailureEvent = event;
}
void AppDemmHarnessFractureOrShortFailureEvent(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.HarnessFractureOrShortFailureEvent = event;
}
void AppDemmCanDisconnectedFailureEvent(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.CanDisconnectedFailureEvent = event;
}
void AppDemmHeatOpenFailureEvent(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.HeatOpenFailureEvent = event;
}
void AppDemmHeatShortFailureEvent(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.HeatShortFailureEvent = event;
}
void AppDemmNtcShortOrOpenFailureEvent(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.NtcShortOrOpenFailureEvent = event;
}
void AppDemmTemOver37_5FailureEvent(DemmDtcEvent_e event)
{
    Event.AppDemmEvent.TemOver37_5FailureEvent = event;
}
void AppDemmCanDisconnected_EMSFailureEvent(DemmDtcEvent_e event)
{
     Event.AppDemmEvent.CanDisconnected_EMSFailureEvent= event;
}
void AppDemmPinISCurrentFailureEvent(DemmDtcEvent_e event)
{
     Event.AppDemmEvent.PinISCurrentFailureEvent = event;
}



DemmDtcEvent_e AppDemmSystemVoltageLowFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.SystemVoltageLowFailureEvent;
    Event.AppDemmEvent.SystemVoltageLowFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}


DemmDtcEvent_e AppDemmSystemVoltageHighFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.SystemVoltageHighFailureEvent;
    Event.AppDemmEvent.SystemVoltageHighFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}


DemmDtcEvent_e AppDemmHarnessFractureOrShortFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.HarnessFractureOrShortFailureEvent;
    Event.AppDemmEvent.HarnessFractureOrShortFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}

DemmDtcEvent_e AppDemmCanDisconnectedFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.CanDisconnectedFailureEvent;
    Event.AppDemmEvent.CanDisconnectedFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}

DemmDtcEvent_e AppDemmHeatOpenFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.HeatOpenFailureEvent;
    Event.AppDemmEvent.HeatOpenFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}

DemmDtcEvent_e AppDemmHeatShortFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.HeatShortFailureEvent;
    Event.AppDemmEvent.HeatShortFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}

DemmDtcEvent_e AppDemmNtcShortOrOpenFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.NtcShortOrOpenFailureEvent;
    Event.AppDemmEvent.NtcShortOrOpenFailureEvent = DTC_TEST_EVENT_NULL;
    return (event);
}

DemmDtcEvent_e AppDemmTemOver37_5FailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.TemOver37_5FailureEvent ;
    Event.AppDemmEvent.TemOver37_5FailureEvent  = DTC_TEST_EVENT_NULL;
    return (event);
}

DemmDtcEvent_e AppDemmCanDisconnected_EMSFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.CanDisconnected_EMSFailureEvent ;
    Event.AppDemmEvent.CanDisconnected_EMSFailureEvent  = DTC_TEST_EVENT_NULL;
    return (event);
}
DemmDtcEvent_e AppDemmTemPinISCurrentFailureEventGet(void)
{
    DemmDtcEvent_e event;
    event = Event.AppDemmEvent.PinISCurrentFailureEvent;
    Event.AppDemmEvent.PinISCurrentFailureEvent  = DTC_TEST_EVENT_NULL;
    return (event);

}


void dtconflag(void)
{
     DTCEventFlag=1;     
}

void dtcoffflag(void)
{
     DTCEventFlag=0;     
}

uint8_t getdtcstatus(void)
{
    return DTCEventFlag;    
}

#endif
