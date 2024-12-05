/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-09-15 16:58:58
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-09-16 16:30:17
 * @FilePath: \Source\drv\Keyboard\adc_keyboard_driver.h
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
 *  \file       adc_keyboard_driver.h
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
#ifndef __ADC_KEYBOARD_DRIVER_H
#define __ADC_KEYBOARD_DRIVER_H

/*****************************************************************************
 *  Files Include
 *****************************************************************************/
#include "types.h"
#include "keyboard_config.h"

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
void vAdcKeyInit(void);
void  vAdcKeyCheck(void);
uint8_t u8ReadAdcKeyFlag(uint8_t u8channel);
uint32_t u32ReadAdcKeyContinue(uint8_t u8channel);
#endif


