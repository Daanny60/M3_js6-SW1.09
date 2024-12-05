/*BEGIN_FILE_HDR
***********************************************************************************************
* NOTICE
* This software is the property of HiRain Technologies. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* HiRain Technologies.
***********************************************************************************************
* File Name:       PduR_Cfg.h
***********************************************************************************************
* Project/Product: 
* Title:           
* Author:          rui.guo
***********************************************************************************************
* Description:     
* 
*
* (Requirements, pseudo code and etc.)
***********************************************************************************************
* Limitations:
*
* (limitations)
***********************************************************************************************
***********************************************************************************************
* Revision History£º
*
* Version        Date              Initials                Descriptions
* -------    ----------         ----------           -----------------    
* 1.0        2012/09/19               rui.guo                Original
***********************************************************************************************
*END_FILE_HDR*/

#ifndef _PduR_Cfg_h_
#define _PduR_Cfg_h_


#include "CanTp.h"
#include "Dcm_Cbk.h"


/*
#define gHanldPDUIdCom2CAN0_0x100    0
#define gHanldPDUIdCom2CAN1_0x200    1
#define gHanldPDUIdCom2CAN0   0
#define gHanldPDUIdCom2CAN0   0
*/
                                
/* Container Name:PduRBswModules    Com  */

#define PduRComCancelReceive                   STD_ON
#define PduRComCancelTransmit                  STD_ON
#define PduRComChangeParameterRequestApi       STD_ON
#define PduRComCommunicationInterface          STD_ON
#define PduRComLowerModule                     STD_ON
#define PduRRetransmission
#define PduRTransportProtocol
#define PduRTriggertransmit
#define PduRComTxConfirmation                  STD_ON
#define PduRUpperModule                        STD_OFF
#define PduRUseTag
//#define PduRBswModuleRef


/* Container Name:PduRGeneral    Com  */

#define PDUR_DEV_ERROR_DETECT
#define PDUR_VERSION_INFO_API
#define PDUR_ZERO_COST_OPERATION


#define PDUR_NUM_OF_CANIF_TX_HANLDE    6
#define PDUR_NUM_OF_COM_TX_HANLDE      3

//#define PDUR_NUM_OF_CANIF_TX_HANLDE        PDUR_NUM_OF_CANIF_TX_HANLDE
//#define PUDR_COM_MAX_ID          PDUR_NUM_OF_COM_TX_HANLDE

#define  PduRCanTpDcmRxFun           1
#define  PduRCanTpDcmRxPhy           0
#define  PduRCanTpDcmTxID            0
extern const CanTp_CfgType CanTp_Cfg;
#endif
