#ifndef _PduR_h_
#define _PduR_h_

/*BEGIN_FILE_HDR
***********************************************************************************************
* NOTICE
* This software is the property of HiRain Technologies. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* HiRain Technologies.
***********************************************************************************************
* File Name:       PduR.h
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
* Revision History：
*
* Version        Date              Initials                Descriptions
* -------    ----------         ----------           -----------------    
* 1.0        2012/09/19               rui.guo                Original
***********************************************************************************************
*END_FILE_HDR*/
/*#include "Mem_Map.h"*/
#include "PduR_Types.h"
/*********************************************************************************************************************/
/* Module Identificatiion                                                                                            */
/*********************************************************************************************************************/
#define PduR_NumOfCOMTxObj    COM_TXIPDUNUM
#define PduR_NumOfCanIfRxObj  26u
#define PduR_NumOfCanTpRxSDU  CANTP_NUM_RX_SDUS

#define   PDUR_NULL   ((void*)0)
typedef   uint16  PduR_PathIndex;
//extern const PduRRoutingTable*      gPduRRoutTableComPtr;
//extern void PduR_Init(const PduR_PBConfigType* ConfigPtr);

/* 按照COM Tx(CANIf CanTP ， LINIf LINTP ，FrIf FrTP)顺序*/
extern const PduRRoutingTableIndexType  PduRComRoutingTableIndex[];

/* 按照DCM Tx 顺序*/   /*最小路由*/
extern const PduRRoutingTableIndexType  PduRDcmRoutingTableIndex[2];

/* 按照CanIf Rx 顺序*/
extern const PduRRoutingTableIndexType  PduRCanIfRoutingTableIndex[PduR_NumOfCanIfRxObj];

/* 按照CanTp Rx 顺序*/
//extern const PduRRoutingTableIndexType  PduRCanTpRoutingTableIndex[18];///hhhhh
//extern const PduRRoutingTableIndexType  PduRCanTpRoutingTableIndex[PduR_NumOfCanTpRxSDU];///hhhhh

extern const PduRDestPdu_Type  PduRDestPduHandles[];
extern const PduRSrcPdu_Type PduRSrcPduHandles[];

//extern const PduRRoutingTableIndexType  PduRComRoutingTableIndex[PduR_NumOfCOMTxObj];

#endif /*_PduR_h_ */
