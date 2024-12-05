/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : OsekNm.c
********************************************************************************
*   Project/Product : OsekNm programme
*   Title           : OsekNm.c
*   Author          : yzhai
*                     bo.liu
*                     peng.liu
*                     xiang.li1
*                     jingyun.zhang
********************************************************************************

*   Description     : The configuration codes of OSEKNM module                    
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
*   01.05.00    18/06/2013    jingyun.zhang N/A          HrAsrOsekNm130618-01
*   02.00.00    10/03/2014    rui.guo       N/A          HrAsrOsekNm140310-01
********************************************************************************
* END_FILE_HDR*/

/*******************************************************************************
*   Includes 
*******************************************************************************/

#include "OsekNm_Cfg.h"

/*const configuaration array*/

#define  OSEKNM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"/* PRQA S 5087 */

CONST(nmChannelConfigUnion,OSEKNM_CONFIG_CONST) nmChannelConfigAry[NM_NUMBER_OF_CHANNELS]  =
{
    (uint8)CN0_CONFIGURATION,

};/*Configuration for each channel*/

/*NM Channel  to Can(Logic can controller) */
CONST(uint8,OSEKNM_CONFIG_CONST) NmToLogCanChnAry[NM_NUMBER_OF_CHANNELS] = 
{
    0
};
/*Can (Logic can controller) to NM Channel */
CONST(uint8,OSEKNM_CONFIG_CONST) LogCanToNmChnAry[NM_CANIF_USED_CONTROLLER_NUM] =  
{
    0
};

/*NM TxPDUID in CanIfTxPduConfig*/
CONST(PduIdType,OSEKNM_CONFIG_CONST) nmTxPduIdAry[NM_NUMBER_OF_CHANNELS] =   
{
    1  
};

/*the node ID for different channel.*/
CONST(nmNetIdType,OSEKNM_CONFIG_CONST) nmChannelNodeID[NM_NUMBER_OF_CHANNELS] =
{
    0x26 /* d50 3, FOR F60  5*/
};

/* NM Basic ID in the network */
CONST(uint32,OSEKNM_CONFIG_CONST) nmPduBasicIDAry[NM_NUMBER_OF_CHANNELS] =
{
    0x400UL
};

/* NM Message idbase mask*/
CONST(uint32,OSEKNM_CONFIG_CONST) nmPduBasicIDMaskAry[NM_NUMBER_OF_CHANNELS] = 
{
    0x400UL
};

/*NM Message source id mask*/
CONST(uint32,OSEKNM_CONFIG_CONST) nmPduSouceIDMaskAry[NM_NUMBER_OF_CHANNELS] =
{
    0x7fUL
};

/* Typical time interval between two ring messages */
CONST(nmTimerCntType,OSEKNM_CONFIG_CONST) nmCountTypAry[NM_NUMBER_OF_CHANNELS] =
{
    100
};

/* Maximum time interval between two ring messages */
CONST(nmTimerCntType,OSEKNM_CONFIG_CONST) nmCountMaxAry[NM_NUMBER_OF_CHANNELS] =   
{
    260 /*260*/
};

/* time Interval between two ring messages with NMLimpHome ID */
CONST(nmTimerCntType,OSEKNM_CONFIG_CONST) nmCountErrAry[NM_NUMBER_OF_CHANNELS] =  
{
    1000
};

/* time the NM waits before transition in NMBusSleep */
CONST(nmTimerCntType,OSEKNM_CONFIG_CONST) nmCountWbsAry[NM_NUMBER_OF_CHANNELS] =   
{
    1500 /*5000*/
};

CONST(nmTimerCntType,OSEKNM_CONFIG_CONST) nmCountTxAry [NM_NUMBER_OF_CHANNELS] =  
{
    30
};


CONST(nmTimerCntType,OSEKNM_CONFIG_CONST) 
    nmCountBusoffFastTimerAry[NM_NUMBER_OF_CHANNELS] = 
{
    50
};


CONST(nmTimerCntType,OSEKNM_CONFIG_CONST) 
    nmCountBusoffSlowTimerAry[NM_NUMBER_OF_CHANNELS] = 
{
    1000
};
/*Fast Recover times*/
CONST(uint16,OSEKNM_CONFIG_CONST) 
    nmBusOffFastRecoveryAry[NM_NUMBER_OF_CHANNELS] =
{
    10
};
/*Slow Recover times*/ 
CONST(uint16,OSEKNM_CONFIG_CONST) 
    nmBusOffSlowRecoveryAry[NM_NUMBER_OF_CHANNELS] = 
{
    0xffff
};
CONST(nmRxTxCntType,OSEKNM_CONFIG_CONST) nmRxLimitAry[NM_NUMBER_OF_CHANNELS] = 
{
    4
};
CONST(nmRxTxCntType,OSEKNM_CONFIG_CONST) nmTxLimitAry[NM_NUMBER_OF_CHANNELS] = 
{
    8
};

/******************************************************************************* 
* Name:        nmConfig
* Description: 
* Values:      
*******************************************************************************/
CONST(uint8,OSEKNM_CONFIG_CONST) nmNumberOfCfgNodeAry[NM_NUMBER_OF_CHANNELS] = 
{
    NM_CHN0_NUMBER_OF_CONFIG_NODES,
};
STATIC VAR(uint8,OSEKNM_CONFIG_DATA) 
    nmChn0NormalCfgRecordAry[NM_CHN0_NUMBER_OF_CFG_BYTE];
//STATIC VAR(uint8, OSEKNM_CONFIG_DATA) 
//    nmChn1NormalCfgRecordAry[NM_CHN1_NUMBER_OF_CFG_BYTE];

CONSTP2VAR(uint8,OSEKNM_CONFIG_DATA,OSEKNM_CONFIG_DATA) nmNormalCfgPtrAry[NM_NUMBER_OF_CHANNELS] = 
{
    nmChn0NormalCfgRecordAry
};

STATIC VAR(uint8,OSEKNM_CONFIG_DATA) 
    nmChn0LimpHomeCfgRecordAry[NM_CHN0_NUMBER_OF_CFG_BYTE];

//STATIC VAR(uint8,OSEKNM_CONFIG_DATA) 
//    nmChn1LimpHomeCfgRecordAry[NM_CHN1_NUMBER_OF_CFG_BYTE];

CONSTP2VAR(uint8,OSEKNM_CONFIG_DATA,OSEKNM_CONFIG_DATA) nmLimpHomeCfgPtrAry[NM_NUMBER_OF_CHANNELS] = 
{                               
    nmChn0LimpHomeCfgRecordAry

};

#if defined (NM_FOR_FAW) 
#if(T_ACTIVE_MINI_ON == STD_ON) /*OsekNm_013*/
const nmTimerCntType nmCountActMinAry[NM_NUMBER_OF_CHANNELS] = {100};/*Timer:4000ms*/
#endif
#if(T_LIMPHOME_MINI_ON == STD_ON)
const nmTimerCntType nmCountTLimpHAry[NM_NUMBER_OF_CHANNELS] = {200};/*Timer:4000ms*/
#endif
#endif
#define  OSEKNM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"/* PRQA S 5087 */

