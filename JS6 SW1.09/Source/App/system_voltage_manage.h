/**************************************************************************//**
 *
 *  \copyright  This software is the property of TAIKING Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              TAIKING Technologies.
 *
 *  \brief      This is the source file of the system voltage manage.
 *
 *  \file       system_voltage_manage.h
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
#ifndef _SYSTEM_VOLTAGE_MANAGE_H
#define _SYSTEM_VOLTAGE_MANAGE_H


#include "types.h"
#include "Adc_Drv.h"
/*****************************************************************************
 *   Macro Definitions
 *****************************************************************************/
#define SYSTEM_VOLTAGE_MODE_NORMAL                      0
#define SYSTEM_VOLTAGE_MODE_ENTER_LOW_FORM_NORMAL       1
#define SYSTEM_VOLTAGE_MODE_LOW                         2
#define SYSTEM_VOLTAGE_MODE_ENTER_NORMAL_FORM_LOW       3
#define SYSTEM_VOLTAGE_MODE_ENTER_UNDER_FORM_LOW        4
#define SYSTEM_VOLTAGE_MODE_UNDER                       5
#define SYSTEM_VOLTAGE_MODE_ENTER_LOW_FORM_UNDER        6
#define SYSTEM_VOLTAGE_MODE_ENTER_HIGH_FORM_NORMAL      7   
#define SYSTEM_VOLTAGE_MODE_HIGH                        8   
#define SYSTEM_VOLTAGE_MODE_ENTER_NORMAL_FORM_HIGH      9  
#define SYSTEM_VOLTAGE_MODE_ENTER_OVER_FORM_HIGH        10
#define SYSTEM_VOLTAGE_MODE_OVER                        11
#define SYSTEM_VOLTAGE_MODE_ENTER_HIGH_FORM_OVER        12


#define SYSTEM_VOLTAGE_DTC_PERIOD_TIME    100
#define SYSTEM_VOLTAGE_DTC_TIMER_3S (3*1000/SYSTEM_VOLTAGE_DTC_PERIOD_TIME)

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
void system_voltage_manage_task(void);
uint8_t system_voltage_mode_get(void);
void system_voltage_mode_low_power_set(void);
void system_voltage_mode_low_power_recover(void);
uint8_t SystemVoltageGetForDtcSnapShot(void);
void SystemVoltageDTCTask(void);

adc_result_mv_type System_VoltageMv_Get(void);
#endif