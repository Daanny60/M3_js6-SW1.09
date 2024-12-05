/**************************************************************************//**
 *
 *  \copyright  This software is the property of TAIKING Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              TAIKING Technologies.
 *
 *  \brief      This is the source file of the system voltage manage.
 *
 *  \file       system_voltage_manage.c
 *  \ingroup    system voltage manage
 *  \author     by.lin
 *
 *  \version    1.0.0
 *  \date       22/04/2020
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      01.00.00 | 22/04/2020 | by.lin           | N/A | 
 *
******************************************************************************/
#include "system_voltage_manage.h"
#include "fm33lg0xx_fl_conf.h"
#include "Event.h"
#include "App_Can.h"
#include "App_Demm.h"

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/

/*
#define SYSTEM_VOLTAGE_UNDER_FUN_ENABLE 
#define SYSTEM_VOLTAGE_OVER_FUN_ENABLE
*/

#define SYSTEM_VOLTAGE_MODE_TIME    1000/10



#define ENTER_SYSTEM_VOLTAGE_LOW_THRESHOLD          6700
#define EXIT_SYSTEM_VOLTAGE_LOW_THRESHOLD           7500

#define ENTER_SYSTEM_VOLTAGE_UNDER_THRESHOLD        6000
#define EXIT_SYSTEM_VOLTAGE_UNDER_THRESHOLD         6500

#define ENTER_SYSTEM_VOLTAGE_HIGH_THRESHOLD         17800
#define EXIT_SYSTEM_VOLTAGE_HIGG_THRESHOLD          17000

#define ENTER_SYSTEM_VOLTAGE_OVER_THRESHOLD         25450
#define EXIT_SYSTEM_VOLTAGE_OVER_THRESHOLD          24740
/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/
typedef struct _tage_SystemVoltagePar SystemVoltagePar_t;
typedef struct _tag_SystemVoltageManage SystemVoltageManage_t;
typedef struct _tag_SystemVoltageManageDtc SystemVoltageManageDtc_t;
typedef struct _tag_FaultMature FaultMature_t;
typedef struct _tag_FaultMatureDtc FaultMatureDtc_t;
/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/
struct _tage_SystemVoltagePar
{
  uint16_t EnterThreshold;
  uint16_t ExitThreshold;
};


struct _tag_SystemVoltageManage
{
  uint8_t Mode;
  uint8_t Timer;
  const struct _tage_SystemVoltagePar LowVoltage;
#if defined (SYSTEM_VOLTAGE_UNDER_FUN_ENABLE)
  const struct _tage_SystemVoltagePar UnderVoltage;
#endif
  const struct _tage_SystemVoltagePar HighVoltage;
#if defined (SYSTEM_VOLTAGE_OVER_FUN_ENABLE)
  const struct _tage_SystemVoltagePar OverVoltage;
#endif
};

struct _tag_SystemVoltageManageDtc
{
  uint16_t VoltageLowThreshold;
  uint16_t VoltageLowRecoverThreshold;
  uint16_t VoltageHighThreshold;
  uint16_t VoltageHighRecoverThreshold;
};



struct _tag_FaultMature
{
  uint16_t u16FaultMatureCnt;
  uint16_t u16FaultRecoverCnt;
  const uint16_t u16FaultMatureCntMax;
  const uint16_t u16FaultRecoverCntMax;
};

struct _tag_FaultMatureDtc
{
   struct _tag_FaultMature SystemVoltageLow;
   struct _tag_FaultMature SystemVoltageHigh;
};


  

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
static SystemVoltageManage_t gs_SystemVoltageManage = 
{
  SYSTEM_VOLTAGE_MODE_NORMAL,
  0,
  {
    /* 8.5v*/
    ENTER_SYSTEM_VOLTAGE_LOW_THRESHOLD,   
    /* 9 v*/
    EXIT_SYSTEM_VOLTAGE_LOW_THRESHOLD,
  },
#if defined (SYSTEM_VOLTAGE_UNDER_FUN_ENABLE)
  {
    /* 6v*/
    ENTER_SYSTEM_VOLTAGE_UNDER_THRESHOLD,
    /*6.5v*/
    EXIT_SYSTEM_VOLTAGE_UNDER_THRESHOLD,
  },
#endif
  {
    /* 16.5 v*/
    ENTER_SYSTEM_VOLTAGE_HIGH_THRESHOLD, 
    /* 16 v*/ 
    EXIT_SYSTEM_VOLTAGE_HIGG_THRESHOLD,
  },
#if defined (SYSTEM_VOLTAGE_OVER_FUN_ENABLE)  
  {
    /* 18.5v */
    ENTER_SYSTEM_VOLTAGE_OVER_THRESHOLD,  
    /* 18 v*/
    EXIT_SYSTEM_VOLTAGE_OVER_THRESHOLD,
  },
#endif
};

static SystemVoltageManageDtc_t gs_SystemVoltageManageDtc = 
{
  8500,//less than 8.5v
  9000,//more than 9v
  16500,//more than 16.5v
  16000,//less than 16v
};



FaultMatureDtc_t gs_FaultMature = 
{
  {
    0,
    0,
    30,
    10,
  },
  {
    0,
    0,
    30,
    10,
  },
};  

static adc_result_mv_type system_voltage;

/**
 * @brief Check the system voltage DTC condition
 * 
 * @return uint8_t 0=Condition not met, 1=Condition met
 */
static uint8_t u8MonitorVoltDtcCondition(void)
{

  uint8_t u8conditionRtn = 0;
  uint8_t u8conditionIGN = 0;
  uint8_t u8conditionEngine = 0;

  u8conditionIGN = u8GetMonitorDtcCondition();
  //u8conditionIGN = 1; //for test
  //Condition 2: EngineState!=0x02 CRANK
  if(eReadEngineState() != ENGINESTATE_CRANK)
  {
    u8conditionEngine = 1;
  }
  else
  {
    u8conditionEngine = 0;
  }

  //If both condition 1 and condition 2 are met, return 1
  if(u8conditionIGN == 1 && u8conditionEngine == 1)
  {
    u8conditionRtn = 1;
  }
  else
  {
    u8conditionRtn = 0;
  }

  return u8conditionRtn;

}




/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/


/**
 * @brief call period 100ms
 * 
 */
void SystemVoltageDTCTask(void)
{
  uint8_t u8VoltDtcCondition = 0;
  u8VoltDtcCondition = u8MonitorVoltDtcCondition();
  if(1 == u8VoltDtcCondition)
  {
    //Trigger DTC
    if(system_voltage < gs_SystemVoltageManageDtc.VoltageLowThreshold)
    {
        gs_FaultMature.SystemVoltageLow.u16FaultMatureCnt ++;
        if(gs_FaultMature.SystemVoltageLow.u16FaultMatureCnt >= gs_FaultMature.SystemVoltageLow.u16FaultMatureCntMax)
        {
            gs_FaultMature.SystemVoltageLow.u16FaultMatureCnt = gs_FaultMature.SystemVoltageLow.u16FaultMatureCntMax;
            AppDemmSystemVoltageLowFailureEvent(DTC_TEST_EVENT_FAILED);
        }
    }
    else
    {
        gs_FaultMature.SystemVoltageLow.u16FaultMatureCnt = 0;
    }
    if (system_voltage > gs_SystemVoltageManageDtc.VoltageHighThreshold)
    {
        gs_FaultMature.SystemVoltageHigh.u16FaultMatureCnt ++;
        if(gs_FaultMature.SystemVoltageHigh.u16FaultMatureCnt >= gs_FaultMature.SystemVoltageHigh.u16FaultMatureCntMax)
        {
            gs_FaultMature.SystemVoltageHigh.u16FaultMatureCnt = gs_FaultMature.SystemVoltageHigh.u16FaultMatureCntMax;
            AppDemmSystemVoltageHighFailureEvent(DTC_TEST_EVENT_FAILED);
        }
    }
    else
    {
        gs_FaultMature.SystemVoltageHigh.u16FaultMatureCnt = 0;
    }
    //Recover DTC
    if(system_voltage > gs_SystemVoltageManageDtc.VoltageLowRecoverThreshold)
    {
        gs_FaultMature.SystemVoltageLow.u16FaultRecoverCnt ++;
        if(gs_FaultMature.SystemVoltageLow.u16FaultRecoverCnt >= gs_FaultMature.SystemVoltageLow.u16FaultRecoverCntMax)
        {
            gs_FaultMature.SystemVoltageLow.u16FaultRecoverCnt = gs_FaultMature.SystemVoltageLow.u16FaultRecoverCntMax;
            AppDemmSystemVoltageLowFailureEvent(DTC_TEST_EVENT_PASSED);
        }
    }
    else
    {
        gs_FaultMature.SystemVoltageLow.u16FaultRecoverCnt = 0;
    }
    if(system_voltage < gs_SystemVoltageManageDtc.VoltageHighRecoverThreshold)
    {
        gs_FaultMature.SystemVoltageHigh.u16FaultRecoverCnt ++;
        if(gs_FaultMature.SystemVoltageHigh.u16FaultRecoverCnt >= gs_FaultMature.SystemVoltageHigh.u16FaultRecoverCntMax)
        {
            gs_FaultMature.SystemVoltageHigh.u16FaultRecoverCnt = gs_FaultMature.SystemVoltageHigh.u16FaultRecoverCntMax;
            AppDemmSystemVoltageHighFailureEvent(DTC_TEST_EVENT_PASSED);
        }
    }
    else
    {
        gs_FaultMature.SystemVoltageHigh.u16FaultRecoverCnt = 0;
    }
  }
  else //Monitor condition not met, reset the counter
  {
    gs_FaultMature.SystemVoltageLow.u16FaultMatureCnt = 0;
    gs_FaultMature.SystemVoltageHigh.u16FaultMatureCnt = 0;
    gs_FaultMature.SystemVoltageLow.u16FaultRecoverCnt = 0;
    gs_FaultMature.SystemVoltageHigh.u16FaultRecoverCnt = 0;
  }



}


uint8_t SystemVoltageGetForDtcSnapShot(void)
{	
    system_voltage /= 100;
    return (uint8_t)(system_voltage);
}

/**************************************************************************//**
 *
 *  \details    app led task.
 *
 *  \return None.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/

void system_voltage_manage_task(void)
{

 

  system_voltage = ADC_ResultMv_Get(ADC_POWER_12V_RESULT_INDEX);
  system_voltage = system_voltage * 57 / 10 + 700;


  switch(gs_SystemVoltageManage.Mode)
  {
    case SYSTEM_VOLTAGE_MODE_NORMAL:
      if(system_voltage <= gs_SystemVoltageManage.LowVoltage.EnterThreshold)
      {
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_ENTER_LOW_FORM_NORMAL;
        gs_SystemVoltageManage.Timer = 0;
      }
      else if(system_voltage >= gs_SystemVoltageManage.HighVoltage.EnterThreshold)
      {
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_ENTER_HIGH_FORM_NORMAL;
        gs_SystemVoltageManage.Timer = 0;
      }
      else
      {
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_NORMAL;
        gs_SystemVoltageManage.Timer = 0;
      }
      break;
    case SYSTEM_VOLTAGE_MODE_ENTER_LOW_FORM_NORMAL:
      if(system_voltage <= gs_SystemVoltageManage.LowVoltage.EnterThreshold)
      {
        gs_SystemVoltageManage.Timer ++;
        if(gs_SystemVoltageManage.Timer >= SYSTEM_VOLTAGE_MODE_TIME)
        {
          gs_SystemVoltageManage.Timer = 0;
          gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_LOW;
        }
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_NORMAL;
      }      
      break;
    case SYSTEM_VOLTAGE_MODE_LOW:
      if(system_voltage >= gs_SystemVoltageManage.LowVoltage.ExitThreshold)
      {
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_ENTER_NORMAL_FORM_LOW;
        gs_SystemVoltageManage.Timer = 0;
      }
#if defined (SYSTEM_VOLTAGE_UNDER_FUN_ENABLE)
      else if(system_voltage <= gs_SystemVoltageManage.UnderVoltage.EnterThreshold)
      {
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_ENTER_UNDER_FORM_LOW;
        gs_SystemVoltageManage.Timer = 0;
      }
#endif
      else
      {
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_LOW;
        gs_SystemVoltageManage.Timer = 0;
      }     
      break;
    case SYSTEM_VOLTAGE_MODE_ENTER_NORMAL_FORM_LOW:
      if(system_voltage >= gs_SystemVoltageManage.LowVoltage.ExitThreshold)
      {
        gs_SystemVoltageManage.Timer ++;
        if(gs_SystemVoltageManage.Timer >= SYSTEM_VOLTAGE_MODE_TIME)
        {
          gs_SystemVoltageManage.Timer = 0;
          gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_NORMAL;
        }
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_LOW;
      }     
      break;
#if defined (SYSTEM_VOLTAGE_UNDER_FUN_ENABLE)
    case SYSTEM_VOLTAGE_MODE_ENTER_UNDER_FORM_LOW:
      if(system_voltage <= gs_SystemVoltageManage.UnderVoltage.EnterThreshold)
      {
        gs_SystemVoltageManage.Timer ++;
        if(gs_SystemVoltageManage.Timer >= SYSTEM_VOLTAGE_MODE_TIME)
        {
          gs_SystemVoltageManage.Timer = 0;
          gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_UNDER;
        }
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_LOW;
      }      
      break;

    case SYSTEM_VOLTAGE_MODE_UNDER:
      if(system_voltage >= gs_SystemVoltageManage.UnderVoltage.ExitThreshold)
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_ENTER_LOW_FORM_UNDER;             
      }           
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_UNDER;
      }           
      break;
    case SYSTEM_VOLTAGE_MODE_ENTER_LOW_FORM_UNDER:
      if(system_voltage >= gs_SystemVoltageManage.UnderVoltage.ExitThreshold)
      {
        gs_SystemVoltageManage.Timer ++;
        if(gs_SystemVoltageManage.Timer >= SYSTEM_VOLTAGE_MODE_TIME)
        {
          gs_SystemVoltageManage.Timer = 0;
          gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_LOW;
        }
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_UNDER;
      }        
      break;
#endif
    case SYSTEM_VOLTAGE_MODE_ENTER_HIGH_FORM_NORMAL:
      if(system_voltage >= gs_SystemVoltageManage.HighVoltage.EnterThreshold)
      {
        gs_SystemVoltageManage.Timer ++;
        if(gs_SystemVoltageManage.Timer >= SYSTEM_VOLTAGE_MODE_TIME)
        {
          gs_SystemVoltageManage.Timer = 0;
          gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_HIGH;
        }                
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_NORMAL;
      }                 
      break;
    case SYSTEM_VOLTAGE_MODE_HIGH:
      if(system_voltage <= gs_SystemVoltageManage.HighVoltage.ExitThreshold)
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_ENTER_NORMAL_FORM_HIGH;
      }
#if defined (SYSTEM_VOLTAGE_OVER_FUN_ENABLE)  
      else if(system_voltage >= gs_SystemVoltageManage.OverVoltage.EnterThreshold)
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_ENTER_OVER_FORM_HIGH;
      }
#endif
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_HIGH;
      }     
      break;
    case SYSTEM_VOLTAGE_MODE_ENTER_NORMAL_FORM_HIGH:
      if(system_voltage <= gs_SystemVoltageManage.HighVoltage.ExitThreshold)
      {
        gs_SystemVoltageManage.Timer ++;
        if(gs_SystemVoltageManage.Timer >= SYSTEM_VOLTAGE_MODE_TIME)
        {
          gs_SystemVoltageManage.Timer = 0;
          gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_NORMAL;
        }                 
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_HIGH;
      }
      break;
#if defined (SYSTEM_VOLTAGE_OVER_FUN_ENABLE)  
    case SYSTEM_VOLTAGE_MODE_ENTER_OVER_FORM_HIGH:
      if(system_voltage >= gs_SystemVoltageManage.OverVoltage.EnterThreshold)
      {
        gs_SystemVoltageManage.Timer ++;
        if(gs_SystemVoltageManage.Timer >= SYSTEM_VOLTAGE_MODE_TIME)
        {
          gs_SystemVoltageManage.Timer = 0;
          gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_OVER;
        }                 
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_HIGH;
      }    
      break;
    case SYSTEM_VOLTAGE_MODE_OVER:
      if(system_voltage <= gs_SystemVoltageManage.OverVoltage.ExitThreshold)
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_ENTER_HIGH_FORM_OVER;
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_OVER;
      }     
      break;
    case SYSTEM_VOLTAGE_MODE_ENTER_HIGH_FORM_OVER:
      if(system_voltage <= gs_SystemVoltageManage.OverVoltage.ExitThreshold)
      {
        gs_SystemVoltageManage.Timer ++;
        if(gs_SystemVoltageManage.Timer >= SYSTEM_VOLTAGE_MODE_TIME)
        {
          gs_SystemVoltageManage.Timer = 0;
          gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_HIGH;
        }                  
      }
      else
      {
        gs_SystemVoltageManage.Timer = 0;
        gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_OVER;
      }
      break;
#endif
    default:
      gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_NORMAL;
      gs_SystemVoltageManage.Timer = 0;
      break;
  }

}


/**************************************************************************//**
 *
 *  \details    system_voltage_mode_get
 *
 *  \return None.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
uint8_t system_voltage_mode_get(void)
{
  uint8_t system_voltage_mode;

  switch(gs_SystemVoltageManage.Mode)
  {
    case SYSTEM_VOLTAGE_MODE_NORMAL:
    case SYSTEM_VOLTAGE_MODE_ENTER_LOW_FORM_NORMAL:
    case SYSTEM_VOLTAGE_MODE_ENTER_HIGH_FORM_NORMAL:
      system_voltage_mode = SYSTEM_VOLTAGE_MODE_NORMAL;
      break;
    case SYSTEM_VOLTAGE_MODE_LOW:
    case SYSTEM_VOLTAGE_MODE_ENTER_NORMAL_FORM_LOW:
    case SYSTEM_VOLTAGE_MODE_ENTER_UNDER_FORM_LOW:
      system_voltage_mode = SYSTEM_VOLTAGE_MODE_LOW;
      break;
    case SYSTEM_VOLTAGE_MODE_UNDER:
    case SYSTEM_VOLTAGE_MODE_ENTER_LOW_FORM_UNDER:
      system_voltage_mode = SYSTEM_VOLTAGE_MODE_UNDER;
      break;
    case SYSTEM_VOLTAGE_MODE_HIGH:
    case SYSTEM_VOLTAGE_MODE_ENTER_NORMAL_FORM_HIGH:
    case SYSTEM_VOLTAGE_MODE_ENTER_OVER_FORM_HIGH:
      system_voltage_mode = SYSTEM_VOLTAGE_MODE_HIGH;
      break;
    case SYSTEM_VOLTAGE_MODE_OVER:
    case SYSTEM_VOLTAGE_MODE_ENTER_HIGH_FORM_OVER:
      system_voltage_mode = SYSTEM_VOLTAGE_MODE_OVER;
      break;
    default:
      system_voltage_mode = SYSTEM_VOLTAGE_MODE_NORMAL;
      gs_SystemVoltageManage.Mode = SYSTEM_VOLTAGE_MODE_NORMAL;
      break;
  }
  return(system_voltage_mode);

}


/**************************************************************************//**
 *
 *  \details    system_voltage_mode_low_power_set
 *
 *  \return None.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void system_voltage_mode_low_power_set(void)
{
}

/**************************************************************************//**
 *
 *  \details    system_voltage_mode_low_power_recover
 *
 *  \return None.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void system_voltage_mode_low_power_recover(void)
{
 
}


adc_result_mv_type System_VoltageMv_Get(void)
{
  return system_voltage;
}