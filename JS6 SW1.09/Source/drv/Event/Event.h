#ifndef __EVENT_H
#define __EVENT_H

#define DIA_KEYSTUCK2 0
typedef enum
{
    DTC_TEST_EVENT_NULL = 0,
    DTC_TEST_EVENT_FAILED,
    DTC_TEST_EVENT_PASSED,
}DemmDtcEvent_e;



void EventInit(void);

#if(DIA_KEYSTUCK2==1)
void AppDemmRightPhoneSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmRightBackSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmRightSilenceSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmRightStarSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmLeftCrossCountrySwitchStuckFailureEventSet(DemmDtcEvent_e event);

void AppDemmLeftMediaSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmLeftSpeedAddSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmLeftDisanceAddSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmLeftDisanceDecSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmLeftSpeedDecSwitchStuckFailureEventSet(DemmDtcEvent_e event);

void AppDemmLeftInformationSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmLeftAccSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmRightVoiceSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmRightNextSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmRightMenuSwitchStuckFailureEventSet(DemmDtcEvent_e event);

void AppDemmLeftPickSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmRightPickSwitchStuckFailureEventSet(DemmDtcEvent_e event);
void AppDemmHeatShortFailureEventSet(DemmDtcEvent_e event);
void AppDemmHeatOpenFailureEventSet(DemmDtcEvent_e event);
void AppDemmNtcShortFailureEventSet(DemmDtcEvent_e event);
void AppDemmNtcOpenFailureEventSet(DemmDtcEvent_e event);
///////////////////////////////////////////////////////////////////////////
DemmDtcEvent_e AppDemmRightPhoneSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmRightBackSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmRightSilenceSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmRightStarSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmLeftCrossCountrySwitchStuckFailureEventGet(void);

DemmDtcEvent_e AppDemmLeftMediaSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmLeftSpeedAddSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmLeftDisanceAddSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmLeftDisanceDecSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmLeftSpeedDecSwitchStuckFailureEventGet(void);

DemmDtcEvent_e AppDemmLeftInformationSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmLeftAccSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmRightVoiceSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmRightNextSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmRightMenuSwitchStuckFailureEventGet(void);

DemmDtcEvent_e AppDemmLeftPickSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmRightPickSwitchStuckFailureEventGet(void);
DemmDtcEvent_e AppDemmHeatShortFailureEventGet(void);
DemmDtcEvent_e AppDemmHeatOpenFailureEventGet(void);
DemmDtcEvent_e AppDemmNtcShortFailureEventGet(void);
DemmDtcEvent_e AppDemmNtcOpenFailureEventGet(void);
#else
void AppDemmSystemVoltageLowFailureEvent(DemmDtcEvent_e event);
void AppDemmSystemVoltageHighFailureEvent(DemmDtcEvent_e event);
void AppDemmHarnessFractureOrShortFailureEvent(DemmDtcEvent_e event);
void AppDemmCanDisconnectedFailureEvent(DemmDtcEvent_e event);
void AppDemmHeatOpenFailureEvent(DemmDtcEvent_e event);
void AppDemmHeatShortFailureEvent(DemmDtcEvent_e event);
void AppDemmNtcShortOrOpenFailureEvent(DemmDtcEvent_e event);
void AppDemmTemOver37_5FailureEvent(DemmDtcEvent_e event);
DemmDtcEvent_e AppDemmSystemVoltageLowFailureEventGet(void);
DemmDtcEvent_e AppDemmSystemVoltageHighFailureEventGet(void);
DemmDtcEvent_e AppDemmHarnessFractureOrShortFailureEventGet(void);
DemmDtcEvent_e AppDemmCanDisconnectedFailureEventGet(void);
DemmDtcEvent_e AppDemmHeatOpenFailureEventGet(void);
DemmDtcEvent_e AppDemmHeatShortFailureEventGet(void);
DemmDtcEvent_e AppDemmNtcShortOrOpenFailureEventGet(void);
DemmDtcEvent_e AppDemmTemOver37_5FailureEventGet(void);
void dtconflag(void);
void dtcoffflag(void);
uint8_t getdtcstatus(void);
#endif

#endif