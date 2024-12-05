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
#ifndef __KEYBOARD_DRIVER_H
#define __KEYBOARD_DRIVER_H

/*****************************************************************************
 *  Files Include
 *****************************************************************************/

#include "types.h"

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
void keyboard_task(void);
void keyboard_init(void);
#endif