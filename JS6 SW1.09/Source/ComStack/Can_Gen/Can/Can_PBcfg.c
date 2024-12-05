/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Can_PBcfg.c
*
********************************************************************************
*   Project/Product : AUTOSAR 4.0
*   Title           : CAN Driver(MSCAN) module configuration File
*   Author          : Hirain
********************************************************************************
*   Description     : This file contains the post-build time configurable 
*                     parameters of CAN Driver                       
*
********************************************************************************
*   Limitations     : None
*
********************************************************************************
*
********************************************************************************
*   Revision History:
* 
*   Version     Date          Initials      CR#          Descriptions
*   ---------   ----------    ------------  ----------   ---------------
*   02.00.01    15/08/2013    NingChen      N/A          D10_MSCAN_130815_1
*
********************************************************************************
* END_FILE_HDR*/


/*******************************************************************************
*   Includes 
*******************************************************************************/
#include "Can.h"
#include "mf_config.h"
#include "fm33lg0xx_fl.h"
/*******************************************************************************
*   Config Parameters   
*******************************************************************************/
#define CAN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"

CONST(uint8, CAN_CONFIG_CONST) CanControllerIDtoPhys[CAN_USED_CONTROLLER_NUM]=
{
    #if (CAN_USED_CONTROLLER_NUM >= 1)
        0u,
    #endif
};


CONST(Can_RegInitType, CAN_CONFIG_CONST) CanRegInit[CAN_USED_CONTROLLER_NUM] =
{
  #if (CAN_USED_CONTROLLER_NUM >= 1)
  {
  /* 波特率预分频 */
    0x00000000ul,
    /* 波特率设置= CAN_CLK/(BRP+1)/(TS1_Tq+TS2_Tq+1);  8M/(0+1)/(12+3+1)=500K */
    /* 采样点 = (TS1_Tq+1)/(TS1_Tq+TS2_Tq+1+1);  (12+1)/(12+3+1) = 81.75% */
    FL_CAN_SJW_2Tq << 7 | FL_CAN_TS2_3Tq << 4 | FL_CAN_TS1_12Tq,
    CAN_ICR_CRXNEMP_Msk | CAN_ICR_CBSOFF_Msk | CAN_IER_TXOKIE_Msk,
    CAN_AFR_UAF_Msk,
    /*CanInitAFMR1*/
    0x000,
    /*CanInitAFIR1*/
    0x000,

    0x700,
    0x400,

    0x700,
    0x500,

    0x700,
    0x700,
  },
  #endif
};


CONST(Can_HardwareObjectConfigType, CAN_CONFIG_CONST)
                                     CanHardwareObjectConfig[CAN_USED_HOH_NUM] =
{
   {
        /* CanObjectId0*/
        0,
        0,
        CAN_HOH_RX_MASK|CAN_HOH_BASIC_MASK,
    },
   {
        /* CanObjectId1*/
        0,
        0,
        CAN_HOH_TX_MASK|CAN_HOH_BASIC_MASK,
    },
   {
        /* CanObjectId2*/
        0,
        1,
        CAN_HOH_TX_MASK|CAN_HOH_BASIC_MASK,
    },
   {
        /* CanObjectId2*/
        0,
        2,
        CAN_HOH_TX_MASK|CAN_HOH_BASIC_MASK,
    },
};

CONST(Can_HwHandleType, CAN_CONFIG_CONST)
                                 CanControllerIDtoHRH[CAN_USED_CONTROLLER_NUM] =
{
    0,
};

#define CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"

