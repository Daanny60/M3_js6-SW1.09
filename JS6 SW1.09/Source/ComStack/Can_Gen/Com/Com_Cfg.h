/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       Com_Cfg.h
********************************************************************************
*   Project/Product: COM programme
*   Title:           Com_Cfg.h
*   Author:          bo.liu        2010-2011
*                    donger.yang   2012-
********************************************************************************
*   Description:     Define configure data for Com
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
*   03.00.xx  
********************************************************************************
*END_FILE_HDR*/

#ifndef _COM_CFG_H_
#define _COM_CFG_H_

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "Com_Types.h"
/*adding underlying layer's head files here if necessary*/ 
#include "CanIf.h"
/*******************************************************************************
*   Macro define
*******************************************************************************/
#define COM_ENABLE_RETRANSMIT
#define COM_ENABLE_ACTIVEPORPERTY
#define COM_ENABLE_TX_CONFIRMATIONFUN
#define COM_ENABLE_TX_CONFIRMATIONFLAG
#define COM_ENABLE_TX_TO_INDFUN
#define COM_ENABLE_TX_TO_INDFLAG
#define COM_ENABLE_RX_INDICATIONFUN
#define COM_ENABLE_RX_TIMEOUTINDICATIONFUN
#define COM_ENABLE_RX_TIMEOUTINDICATIONFLAG
#define COM_ENABLE_RX_TIMEOUTVAULE
#define COM_TXMAIN_FUNCTION_PERIOD                      5u 
#define COM_RXMAIN_FUNCTION_PERIOD                      5u 
#define COM_SIGNALROUTINEMAIN_FUNCTION_PERIOD           5u 
/******************************************************************************/
/* number of Tx object                                                        */
/******************************************************************************/
#define COM_TXIPDUNUM	  1u
/******************************************************************************/
/* Handles of send messages                                                   */
/******************************************************************************/
#define COM_TXIPDU_Message1          0u


/******************************************************************************/
/* number of Tx object Signal                                                 */
/******************************************************************************/
#define COM_TXSIGNUM	 5u
/******************************************************************************/
/* Handles of send signals                                                    */
/******************************************************************************/
#define  COM_TX_SIG1    0u
#define  COM_TX_SIG2    1u
#define  COM_TX_SIG3    2u
#define  COM_TX_SIG4    3u
#define  COM_TX_SIG5    4u

/******************************************************************************/
/* Send Signal structures                                                     */ 
/******************************************************************************/
/*M3 信号定义*/                         
typedef struct {
  COM_UINT8 Tx_signal1     : 3;  //1.0-1.2 HEAT_STATUS
  COM_UINT8 Tx_signal2     : 2;  //1.3-1.4 Heat_result
  COM_UINT8 Unused0        : 3;  //1.5-1.7
  COM_UINT8 Unused1        : 8;  //2.0-2.7
  COM_UINT8 Unused2        : 8;  //3.4-3.7
  COM_UINT8 Unused3        : 8;  //4.0-4.7
  COM_UINT8 Unused4        : 8;  //5.4-5.7
  COM_UINT8 Tx_signal3     : 8;  //6.6-6.7  软件版本
  COM_UINT8 Tx_signal4     : 8;  //7.0-7.7  硬件版本
  COM_UINT8 Tx_signal5     : 8;  //8.0-8.7  DTC
}Ipdu_TxMessage1_Type;



/********************************************************************************/   
/* Send Message unions */
/********************************************************************************/   
typedef union {
Ipdu_TxMessage1_Type Ipdu_TxMessage1;
  COM_UINT8 _c[8];
}Ipdu_TxMessage1_bufType;


/******************************************************************************/   
/* Databuffer for sended objects                                              */ 
/******************************************************************************/   
extern Ipdu_TxMessage1_bufType         Ipdu_TxMessage1;

/******************************************************************************/   
/* Databuffer for Sended Message default Value                                */  
/******************************************************************************/   
extern COM_CONST Ipdu_TxMessage1_bufType         Ipdu_TxMessage1_DefualtValue;



#if (COM_TXIPDUNUM>=1)
/******************************************************************************/   
/*used for application:message tx timeout indication flag                     */
/******************************************************************************/   
#ifdef COM_ENABLE_TX_TO_INDFLAG
extern COM_UINT8 Com_TxIpduTimeOutFlag[COM_TXIPDUNUM];
#define COM_TXIPDU_Message1_TO_FLAG       Com_TxIpduTimeOutFlag[COM_TXIPDU_Message1]
#endif
/******************************************************************************/   
/* used for application:MFSage tx timeout indication Function                */
/******************************************************************************/   
#ifdef COM_ENABLE_TX_TO_INDFUN
extern void AppIpdu_Message1_TxTOIndication(void);

#endif

/******************************************************************************/   
/* tx confirmation flag definition                                            */
/******************************************************************************/   
#ifdef COM_ENABLE_TX_CONFIRMATIONFLAG
extern COM_UINT8 Com_TxIdpuConfFlag[COM_TXIPDUNUM];
#define COM_TXIPDU_Message1_TO_FLAG    Com_TxIdpuConfFlag[COM_TXIPDU_Message1]
#endif

/******************************************************************************/   
/*                   tx confirmation function definition                      */
/******************************************************************************/   
#ifdef COM_ENABLE_TX_CONFIRMATIONFUN
extern void AppTxIpdu_Message1_Conf(void);
#endif


#endif /*#if (COM_TXIPDUNUM>=1)*/

/******************************************************************************/   
/* number of Rx object Ipdu                                                   */
/******************************************************************************/   
#define COM_RXIPDUNUM 2u
/******************************************************************************/   
/* Handles of receive messages                                                */
/******************************************************************************/   
#define COM_RXIPDU_Message1                  0u
#define COM_RXIPDU_Message2                  1u

/******************************************************************************/   
/*the buffer size of Message received                                         */
/******************************************************************************/   
#define COM_RXIPDU_Message1_BUFFER_SIZE             	8u
#define COM_RXIPDU_Message2_BUFFER_SIZE                	8u

/******************************************************************************/   
/* number of Rx object Signal                                                 */
/******************************************************************************/   
#define COM_RXSIGNUM	 5u
/******************************************************************************/   
/* Handles of receive signals                                                 */
/******************************************************************************/   
#define  COM_RX_SIG1                   0u
#define  COM_RX_SIG2                   1u
#define  COM_RXSIG_HVTSTS              2u
#define  COM_RXSIG_KEYSTATE            3u
#define  COM_RXSIG_VEHSPD              4u


/******************************************************************************/   
/* Receive Signal structures                                                  */ 
/******************************************************************************/   
/*0X2CD*/
typedef struct 
{
  COM_UINT8 Unused0             : 8;            //1.0-1.7
  COM_UINT8 Unused1             : 8;            //2.0-2.7
  COM_UINT8 Unused2             : 7;            //3.0-3.6
  COM_UINT8 Rx_signal1          : 1;            //3.7     加热请求 SteerWhllHeatSW
  COM_UINT8 Unused3             : 8;            //4.0-4.7
  COM_UINT8 Unused4             : 8;            //5.0-5.7
  COM_UINT8 Unused5             : 8;            //6.0-6.7
  COM_UINT8 Unused7             : 8;            //7.0-7.7
  COM_UINT8 Unused8             : 8;            //8.0-8.7
}RxIpdu_Message1_Type;
 

/*0X711 */
typedef struct 
{
  COM_UINT8 Unused0             : 8;            //1.0-1.7
  COM_UINT8 Unused1             : 3;            //2.0-2.2
  COM_UINT8 Rx_signal2          : 2;            //2.3-2.4   发动机状态 EngState
  COM_UINT8 Unused2             : 3;            //2.5-2.7
  COM_UINT8 Unused3             : 8;            //3.0-3.7
  COM_UINT8 Unused4             : 8;            //4.0-4.7
  COM_UINT8 Unused5             : 8;            //5.0-5.6
  COM_UINT8 Unused6             : 8;            //6.0-6.7
  COM_UINT8 Unused7             : 8;            //7.0-7.7
  COM_UINT8 Unused8             : 8;            //8.0-8.7
}RxIpdu_Message2_Type;


//type 193
typedef struct 
{
  COM_UINT8 Unused0             : 8;            //1.0-1.7
  COM_UINT8 Unused1             : 8;            //2.0-2.7
  COM_UINT8 Unused2             : 8;            //3.0-3.7
  COM_UINT8 Unused3             : 8;            //4.0-4.7
  COM_UINT8 Unused4             : 8;            //5.0-5.7
  COM_UINT8 Unused5             : 8;            //6.0-6.7
  COM_UINT8 Unused6             : 5;            //7.0-7.4
  COM_UINT8 HvtSts              : 1;            //7.5
  COM_UINT8 Unused7             : 2;            //7.6-7.7  
  COM_UINT8 Unused8             : 8;            //8.0-8.7
}Ipdu_193_Type;

//frame:1F4 sigpos:3.0-3.2 signame:KeyState other:unused
typedef struct 
{
  COM_UINT8 Unused0             : 8;            //1.0-1.7
  COM_UINT8 Unused1             : 8;            //2.0-2.7
  COM_UINT8 Unused2             : 8;            //3.0-3.2
  COM_UINT8 Unused3             : 8;            //4.0-4.7
  COM_UINT8 Unused4             : 8;            //5.0-5.7
  COM_UINT8 KeyState            : 3;            //6.0-6.2
  COM_UINT8 Unused5             : 5;            //6.3-6.7
  COM_UINT8 Unused6             : 8;            //7.0-7.7
  COM_UINT8 Unused7             : 8;            //8.0-8.7

}Ipdu_1F4_Type;

//frame:23C sigpos:msb 3.0-3.6 lsb 4.0-4.7 signame:VehSpd other:unused
typedef struct 
{
  COM_UINT8 Unused0             : 8;            //1.0-1.7
  COM_UINT8 Unused1             : 8;            //2.0-2.7
  COM_UINT8 VehSpdMsb           : 7;            //3.0-3.6
  COM_UINT8 Unused2             : 1;            //3.7
  COM_UINT8 VehSpdLsb           : 8;            //4.0-4.7
  COM_UINT8 Unused3             : 8;            //5.0-5.7
  COM_UINT8 Unused4             : 8;            //6.0-6.7
  COM_UINT8 Unused5             : 8;            //7.0-7.7
  COM_UINT8 Unused6             : 8;            //8.0-8.7
}Ipdu_23C_Type;


/********************************************************************************/   
/* Receive Message unions */
/********************************************************************************/   
typedef union {
RxIpdu_Message1_Type RxIpdu_Message1;
  COM_UINT8 _c[8];
}RxIpdu_Message1_bufType;

typedef union {
RxIpdu_Message2_Type RxIpdu_Message2;
  COM_UINT8 _c[8];
}RxIpdu_Message2_bufType;

typedef union {
Ipdu_193_Type Ipdu_193;
  COM_UINT8 _c[8];
}Ipdu_193_bufType;

typedef union {
Ipdu_1F4_Type Ipdu_1F4;
  COM_UINT8 _c[8];
}Ipdu_1F4_bufType;

typedef union {
Ipdu_23C_Type Ipdu_23C;
  COM_UINT8 _c[8];
}Ipdu_23C_bufType;

/******************************************************************************/   
/* Databuffer for receive objects                                             */ 
/******************************************************************************/   
extern RxIpdu_Message1_bufType         RxIpdu_Message1;
extern RxIpdu_Message2_bufType         RxIpdu_Message2;
extern Ipdu_193_bufType         RxIpdu_193;
extern Ipdu_1F4_bufType         RxIpdu_1F4;
extern Ipdu_23C_bufType         RxIpdu_23C;

/******************************************************************************/   
/* DefualtValue Databuffer for receive objects                                */  
/******************************************************************************/   
extern COM_CONST RxIpdu_Message1_bufType            RxIpdu_Message1_DefualtValue;  
extern COM_CONST RxIpdu_Message2_bufType            RxIpdu_Message2_DefualtValue;  
extern COM_CONST Ipdu_193_bufType            RxIpdu_193DefualtValue;
extern COM_CONST Ipdu_1F4_bufType            RxIpdu_1F4DefualtValue;
extern COM_CONST Ipdu_23C_bufType            RxIpdu_23CDefualtValue;

/******************************************************************************/   
/* Rx Timeout Value Databuffer for receive objects                            */
/******************************************************************************/   
#ifdef COM_ENABLE_RX_TIMEOUTVAULE        
extern COM_CONST  RxIpdu_Message1_bufType                  RxIpdu_Message1_TimeoutValue;
extern COM_CONST  RxIpdu_Message2_bufType                     RxIpdu_Message2_TimeoutValue;
extern COM_CONST  Ipdu_193_bufType                     RxIpdu_193TimeoutValue;
extern COM_CONST  Ipdu_1F4_bufType                     RxIpdu_1F4TimeoutValue;
extern COM_CONST  Ipdu_23C_bufType                     RxIpdu_23CTimeoutValue;

#endif

#if (COM_RXIPDUNUM>=1)
/******************************************************************************/   
/*Rx indication function definition                                           */
/******************************************************************************/   
#ifdef COM_ENABLE_RX_INDICATIONFUN
extern void AppIpdu_Message1_Ind(void); 
extern void AppIpdu_Message2_Ind(void);
extern void AppIpdu_193_Ind(void);
extern void AppIpdu_1F4_Ind(void);
extern void AppIpdu_23C_Ind(void);
#endif

/******************************************************************************/   
/*rx Ipdu timeout indication flag definition                                  */
/******************************************************************************/ 
/*M3 暂时没用到，后面有需求再改*/  
#ifdef COM_ENABLE_RX_TIMEOUTINDICATIONFLAG
extern COM_UINT8 Com_RxIpduTimeOutFlag[COM_RXIPDUNUM];
#define COM_RXIPDUADS_560_TIMEOUTFLAG      Com_RxIpduTimeOutFlag[0]
#define COM_RXIPDU_117_TIMEOUTFLAG         Com_RxIpduTimeOutFlag[1]
#define COM_RXIPDU_193_TIMEOUTFLAG         Com_RxIpduTimeOutFlag[2]
#define COM_RXIPDU_1F4_TIMEOUTFLAG         Com_RxIpduTimeOutFlag[3]
#define COM_RXIPDU_23C_TIMEOUTFLAG         Com_RxIpduTimeOutFlag[4]
#endif

/******************************************************************************/   
/*rx Ipdu timeout indication function definition                              */
/******************************************************************************/   
#ifdef COM_ENABLE_RX_TIMEOUTINDICATIONFUN
extern void AppRxIpdu_Message1_TimeOutInd(void);
extern void AppRxIpdu_Message2_TimeOutInd(void);
extern void AppRxIpdu_193_TimeOutInd(void);
extern void AppRxIpdu_1F4_TimeOutInd(void);
extern void AppRxIpdu_23C_TimeOutInd(void);
#endif


#endif /*#if (COM_RXIPDUNUM>=1)*/



#if((COM_TXIPDUNUM>=1) || (COM_RXIPDUNUM>=1))
#define COM_IPDUGROUPNUM    2u
#define COM_IPDUGROUP1   0x00000001
#define COM_IPDUGROUP2   0x00000002
#endif

/*******************************************************************************
*   data declaration
*******************************************************************************/

void  COM_SendSigSWS_HeatStatus(COM_CONST void* SigValue);
#if(COM_TXIPDUNUM>=1)
extern PduInfoType COM_TxDataPtr[COM_TXIPDUNUM];
extern COM_CONST PduInfoType COM_TxDataInitInfo[COM_TXIPDUNUM]; 
extern COM_CONST COM_UINT8 COM_TxIpduSigNum[COM_TXIPDUNUM];
extern COM_CONST COM_TxSigStruct COM_TxSigTable[COM_TXSIGNUM];
extern COM_CONST COM_UINT8* COM_TxIpduDefaultValue[COM_TXIPDUNUM];
#ifdef COM_ENABLE_TX_CONFIRMATIONFUN
extern COM_CONST COM_TxConfirmationFun COM_IpduTxConfirmFunPtr[COM_TXIPDUNUM];
extern void Com_TxIpduCallout(PduIdType PduId);
#endif

#ifdef COM_ENABLE_TX_TO_INDFUN
extern COM_CONST COM_TxTimeOutFun COM_IpduTxTimeOutFunPtr[COM_TXIPDUNUM];
#endif

#ifdef COM_ENABLE_LPDUTX_ERRORINDICATIONFUN
extern COM_CONST COM_ErrorIndicationFun COM_IpduTxErrorIndicationFunPtr[COM_TXIPDUNUM];
#endif

extern COM_CONST COM_UINT8 COM_IpduTxTypeTable[COM_TXIPDUNUM];
extern COM_CONST COM_UINT16 COM_IdpuTxFastCycleTable[COM_TXIPDUNUM];
#ifdef COM_ENABLE_ACTIVEPORPERTY 
extern FUNC(void, COM_PUBLIC_CODE) Com_ActiveSig
(
    Com_SignalIdType SignalId
);
extern FUNC(void, COM_PUBLIC_CODE) Com_DisactiveSig
(
    Com_SignalIdType SignalId
);
#endif
extern COM_CONST COM_UINT16 COM_IdpuTxCycCntTable[COM_TXIPDUNUM];
extern COM_CONST COM_UINT16 COM_TxIpduOffsetInTxSigTable[COM_TXIPDUNUM]; 
extern COM_CONST COM_UINT16 COM_IpduTxDelayCntTable[COM_TXIPDUNUM];
extern COM_CONST COM_UINT16 COM_IpduRepetitionTxCycCntTable[COM_TXIPDUNUM];
extern COM_CONST COM_UINT8 COM_IpduRepetitionTxNumTable[COM_TXIPDUNUM];
extern COM_CONST COM_UINT16 COM_IpduTxTimeOutCntTable[COM_TXIPDUNUM]; 
extern COM_CONST COM_UINT16 COM_IpduTxOffsetTimerTable[COM_TXIPDUNUM];
#endif /*#if(COM_TXIPDUNUM>=1)*/

#if(COM_RXIPDUNUM>=1)
extern COM_CONST PduInfoType COM_RxDataInitInfo[COM_RXIPDUNUM];
extern COM_CONST COM_UINT8 COM_RxIpduSigNum[COM_RXIPDUNUM];
extern COM_CONST COM_UINT16 COM_RxIpduOffsetInRxSigTable[COM_RXIPDUNUM];
extern COM_CONST COM_RxSigStruct COM_RxSigTable[COM_RXSIGNUM];
extern COM_CONST COM_IpduRxTimeOut_Struct COM_IpduRxTOTimerTable[COM_RXIPDUNUM];
extern COM_CONST COM_UINT8* COM_RxIpduDefaultValue[COM_RXIPDUNUM];

#ifdef COM_ENABLE_RX_INDICATIONFUN
extern COM_CONST COM_RxIndicationFun COM_IpduRxIndicationFunPtr[COM_RXIPDUNUM]; 
#endif

#ifdef COM_ENABLE_RX_TIMEOUTINDICATIONFUN 
extern COM_CONST COM_TimeoutIndicationFun COM_IpduRxTimeoutFunPtr[COM_RXIPDUNUM];
#endif

#ifdef COM_ENABLE_LPDURX_ERRORINDICATIONFUN
extern COM_CONST COM_ErrorIndicationFun COM_IpduRxErrorIndicationFunPtr[COM_RXIPDUNUM];
#endif

#ifdef COM_ENABLE_LPDURXSTART_INDICATIONFUN
extern COM_CONST COM_LpduRxStartIndicationFun COM_IpduRxStartIndicationFunPtr[COM_RXIPDUNUM];
#endif

#ifdef COM_ENABLE_RX_TIMEOUTVAULE
extern COM_CONST COM_UINT8* COM_IpduRxTimeoutValue[COM_RXIPDUNUM];
#endif

extern COM_CONST ComTpBufferSize COM_TpRxIpduBufferSize[COM_RXIPDUNUM];

extern void Com_RxIpduCallout(PduIdType PduId);
#endif /*#if(COM_RXIPDUNUM>=1)*/


#if(COM_TXIPDUNUM>=1)
extern COM_CONST COM_SendSignalFun COM_SendSignalFunPrt[COM_TXSIGNUM];
#endif
#if(COM_RXIPDUNUM>=1)
extern COM_CONST COM_ReceivedSignalFun COM_ReceivedSignalFunPrt[COM_RXSIGNUM];
#endif

#ifdef COM_ENABLE_ROUTESIGNAL
#if((COM_TXIPDUNUM>=1) && (COM_RXIPDUNUM>=1) && (COM_GWIPDUNUM>=1) && (COM_GWSIGNUM>=1))
extern COM_CONST COM_SignalGatewayIpduInfoType COM_SignalGatewayIpduInfo[COM_GWIPDUNUM];
extern COM_CONST COM_SignalGatewayInfoType COM_SignalGatewayInfo[COM_GWSIGNUM];
#endif /*#if((COM_TXIPDUNUM>=1) && (COM_RXIPDUNUM>=1))*/
#endif /*#ifdef COM_ENABLE_ROUTESIGNAL*/


#if(COM_IPDUGROUPNUM>=1)
extern COM_CONST COM_GroupIpduInfoType COM_GroupIpduInfo[];
extern COM_CONST COM_IpduGroupInfoType COM_IpduGroupInfo[COM_IPDUGROUPNUM];
extern void COM_UserIPDUControl(Com_IpduGroupVector IpduGroupVector);
#endif
#if(COM_TXIPDUNUM>=1)
extern COM_CONST COM_UINT16 COM_UnderlyHandle[COM_TXIPDUNUM];
#define Com_TransmitLPDU(ComTxPduId,PduInfoPtr)     CanIf_Transmit(COM_UnderlyHandle[ComTxPduId],PduInfoPtr) 
#define Com_Transmit(ComTxPduId,PduInfoPtr)	        CanIf_Transmit(COM_UnderlyHandle[ComTxPduId],PduInfoPtr)
#endif

#endif



