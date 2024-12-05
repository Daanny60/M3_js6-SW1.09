/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-09-15 16:58:58
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-09-16 17:04:56
 * @FilePath: \Source\drv\Keyboard\keyboard_driver.c
 * @Description: 
 * 
 * Copyright (c) 2022 by error: git config user.name && git config user.email & please set dead value or install git, All Rights Reserved. 
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of TAIKING Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              TAIKING Technologies.
 *
 *  \brief      This is the source file of the adc keyboard driver.
 *
 *  \file       keyboard_driver.h
 *  \ingroup    driver
 *  \author     by.lin
 *
 *  \version    1.0.0
 *  \date       13/04/2021
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      01.00.00 | 13/04/2021 | by.lin           | N/A | 
 *
******************************************************************************/




/*****************************************************************************
 *  Files Include
 *****************************************************************************/
#include "keyboard_driver.h"
#include "keyboard_io_driver.h"
#include "adc_keyboard_driver.h"
#include "system_voltage_manage.h"
#include "App_Keyboard.h"



/*****************************************************************************
|Prototype         : keyboard_task
|Called by         : 
|Preconditions     : -
|Input parameters  : None
|Output parameters : None
|Return value      : None
|Description       : 
*****************************************************************************/
void keyboard_task(void)
{
  uint8_t system_voltage_mod;

  system_voltage_mod = system_voltage_mode_get();

  if(SYSTEM_VOLTAGE_MODE_NORMAL != system_voltage_mod)
  {
    keyboard_init();
  }
  else
  {
   AppKeyboard_Task();
  }


}
/*****************************************************************************
|Prototype         : keyboard_init
|Called by         : 
|Preconditions     : -
|Input parameters  : None
|Output parameters : None
|Return value      : None
|Description       : 
*****************************************************************************/
void keyboard_init(void)
{
vAppKeyBoardInit();  
}
