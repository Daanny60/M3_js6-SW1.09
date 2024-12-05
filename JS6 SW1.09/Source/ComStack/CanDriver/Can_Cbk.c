/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       Can_Cbk.c
********************************************************************************
*   Project/Product: CAN programme
*   Title:           Can_Cbk.c
*   Author:          sijia.hao
********************************************************************************
*   Description:     Defination of Interface for Application Layer
* 
********************************************************************************
*   Limitations:      None
*
********************************************************************************
*
********************************************************************************
*   Revision History:
* 
*   Version       Date    Initials      CR#     Descriptions
*   -------   ----------  ----------    ------  -------------------
*   01.00.00
********************************************************************************
*END_FILE_HDR*/
/*******************************************************************************
*   Includes
*******************************************************************************/
#include "Can.h"
#include "CanIf_Cfg.h"

#if(STD_ON == CANIF_SUPPORT_OSEK_NM) 
#endif

void CanNm_ControllerPassive(uint8 Controller)
{
#if(STD_ON == CANIF_SUPPORT_OSEK_NM)  

#endif   
    
}
void CanNm_ControllerActive(uint8 Controller)
{
#if(STD_ON == CANIF_SUPPORT_OSEK_NM)  

#endif   
    
}



































