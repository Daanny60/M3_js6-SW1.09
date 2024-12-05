/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       CanTp_Cbk.h
********************************************************************************
*   Project/Product : CanTp programme
*   Title:          : CanTp_Cbk.h
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


#ifndef _CANTP_CBK_H_
#define _CANTP_CBK_H_

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "ComStack_Types.h"

/*******************************************************************************
*   Prototypes of export functions
*******************************************************************************/
extern void CanTp_RxIndication(PduIdType CanTpRxPduId, const PduInfoType* pCanTpRxPduPtr);
extern void CanTp_TxConfirmation(PduIdType CanIfTxPduId);


#endif
