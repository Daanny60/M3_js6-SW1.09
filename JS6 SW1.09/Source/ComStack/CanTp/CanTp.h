/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       CanTp.h
********************************************************************************
*   Project/Product : CanTp programme
*   Title:          : CanTp.h
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


#ifndef CANTP_H
#define CANTP_H


/*******************************************************************************
*   Includes
*******************************************************************************/

#include "ComStack_Types.h"
#include "CanTp_Cfg.h"


#define CANTP_VENDOR_ID             (uint16)(0xFFFFu)
#define CANTP_MODULE_ID             (uint16)(0x0023u)

/* CanTp Component release Version  */
#define CANTP_SW_MAJOR_VERSION      (uint8)(0x01u)
#define CANTP_SW_MINOR_VERSION      (uint8)(0x00u)
#define CANTP_SW_PATCH_VERSION      (uint8)(0x00u)
/* Autosar release version */
#define CANTP_AR_MAJOR_VERSION      (uint8)(0x04u)
#define CANTP_AR_MINOR_VERSION      (uint8)(0x00u)
#define CANTP_AR_PATCH_VERSION      (uint8)(0x00u)

/*******************************************************************************
*   Global data types and structures
*******************************************************************************/
/* AUTOSAR compliant */

typedef enum /* CanTpInternalState */
{
  CANTP_OFF = 0,
  CANTP_ON
} CanTpInternalState;

typedef enum
{
  CANTP_RX_CHANNEL = 0,
  CANTP_TX_CHANNEL
} CanTpChannelType;


typedef enum
{
  CANTP_STANDARD = 0,
  CANTP_EXTENDED,
  CANTP_MIXED
} CanTpAddressingFormat;

typedef enum
{
  CANTP_PHYSICAL = 0,
  CANTP_FUNCTIONAL
} CanTpTaType;

typedef enum
{
  CANTP_BS_PARAMETER = 0,
  CANTP_STMIN_PARAMETER
} CanTpParameterType;

/*******************************************************************************
*   Types for static-configuration (ROM/Flash) 
*******************************************************************************/
/*RxNsdu (ROM)*/ 
typedef struct
{ 
    PduIdType      CanTpChannelId;
    PduIdType      CanTpRxNPduId;
    PduIdType      CanTpTxFcNPduId;
    uint16         CanTpRxDl;
    uint16         CanTpNar;
    uint16         CanTpNbr;
    uint16         CanTpNcr;  
    uint8          CanTpRxPaddingActive;
    uint8          CanTpRxTaType;
    uint8          CanTpRxAddrFormat;
    uint8          CanTpBs;
    uint8          CanTpSTmin;
    uint8          CanTpRxWftMax;
    uint8          CanTpRxNAe;
    uint8          CanTpRxNSa;
    uint8          CanTpRxNTa;
} CanTp_RxNsduCfgType;

/*TxNsdu (ROM)*/
typedef struct 
{ 
    PduIdType      CanTpChannelId;
    PduIdType      CanTpTxNPduId;
    PduIdType      CanTpRxFcNPduId;
    uint16         CanTpTxDl;
    uint16         CanTpNas;
    uint16         CanTpNbs;
    uint16         CanTpNcs;
    uint8          CanTpTxPaddingActive;
    uint8          CanTpTxTaType;
    uint8          CanTpTxAddrFormat;
    uint8          CanTpTxNAe;
    uint8          CanTpTxNSa;
    uint8          CanTpTxNTa;
} CanTp_TxNsduCfgType;

/*RxPdu mapping table */
typedef struct 
{
    PduIdType RxSduIdx;
    PduIdType TxSduIdx;
} CanTp_RxPduMapType;

typedef struct
{
   boolean  ChannelMode;
}CanTpChannelModeType;

/*******************************************************************************
*   CanTp Overall (ROM)
*******************************************************************************/
/* Struct to hold the complete CanTp-configuration */
typedef struct 
{ 
    const CanTp_RxNsduCfgType* const CanTp_RxNsduCfg;
    const CanTp_TxNsduCfgType* const CanTp_TxNsduCfg;
    const CanTpChannelModeType* const ChannelMode;
} CanTp_CfgType;

/*******************************************************************************
*   External administration data (RAM)
*******************************************************************************/
typedef struct 
{
    const CanTp_CfgType*    CfgPtr;
    CanTpInternalState      InternalState;
} CanTp_AdminDataType;

/*******************************************************************************
*   Global defines
*******************************************************************************/
#define CANTP_MODE_HALF_DUPLEX      0x00
#define CANTP_MODE_FULL_DUPLEX      0x01


#define CANTP_MAX_FRAME_LENGTH      0x08u
#define CANTP_UNUSED                0xFFu

/*******************************************************************************
*   Gloable Data definitions
*******************************************************************************/
extern const CanTp_CfgType CanTp_Cfg;

/*******************************************************************************
*   Prototypes of export functions
*******************************************************************************/
extern void           CanTp_Init(const void* const pCfgPtr);

extern void           CanTp_Shutdown(void);
extern void           CanTp_MainFunction(void);
extern Std_ReturnType CanTp_Transmit(PduIdType CanTpTxSduId, const PduInfoType* pData);
#if (CANTP_TC == STD_ON)
extern Std_ReturnType CanTp_CancelTransmitRequest(PduIdType CanTpTxSduId);
#endif
extern Std_ReturnType CanTp_CancelReceiveRequest(PduIdType CanTpRxSduId);
extern Std_ReturnType CanTp_ChangeParameterRequest(PduIdType CanTpSduId,CanTpParameterType parameter,uint16 value);

#if (CANTP_VERSION_INFO_API == STD_ON)
extern void           CanTp_GetVersionInfo(Std_VersionInfoType* pVersionInfo);
#endif                                                     


#endif