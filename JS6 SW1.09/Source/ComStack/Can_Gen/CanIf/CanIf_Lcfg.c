/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : CanIf_Lcfg.c
*
********************************************************************************
*   Project/Product : AUTOSAR 4.0
*   Title           : CAN Interface module configuration File
*   Author          : Hirain
********************************************************************************
*   Description     : for link time configurable parameters.
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
*   02.02.04    15/08/2013    NingChen      N/A          D10_CANIF_130815_01
*
********************************************************************************
* END_FILE_HDR*/



/*******************************************************************************
*   Includes 
*******************************************************************************/
#include "CanIf.h"
#include "Com_Cbk.h"
#include "CanTp_Cbk.h"
#include "OsekNm.h"
//#include "CanNm.h"

/*******************************************************************************
*   Macro
*******************************************************************************/
#define CANIF_CFG_TOTAL_TXPDUID                      (2u)

#define CANIF_CFG_TOTAL_RXPDUID                      (4u)

#if (STD_ON == CANIF_RX_PDU_RANGE_CANID)
    #define CANIF_CFG_RANGE_CANID_RXPDUID        (1u)
#endif

#define CANIF_CFG_TOTAL_HRH                          (1u)

/*******************************************************************************
*   Global Data Define, No need to be configed
*******************************************************************************/
#define CANIF_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
    VAR(boolean, CANIF_PRIVATE_DATA) 
                              CanIfTxBufferPDUFillflag[CANIF_CFG_TOTAL_TXPDUID];

    VAR(uint8, CANIF_PRIVATE_DATA) CanIfTxBufferSdu[CANIF_CFG_TOTAL_TXPDUID][8];

    VAR(Can_PduType, CANIF_PRIVATE_DATA) 
                                      CanIfTxBufferPdu[CANIF_CFG_TOTAL_TXPDUID];
#endif

#if (STD_ON == CANIF_PUBLIC_SETDYNAMICTXID_API)
    VAR(Can_IdType, CANIF_PRIVATE_DATA) 
                             CanIfTxPduCanIdForDynamic[CANIF_CFG_TOTAL_TXPDUID]; 
#endif 

#if(STD_ON == CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API)
    VAR(CanIf_NotifStatusType, CANIF_PRIVATE_DATA)
                                    CanIfTxNotifStatus[CANIF_CFG_TOTAL_TXPDUID];      
#endif

#if(STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
    VAR(CanIf_NotifStatusType, CANIF_PRIVATE_DATA)
                                    CanIfRxNotifStatus[CANIF_CFG_TOTAL_RXPDUID];   
#endif

#if (STD_ON == CANIF_PUBLIC_READRXPDU_DATA_API)
    VAR(uint8, CANIF_PRIVATE_DATA) 
                               CanIfRxBufferSduData[CANIF_CFG_TOTAL_RXPDUID][8]; 
    VAR(PduLengthType, CANIF_PRIVATE_DATA) 
                               CanIfRxBufferSduLength[CANIF_CFG_TOTAL_RXPDUID] ;        
#endif

#define CANIF_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"
CONST(PduIdType, CANIF_CONFIG_CONST) CanIfInitNumberOfCanRxPduIds 
                                                      = CANIF_CFG_TOTAL_RXPDUID;
CONST(PduIdType, CANIF_CONFIG_CONST) CanIfInitNumberOfCanTxPduIds 
                                                      = CANIF_CFG_TOTAL_TXPDUID; 
CONST(Can_HwHandleType, CANIF_CONFIG_CONST) CanIfInitNumberofHrh 
                                                          = CANIF_CFG_TOTAL_HRH;  

/*******************************************************************************
*   Config Parameters   
*******************************************************************************/

#if (STD_ON == CANIF_RX_PDU_RANGE_CANID)
CONST(CanIf_RxPduCanIdRangeType, CANIF_CONFIG_CONST)
                     CanIfRxPduCanIdRangeConfig[CANIF_CFG_RANGE_CANID_RXPDUID]=
{
    {
        0x400,   /* CanIfRxPduCanIdRangeLowerCanId*/
        0x47F,   /* CanIfRxPduCanIdRangeLowerCanId*/
       (Range_RxIndication)CanNm_RxIndication  /* CanNm_RxIndication*/
    }
};
#endif

/*M3 接收信号配置*/
CONST(CanIf_RxPduConfigType, CANIF_CONFIG_CONST)
                                     CanIfRxPduConfig[CANIF_CFG_TOTAL_RXPDUID] =
{

    /* 0*/   /* CanIfRxPduId*/ 
    {
        #if (STD_ON == CANIF_UPPER_LAYER_HANDLE)
        0,             /* CanIfRxPduUpperLayerHandle */
        #endif
        0x2CD,         /* CanId*/
        STANDARD_CAN,  /* CanIdType */
        0,             /*CanIfRxPduHrhIdRef*/
        8,             /* CanIfRxPduDlc */
        Com_RxIndication,      /* CanIfRxPduUserRxIndicationName */
        #if(STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
        FALSE,         /* CanIfRxPduReadNotifyStatus */
        #endif
        #if(STD_ON == CANIF_PUBLIC_READRXPDU_DATA_API)
        FALSE,         /* CanIfRxPduReadData */
        #endif
        #if(STD_ON == CANIF_RX_PDU_RANGE_CANID)
        NULL_PTR,      /* CanIfRxPduRangePtr */
        #endif
        #if(CANIF_FILTER_TYPE_INDEX == CANIF_PRIVATE_SOFTWARE_FILTER_TYPE)
        1,      /*CanIfRxPduNextHrh*/
        #endif

    },
    /* 1*/   /* CanIfRxPduId*/ 
    {
        #if (STD_ON == CANIF_UPPER_LAYER_HANDLE)
        1,             /* CanIfRxPduUpperLayerHandle */
        #endif
        0x117,         /* CanId*/
        STANDARD_CAN,  /* CanIdType */
        0,             /*CanIfRxPduHrhIdRef*/
        8,             /* CanIfRxPduDlc */
        Com_RxIndication,      /* CanIfRxPduUserRxIndicationName */
        #if(STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
        FALSE,         /* CanIfRxPduReadNotifyStatus */
        #endif
        #if(STD_ON == CANIF_PUBLIC_READRXPDU_DATA_API)
        FALSE,         /* CanIfRxPduReadData */
        #endif
        #if(STD_ON == CANIF_RX_PDU_RANGE_CANID)
        NULL_PTR,      /* CanIfRxPduRangePtr */
        #endif
        #if(CANIF_FILTER_TYPE_INDEX == CANIF_PRIVATE_SOFTWARE_FILTER_TYPE)
        2,      /*CanIfRxPduNextHrh*/
        #endif

    },   

    /* 2*/   /* CanIfRxPduId*/ 
    {
        #if (STD_ON == CANIF_UPPER_LAYER_HANDLE)
        0,             /* CanIfRxPduUpperLayerHandle */
        #endif
        0x786,         /* CanId*/
        STANDARD_CAN,  /* CanIdType */
        0,             /*CanIfRxPduHrhIdRef*/
        8,             /* CanIfRxPduDlc */
        CanTp_RxIndication,      /* CanIfRxPduUserRxIndicationName */
        #if(STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
        FALSE,         /* CanIfRxPduReadNotifyStatus */
        #endif
        #if(STD_ON == CANIF_PUBLIC_READRXPDU_DATA_API)
        FALSE,         /* CanIfRxPduReadData */
        #endif
        #if(STD_ON == CANIF_RX_PDU_RANGE_CANID)
        NULL_PTR,      /* CanIfRxPduRangePtr */
        #endif
        #if(CANIF_FILTER_TYPE_INDEX == CANIF_PRIVATE_SOFTWARE_FILTER_TYPE)
        3,      /*CanIfRxPduNextHrh*/
        #endif

    },

    /* 7*/   /* CanIfRxPduId*/ 
    {
        #if (STD_ON == CANIF_UPPER_LAYER_HANDLE)
        1,             /* CanIfRxPduUpperLayerHandle */
        #endif
        0x7df,         /* CanId*/
        STANDARD_CAN,  /* CanIdType */
        0,             /*CanIfRxPduHrhIdRef*/
        8,             /* CanIfRxPduDlc */
        CanTp_RxIndication,      /* CanIfRxPduUserRxIndicationName */
        #if(STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
        FALSE,         /* CanIfRxPduReadNotifyStatus */
        #endif
        #if(STD_ON == CANIF_PUBLIC_READRXPDU_DATA_API)
        FALSE,         /* CanIfRxPduReadData */
        #endif
        #if(STD_ON == CANIF_RX_PDU_RANGE_CANID)
        NULL_PTR,      /* CanIfRxPduRangePtr */
        #endif
        #if(CANIF_FILTER_TYPE_INDEX == CANIF_PRIVATE_SOFTWARE_FILTER_TYPE)
        CANIF_LAST_HRH_RXPDUID,      /*CanIfRxPduNextHrh*/
        #endif

    }
};
#if(CANIF_FILTER_TYPE_INDEX == CANIF_PRIVATE_SOFTWARE_FILTER_TYPE)
CONST(Can_HwHandleType, CANIF_CONFIG_CONST)
                                     CanIfHrhFirstRxPduID[CANIF_CFG_TOTAL_HRH] =
{
    /* HRH-0 */
    0,
};
#endif

#if(STD_ON == CANIF_HRH_RANGE_FILTER)
CONST(CanIf_HrhRangeCfgType, CANIF_CONFIG_CONST)
                                         CanIfHrhRangeCfg[CANIF_CFG_TOTAL_HRH] =
{
     /*HRH-0*/
     {
         0x13,  			/*CanIfHrhRangeRxPduLowerCanId*/
         0x7df,  			/*CanIfHrhRangeRxPduUpperCanId */
         STANDARD_CAN   			/* CanIfHrhRangeRxPduRangeCanIdType */
     },
};
#endif

/*M3 发送信号配置*/
CONST(CanIf_TxPduConfigType, CANIF_CONFIG_CONST)
                                     CanIfTxPduConfig[CANIF_CFG_TOTAL_TXPDUID] =
{
    /*0*/       /* CanIfTxPduId*/
    {
        #if (STD_ON == CANIF_UPPER_LAYER_HANDLE)
        0, 							/* CanIfTxPduUpperLayerHandle*/
        #endif
        0x36A, 						/* CanId*/
        STANDARD_CAN,				/* CanIdType */
        1,							/* CanIfTxPduHthIdRef */
        8,							/* CanIfTxPduDLC */
        Com_TxConfirmation,      /* CanIfRxPduUserRxIndicationName */
        #if (STD_ON == CANIF_PUBLIC_SETDYNAMICTXID_API)
        STATIC_TXPDU,           /* CanIfTxPduType */
        #endif
        #if(STD_ON == CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API)
        FALSE,    			/* CanIfTxPduReadNotifyStatus */
        #endif
    },
    /*1*/       /* CanIfTxPduId*/
    {
        #if (STD_ON == CANIF_UPPER_LAYER_HANDLE)
        0, 							/* CanIfTxPduUpperLayerHandle*/
        #endif
        0x78E, 						/* CanId*/
        STANDARD_CAN,				/* CanIdType */
        1,							/* CanIfTxPduHthIdRef */
        8,							/* CanIfTxPduDLC */
        CanTp_TxConfirmation,      /* CanIfRxPduUserRxIndicationName */
        #if (STD_ON == CANIF_PUBLIC_SETDYNAMICTXID_API)
        STATIC_TXPDU,           /* CanIfTxPduType */
        #endif
        #if(STD_ON == CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API)
        FALSE,    			/* CanIfTxPduReadNotifyStatus */
        #endif
    },
    
};

#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"
