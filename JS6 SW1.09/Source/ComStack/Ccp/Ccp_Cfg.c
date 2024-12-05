/*  BEGIN_FILE_HDR
********************************************************************************
*  NOTICE
*  This software is the property of HiRain Technologies. Any information 
*  contained in this doc should not be reproduced, or used, or disclosed 
*  without the written authorization from HiRain Technologies.
********************************************************************************
*  File Name      : Ccp_Cfg.c
********************************************************************************
*  Project/Product: CCP module
*  Title          : Ccp_Cfg.c
*  Author         : Peng.Liu
********************************************************************************
*  Description    : This file includes the configuration codes of CCP module.
*
********************************************************************************
*  Limitations    :None
* 
********************************************************************************
* 
********************************************************************************
*  Revision History:
*                                                                     
*   Version       Date         Initials      CR#          Descriptions
*  ---------   ----------    ------------  ----------   ---------------
*   02.20.00    17/02/14      Wenrui.Li     N/A          HrAsrCcp140217-01 
********************************************************************************
*  END_FILE_HDR*/
/******************************************************************************* 
*  Includes 
*******************************************************************************/
#include "Ccp.h"

/******************************************************************************* 
*  Configuration  
*******************************************************************************/
#define CCP_START_SEC_CONFIG_DATA_UNSPECIFIED  
#include "MemMap.h" 
CONST(uint8,CCP_CONFIG_CONST) Ccp_Station_ID[CCP_ECU_NAME_LENGTH] = 
{
    CCP_ECU_NAME    /*The name of slave device*/
};
#define CCP_STOP_SEC_CONFIG_DATA_UNSPECIFIED  
#include "MemMap.h"  
 
 
