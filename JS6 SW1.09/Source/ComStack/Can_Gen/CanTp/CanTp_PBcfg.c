/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       CanTp_PBcfg.c
********************************************************************************
*   Project/Product : CanTp programme
*   Title:          : CanTp_PBcfg.c
*   Author:         : donger.yang
********************************************************************************
*   Description:      Defination of Interface for CAN driver and other modes
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
*   02.02.xx  
********************************************************************************
*END_FILE_HDR*/


/*******************************************************************************
*   Includes
*******************************************************************************/
#include "CanTp.h"
#include "CanTp_Cfg.h"



const CanTp_RxNsduCfgType CanTp_RxNsduCfg[];
const CanTp_TxNsduCfgType CanTp_TxNsduCfg[];
const CanTpChannelModeType CanTpChannelMode[CANTP_CHANNELS];
const CanTp_CfgType CanTp_Cfg = 
{   
    CanTp_RxNsduCfg /* Pointer to RxNsdu-cfg-table */, 
    CanTp_TxNsduCfg /* Pointer to TxNsdu-cfg-table */, 
    CanTpChannelMode /* Extended SDU indirection table */,  
};


const CanTp_RxNsduCfgType CanTp_RxNsduCfg[CANTP_NUM_RX_SDUS] = 
{
{ 
    0 /*Tp Channel Num */, 
    0 /* CANTP_RXNPDU_ID; from  CanIf */, 
    2 /* CANTP_TXFC_NPDU_ID; to CanIf */, 
    1u /* CANTP_DL */, 
    100,//70 /* CANTP_NAR */, 
    70 /* CANTP_NBR */, 
    1000,//150 /* CANTP_NCR */,    
    CANTP_ON /* CANTP_PADDING_ACTIVATION */, 
    CANTP_PHYSICAL /* CANTP_TA_TYPE */,  
    CANTP_STANDARD /* CANTP_ADDRESSING_FORMAT */, 
    0,//8 /* CANTP_BS */,
    20 /* CANTP_STMIN */,
    1 /* CANTP_WFTMAX */,
    CANTP_UNUSED /* CANTP_RX_NAE */,
    CANTP_UNUSED /* CANTP_RX_NSA */, 
    CANTP_UNUSED /* CANTP_RX_NTA */, 
},
{ 
    0 /*Tp Channel Num */, 
    1 /* CANTP_RXNPDU_ID; from  CanIf */, 
    2 /* CANTP_TXFC_NPDU_ID; to CanIf */, 
    1u /* CANTP_DL */, 
    100,//70 /* CANTP_NAR */, 
    70 /* CANTP_NBR */, 
    1000,//150 /* CANTP_NCR */,    
    CANTP_ON /* CANTP_PADDING_ACTIVATION */, 
    CANTP_FUNCTIONAL /* CANTP_TA_TYPE */,  
    CANTP_STANDARD /* CANTP_ADDRESSING_FORMAT */, 
    0,//8 /* CANTP_BS */,
    20 /* CANTP_STMIN */,
    1 /* CANTP_WFTMAX */,
    CANTP_UNUSED /* CANTP_RX_NAE */,
    CANTP_UNUSED /* CANTP_RX_NSA */, 
    CANTP_UNUSED /* CANTP_RX_NTA */, 
}
};



const CanTp_TxNsduCfgType CanTp_TxNsduCfg[CANTP_NUM_TX_SDUS] = 
{
	{
		0 /*Tp Channel Num */, 
		2 /* CANTP_TXNPDU_ID; to CanIf */,  
		0 /* CANTP_RXFC_NPDU_ID; from CanIf */,
		1 /* CANTP_DL */, 
	  100,//70 /* CANTP_NAS */, 
		1000,//150 /* CANTP_NBS */, 
		70 /* CANTP_NCS */,  
		CANTP_ON /* CANTP_PADDING_ACTIVATION */, 
    CANTP_PHYSICAL /* CANTP_TA_TYPE */,  
    CANTP_STANDARD /* CANTP_ADDRESSING_FORMAT */, 
    CANTP_UNUSED /* CANTP_RX_NAE */,
		CANTP_UNUSED /* CANTP_TX_NSA */, 
		CANTP_UNUSED /* CANTP_TX_NTA */, 
	}



};




const CanTpChannelModeType CanTpChannelMode[CANTP_CHANNELS]=
{
  {
		CANTP_MODE_HALF_DUPLEX
  },
};

