/*  BEGIN_FILE_HDR
************************************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information contained in this 
*   doc should not be reproduced, or used, or disclosed without the written authorization from 
*   HiRain Technologies.
************************************************************************************************
*   File Name       : CanTrcv.h
************************************************************************************************
*   Project/Product : All
*   Title           : CanTrcv module header File
*   Author          : sijia.hao
************************************************************************************************
*   Description     : 
*
************************************************************************************************
*   Limitations     : only used for MSCAN
*
************************************************************************************************
*
************************************************************************************************
*   Revision History:
* 
*   Version     Date          Initials      CR#          Descriptions
*   ---------   ----------    ------------  ----------   ---------------
*   1.0         2014/06/01    sijia.hao     N/A          Original
*
************************************************************************************************
* END_FILE_HDR*/
#ifndef _CANTRCV_H_
#define _CANTRCV_H_


#include "Std_Types.h"

#define CAN_TRSCV_TYPE    (1U)   /* 0: TLE6250;  1: TLE6251DS /TJA1040 */

void CanTrcv_Init(void);
void CanTrcv_Disable(void);
void CanTrcv_Enable(void);

#endif
