/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       CanTp.c
********************************************************************************
*   Project/Product : CanTp programme
*   Title:          : CanTp.c
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
*   01.00.00  05/24/2010  qiuying.ma    N/A     Original
*   01.01.00  01/08/2013  rui.guo       N/A     HrAsrCanTp130108-01
*   01.02.00  04/12/2013  donger.yang   N/A     HrAsrCanTp130412-01
*   01.02.01  07/17/2013  donger.yang   N/A     HrAsrCanTp130717-01
*   02.02.02  08/13/2013  donger.yang   N/A     HrAsrCanTp130813-01
*   02.02.03  08/25/2013  donger.yang   N/A     HrAsrCanTp130825-01
*   02.02.04  08/27/2013  donger.yang   N/A     HrAsrCanTp130827-01
*   02.02.05  09/03/2013  donger.yang   N/A     HrAsrCanTp130903-01
********************************************************************************
*END_FILE_HDR*/

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "fm33lg0xx_fl.h"
#include "CanTp.h"
#include "CanTp_Cbk.h"
#include "CanTp_Cfg.h"



/*******************************************************************************
*   Internal data types and structures
*******************************************************************************/
/* Rx-status (RAM) */
typedef struct 
{
    PduIdType       CurrentRxSduId; 
    uint8           CurrentRxFormatOffset;
    uint8           ChannelState;/* activation status */ 
    uint16          Timer;
    PduInfoType     RxNPduInfo;
    PduLengthType   DataIndex;  
    PduLengthType   DataLength;
    PduLengthType   FrameIndex;
    PduLengthType   AvailableBufLength;
    uint8           WFTCounter;
    uint8           BlockSizeCounter;  /* actual values */
    uint8           BlockSize;
    uint8           STMin; 
    uint8           FlowStatus:4;
    uint8           ExpectedSN:4;
    uint8           ChannelLastState;
    uint8           RxPreCancel;  
    uint8           RxErrorType;  
} CanTp_RxStateType;

/* Tx-status (RAM) */
typedef struct 
{
    PduIdType       CurrentTxSduId; 
    uint8           CurrentTxFormatOffset;
    uint8           ChannelState;
    uint16          Timer;
    PduInfoType     TxNPduInfo;
    PduLengthType   DataIndex;
    PduLengthType   DataLength;
    PduLengthType   FrameIndex;
    PduLengthType   RemainTxDataCnt;
    PduLengthType   PaddingLength;
    uint8           BlockSizeCounter;
    uint8           STMin;
    uint8           SeqNumber:4;
    uint8           Res:4;
    uint8           TxErrorType;
} CanTp_TxStateType;
/*******************************************************************************
*   Macro definitions 
*******************************************************************************/
/* Internal and iso 15765-2 compliant */
/* CanTp_FrameType */
#define cCanTp_SF                   ((uint8)0x00)
#define cCanTp_FF                   ((uint8)0x10)
#define cCanTp_CF                   ((uint8)0x20)
#define cCanTp_FC                   ((uint8)0x30)
#define cCanTp_FC_FS_CTS            ((uint8)0x00)
#define cCanTp_FC_FS_Wait           ((uint8)0x01)
#define cCanTp_FC_FS_Overrun        ((uint8)0x02)

/* Masks for frame parsing */
#define cCanTp_DLMaskSF             ((uint8)0x0F)
#define cCanTp_DLMaskFF             ((uint8)0x0F)
#define cCanTp_FrameTypeMask        ((uint8)0xF0)
#define cCanTp_FSMask               ((uint8)0x0F)
#define cCanTp_SNMask               ((uint8)0x0F)
/* Start value for sequence number counter*/
#define cCanTpSNStartValue          ((uint8)0x01)

/*******************************************************************************
*   Transport protocol global status codes
*******************************************************************************/
/* CanTp_RxStateType */
#define cCanTpRxState_Idle                          ((uint8)0x00) 
#define cCanTpRxState_WaitCF                        ((uint8)0x01)
#define cCanTpRxState_WaitForTxFC                   ((uint8)0x02)
#define cCanTpRxState_WaitForTxFCOverrun            ((uint8)0x03)
#define cCanTpRxState_WaitForTxFCOverrunFFReqBuff   ((uint8)0x04)

#define cCanTpRxState_WaitForTxFCWait               ((uint8)0x05)
#define cCanTpRxState_WaitForTxFCWaitFFReqBuff      ((uint8)0x06)

#define cCanTpRxState_WaitForRxBufferSF             ((uint8)0x07)
#define cCanTpRxState_WaitForRxBufferFF             ((uint8)0x08)
#define cCanTpRxState_WaitForRxBufferLastCF         ((uint8)0x09)
#define cCanTpRxState_WaitForRxBufferLastCFBlock    ((uint8)0x0a)
#define cCanTpRxState_WaitForRxBufferGoingCF        ((uint8)0x0b)


#define cCanTpRxState_WaitForSFRequestBuffer        ((uint8)0x0c)
#define cCanTpRxState_WaitForFFRequestBuffer        ((uint8)0x0d)
#define cCanTpRxState_NoBuffer                      ((uint8)0x0e)


#define cCanTpRxState_WaitForFCConf                 ((uint8)0x10)        
#define cCanTpRxState_WaitForFCWaitConf             ((uint8)0x20)
#define cCanTpRxState_WaitForFCWaitConfFFReqBuff    ((uint8)0x30)
#define cCanTpRxState_WaitForFCOverConf             ((uint8)0x40)
#define cCanTpRxState_WaitForFCOverConfFFReqBuff    ((uint8)0x50)

/* CanTp_TxStateType */
#define cCanTpTxState_Idle                     ((uint8)0x00)
#define cCanTpTxState_WaitFC                   ((uint8)0x01)
#define cCanTpTxState_WaitForTpTxCF            ((uint8)0x02)
#define cCanTpTxState_WaitForTxStart           ((uint8)0x03)
#define cCanTpTxState_Error                    ((uint8)0x04)

#define cCanTpTxState_ReCopy                   ((uint8)0x08)

#define cCanTpTxState_WaitForSFConf            ((uint8)0x10)
#define cCanTpTxState_WaitForFFConf            ((uint8)0x20)
#define cCanTpTxState_WaitForCFConf            ((uint8)0x30)
#define cCanTpTxState_WaitForLastCFConf        ((uint8)0x40)

#define cCanTpState_Retransmit                 ((uint8)0x80)

/*******************************************************************************
*   TP ISO offsets in CAN-frame
*******************************************************************************/
#define TARGET_OFFSET                       ((uint8)0)
#define AE_OFFSET                           ((uint8)0)

/*******************************************************************************
*   TP ISO offsets in RX CAN-frame and protocol lengths
*******************************************************************************/
#define RX_TPCI_OFFSET(CanTpSduId)       (uint8)RxFormatOffset[CanTpSduId]
#define RX_DL_OFFSET(CanTpSduId)         (uint8)(1u+RxFormatOffset[CanTpSduId])
#define RX_FF_OFFSET(CanTpSduId)         (uint8)(2u+RxFormatOffset[CanTpSduId])
#define RX_SF_OFFSET(CanTpSduId)         (uint8)(1u+RxFormatOffset[CanTpSduId])
#define RX_CF_OFFSET(CanTpSduId)         (uint8)(1u+RxFormatOffset[CanTpSduId])
#define RX_BS_OFFSET(CanTpSduId)         (uint8)(1u+RxFormatOffset[CanTpSduId])
#define RX_STMIN_OFFSET(CanTpSduId)      (uint8)(2u+RxFormatOffset[CanTpSduId])
#define RX_SF_DataLength(CanTpSduId)     (uint8)(7u-RxFormatOffset[CanTpSduId])
#define RX_FF_DataLength(CanTpSduId)     (uint8)(6u-RxFormatOffset[CanTpSduId])
#define RX_FC_DataLength(CanTpSduId)     (uint8)(3u+RxFormatOffset[CanTpSduId])
#define RX_CF_DataLength(CanTpSduId)     (uint8)(7u-RxFormatOffset[CanTpSduId])
/*******************************************************************************
*   TP ISO offsets in TX CAN-frame and protocol lengths
*******************************************************************************/
#define TX_NPCI_OFFSET(CanTpSduId)       (uint8)TxFormatOffset[CanTpSduId]
#define TX_DL_OFFSET(CanTpSduId)         (uint8)(1u+TxFormatOffset[CanTpSduId])
#define TX_FF_OFFSET(CanTpSduId)         (uint8)(2u+TxFormatOffset[CanTpSduId])
#define TX_SF_OFFSET(CanTpSduId)         (uint8)(1u+TxFormatOffset[CanTpSduId])
#define TX_CF_OFFSET(CanTpSduId)         (uint8)(1u+TxFormatOffset[CanTpSduId])
#define TX_BS_OFFSET(CanTpSduId)         (uint8)(1u+TxFormatOffset[CanTpSduId])
#define TX_STMIN_OFFSET(CanTpSduId)      (uint8)(2u+TxFormatOffset[CanTpSduId])
#define TX_SF_DataLength(CanTpSduId)     (uint8)(7u-TxFormatOffset[CanTpSduId])
#define TX_FF_DataLength(CanTpSduId)     (uint8)(6u-TxFormatOffset[CanTpSduId])
#define TX_FC_DataLength(CanTpSduId)     (uint8)(3u+TxFormatOffset[CanTpSduId])
#define TX_CF_DataLength(CanTpSduId)     (uint8)(7u-TxFormatOffset[CanTpSduId])

/*******************************************************************************
*   CanTpRxNSdu{RxNsdu}
*******************************************************************************/
#define CANTP_RXTPCHANNEL_ID(CanTpSduId)        CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpChannelId
#define CANTP_RXNPDU_ID(CanTpSduId)             CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxNPduId
#define CANTP_TXFC_NPDU_ID(CanTpSduId)          CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpTxFcNPduId
#define CANTP_RX_DL(CanTpSduId)                 CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxDl
#define CANTP_NAR(CanTpSduId)                   (uint16)(CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpNar/CANTP_MAIN_FUNCTION_PERIOD)
#define CANTP_NBR(CanTpSduId)                   (uint16)(CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpNbr/CANTP_MAIN_FUNCTION_PERIOD)
#define CANTP_NCR(CanTpSduId)                   (uint16)(CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpNcr/CANTP_MAIN_FUNCTION_PERIOD)
#define CANTP_RX_PADDING_ACTIVATION(CanTpSduId) CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxPaddingActive
#define CANTP_RX_TA_TYPE(CanTpSduId)            CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxTaType
#define CANTP_RX_ADDRESSING_FORMAT(CanTpSduId)  CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxAddrFormat
#define CANTP_BS(CanTpSduId)                    CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpBs
#define CANTP_STMIN(CanTpSduId)                 CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpSTmin
#define CANTP_WFTMAX(CanTpSduId)                CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxWftMax
#define CANTP_RX_NAE(CanTpSduId)                CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxNAe
#define CANTP_RX_NSA(CanTpSduId)                CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxNSa
#define CANTP_RX_NTA(CanTpSduId)                CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxNAe
 

/*******************************************************************************
*   CanTpTxNSdu{TxNsdu}
*******************************************************************************/
#define CANTP_TXTPCHANNEL_ID(CanTpSduId)        CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpChannelId
#define CANTP_TXNPDU_ID(CanTpSduId)             CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpTxNPduId
#define CANTP_RXFC_NPDU_ID(CanTpSduId)          CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpRxFcNPduId
#define CANTP_RXFC_TA_TYPE(CanTpSduId)          CanTp_Cfg.CanTp_RxNsduCfg[CanTpSduId].CanTpRxTaType
#define CANTP_TX_DL(CanTpSduId)                 CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpTxDl
#define CANTP_NAS(CanTpSduId)                   (uint16)(CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpNas/CANTP_MAIN_FUNCTION_PERIOD)
#define CANTP_NBS(CanTpSduId)                   (uint16)(CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpNbs/CANTP_MAIN_FUNCTION_PERIOD)
#define CANTP_NCS(CanTpSduId)                   (uint16)(CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpNcs/CANTP_MAIN_FUNCTION_PERIOD)
#define CANTP_TX_PADDING_ACTIVATION(CanTpSduId) CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpTxPaddingActive
#define CANTP_TX_TA_TYPE(CanTpSduId)            CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpTxTaType
#define CANTP_TX_ADDRESSING_FORMAT(CanTpSduId)  CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpTxAddrFormat
#define CANTP_TX_NAE(CanTpSduId)                CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpTxNAe
#define CANTP_TX_NSA(CanTpSduId)                CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpTxNSa
#define CANTP_TX_NTA(CanTpSduId)                CanTp_Cfg.CanTp_TxNsduCfg[CanTpSduId].CanTpTxNAe


#define CANTP_CHANNEL_MODE(CanTpChannel)  CanTp_Cfg.ChannelMode[CanTpChannel].ChannelMode

/*******************************************************************************
*   Gloable Data definitions
*******************************************************************************/
/* Buffer to receive CanTp-frames */
static uint8 CanTp_RxBuf[CANTP_CHANNELS][CANTP_MAX_FRAME_LENGTH];
/* Buffer to store fragments when Tx-buffer is provided in slices */
static uint8 CanTp_TxBuf[CANTP_CHANNELS][CANTP_MAX_FRAME_LENGTH];
/* Administration data in RAM */
/* Channel-, connection-administration data */
static CanTp_RxStateType CanTp_RxState[CANTP_CHANNELS];
static CanTp_TxStateType CanTp_TxState[CANTP_CHANNELS];
static uint8  RxFormatOffset[CANTP_CHANNELS];
static uint8  TxFormatOffset[CANTP_CHANNELS];
static CanTp_AdminDataType CanTp_AdminData;
static RetryInfoType RetryInfoStruct[CANTP_NUM_TX_SDUS];



/*******************************************************************************
*   Prototypes of internal functions
*******************************************************************************/
 static void CanTp_RxMainFunction(PduIdType CanTpRxSduId);
 static void CanTp_TxMainFunction(PduIdType CanTpTxSduId);
 static void CanTp_ChannelInit(PduIdType tpChannel);
 static void CanTp_RxSFPdu(PduIdType RxSduId,uint16 CanTpRxChannelId,uint8* SFDataBuf,uint8 CanDlc);
 static void CanTp_RxFFPdu(PduIdType RxSduId,uint16 CanTpRxChannelId,uint8* FFDataBuf,uint8 CanDlc);
 static void CanTp_RxCFPdu(PduIdType RxSduId,uint16 CanTpRxChannelId,uint8* CFDataBuf,uint8 CanDlc);
 static void CanTp_RxFCPdu(PduIdType RxSduId,uint16 CanTpTxChannelId,uint8* FCDataBuf,uint8 CanDlc);
 static void CanTp_AssembleFC(PduIdType CanTpRxPduId,uint8 FlowStatus);
 static void CanTp_DataCopy( uint8* dest, uint8* src,uint16 cnt);

 
 static void CanTp_SetFormatOffset(CanTpChannelType tpChannelType,PduIdType tpChannel,PduIdType CanTpSduId);

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_ChannelInit
*                
* Description:   This function initializes the CanTp module.
*                                         
* Inputs:        tpChannel: The number of Tp Channel
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
void CanTp_ChannelInit(PduIdType tpChannel)
{
  uint32_t primask;
    /* Init Rx Status */ 
 
    CanTp_CanInterruptDisable();

    {
        RxFormatOffset[tpChannel]=0;
        CanTp_RxState[tpChannel].CurrentRxSduId = 0xFF;
        CanTp_RxState[tpChannel].CurrentRxFormatOffset=0;


        /* activation status */
        CanTp_RxState[tpChannel].Timer=0;
        CanTp_RxState[tpChannel].ChannelState=cCanTpRxState_Idle;
        CanTp_RxState[tpChannel].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[tpChannel][0];
        CanTp_RxState[tpChannel].RxNPduInfo.SduLength=0;

        /* Lengths */
        CanTp_RxState[tpChannel].DataIndex=0;
        CanTp_RxState[tpChannel].DataLength=0;
        CanTp_RxState[tpChannel].FrameIndex=0;

        /* buffer admin */
        CanTp_RxState[tpChannel].AvailableBufLength=0;
        CanTp_RxState[tpChannel].WFTCounter=0;
        /* actual values */
        CanTp_RxState[tpChannel].BlockSizeCounter=0;
        CanTp_RxState[tpChannel].BlockSize=0xFF;
        CanTp_RxState[tpChannel].STMin=0xFF;
        /* Bittypes */
        CanTp_RxState[tpChannel].ExpectedSN=0;

        CanTp_RxState[tpChannel].FlowStatus=cCanTpRxState_Idle;
        CanTp_RxState[tpChannel].RxErrorType=E_OK;
        CanTp_RxState[tpChannel].ChannelLastState=0;
        CanTp_RxState[tpChannel].RxPreCancel = 0;

    }

    /***** Init Tx Status ***********************************************************/  
    {
        TxFormatOffset[tpChannel]=0;    
        CanTp_TxState[tpChannel].CurrentTxSduId = 0xFF;
        CanTp_TxState[tpChannel].CurrentTxFormatOffset=0;
        CanTp_TxState[tpChannel].PaddingLength=0;
        CanTp_TxState[tpChannel].TxErrorType=E_OK;

        /* activation status */
        CanTp_TxState[tpChannel].Timer=0;
        CanTp_TxState[tpChannel].ChannelState=cCanTpTxState_Idle;
        CanTp_TxState[tpChannel].TxNPduInfo.SduDataPtr=&CanTp_TxBuf[tpChannel][0];
        CanTp_TxState[tpChannel].TxNPduInfo.SduLength=0;

        /* Lengths */
        CanTp_TxState[tpChannel].DataIndex=0;
        CanTp_TxState[tpChannel].DataLength=0;
        CanTp_TxState[tpChannel].RemainTxDataCnt=0;
        CanTp_TxState[tpChannel].FrameIndex=0;

        /* actual values */
        CanTp_TxState[tpChannel].BlockSizeCounter=0;
        CanTp_TxState[tpChannel].STMin=0;

        /* Bittypes */
        CanTp_TxState[tpChannel].SeqNumber=0;        
    }

    CanTp_CanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_Init
*                
* Description:   This function initializes the CanTp module.
*                                         
* Inputs:        pCfgPtr: Pointer to the CanTp post-build configuration data
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
void CanTp_Init(const void* const pCfgPtr)
{
    uint8  tpChannel;
    uint8  i;   
 
    CanTp_AdminData.CfgPtr=pCfgPtr;

    if(CanTp_AdminData.InternalState==CANTP_OFF)
    {
        CanTp_AdminData.InternalState=CANTP_ON;
    } 

    for (i=0;i<(uint8)CANTP_NUM_TX_SDUS;    i++)
    {
        RetryInfoStruct[i].TpDataState = TP_DATACONF;
    }

    /* Init channels */   
    for (tpChannel=0;tpChannel<(uint8)CANTP_CHANNELS;tpChannel++)
    {
        (void)CanTp_ChannelInit(tpChannel);
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_Shutdown
*                
* Description:   This function is called to shutdown the CanTp module.
*                                         
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
void CanTp_Shutdown(void)
{
    CanTp_Init(CanTp_AdminData.CfgPtr);
    CanTp_AdminData.InternalState=CANTP_OFF;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_SetFormatOffset
*                
* Description:   This function is called set the offset of application data in
*                in Rx/Tx buffer
*                                         
* Inputs:        tpChannelType: Channel Type(Tx/Rx)
*                tpChannel:     Channel Id
*                CanTpSduId:    SduId
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_SetFormatOffset(CanTpChannelType tpChannelType,PduIdType tpChannel,PduIdType CanTpSduId)
{

    /* RX */    
    if(tpChannelType==CANTP_RX_CHANNEL)
    {
        if( (CANTP_RX_ADDRESSING_FORMAT(CanTpSduId) == CANTP_EXTENDED)||\
            (CANTP_RX_ADDRESSING_FORMAT(CanTpSduId) == CANTP_MIXED) )
        {
            RxFormatOffset[tpChannel]=1u;
        }
        else
        {
            RxFormatOffset[tpChannel]=0;
        }

        CanTp_RxState[tpChannel].CurrentRxSduId = CanTpSduId;
    }

    /* TX */  
    else
    {
        if( (CANTP_TX_ADDRESSING_FORMAT(CanTpSduId) == CANTP_EXTENDED)||\
            (CANTP_TX_ADDRESSING_FORMAT(CanTpSduId) == CANTP_MIXED) )
        {
            TxFormatOffset[tpChannel]=1u;
        }
        else
        {
            TxFormatOffset[tpChannel]=0;
        }

        CanTp_TxState[tpChannel].CurrentTxSduId = CanTpSduId;
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_Transmit
*                
* Description:   This service is used to request the transfer of segmented data
*                                         
* Inputs:        CanTpTxSduId: SduId
*                pData:        Tx data point
*                   
* Outputs:       E_OK:     Operation Accept
*                E_NOT_OK: Operation Failed
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
Std_ReturnType CanTp_Transmit(PduIdType CanTpTxSduId, const PduInfoType* pData)
{
    Std_ReturnType  ErrorStatus=E_OK; 
    PduIdType CanTpTxChannelId;  
    uint32_t primask;

  
    CanTp_CanInterruptDisable();

    CanTpTxChannelId = CANTP_TXTPCHANNEL_ID(CanTpTxSduId);

    if(CanTpTxChannelId>=CANTP_NUM_TX_CHANNELS)
    {
        ErrorStatus = E_NOT_OK;
    }
    else if(CanTp_TxState[CanTpTxChannelId].ChannelState!= cCanTpTxState_Idle)
    {    
        ErrorStatus = E_NOT_OK;
    }
    else
    {

    }

    if(ErrorStatus == E_OK)
    {
        /*N-SDU length less than the minimum data length.*/
        if (pData->SduLength <CANTP_TX_DL(CanTpTxSduId)) 
        {
            ErrorStatus = E_NOT_OK;
        }
        else if(pData->SduLength == 0)
        {
            ErrorStatus = E_NOT_OK;
        }
        else if(pData->SduLength > 4095u)     
        {
            ErrorStatus = E_NOT_OK;
        }
        else if(CANTP_TX_TA_TYPE(CanTpTxSduId)==CANTP_FUNCTIONAL)
        {
            if(pData->SduLength > TX_SF_DataLength(CanTpTxChannelId))
            {
                ErrorStatus = E_NOT_OK;
            }
            else
            {

            }

        }
        else if(CANTP_CHANNEL_MODE(CanTpTxChannelId) == CANTP_MODE_HALF_DUPLEX)
        {
            if (CanTp_RxState[CanTpTxChannelId].ChannelState!= cCanTpRxState_Idle)
            {                                                                         
                ErrorStatus = E_NOT_OK;
            } 
            else
            {
            }
        }
        else
        { 
        }

        if(ErrorStatus == E_OK)
        {
            CanTp_ChannelInit(CanTpTxChannelId);
            CanTp_SetFormatOffset(CANTP_TX_CHANNEL,CanTpTxChannelId,CanTpTxSduId);

            /* check and modify state and timer */
            CanTp_TxState[CanTpTxChannelId].ChannelState = cCanTpTxState_WaitForTxStart;
            CanTp_TxState[CanTpTxChannelId].Timer = 1u;
            CanTp_TxState[CanTpTxChannelId].DataLength=pData->SduLength;
        }
    }
    else
    {
    }

    CanTp_CanInterruptRestore();
       
    return ErrorStatus;

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_RxIndication
*                
* Description:   This function is called by the CAN Interface after a successful 
                 reception of a Rx CAN L-PDU
*                                         
* Inputs:        CanTpRxPduId:   SduId
*                pCanTpRxPduPtr: Rx data information point
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
void CanTp_RxIndication(PduIdType CanTpRxPduId, const PduInfoType* pCanTpRxPduPtr)
{       
    Std_ReturnType  ErrorStatus;
    uint16 CanTpChannelId = CANTP_UNUSED;
    uint8 CanTpReceiveData[8];
    uint8 byteoffset = 0;
    uint32_t primask;


    CanTp_CanInterruptDisable();
    if(CanTpRxPduId<CANTP_NUM_RX_SDUS) 
    { 
        CanTp_DataCopy(&CanTpReceiveData[0], pCanTpRxPduPtr->SduDataPtr,pCanTpRxPduPtr->SduLength);

        /* check CanTp_RxNsduCfg first */    
        CanTpChannelId = CANTP_RXTPCHANNEL_ID(CanTpRxPduId);
        /* Addressing Format is Extended */
        if(CANTP_RX_ADDRESSING_FORMAT(CanTpRxPduId)==CANTP_EXTENDED)
        {
            /*Check Extend Address */
            if(CanTpReceiveData[TARGET_OFFSET]!=CANTP_RX_NTA(CanTpRxPduId))
            {              
                ErrorStatus=E_NOT_OK;
            }
            else
            {
                ErrorStatus = E_OK;
                byteoffset = 1u;
            }
        }    

        /*Addressing Format is Mixed */
        else if(CANTP_RX_ADDRESSING_FORMAT(CanTpRxPduId)==CANTP_MIXED)
        {
            /*Check    AE Address */
            if(CanTpReceiveData[AE_OFFSET]!=CANTP_RX_NAE(CanTpRxPduId)) 
            {
                ErrorStatus = E_NOT_OK;
            }
            else
            {
                ErrorStatus = E_OK;
                byteoffset = 1u;
            }
        }
        else
        {
            ErrorStatus = E_OK;
            byteoffset = 0;
        }                

        if( ErrorStatus == E_OK)
        {    
            switch(CanTpReceiveData[byteoffset] & cCanTp_FrameTypeMask)       
            {                
                case cCanTp_SF:
                    {
                        if(CanTp_RxState[CanTpChannelId].CurrentRxSduId == CanTpRxPduId)
                        {
                            /*Terminate the current reception, report an indication, with 
                            parameter Result set to NTFRSLT_E_UNEXP_PDU, to the upper layer*/
                            CanTp_RxState[CanTpChannelId].RxPreCancel = 1u;
                            CanTp_RxState[CanTpChannelId].CurrentRxFormatOffset = RxFormatOffset[CanTpChannelId];
                        }
                        else
                        {
                            CanTp_RxState[CanTpChannelId].RxPreCancel = 0;
                        }
                    
                        RxFormatOffset[CanTpChannelId]=byteoffset;
                        CanTp_RxSFPdu(CanTpRxPduId,CanTpChannelId,&CanTpReceiveData[0],(uint8)pCanTpRxPduPtr->SduLength);

                        break;
                    }


                case cCanTp_FF:
                    {
                        if(CanTp_RxState[CanTpChannelId].CurrentRxSduId == CanTpRxPduId)
                        {
                            /*Terminate the current reception, report an indication, with parameter 
                            Result set to NTFRSLT_E_UNEXP_PDU, to the upper layer*/
                            CanTp_RxState[CanTpChannelId].RxPreCancel = 1u;

                            /*save the current offset*/
                            CanTp_RxState[CanTpChannelId].CurrentRxFormatOffset = RxFormatOffset[CanTpChannelId];
                        }
                        else
                        {
                            CanTp_RxState[CanTpChannelId].RxPreCancel = 0;
                        }
                    
                        RxFormatOffset[CanTpChannelId]=byteoffset;

                        CanTp_RxFFPdu(CanTpRxPduId,CanTpChannelId,&CanTpReceiveData[0],(uint8)pCanTpRxPduPtr->SduLength);

                        break;
                    }

                case cCanTp_CF:
                    {
                        RxFormatOffset[CanTpChannelId]=byteoffset;
                        CanTp_RxState[CanTpChannelId].CurrentRxSduId = CanTpRxPduId;

                        /*Receive ConsectiveFrame*/
                        CanTp_RxCFPdu(CanTpRxPduId,CanTpChannelId,&CanTpReceiveData[0],(uint8)pCanTpRxPduPtr->SduLength);
                        break;
                    }
               
                case cCanTp_FC:
                    {
                        CanTp_RxFCPdu(CanTpRxPduId,CanTpChannelId,&CanTpReceiveData[0],(uint8)pCanTpRxPduPtr->SduLength);                        
                        break;
                    }

        
                default:
                    {
                        break;
                    }            

            }
        }
        else
        {

        }  
    } 
    else
    {
    //report error here
    }

    CanTp_CanInterruptRestore();      
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_TxConfirmation
*                
* Description:   All transmitted CAN frames(SF, FF, CF, FC) belonging to the CAN 
*                Transport Layer will be confirmed by this function
*                                         
* Inputs:        CanTpTxPduId:   SduId
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
void CanTp_TxConfirmation(PduIdType CanTpTxPduId)
{
    PduIdType CanTpChannelId = CANTP_UNUSED;
        
    if(CanTpTxPduId<CANTP_NUM_TX_SDUS) 
    {
        CanTpChannelId=CANTP_TXTPCHANNEL_ID(CanTpTxPduId);
        /* Tx Status */
        if(CanTp_TxState[CanTpChannelId].ChannelState != cCanTpTxState_Idle) 
        {
            switch(CanTp_TxState[CanTpChannelId].ChannelState)
            {                        
                case cCanTpTxState_WaitForSFConf:
                case cCanTpTxState_WaitForLastCFConf: 
                {
                    /* inform application */
                    CanTp_NUSDataConfirm(CanTpTxPduId,NTFRSLT_OK );
                    CanTp_ChannelInit(CanTpChannelId);
                    break;
                }
                case cCanTpTxState_WaitForFFConf:
                {
                    CanTp_TxState[CanTpChannelId].DataIndex+=TX_FF_DataLength(CanTpChannelId);
                    CanTp_TxState[CanTpChannelId].ChannelState=cCanTpTxState_WaitFC;
                    CanTp_TxState[CanTpChannelId].Timer=CANTP_NBS(CanTpTxPduId);
                    break;
                }
                case cCanTpTxState_WaitForCFConf:
                {    
                    CanTp_TxState[CanTpChannelId].DataIndex+=TX_CF_DataLength(CanTpChannelId);
                    if(CanTp_TxState[CanTpChannelId].BlockSizeCounter!=0)
                    {
                        CanTp_TxState[CanTpChannelId].BlockSizeCounter--;
                        if(CanTp_TxState[CanTpChannelId].BlockSizeCounter==0)
                        {
                            CanTp_TxState[CanTpChannelId].ChannelState=cCanTpTxState_WaitFC;
                            CanTp_TxState[CanTpChannelId].Timer=CANTP_NBS(CanTpTxPduId);
                        }
                        else   
                        {
                            CanTp_TxState[CanTpChannelId].ChannelState=cCanTpTxState_WaitForTpTxCF;
                            CanTp_TxState[CanTpChannelId].Timer=CanTp_TxState[CanTpChannelId].STMin;
                        }
                    }
                    else
                    {    
                        CanTp_TxState[CanTpChannelId].ChannelState=cCanTpTxState_WaitForTpTxCF;
                        CanTp_TxState[CanTpChannelId].Timer=CanTp_TxState[CanTpChannelId].STMin;
                    }
                    break;
                }              
                default:
                {
                    CanTp_ChannelInit(CanTpChannelId);
                    break;
                }
            }                    
        }
        /**Rx Status**/ 
        else if(CanTp_RxState[CanTpChannelId].ChannelState != cCanTpRxState_Idle) 
        {
            switch(CanTp_RxState[CanTpChannelId].ChannelState)
            {
                case cCanTpRxState_WaitForFCConf:
                {
                    CanTp_RxState[CanTpChannelId].ChannelState=cCanTpRxState_WaitCF;
                    CanTp_RxState[CanTpChannelId].Timer=CANTP_NCR(CanTp_RxState[CanTpChannelId].CurrentRxSduId);
                    break;
                  }              
                case cCanTpRxState_WaitForFCWaitConf:
                { 
                    CanTp_RxState[CanTpChannelId].WFTCounter++;
                    if(CanTp_RxState[CanTpChannelId].WFTCounter >= CANTP_WFTMAX(CanTp_RxState[CanTpChannelId].CurrentRxSduId))
                    {
                        CanTp_NUSDataIndication(CanTp_RxState[CanTpChannelId].CurrentRxSduId,NTFRSLT_E_WFT_OVRN);
                    }
                    else
                    {
                        CanTp_RxState[CanTpChannelId].ChannelState     = CanTp_RxState[CanTpChannelId].ChannelLastState;
                        CanTp_RxState[CanTpChannelId].Timer=CANTP_NBR(CanTp_RxState[CanTpChannelId].CurrentRxSduId);
                    }
                    break;                
                }
                case cCanTpRxState_WaitForFCWaitConfFFReqBuff:
                {
                    CanTp_RxState[CanTpChannelId].WFTCounter++;
                    if(CanTp_RxState[CanTpChannelId].WFTCounter >= CANTP_WFTMAX(CanTp_RxState[CanTpChannelId].CurrentRxSduId))
                    {
                        /*init Rx channel ,do not inform upper*/
                        CanTp_ChannelInit(CanTpChannelId);
                    }
                    else
                    {
                        CanTp_RxState[CanTpChannelId].ChannelState     = CanTp_RxState[CanTpChannelId].ChannelLastState;
                        CanTp_RxState[CanTpChannelId].Timer=CANTP_NBR(CanTp_RxState[CanTpChannelId].CurrentRxSduId);
                    }
                    break;    
                }
                case cCanTpRxState_WaitForFCOverConf:
                {
                    CanTp_NUSDataIndication(CanTp_RxState[CanTpChannelId].CurrentRxSduId,NTFRSLT_E_NO_BUFFER);
                    break;
                }
                case cCanTpRxState_WaitForFCOverConfFFReqBuff:
                {
                    CanTp_ChannelInit(CanTpChannelId);
                    break;        
                }
                  default:
                {
                    CanTp_ChannelInit(CanTpChannelId);
                    break;
                }
            }                
        } 
        else
        {
        }   
    }
    else
    {
    }
}        

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_MainFunction
*                
* Description:   The main function for scheduling the CAN TP.
*                                         
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
void CanTp_MainFunction(void)
{
    PduIdType  CanTpChanelId;
    /* Rx Task */
    for (CanTpChanelId=0;CanTpChanelId<CANTP_CHANNELS;CanTpChanelId++)
    {
        CanTp_RxMainFunction(CanTpChanelId);
    }
    /* Tx Task */
    for (CanTpChanelId=0;CanTpChanelId<CANTP_CHANNELS;CanTpChanelId++)
    {
        CanTp_TxMainFunction(CanTpChanelId);
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_GetVersionInfo
*                
* Description:   This function returns the version information of the CanTp module
*                                         
* Inputs:        pVersionInfo: Version information point
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if (CANTP_VERSION_INFO_API == STD_ON)
void CanTp_GetVersionInfo(Std_VersionInfoType* pVersionInfo)
{
    pVersionInfo->vendorID = CANTP_VENDOR_ID;
    pVersionInfo->moduleID = CANTP_MODULE_ID;
    pVersionInfo->instanceID = 0;
    pVersionInfo->sw_major_version = CANTP_SW_MAJOR_VERSION;
    pVersionInfo->sw_minor_version = CANTP_SW_MINOR_VERSION;
    pVersionInfo->sw_patch_version = CANTP_SW_PATCH_VERSION;
}
#endif

#if (CANTP_TC == STD_ON)

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_CancelTransmitRequest
*                
* Description:   This service cancels the transmission of an N-SDU that has 
*                already requested for transmission by calling CanTp_Transmit service
*                                         
* Inputs:        CanTpTxSduId: Sdu Id
*                   
* Outputs:       E_OK:     Operation accepted
*                E_NOT_OK: Operation failed
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
Std_ReturnType CanTp_CancelTransmitRequest(PduIdType CanTpTxSduId)
{
    Std_ReturnType result=E_NOT_OK;
    uint32_t primask;


    CanTp_CanInterruptDisable();

    if(CanTpTxSduId < CANTP_NUM_TX_CHANNELS)
    {
        if(CanTp_TxState[CANTP_TXTPCHANNEL_ID(CanTpTxSduId)].ChannelState!=cCanTpTxState_Idle)
        {
            if( (CanTp_TxState[CANTP_TXTPCHANNEL_ID(CanTpTxSduId)].CurrentTxSduId == CanTpTxSduId)&&\
                (CanTp_TxState[CANTP_TXTPCHANNEL_ID(CanTpTxSduId)].ChannelState != cCanTpTxState_WaitForLastCFConf) )
            {
                if(E_OK == CanTp_CanCancelTransmit(CanTpTxSduId))
                {         
                    CanTp_ChannelInit(CANTP_TXTPCHANNEL_ID(CanTpTxSduId));
                    result = E_OK;
                }
                else
                {
                    result = E_NOT_OK;
                }
            }
            else
            {
                
            }
        }  
        else
        {
                
        }
    }
    else
    {
                
    }

    CanTp_CanInterruptRestore();

    return result;
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_CancelReceiveRequest
*                
* Description:   This service is used to cancel the reception of an ongoing N-SDU
*                                         
* Inputs:        CanTpRxSduId: Sdu Id
*                   
* Outputs:       E_OK:     Operation accepted
*                E_NOT_OK: Operation failed
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/       
Std_ReturnType CanTp_CancelReceiveRequest(PduIdType CanTpRxSduId)
{
    Std_ReturnType result = E_NOT_OK;   
    uint32_t primask;
    
  
    CanTp_CanInterruptDisable();

    if(CanTpRxSduId < CANTP_NUM_RX_CHANNELS)
    {
        if(CanTp_RxState[CANTP_RXTPCHANNEL_ID(CanTpRxSduId)].ChannelState!=cCanTpRxState_Idle)
        {
            if(CanTp_RxState[CANTP_RXTPCHANNEL_ID(CanTpRxSduId)].CurrentRxSduId == CanTpRxSduId)
            {
                CanTp_ChannelInit(CANTP_RXTPCHANNEL_ID(CanTpRxSduId));
                result=E_OK;
            }
            else
            {
            }
        } 
        else
        {
        }
    }  
    else
    {
    }

    CanTp_CanInterruptRestore();

    return result;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_ChangeParameterRequest
*                
* Description:   This service is used to request the change of reception parameters 
*                BS and STmin for a specified N-SDU
*                                         
* Inputs:        CanTpSduId: Sdu Id
*                parameter:  CANTP_BS_PARAMETER:     BS requested to be changed 
*                            CANTP_STMIN_PARAMETER : STIMIN requested to be changed 
*                value:      Value requested to be changed to     
*                   
* Outputs:       NTFRSLT_PARAMETER_OK: Operation accepted
*                NTFRSLT_E_RX_ON:      Operation failed
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
Std_ReturnType CanTp_ChangeParameterRequest(PduIdType CanTpSduId,CanTpParameterType parameter,uint16 value)
{
    Std_ReturnType result=NTFRSLT_PARAMETER_OK;

    if((CanTp_RxState[CanTpSduId].ChannelState)==cCanTpRxState_Idle)
    {
        if(parameter==CANTP_BS_PARAMETER)
        {
            CanTp_RxState[CanTpSduId].BlockSize=(uint8)value;     
        }
        else
        {
            CanTp_RxState[CanTpSduId].STMin=(uint8)value;
        }
    }
    else
    {
        result=NTFRSLT_E_RX_ON;
    }

    return result;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_RxMainFunction
*                
* Description:   This function is called by CanTp_MainFunction to handle of CAN TP
*                                         
* Inputs:        CanTpRxChannelId: Channel Id     
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_RxMainFunction(PduIdType CanTpRxChannelId)
{
    BufReq_ReturnType  BufReqResult;
    uint32_t primask;


    CanTp_CanInterruptDisable();

    if(CanTp_RxState[CanTpRxChannelId].ChannelState != cCanTpRxState_Idle)
    {            
        /* Set a prestate for checking a interrupt while decrementing */                
        if(CanTp_RxState[CanTpRxChannelId].Timer!= 0) 
        {
            CanTp_RxState[CanTpRxChannelId].Timer--; 
        }
        else
        {
        }

        if((CanTp_RxState[CanTpRxChannelId].ChannelState & cCanTpState_Retransmit)==cCanTpState_Retransmit)
        {
            if(CanTp_RxState[CanTpRxChannelId].Timer== 0)
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState^=cCanTpState_Retransmit;                
            }
            else
            {
                CanTp_AssembleFC(CanTpRxChannelId,CanTp_RxState[CanTpRxChannelId].FlowStatus);                              
            }
        }
        else if((CanTp_RxState[CanTpRxChannelId].ChannelState) ==cCanTpRxState_WaitForSFRequestBuffer)
        {
            BufReqResult=CanTp_NUSDataSFIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,CanTp_RxState[CanTpRxChannelId].DataLength,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);                      
            if(BufReqResult==BUFREQ_OK) 
            {
                BufReqResult = CanTp_CopyRxData(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);                             
                if(BufReqResult==BUFREQ_OK) 
                {
                    CanTp_NUSDataIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,NTFRSLT_OK);
                }
                else if(BufReqResult==BUFREQ_E_BUSY)
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferSF;
                }
                else
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer;
                    CanTp_RxState[CanTpRxChannelId].Timer=0;
                }
                
            }
            else if(BufReqResult==BUFREQ_E_BUSY)
            {
            }
            /* BUFREQ_E_NOT_OK     BUFREQ_E_OVFL */
            else
            {
                CanTp_RxState[CanTpRxChannelId].Timer=0;
            }
        }    
        else if((CanTp_RxState[CanTpRxChannelId].ChannelState) ==cCanTpRxState_WaitForRxBufferSF)
        {
            BufReqResult=CanTp_CopyRxData(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);
            if(BufReqResult==BUFREQ_OK)
            {                                      
                CanTp_NUSDataIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,NTFRSLT_OK);
            }
            else if(BufReqResult==BUFREQ_E_BUSY)
            {
            }

            /* BUFREQ_E_NOT_OK        BUFREQ_E_OVFL */
            else
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer;
                CanTp_RxState[CanTpRxChannelId].Timer=0;
            }

        }
        else if((CanTp_RxState[CanTpRxChannelId].ChannelState) ==cCanTpRxState_WaitForFFRequestBuffer)
        {
            BufReqResult=CanTp_NUSDataFFIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,CanTp_RxState[CanTpRxChannelId].DataLength,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);                     
            if(BufReqResult==BUFREQ_OK)
            { 
                if(CanTp_RxState[CanTpRxChannelId].AvailableBufLength>=CanTp_RxState[CanTpRxChannelId].DataLength)
                {                                                                                                 
                    CanTp_RxState[CanTpRxChannelId].BlockSizeCounter=CANTP_BS(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
                }
                else
                {                                                                                                                                                                                                                        
                    CanTp_RxState[CanTpRxChannelId].BlockSizeCounter=(uint8)(CanTp_RxState[CanTpRxChannelId].AvailableBufLength/RX_CF_DataLength(CanTpRxChannelId));
                }

                CanTp_RxState[CanTpRxChannelId].BlockSize=CanTp_RxState[CanTpRxChannelId].BlockSizeCounter;
                CanTp_RxState[CanTpRxChannelId].STMin = CANTP_STMIN(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
                BufReqResult = CanTp_CopyRxData(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength); 
                            
                if(BufReqResult == BUFREQ_OK)
                {
                    CanTp_RxState[CanTpRxChannelId].DataIndex=RX_FF_DataLength(CanTpRxChannelId);
                    CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFC;
                    CanTp_RxState[CanTpRxChannelId].Timer = 0;
                } 
                else if(BufReqResult==BUFREQ_E_BUSY) 
                { 
                    CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferFF;
    
                    if(CanTp_RxState[CanTpRxChannelId].Timer<=1u) 
                    {
                        if(CANTP_WFTMAX(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId) == 0) 
                        {
                            CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer;
                            CanTp_RxState[CanTpRxChannelId].Timer=0; 
                        }
                        else
                        {             
                            CanTp_RxState[CanTpRxChannelId].ChannelLastState = CanTp_RxState[CanTpRxChannelId].ChannelState;
                            CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForTxFCWait;
                            CanTp_RxState[CanTpRxChannelId].Timer = 0; 
                        }
                    }
                    else
                    {
                    }
                }
                /* BUFREQ_E_NOT_OK        BUFREQ_E_OVFL */
                else
                { 
                    CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFCOverrun;
                    CanTp_RxState[CanTpRxChannelId].Timer = 0;/*Tx FC in the current task*/
                }     
            }                                             
            else if(BufReqResult==BUFREQ_E_BUSY)/*BUFREQ_E_BUSY*/
            { 
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForFFRequestBuffer;
                if(CanTp_RxState[CanTpRxChannelId].Timer<=1u) 
                {
                    if(CANTP_WFTMAX(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId) == 0) 
                    {
                        CanTp_RxState[CanTpRxChannelId].Timer = 0; 
                    }
                    else
                    {             
                        CanTp_RxState[CanTpRxChannelId].ChannelLastState = CanTp_RxState[CanTpRxChannelId].ChannelState;
                        CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForTxFCWaitFFReqBuff;
                        CanTp_RxState[CanTpRxChannelId].Timer = 0;/*Tx FCWait in the current task*/
                    }
                }
                else
                {

                }
            } 
            /* BUFREQ_E_NOT_OK        BUFREQ_E_OVFL */         
            else
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFCOverrunFFReqBuff;
                CanTp_RxState[CanTpRxChannelId].Timer = 0; 
            }

        }
        else if((CanTp_RxState[CanTpRxChannelId].ChannelState) ==cCanTpRxState_WaitForRxBufferFF)
        {
            BufReqResult=CanTp_CopyRxData(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);
            if(BufReqResult == BUFREQ_OK)
            {
                CanTp_RxState[CanTpRxChannelId].DataIndex=RX_FF_DataLength(CanTpRxChannelId); 
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFC;
                CanTp_RxState[CanTpRxChannelId].Timer = 0; 
            }
            else if(BufReqResult==BUFREQ_E_BUSY)
            { 
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferFF;
    
                if(CanTp_RxState[CanTpRxChannelId].Timer<=1u)
                {
                    if(CANTP_WFTMAX(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId) == 0) 
                    {
                        CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer;
                        CanTp_RxState[CanTpRxChannelId].Timer = 0; 
                    }
                    else
                    {     
                        CanTp_RxState[CanTpRxChannelId].ChannelLastState = CanTp_RxState[CanTpRxChannelId].ChannelState;
                        CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForTxFCWait;
                        CanTp_RxState[CanTpRxChannelId].Timer = 0;/*Tx FCWait in the current task*/
                    }
                }
                else
                {
                }
            } 
            /* BUFREQ_E_OVFL        BUFREQ_E_NOT_OK */
            else
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFCOverrun;
                CanTp_RxState[CanTpRxChannelId].Timer = 0; 
            }

        }
        else if((CanTp_RxState[CanTpRxChannelId].ChannelState) ==cCanTpRxState_WaitForRxBufferGoingCF)
        {
            BufReqResult=CanTp_CopyRxData(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);                            
            CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduLength=RX_CF_DataLength(CanTpRxChannelId);
            CanTp_RxState[CanTpRxChannelId].DataIndex+=RX_CF_DataLength(CanTpRxChannelId);
            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitCF;                                

            if(BufReqResult==BUFREQ_OK)
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitCF;    
                CanTp_RxState[CanTpRxChannelId].Timer =CANTP_NCR(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId) -\
                    ((CANTP_STMIN(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId)/CANTP_MAIN_FUNCTION_PERIOD)-CanTp_RxState[CanTpRxChannelId].Timer);
        
            }
            else if(BufReqResult==BUFREQ_E_BUSY)
            {      
                CanTp_RxState[CanTpRxChannelId].DataIndex-=RX_CF_DataLength(CanTpRxChannelId);
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferGoingCF;                    
                if(CanTp_RxState[CanTpRxChannelId].Timer == 0)
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer; 
                }
                else
                {
                }
            }
            /* BUFREQ_E_NOT_OK    BUFREQ_E_OVFL */
            else 
            { 
                CanTp_RxState[CanTpRxChannelId].Timer=0;
                CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer; 
            }

        }
        else if((CanTp_RxState[CanTpRxChannelId].ChannelState) ==cCanTpRxState_WaitForRxBufferLastCFBlock)
        {
            BufReqResult=CanTp_CopyRxData(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);                            
            CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduLength=RX_CF_DataLength(CanTpRxChannelId);
            CanTp_RxState[CanTpRxChannelId].DataIndex+=RX_CF_DataLength(CanTpRxChannelId);

            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFC;                                
            BufReqResult=CanTp_CopyRxData(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);                             

            if(BufReqResult==BUFREQ_OK)
            {                                                                                                                                                                     
                if(CanTp_RxState[CanTpRxChannelId].AvailableBufLength>=(PduLengthType)(CanTp_RxState[CanTpRxChannelId].DataLength-CanTp_RxState[CanTpRxChannelId].DataIndex))
                {
                    CanTp_RxState[CanTpRxChannelId].BlockSizeCounter=CANTP_BS(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
                }
                else
                {                                                                                                                                                                                                                                                         
                    CanTp_RxState[CanTpRxChannelId].BlockSizeCounter=(uint8)(CanTp_RxState[CanTpRxChannelId].AvailableBufLength/RX_CF_DataLength(CanTpRxChannelId))+1;
                }

                CanTp_RxState[CanTpRxChannelId].BlockSize=CanTp_RxState[CanTpRxChannelId].BlockSizeCounter;
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFC;
                CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_CTS;
                CanTp_RxState[CanTpRxChannelId].Timer=0;                                                     
            }                                              
            else if(BufReqResult==BUFREQ_E_BUSY)
            { 
                CanTp_RxState[CanTpRxChannelId].DataIndex-=RX_CF_DataLength(CanTpRxChannelId);
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferLastCFBlock;
                if(CanTp_RxState[CanTpRxChannelId].Timer<=1u)
                {
                    if(CANTP_WFTMAX(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId) == 0)
                    {
                        CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer;
                        CanTp_RxState[CanTpRxChannelId].Timer = 0;
                    }
                    else
                    {     
                        CanTp_RxState[CanTpRxChannelId].ChannelLastState = CanTp_RxState[CanTpRxChannelId].ChannelState;
                        CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForTxFCWait;
                        CanTp_RxState[CanTpRxChannelId].Timer = 0;
                    }        
                }
                else
                {

                }
            }
            else/* BUFREQ_E_NOT_OK          BUFREQ_E_OVFL */
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFCOverrun;
                CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_Overrun;
                CanTp_RxState[CanTpRxChannelId].Timer=0;     
            }

        }
        else if((CanTp_RxState[CanTpRxChannelId].ChannelState) ==cCanTpRxState_WaitForRxBufferLastCF)
        {                        
            BufReqResult = CanTp_CopyRxData(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);
            CanTp_RxState[CanTpRxChannelId].DataIndex=CanTp_RxState[CanTpRxChannelId].DataLength;                                                      

            if(BufReqResult==BUFREQ_OK)
            {                                                                                                                                                                     
                CanTp_NUSDataIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,NTFRSLT_OK);                                                     
            }                                              
            else if(BufReqResult==BUFREQ_E_BUSY)
            { 
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferLastCF;
                if(CanTp_RxState[CanTpRxChannelId].Timer == 0)
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer;
                }
                else
                {

                }

            }
            /* BUFREQ_E_NOT_OK          BUFREQ_E_OVFL */
            else
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState = cCanTpRxState_NoBuffer;/* inform appl with relust NTFRSLT_E_NO_BUFFER */
                CanTp_RxState[CanTpRxChannelId].Timer = 0;/*Transmit FC in the task */                             
            }

        }
        else
        {

        }

        if((CanTp_RxState[CanTpRxChannelId].Timer==0)&&(CanTp_RxState[CanTpRxChannelId].ChannelState != cCanTpRxState_Idle))
        {
            switch(CanTp_RxState[CanTpRxChannelId].ChannelState)
            {
                case cCanTpRxState_WaitForTxFC:
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForFCConf;
                    CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_CTS;
                    CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NAR(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
                    CanTp_AssembleFC(CanTpRxChannelId,cCanTp_FC_FS_CTS); 
                    break;    
                }

                case cCanTpRxState_WaitForTxFCOverrun:
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForFCOverConf;
                    CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_Overrun;
                    CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NAR(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
                    CanTp_AssembleFC(CanTpRxChannelId,cCanTp_FC_FS_Overrun);
                    break;
                }
    
                case cCanTpRxState_WaitForTxFCOverrunFFReqBuff:
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForFCOverConfFFReqBuff;//cCanTpRxState_WaitForFCOverConf;
                    CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_Overrun;
                    CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NAR(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
                    CanTp_AssembleFC(CanTpRxChannelId,cCanTp_FC_FS_Overrun);
                    break;    
                }    
    
                case cCanTpRxState_WaitForTxFCWait:
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForFCWaitConf;
                    CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_Wait;
                    CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NAR(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
                    CanTp_AssembleFC(CanTpRxChannelId,cCanTp_FC_FS_Wait);    
                    break;
                }

                case cCanTpRxState_WaitForTxFCWaitFFReqBuff:
                {
                    CanTp_RxState[CanTpRxChannelId].ChannelState =cCanTpRxState_WaitForFCWaitConfFFReqBuff;
                    CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_Wait;
                    CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NAR(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
                    CanTp_AssembleFC(CanTpRxChannelId,cCanTp_FC_FS_Wait); 
                    break;
                }

                case cCanTpRxState_WaitCF:
                {
                    CanTp_NUSDataIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,(uint16)NTFRSLT_E_TIMEOUT_CR); /* Notify application */
                    break;
                }

                case cCanTpRxState_WaitForFCConf:          
                case cCanTpRxState_WaitForFCWaitConf:  
                case cCanTpRxState_WaitForFCOverConf:
                {
                    CanTp_NUSDataIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,(uint16)NTFRSLT_E_TIMEOUT_A); /* Notify application */
                    break;
                }

                case cCanTpRxState_WaitForRxBufferSF:
                case cCanTpRxState_NoBuffer:    
                case cCanTpRxState_WaitForRxBufferLastCF:
                {
                    CanTp_NUSDataIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,NTFRSLT_E_NO_BUFFER );
                    break;
                }    
                default:
                {
                    CanTp_ChannelInit(CanTpRxChannelId);
                    break;
                }
            }
        }
        else
        {

        }
 
    }
    else
    {

    }

    CanTp_CanInterruptRestore();
 }

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_RxMainFunction
*                
* Description:   This function is called by CanTp_MainFunction to handle of CAN TP 
*                timeouts N_As, N_Bs, N_Cs
*                                         
* Inputs:        CanTpTxChannelId: Channel Id     
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_TxMainFunction(PduIdType CanTpTxChannelId)
{
    uint8 ReturnState =E_OK;
    PduInfoType CanTpTxNPduInfo;
    uint8 i;
    uint32_t primask;


    CanTp_CanInterruptDisable();
    /* Set a prestate for checking a interrupt while decrementing */
    if(CanTp_TxState[CanTpTxChannelId].Timer!= 0) 
    {
        CanTp_TxState[CanTpTxChannelId].Timer--;  

        if((CanTp_TxState[CanTpTxChannelId].ChannelState & cCanTpState_Retransmit)==cCanTpState_Retransmit)
        {
            if(CanTp_TxState[CanTpTxChannelId].Timer== 0)
            {
                CanTp_NUSDataConfirm(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId,NTFRSLT_E_TIMEOUT_A);
                CanTp_ChannelInit(CanTpTxChannelId);        
            }
            else
            {
                if(E_OK == CanTp_CanTransmit(CANTP_TXNPDU_ID(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId),&CanTp_TxState[CanTpTxChannelId].TxNPduInfo))
                {
                    CanTp_TxState[CanTpTxChannelId].ChannelState^=cCanTpState_Retransmit;
                }
                else
                {
                }
            }
        }
        else
        {    
            if(CanTp_TxState[CanTpTxChannelId].Timer== 0) /* Set a prestate for checking a interrupt while decrementing */
            {
                switch(CanTp_TxState[CanTpTxChannelId].ChannelState)
                {
                    case cCanTpTxState_WaitForTxStart:
                    {        
                        if(CanTp_TxState[CanTpTxChannelId].DataLength <=TX_SF_DataLength(CanTpTxChannelId)) 
                        {
                            CanTp_TxState[CanTpTxChannelId].ChannelState = cCanTpTxState_WaitForSFConf;
                            CanTp_TxState[CanTpTxChannelId].FrameIndex=0;                     
                            CanTp_TxBuf[CanTpTxChannelId][TX_NPCI_OFFSET(CanTpTxChannelId)]=(uint8)CanTp_TxState[CanTpTxChannelId].DataLength;
                            CanTpTxNPduInfo.SduDataPtr=&CanTp_TxBuf[CanTpTxChannelId][TX_SF_OFFSET(CanTpTxChannelId)];
                            CanTpTxNPduInfo.SduLength=CanTp_TxState[CanTpTxChannelId].DataLength;
                            if(CANTP_TX_PADDING_ACTIVATION(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId)==CANTP_ON)
                            {
                                CanTp_TxState[CanTpTxChannelId].TxNPduInfo.SduLength=CANTP_MAX_FRAME_LENGTH;
                                CanTp_TxState[CanTpTxChannelId].PaddingLength =(uint8)(TX_SF_DataLength(CanTpTxChannelId)-(uint8)CanTp_TxState[CanTpTxChannelId].DataLength);              
                                for (i=(uint8)(CANTP_MAX_FRAME_LENGTH-CanTp_TxState[CanTpTxChannelId].PaddingLength);i<CANTP_MAX_FRAME_LENGTH;i++)
                                {
                                    CanTp_TxBuf[CanTpTxChannelId][i]=CANTP_FILLPATTERN;
                                }
                            }
                            else
                            {
                                CanTp_TxState[CanTpTxChannelId].TxNPduInfo.SduLength=CanTp_TxState[CanTpTxChannelId].DataLength+1u+TX_NPCI_OFFSET(CanTpTxChannelId);
                            }             
                        }
                        else 
                        {
                            CanTp_TxState[CanTpTxChannelId].ChannelState = cCanTpTxState_WaitForFFConf;
                            CanTp_TxState[CanTpTxChannelId].RemainTxDataCnt=CanTp_TxState[CanTpTxChannelId].DataLength;
                            CanTp_TxState[CanTpTxChannelId].DataIndex=0;

                            CanTp_TxState[CanTpTxChannelId].FrameIndex=(uint16)(CanTp_TxState[CanTpTxChannelId].DataLength/TX_CF_DataLength(CanTpTxChannelId));  /* Correct calculation means txSEG=(count-6+6)/7 or (count-5+5)/6 */ 
                            if(CANTP_TX_PADDING_ACTIVATION(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId)==CANTP_ON)
                            {
                                CanTp_TxState[CanTpTxChannelId].PaddingLength = (uint8)((CanTp_TxState[CanTpTxChannelId].FrameIndex*TX_CF_DataLength(CanTpTxChannelId))-(CanTp_TxState[CanTpTxChannelId].DataLength-TX_FF_DataLength(CanTpTxChannelId)));
                            }
                            else
                            {
                            }
                            CanTp_TxState[CanTpTxChannelId].TxNPduInfo.SduLength=CANTP_MAX_FRAME_LENGTH;
                            CanTp_TxState[CanTpTxChannelId].SeqNumber=cCanTpSNStartValue;
                            CanTp_TxBuf[CanTpTxChannelId][TX_NPCI_OFFSET(CanTpTxChannelId)]=(cCanTp_FF)|((uint8)(CanTp_TxState[CanTpTxChannelId].DataLength >> 8u));        
                            CanTp_TxBuf[CanTpTxChannelId][TX_DL_OFFSET(CanTpTxChannelId)]=(uint8)CanTp_TxState[CanTpTxChannelId].DataLength;                        
                            CanTpTxNPduInfo.SduDataPtr=&CanTp_TxBuf[CanTpTxChannelId][TX_FF_OFFSET(CanTpTxChannelId)];
                            CanTpTxNPduInfo.SduLength=TX_FF_DataLength(CanTpTxChannelId);  
                        }  
                        CanTp_TxState[CanTpTxChannelId].Timer = CANTP_NAS(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId) - 1u;
                        /* start transmission */        
                        ReturnState = (uint8)CanTp_CopyTxData(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId,&CanTpTxNPduInfo,&RetryInfoStruct[CanTp_TxState[CanTpTxChannelId].CurrentTxSduId],&CanTp_TxState[CanTpTxChannelId].RemainTxDataCnt);
                        if((BUFREQ_E_NOT_OK == ReturnState)||(BUFREQ_E_OVFL == ReturnState))
                        {
                            CanTp_ChannelInit(CanTpTxChannelId);
                        }
                        else if(BUFREQ_E_BUSY == ReturnState)
                        {
                            CanTp_TxState[CanTpTxChannelId].ChannelState |=cCanTpTxState_ReCopy;
                        }
                        else
                        {
                            if(RetryInfoStruct[CanTp_TxState[CanTpTxChannelId].CurrentTxSduId].TpDataState == TP_DATACONF)
                            {
                                if(CANTP_TX_ADDRESSING_FORMAT(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId)==CANTP_EXTENDED)/*Addressing Format is Extended */
                                {
                                    CanTp_TxBuf[CanTpTxChannelId][TARGET_OFFSET]=CANTP_TX_NTA(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId);/*Target Address */
                                }
                                else if(CANTP_TX_ADDRESSING_FORMAT(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId)==CANTP_MIXED)/*Addressing Format is Mixed */
                                {
                                    CanTp_TxBuf[CanTpTxChannelId][AE_OFFSET]=CANTP_TX_NAE(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId);/*Extended Address */
                                }
                                else
                                {
                                }         
                                if(CanTp_CanTransmit(CANTP_TXNPDU_ID(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId),&CanTp_TxState[CanTpTxChannelId].TxNPduInfo)!=E_OK)
                                {  
                                    CanTp_TxState[CanTpTxChannelId].ChannelState |= cCanTpState_Retransmit;
                                }
                                else
                                {
                                }
                            }
                        }

                          break;    
                    }        
                    case  cCanTpTxState_WaitFC:
                    {
                        CanTp_NUSDataConfirm(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId,NTFRSLT_E_TIMEOUT_BS); 
                        CanTp_ChannelInit(CanTpTxChannelId);
                        break;
                    }
                    case  cCanTpTxState_WaitForTpTxCF:
                    {
                        CanTp_TxState[CanTpTxChannelId].FrameIndex--;  
                        CanTp_TxBuf[CanTpTxChannelId][TX_NPCI_OFFSET(CanTpTxChannelId)]=(cCanTp_CF)|(uint8)CanTp_TxState[CanTpTxChannelId].SeqNumber;
                        if(CanTp_TxState[CanTpTxChannelId].SeqNumber==0x0F)
                        {
                            CanTp_TxState[CanTpTxChannelId].SeqNumber=0;
                        }
                        else
                        {
                            CanTp_TxState[CanTpTxChannelId].SeqNumber++;
                        }

                        if(CanTp_TxState[CanTpTxChannelId].RemainTxDataCnt!=(CanTp_TxState[CanTpTxChannelId].DataLength-CanTp_TxState[CanTpTxChannelId].DataIndex))
                        {
                            CanTp_NUSDataConfirm(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId,NTFRSLT_E_NOT_OK );
                            CanTp_ChannelInit(CanTpTxChannelId); 
                        }
                        else
                        {
                            CanTpTxNPduInfo.SduDataPtr=&CanTp_TxBuf[CanTpTxChannelId][TX_CF_OFFSET(CanTpTxChannelId)];                        
                            if (CanTp_TxState[CanTpTxChannelId].FrameIndex == 0)/*Transmit the last CF*/
                            {
                                CanTpTxNPduInfo.SduLength=CanTp_TxState[CanTpTxChannelId].RemainTxDataCnt;
                                CanTp_TxState[CanTpTxChannelId].ChannelState=cCanTpTxState_WaitForLastCFConf;                             
                                if(CANTP_TX_PADDING_ACTIVATION(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId)==CANTP_ON)
                                {
                                    CanTp_TxState[CanTpTxChannelId].TxNPduInfo.SduLength=CANTP_MAX_FRAME_LENGTH;
                                    for (i=(uint8)(CANTP_MAX_FRAME_LENGTH-CanTp_TxState[CanTpTxChannelId].PaddingLength);i<CANTP_MAX_FRAME_LENGTH;i++)
                                    {
                                        CanTp_TxBuf[CanTpTxChannelId][i]=CANTP_FILLPATTERN;
                                    }
                                }
                                else
                                {
                                    CanTp_TxState[CanTpTxChannelId].TxNPduInfo.SduLength=CanTpTxNPduInfo.SduLength+TX_CF_OFFSET(CanTpTxChannelId);
                                }
                            }
                            else
                            {
                                CanTpTxNPduInfo.SduLength=TX_CF_DataLength(CanTpTxChannelId);
                                CanTp_TxState[CanTpTxChannelId].TxNPduInfo.SduLength=CANTP_MAX_FRAME_LENGTH;                                
                                CanTp_TxState[CanTpTxChannelId].ChannelState=cCanTpTxState_WaitForCFConf;    
                            }
                            CanTp_TxState[CanTpTxChannelId].Timer=CANTP_NAS(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId);                      
                            (void)CanTp_CopyTxData(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId,&CanTpTxNPduInfo,NULL_PTR,&CanTp_TxState[CanTpTxChannelId].RemainTxDataCnt);

                            if(CANTP_TX_ADDRESSING_FORMAT(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId)==CANTP_EXTENDED)
                            {
                                CanTp_TxBuf[CanTpTxChannelId][TARGET_OFFSET]=CANTP_TX_NTA(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId);
                            }
                            else if(CANTP_TX_ADDRESSING_FORMAT(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId)==CANTP_MIXED)
                            {
                                CanTp_TxBuf[CanTpTxChannelId][AE_OFFSET]=CANTP_TX_NAE(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId);
                            }
                            else
                            {
                            }
                            ReturnState=CanTp_CanTransmit(CANTP_TXNPDU_ID(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId),&CanTp_TxState[CanTpTxChannelId].TxNPduInfo);                                 

                            if(ReturnState!=E_OK)
                            {
                                CanTp_TxState[CanTpTxChannelId].ChannelState|=cCanTpState_Retransmit;
                            }
                        }

                        break;
                    }
    
                    case  cCanTpTxState_WaitForFFConf:
                    case  cCanTpTxState_WaitForSFConf:  
                    case  cCanTpTxState_WaitForCFConf:
                    case  cCanTpTxState_WaitForLastCFConf:
                    {            
                        CanTp_NUSDataConfirm(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId,NTFRSLT_E_TIMEOUT_A );
                        CanTp_ChannelInit(CanTpTxChannelId); 
                        break;
                    }
                    default: 
                    {
                        CanTp_ChannelInit(CanTpTxChannelId);
                        break;
                    }       

                  }    
            }
            else
            {
            }
        }
    }
    else
    {
    }

    CanTp_CanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_RxSFPdu
*                
* Description:   This function is called by CanTp_RxIndication after a successful
*                reception of a Rx SF-PDU
*                                         
* Inputs:        RxSduId:           Sdu Id   
*                CanTpRxChannelId : Channel Id
*                SFDataBuf:         Rx data buffer point
*                CanDlc:            data length   
*
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_RxSFPdu(PduIdType RxSduId,uint16 CanTpRxChannelId,uint8* SFDataBuf,uint8 CanDlc)
{
    BufReq_ReturnType  BufReqResult;
    StatusType TpSFCheckStatus = E_OK;
     
    if(CanDlc<=(SFDataBuf[RX_TPCI_OFFSET(CanTpRxChannelId)]&cCanTp_DLMaskSF))
    {
        /*CAN data link error,abort CAN frame*/
        TpSFCheckStatus = E_NOT_OK;
    } 
    /*check SF Length*/
    else if( ((SFDataBuf[RX_TPCI_OFFSET(CanTpRxChannelId)]&cCanTp_DLMaskSF)>RX_SF_DataLength(CanTpRxChannelId))
            ||((SFDataBuf[RX_TPCI_OFFSET(CanTpRxChannelId)]&cCanTp_DLMaskSF)<CANTP_RX_DL(RxSduId)) )
    {
        TpSFCheckStatus = E_NOT_OK;
    }
    else if(CANTP_CHANNEL_MODE(CanTpRxChannelId)==CANTP_MODE_HALF_DUPLEX) 
    {
        if (CanTp_TxState[CanTpRxChannelId].ChannelState!= cCanTpTxState_Idle)
        {
            TpSFCheckStatus = E_NOT_OK;
        }
        else
        {
        }
    }
    else
    {

    }

    if(TpSFCheckStatus == E_NOT_OK)
    {
        if(CanTp_RxState[CanTpRxChannelId].RxPreCancel == 1u)
        {
            CanTp_RxState[CanTpRxChannelId].RxPreCancel = 0;
            RxFormatOffset[CanTpRxChannelId]    =CanTp_RxState[CanTpRxChannelId].CurrentRxFormatOffset;
        }
        else
        {
        
        }
        
    }
    else
    {
        if(CanTp_RxState[CanTpRxChannelId].RxPreCancel == 1u)
        {
            CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_UNEXP_PDU);
        }
        else
        {
        }
        CanTp_RxState[CanTpRxChannelId].CurrentRxSduId = RxSduId;
        CanTp_RxState[CanTpRxChannelId].DataLength=SFDataBuf[RX_TPCI_OFFSET(CanTpRxChannelId)]&cCanTp_DLMaskSF;
        CanTp_RxState[CanTpRxChannelId].DataIndex=0;
        CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&SFDataBuf[RX_SF_OFFSET(CanTpRxChannelId)];                                                                                                
        CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduLength=SFDataBuf[RX_TPCI_OFFSET(CanTpRxChannelId)]&cCanTp_DLMaskSF;
        BufReqResult=CanTp_NUSDataSFIndication(RxSduId,CanTp_RxState[CanTpRxChannelId].DataLength,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);            
        if(BufReqResult==BUFREQ_OK) 
        {
            BufReqResult = CanTp_CopyRxData(RxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);               
            if(BufReqResult==BUFREQ_OK) 
            {
                CanTp_NUSDataIndication(RxSduId,NTFRSLT_OK);
            }
            else if(BufReqResult==BUFREQ_E_BUSY)
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferSF;
                CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NBR(RxSduId);
                CanTp_DataCopy(&CanTp_RxBuf[CanTpRxChannelId][0], SFDataBuf,CanDlc);
                CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[CanTpRxChannelId][RX_SF_OFFSET(CanTpRxChannelId)];
            }
            /* BUFREQ_E_NOT_OK     BUFREQ_E_OVFL */
            else
            {
                CanTp_NUSDataIndication(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId,NTFRSLT_E_NO_BUFFER );
            }
        }
        else if(BufReqResult==BUFREQ_E_BUSY)
        {
            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForSFRequestBuffer;
            CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NBR(RxSduId);
            CanTp_DataCopy(&CanTp_RxBuf[CanTpRxChannelId][0], SFDataBuf,CanDlc);
            CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[CanTpRxChannelId][RX_SF_OFFSET(CanTpRxChannelId)];
        }
        /* BUFREQ_E_NOT_OK   BUFREQ_E_OVFL */
        else
        {
            CanTp_ChannelInit(CanTpRxChannelId);                 
        }
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_RxFFPdu
*                
* Description:   This function is called by CanTp_RxIndication after a successful
*                reception of a Rx FF-PDU
*                                         
* Inputs:        RxSduId:           Sdu Id   
*                CanTpRxChannelId : Channel Id
*                SFDataBuf:         Rx data buffer point
*                CanDlc:            data length   
*
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_RxFFPdu(PduIdType RxSduId,uint16 CanTpRxChannelId,uint8* FFDataBuf,uint8 CanDlc)
{
    BufReq_ReturnType  BufReqResult;
    PduLengthType CanTpDataLength;
    StatusType TpFFCheckStatus = E_OK;

    CanTpDataLength=(uint16)((uint16)(FFDataBuf[RX_TPCI_OFFSET(CanTpRxChannelId)]&cCanTp_DLMaskFF)<< 8u)\
                    |(uint16)FFDataBuf[RX_DL_OFFSET(CanTpRxChannelId)];

    if(CanDlc<CANTP_MAX_FRAME_LENGTH)
    {
        TpFFCheckStatus = E_NOT_OK;
    }
    else if(CANTP_RX_TA_TYPE(RxSduId)==CANTP_FUNCTIONAL)
    {
        TpFFCheckStatus = E_NOT_OK;
    }                      
    else if(CanTpDataLength<=  RX_SF_DataLength(CanTpRxChannelId))
    {
        TpFFCheckStatus = E_NOT_OK;
    }
    else if(CANTP_CHANNEL_MODE(CanTpRxChannelId)==CANTP_MODE_HALF_DUPLEX) 
    {        
        if (CanTp_TxState[CanTpRxChannelId].ChannelState!= cCanTpTxState_Idle)
        {                                                                                                            
            TpFFCheckStatus = E_NOT_OK;
        }
        else
        {
        }

    }
    else
    {
    }

    if(TpFFCheckStatus == E_NOT_OK)
    {
        if(CanTp_RxState[CanTpRxChannelId].RxPreCancel == 1u)
        {
            CanTp_RxState[CanTpRxChannelId].RxPreCancel = 0;
            RxFormatOffset[CanTpRxChannelId]    =CanTp_RxState[CanTpRxChannelId].CurrentRxFormatOffset;
        }
        else
        {
        }
    }
    else
    {        
        if(CanTp_RxState[CanTpRxChannelId].RxPreCancel == 1u)
        {
            CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_UNEXP_PDU);
        }
        else
        {
        }

        CanTp_RxState[CanTpRxChannelId].CurrentRxSduId = RxSduId;                       
        CanTp_RxState[CanTpRxChannelId].DataLength=CanTpDataLength;    
        CanTp_RxState[CanTpRxChannelId].FrameIndex=(uint16)(CanTp_RxState[CanTpRxChannelId].DataLength/RX_CF_DataLength(CanTpRxChannelId));
        CanTp_RxState[CanTpRxChannelId].ExpectedSN=(uint16)cCanTpSNStartValue;
        CanTp_RxState[CanTpRxChannelId].DataIndex=0;
        CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&FFDataBuf[RX_FF_OFFSET(CanTpRxChannelId)];
        CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduLength=RX_FF_DataLength(CanTpRxChannelId);            
        CanTp_RxState[CanTpRxChannelId].BlockSize=CANTP_BS(RxSduId);
        CanTp_RxState[CanTpRxChannelId].STMin = CANTP_STMIN(RxSduId);

        BufReqResult=CanTp_NUSDataFFIndication(RxSduId,CanTp_RxState[CanTpRxChannelId].DataLength,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);           

        if(BufReqResult==BUFREQ_OK)
        {                    
            if(CanTp_RxState[CanTpRxChannelId].AvailableBufLength>=CanTp_RxState[CanTpRxChannelId].DataLength)
            {                                                                                                    
                CanTp_RxState[CanTpRxChannelId].BlockSizeCounter=CANTP_BS(RxSduId);
            }
            else
            {                                                                                            
                CanTp_RxState[CanTpRxChannelId].BlockSizeCounter=(uint8)(CanTp_RxState[CanTpRxChannelId].AvailableBufLength/RX_CF_DataLength(CanTpRxChannelId));
            }
            
            CanTp_RxState[CanTpRxChannelId].BlockSize=CanTp_RxState[CanTpRxChannelId].BlockSizeCounter;
            CanTp_RxState[CanTpRxChannelId].STMin = CANTP_STMIN(RxSduId);
                      
            BufReqResult = CanTp_CopyRxData(RxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);               
                
            if(BufReqResult == BUFREQ_OK)
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFC;
                CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_CTS;
                CanTp_RxState[CanTpRxChannelId].DataIndex=RX_FF_DataLength(CanTpRxChannelId);
                CanTp_RxState[CanTpRxChannelId].Timer=0u;                        
            }     
            else if(BufReqResult==BUFREQ_E_BUSY)
            { 
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferFF;
                CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NBR(RxSduId);
                CanTp_RxState[CanTpRxChannelId].WFTCounter=0;
                CanTp_DataCopy(&CanTp_RxBuf[CanTpRxChannelId][0], FFDataBuf,CanDlc);
                CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[CanTpRxChannelId][RX_FF_OFFSET(CanTpRxChannelId)];
            } 
            else
            {
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFCOverrun;
                CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_Overrun;
                CanTp_RxState[CanTpRxChannelId].Timer=0;
            }
        }
        else if(BufReqResult==BUFREQ_E_BUSY)
        { 
            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForFFRequestBuffer;
            CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NBR(RxSduId);
            CanTp_RxState[CanTpRxChannelId].WFTCounter=0;
            CanTp_DataCopy(&CanTp_RxBuf[CanTpRxChannelId][0], FFDataBuf,CanDlc);
            CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[CanTpRxChannelId][RX_FF_OFFSET(CanTpRxChannelId)];
        }      
        else
        {
            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFCOverrunFFReqBuff;
            CanTp_RxState[CanTpRxChannelId].Timer=0;
        }

    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_RxCFPdu
*                
* Description:   This function is called by CanTp_RxIndication after a successful
*                reception of a Rx CF-PDU
*                                         
* Inputs:        RxSduId:           Sdu Id   
*                CanTpRxChannelId : Channel Id
*                SFDataBuf:         Rx data buffer point
*                CanDlc:            data length   
*
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_RxCFPdu(PduIdType RxSduId,uint16 CanTpRxChannelId,uint8* CFDataBuf,uint8 CanDlc)
{
    BufReq_ReturnType  BufReqResult;
    PduLengthType LastCFLength=0;
    StatusType TpCFCheckStatus = E_OK;

    if (CanTp_RxState[CanTpRxChannelId].ChannelState != cCanTpRxState_WaitCF)
    {
        TpCFCheckStatus = E_NOT_OK;
    }
    else if(CANTP_RX_TA_TYPE(RxSduId)==CANTP_FUNCTIONAL)
    {
        TpCFCheckStatus = E_NOT_OK;
    }
    else if(CANTP_CHANNEL_MODE(CanTpRxChannelId)==CANTP_MODE_HALF_DUPLEX) 
    {
        if (CanTp_TxState[CanTpRxChannelId].ChannelState!= cCanTpTxState_Idle)
        {                                                                                                             
            TpCFCheckStatus = E_NOT_OK;
        }
        else
        {
        }
    }
    else
    {
    }

    if(TpCFCheckStatus == E_NOT_OK)    
    {
    }
    else if((CFDataBuf[RX_TPCI_OFFSET(CanTpRxChannelId)]&cCanTp_SNMask)!=CanTp_RxState[CanTpRxChannelId].ExpectedSN)
    {
        CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_WRONG_SN);
    }
    else
    {       
        CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CFDataBuf[RX_CF_OFFSET(CanTpRxChannelId)];
        LastCFLength=CanTp_RxState[CanTpRxChannelId].DataLength-CanTp_RxState[CanTpRxChannelId].DataIndex;
        if(LastCFLength<=(uint16)RX_CF_DataLength(CanTpRxChannelId))
        {               
            if(CanDlc<=LastCFLength)
            {
                /*CAN data link error,abort CAN frame*/  
                CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_CAN_WRONG_CFDL);
            }
            else
            {
                CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduLength=LastCFLength;                
                BufReqResult = CanTp_CopyRxData(RxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);
                CanTp_RxState[CanTpRxChannelId].DataIndex=CanTp_RxState[CanTpRxChannelId].DataLength;                            

                if(BufReqResult==BUFREQ_OK)
                {                                                                                   
                    CanTp_NUSDataIndication(RxSduId,NTFRSLT_OK);                           
                }                        
                else if(BufReqResult==BUFREQ_E_BUSY)
                { 
                    CanTp_RxState[CanTpRxChannelId].DataIndex-=LastCFLength;
                    CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferLastCF;
                    CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NBR(RxSduId);
                    CanTp_DataCopy(&CanTp_RxBuf[CanTpRxChannelId][0], CFDataBuf,CanDlc);
                    CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[CanTpRxChannelId][RX_CF_OFFSET(CanTpRxChannelId)];                
                }
                else
                {
                    CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_NO_BUFFER);
                }
        
            }
        }
        else
        {
            if(CanDlc<=RX_CF_DataLength(CanTpRxChannelId))
            {
                /*CAN data link error,abort CAN frame*/
                CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_CAN_WRONG_CFDL);
            }
            else
            {
                CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduLength=RX_CF_DataLength(CanTpRxChannelId);
                BufReqResult=CanTp_CopyRxData(RxSduId,&CanTp_RxState[CanTpRxChannelId].RxNPduInfo,&CanTp_RxState[CanTpRxChannelId].AvailableBufLength);               
                CanTp_RxState[CanTpRxChannelId].DataIndex+=RX_CF_DataLength(CanTpRxChannelId);
                CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitCF;                
                CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NCR(RxSduId);                
                /*check SN */
                if(CanTp_RxState[CanTpRxChannelId].ExpectedSN<cCanTp_SNMask)
                {
                    CanTp_RxState[CanTpRxChannelId].ExpectedSN++;
                }
                else
                {
                    CanTp_RxState[CanTpRxChannelId].ExpectedSN=0;
                }
                /*check BS */
                if(CanTp_RxState[CanTpRxChannelId].BlockSizeCounter!=0)
                {
                    CanTp_RxState[CanTpRxChannelId].BlockSizeCounter--;
                    if(CanTp_RxState[CanTpRxChannelId].BlockSizeCounter==0)
                    {
                        if(BufReqResult==BUFREQ_OK)
                        {                                                                                   
                            if(CanTp_RxState[CanTpRxChannelId].AvailableBufLength>=(PduLengthType)(CanTp_RxState[CanTpRxChannelId].DataLength-CanTp_RxState[CanTpRxChannelId].DataIndex))
                            {
                                CanTp_RxState[CanTpRxChannelId].BlockSizeCounter=CANTP_BS(RxSduId);
                            }
                            else
                            {                                                                                                                                                                                                                                                            
                                CanTp_RxState[CanTpRxChannelId].BlockSizeCounter=(uint8)(CanTp_RxState[CanTpRxChannelId].AvailableBufLength/RX_CF_DataLength(CanTpRxChannelId))+1u;
                            }

                            CanTp_RxState[CanTpRxChannelId].BlockSize=CanTp_RxState[CanTpRxChannelId].BlockSizeCounter;                            
                            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFC;
                            CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_CTS;
                            CanTp_RxState[CanTpRxChannelId].Timer=1;                           
                        }                        
                        else if(BufReqResult==BUFREQ_E_BUSY)
                        { 
                            CanTp_RxState[CanTpRxChannelId].DataIndex-=RX_CF_DataLength(CanTpRxChannelId);
                            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferLastCFBlock;
                            CanTp_RxState[CanTpRxChannelId].Timer=CANTP_NBR(RxSduId);
                            CanTp_RxState[CanTpRxChannelId].WFTCounter=0;
                            CanTp_DataCopy(&CanTp_RxBuf[CanTpRxChannelId][0], CFDataBuf,CanDlc);
                            CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[CanTpRxChannelId][RX_CF_OFFSET(CanTpRxChannelId)];                    
                        }
                        else
                        {
                            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForTxFCOverrun;
                            CanTp_RxState[CanTpRxChannelId].FlowStatus=(uint16)cCanTp_FC_FS_Overrun;
                            CanTp_RxState[CanTpRxChannelId].Timer=1;     

                        }
                    }
                    else/*is NOT the last CF of block*/
                    {
                        if(BufReqResult==BUFREQ_OK)
                        {    
                        }
                        else if(BufReqResult==BUFREQ_E_BUSY)
                        {    
                            CanTp_RxState[CanTpRxChannelId].DataIndex-=RX_CF_DataLength(CanTpRxChannelId);
                            CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferGoingCF;
                            CanTp_RxState[CanTpRxChannelId].Timer=CANTP_STMIN(RxSduId)/CANTP_MAIN_FUNCTION_PERIOD; 
                            CanTp_DataCopy(&CanTp_RxBuf[CanTpRxChannelId][0], CFDataBuf,CanDlc);
                            CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[CanTpRxChannelId][RX_CF_OFFSET(CanTpRxChannelId)];
                            if(CanTp_RxState[CanTpRxChannelId].Timer == 0)
                            {
                                CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_NO_BUFFER);
                            }
                            else
                            {

                            }
        
                        }
                        else
                        {
                            CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_NO_BUFFER);
                        }

                    }
                }
                else/*BS=0*/
                {   
                    if(BufReqResult==BUFREQ_OK)
                    {    
                    }
                    else if(BufReqResult==BUFREQ_E_BUSY)
                    {    
                        CanTp_RxState[CanTpRxChannelId].DataIndex-=RX_CF_DataLength(CanTpRxChannelId);
                        CanTp_RxState[CanTpRxChannelId].ChannelState=cCanTpRxState_WaitForRxBufferGoingCF;
                        CanTp_RxState[CanTpRxChannelId].Timer=CANTP_STMIN(RxSduId)/CANTP_MAIN_FUNCTION_PERIOD; 
                        CanTp_DataCopy(&CanTp_RxBuf[CanTpRxChannelId][0], CFDataBuf,CanDlc);
                        CanTp_RxState[CanTpRxChannelId].RxNPduInfo.SduDataPtr=&CanTp_RxBuf[CanTpRxChannelId][RX_CF_OFFSET(CanTpRxChannelId)];
                        if(CanTp_RxState[CanTpRxChannelId].Timer == 0)
                        {
                            CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_NO_BUFFER);
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                        CanTp_NUSDataIndication(RxSduId,NTFRSLT_E_NO_BUFFER);
                    }

                }        
            }
        }    
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_RxFCPdu
*                
* Description:   This function is called by CanTp_RxIndication after a successful
*                reception of a Rx FC-PDU
*                                         
* Inputs:        RxSduId:           Sdu Id   
*                CanTpRxChannelId : Channel Id
*                SFDataBuf:         Rx data buffer point
*                CanDlc:            data length   
*
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_RxFCPdu(PduIdType RxSduId,uint16 CanTpTxChannelId,uint8* FCDataBuf,uint8 CanDlc)
{ 
    StatusType TpFCCheckStatus = E_OK;

    if(CanDlc<RX_FC_DataLength(CanTpTxChannelId))
    {
        /*CAN data link error,abort CAN frame*/
        TpFCCheckStatus = E_NOT_OK;
    }
    else if((CanTp_TxState[CanTpTxChannelId].ChannelState) !=cCanTpTxState_WaitFC)
    {
        /*FC NotExpected,Ignore*/
        TpFCCheckStatus = E_NOT_OK;
    }
    else if(CANTP_RXFC_TA_TYPE(RxSduId)==CANTP_FUNCTIONAL)
    {
        TpFCCheckStatus = E_NOT_OK;
    }
    else if(CANTP_CHANNEL_MODE(CanTpTxChannelId)==CANTP_MODE_HALF_DUPLEX) 
    {
        if (CanTp_RxState[CanTpTxChannelId].ChannelState!= cCanTpRxState_Idle)
        {                                                                                                             
            TpFCCheckStatus = E_NOT_OK;
        }
        else
        {
        }
    }
    else
    {
    }

    if(TpFCCheckStatus == E_OK)
    {       
        switch(FCDataBuf[TX_NPCI_OFFSET(CanTpTxChannelId)]&cCanTp_FSMask)
        {
            case  cCanTp_FC_FS_CTS:
            {
                CanTp_TxState[CanTpTxChannelId].ChannelState=cCanTpTxState_WaitForTpTxCF;                                     
                CanTp_TxState[CanTpTxChannelId].BlockSizeCounter=FCDataBuf[TX_BS_OFFSET(CanTpTxChannelId)];
                if ((FCDataBuf[TX_STMIN_OFFSET(CanTpTxChannelId)] & 0x80) == 0x80)
                {
                    if ( (FCDataBuf[TX_STMIN_OFFSET(CanTpTxChannelId)] > 0xF0) 
                        && (FCDataBuf[TX_STMIN_OFFSET(CanTpTxChannelId)] <= 0xF9) )
                    {
                        CanTp_TxState[CanTpTxChannelId].STMin = 1u;
                    }
                    else
                    {
                        CanTp_TxState[CanTpTxChannelId].STMin = (uint8)((127u / CANTP_MAIN_FUNCTION_PERIOD)+1u); 
                    }
                }
                else
                {
                    CanTp_TxState[CanTpTxChannelId].STMin = (uint8)(FCDataBuf[TX_STMIN_OFFSET(CanTpTxChannelId)] / CANTP_MAIN_FUNCTION_PERIOD)+1u;
                }
                CanTp_TxState[CanTpTxChannelId].Timer=CanTp_TxState[CanTpTxChannelId].STMin;
                break;
            }
            case  cCanTp_FC_FS_Wait:
            {
                CanTp_TxState[CanTpTxChannelId].ChannelState=cCanTpTxState_WaitFC;
                CanTp_TxState[CanTpTxChannelId].Timer=(uint16)(CANTP_NBS(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId));
                break;            
            }
            case  cCanTp_FC_FS_Overrun:
            {
                CanTp_NUSDataConfirm(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId, NTFRSLT_E_NO_BUFFER);
                CanTp_ChannelInit(CanTpTxChannelId);
                break;
            }
            default:
            {
                CanTp_NUSDataConfirm(CanTp_TxState[CanTpTxChannelId].CurrentTxSduId, NTFRSLT_E_INVALID_FS );    /* inform application */
                CanTp_ChannelInit(CanTpTxChannelId);
                break;
            }
        }
    }
    else
    {
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_AssembleFC
*                
* Description:   This function is called to send FC
*                                         
* Inputs:        CanTpRxChannelId : Channel Id
*                FlowStatus:        Flow status  
*
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_AssembleFC(PduIdType CanTpRxChannelId, uint8 FlowStatus)
{
    static uint8 TempFcMsg[CANTP_MAX_FRAME_LENGTH];
    PduInfoType CanTpTxFCPduInfo;
    PduIdType CanTpTxFCPduIdMaped;
    uint8 i;

    CanTpTxFCPduIdMaped=CANTP_TXFC_NPDU_ID(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);

    if(CANTP_RX_ADDRESSING_FORMAT(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId)==CANTP_EXTENDED)
    {
        TempFcMsg[TARGET_OFFSET]=CANTP_TX_NTA(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
    }
    else if(CANTP_RX_ADDRESSING_FORMAT(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId)==CANTP_MIXED)
    {
        TempFcMsg[AE_OFFSET]=CANTP_TX_NAE(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId);
    }
    else
    {        
    }
    TempFcMsg[TX_NPCI_OFFSET(CanTpRxChannelId)]=(uint8)(cCanTp_FC|FlowStatus);
    TempFcMsg[TX_BS_OFFSET(CanTpRxChannelId)]=CanTp_RxState[CanTpRxChannelId].BlockSize;
    TempFcMsg[TX_STMIN_OFFSET(CanTpRxChannelId)]=CanTp_RxState[CanTpRxChannelId].STMin;
    CanTpTxFCPduInfo.SduDataPtr=TempFcMsg;    
    if(CANTP_RX_PADDING_ACTIVATION(CanTp_RxState[CanTpRxChannelId].CurrentRxSduId)==CANTP_ON)
    {        
        for (i=(uint8)(TX_STMIN_OFFSET(CanTpRxChannelId)+1);i<CANTP_MAX_FRAME_LENGTH;i++)
        {
            TempFcMsg[i]=CANTP_FILLPATTERN;
        }
        CanTpTxFCPduInfo.SduLength=CANTP_MAX_FRAME_LENGTH;
    }
    else
    {       
        CanTpTxFCPduInfo.SduLength=TX_FC_DataLength(CanTpRxChannelId);
    }                               
    if(CanTp_CanTransmit(CanTpTxFCPduIdMaped,&CanTpTxFCPduInfo)!=E_OK)
    {
        CanTp_RxState[CanTpRxChannelId].ChannelState|=cCanTpState_Retransmit;
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CanTp_DataCopy
*                
* Description:   This function is called to Copy data
*                                         
* Inputs:        dest : Data destination
*                src:   Data source
*                cnt:   Data length 
*
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
static void CanTp_DataCopy( uint8* dest, uint8* src,uint16 cnt)
{
    uint16 i;
    for(i=0;i<cnt;i++)
    {
        dest[i]=src[i] ;
    }
}



