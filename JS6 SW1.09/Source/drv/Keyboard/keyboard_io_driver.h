/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-09-15 16:58:58
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-09-16 17:06:10
 * @FilePath: \Source\drv\Keyboard\keyboard_io_driver.h
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
 *  \brief      This is the source file of the keyboard io driver.
 *
 *  \file       keyboard_io_driver.h
 *  \ingroup    driver
 *  \author     by.lin
 *
 *  \version    1.0.0
 *  \date       09/04/2021
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      01.00.00 | 09/04/2021 | by.lin           | N/A |
 *
******************************************************************************/
#ifndef __KEYBOARD_IO_DRIVER_H
#define __KEYBOARD_IO_DRIVER_H

/*****************************************************************************
 *  Files Include
 *****************************************************************************/
#include "types.h"
#include "keyboard_config.h"


/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#if defined (IO_KEYBOARD_FUN_ENABLE)
void  vGpioKeyCheck(void);


#endif
#endif /* end of __KEYBOARD_IO_DRIVER_H */


