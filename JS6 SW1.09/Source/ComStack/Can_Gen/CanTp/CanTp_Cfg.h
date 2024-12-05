
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

#if !defined(CANTP_CFG_H)
#define CANTP_CFG_H

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "CanIf.h"
#include "PduR_CanTp.h"

#define CANTP_VERSION_INFO_API               STD_ON 
#define CANTP_DEV_ERROR_DETECT               STD_OFF
#define CANTP_PROD_ERROR_DETECT              STD_ON
#define CANTP_TC                             STD_ON
#define CANTP_MAIN_FUNCTION_PERIOD           5u  

#define CANTP_FILLPATTERN                   0xAAu

#define CANTP_CHANNELS                              1u 


#define CANTP_NUM_RX_SDUS                   2u   
#define CANTP_NUM_TX_SDUS                   1u    

#define CANTP_NUM_RX_CHANNELS                CANTP_CHANNELS
#define CANTP_NUM_TX_CHANNELS                CANTP_CHANNELS

#define CanTp_CanInterruptDisable()     Can_DisableGlobalInterrupts()
#define CanTp_CanInterruptRestore()    Can_EnableGlobalInterrupts()

#define CanTp_CanTransmit(txSduIdx,pduInfo)    CanIf_Transmit(txSduIdx,pduInfo)
#define CanTp_CanCancelTransmit(txSduIdx)       CanIf_CancelTransmit(txSduIdx)

#define CanTp_NUSDataIndication(rxSduIdx,canTpErrorCode)  {PduR_CanTpRxIndication(rxSduIdx,canTpErrorCode);CanTp_ChannelInit(CANTP_RXTPCHANNEL_ID(rxSduIdx));}

#define CanTp_NUSDataConfirm(txSduIdx,canTpErrorCode)       PduR_CanTpTxConfirmation(txSduIdx,canTpErrorCode)
#define CanTp_NUSDataFFIndication(rxSduIdx,length,bufferSizePtr)   PduR_CanTpStartOfReception(rxSduIdx,length,bufferSizePtr)
                                      
#define CanTp_NUSDataSFIndication(rxSduIdx,length,bufferSizePtr)   PduR_CanTpStartOfReception(rxSduIdx,length,bufferSizePtr)


#define CanTp_CopyRxData(rxSduIdx,pduInfo,bufferSizePtr)   PduR_CanTpCopyRxData(rxSduIdx,pduInfo,bufferSizePtr)

#define CanTp_CopyTxData(txSduIdx,pduInfo,NULL_PTR,bufferSizePtr)   PduR_CanTpCopyTxData(txSduIdx,pduInfo,NULL_PTR,bufferSizePtr)

#endif /* CANTP_CFG_H */
