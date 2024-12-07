/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Dcm.c
********************************************************************************
*   Project/Product : Dcm module
*   Title           : Dcm module Header File 
*   Author          : Hirain
********************************************************************************
*   Description     : Main header file of the AUTOSAR Diagnositc communcation 
*   manager, according to:AUTOSAR_SWS_DiagnosticCommunicationManager.pdf (Release
*   4.0) and ISO14229-1.pdf                      
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
*   01.00.00    12/03/2010    xyliu         N/A          Original
*   01.02.01    22/12/2011    xyliu         N/A          HrAsrDcm111222-01
*   01.02.02    12/09/2012    dongeryang    N/A          HrAsrDcm120912-01
*   02.00.01    27/07/2013    jinbiao.li    N/A          HrAsrDcm130727-01
*   02.00.02    14/08/2013    jinbiao.li    N/A          HrAsrDcm130814-01
*   02.00.03    19/08/2013    jinbiao.li    N/A          HrAsrDcm130819-01
*   02.00.04    27/08/2013    jinbiao.li    N/A          HrAsrDcm130827-01
*   02.01.00    03/09/2013    jinbiao.li    N/A          HrAsrDcm130903-01
********************************************************************************
* END_FILE_HDR*/

#ifndef _DCM_H_
#define _DCM_H_

/*******************************************************************************
* Include files
*******************************************************************************/
#include "Dcm_Types.h"
#include "Dcm_Cfg.h"

#if(ISO_15031_5 == STD_ON)
#include "OBD_Cfg.h"
#include "OBD.h"
#endif

/*******************************************************************************
* Local Macro and Tyedef
*******************************************************************************/
/* PduIdType */
#define UDS_PHYSICAL_ON_CAN_RX   (0u)
#define UDS_FUNCTIONAL_ON_CAN_RX  (1u) 
#define UDS_ON_CAN_TX  (0u)

/* Diagnostic State Machine */
#define DIAG_IDLE  ((uint8)0x01) 
#define DIAG_UDS_INDICATION  ((uint8)0x02)  //诊断状态机接收到 UDS（统一诊断服务）指示。
#define DIAG_UDS_PROCESSING  ((uint8)0x04)  // 表示系统正在执行 UDS 请求的处理逻辑
#define DIAG_UDS_RCRP  ((uint8)0x08)        // 表示系统已经发送了一个响应，但仍在等待确认
#define DIAG_UDS_RCRP_DONE  ((uint8)0x10)   // 表示系统已经发送了一个响应，并且已经确认
#define DIAG_UDS_PERIODIC_TX  ((uint8)0x20)  // 表示系统正在发送一个周期性传输数据

extern volatile uint8_t Flag_diag_session;
/* For 0x22 */
typedef struct
{
    uint16  Did;
    uint16  DidPosition; 
}DidlistType;
 
/* For 0x2A */
#if(DCM_SERVICE_2A_ENABLED == STD_ON)
typedef struct
{
    uint8   Did;
    uint16  DidPosition; 
}PeriodicDidlistType;
#endif 

/* For 0x2C */
#if(DCM_SERVICE_2C_ENABLED == STD_ON)
/* 0x00: byDid, 0x01:ByAddress */
typedef struct
{
    uint8 ByDidOrAddress; 
    uint8 DidOrAddressCounter;
}DynamicalDidDefinedSequence;

typedef struct
{
    uint16   SourceDid;
    uint16   SourceDidPosition;
    uint16   PositionInSourceDataRecord;
    uint16   MemorySize;
}DefinedByDid;

typedef struct
{
    uint32   MemoryAddress;
    uint8    AddressSegment;
    uint32   MemorySize;
  
}DefinedByAddress;

typedef struct
{
    uint16 DynamicalDid;
    /* Posicon in gDcmDsdSubService_2C */
    uint8  DynamicalDidPosion;
    uint8  DefinedTime;
    DynamicalDidDefinedSequence  Sequence[16];
    /* size should be configurable */
    DefinedByDid  ByDid[16];  
    uint8  SourceDidCounter;
    /* size should be configurable */        
    DefinedByAddress  ByAddress[16]; 
    uint8  AddessCounter;
    uint32  TotalDataSize;
}DynamicalDidRelatedInfo;
#endif

/******************************************************************************* 
*  Global variables(Scope:global)
*******************************************************************************/ 
#define DCM_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
        
extern VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                                gUDS_Physical_DiagBuffer[UDS_PHYS_BUFFER_SIZE]; 
extern VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                              gUDS_Functional_DiagBuffer[UDS_FUNC_BUFFER_SIZE]; 

#if(ISO_15031_5_MultiChannel == STD_ON)
extern VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                                gOBD_Physical_DiagBuffer[OBD_PHYS_BUFFER_SIZE];
extern VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                              gOBD_Functional_DiagBuffer[OBD_FUNC_BUFFER_SIZE];
extern VAR(DcmDslBufferSize, DCM_NOINIT_DATA)  gDcmDslAvailableBufferSize[4];
#else
extern VAR(DcmDslBufferSize, DCM_NOINIT_DATA)  gDcmDslAvailableBufferSize[2];
#endif 

extern VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                                 gNegativeResponseBuffer[NEG_RESP_BUFFER_SIZE];
extern VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                          gNegativeResponseBufferForRRCP[NEG_RESP_BUFFER_SIZE];

#if(DCM_SERVICE_2A_ENABLED == STD_ON)
extern VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                             gPeriodicResponseBuffer[PERODIC_RESP_BUFFER_SIZE];
#endif

#if(DCM_SERVICE_86_ENABLED == STD_ON)
extern VAR(Dcm_MsgItemType, DCM_NOINIT_DATA) 
                                gResponseOnEventBuffer[EVENT_RESP_BUFFER_SIZE];
#endif

extern VAR(uint16, DCM_NOINIT_DATA)  gResponseLength;   

#if(DCM_SERVICE_27_ENABLED == STD_ON)
extern Dcm_SecLevelType  gSecLevelType;
#endif

extern VAR(Dcm_SecLevelType, DCM_NOINIT_DATA)  gSesCtrlType;
extern VAR(Dcm_ProtocolType, DCM_NOINIT_DATA)  gActiveProtocol;
extern VAR(Dcm_MsgContextType, DCM_NOINIT_DATA)  gMsgContextType;
extern VAR(VoidEcucSubFunctionNameDef, DCM_NOINIT_DATA)  
                                                           gSubFunctionHandler;
extern VAR(EcucFunctionNameDef, DCM_NOINIT_DATA)  gFunctionHandler; 
extern VAR(Dcm_NegativeResponseCodeType, DCM_NOINIT_DATA)  
                                                         gNegativeResponseCode;
extern VAR(uint8, DCM_NOINIT_DATA)  gDiagState;
extern VAR(uint32, DCM_NOINIT_DATA) gP2ServerTimer;
extern VAR(uint32, DCM_NOINIT_DATA) gS3ServerTimer;
extern VAR(uint8, DCM_NOINIT_DATA)  gP2ServerTimerStartFlag;
extern VAR(uint8, DCM_NOINIT_DATA)  gS3ServerTimerStartFlag;
extern VAR(uint8, DCM_NOINIT_DATA)  gCurrentSecurityAccessRequestLevel;
extern VAR(uint8, DCM_NOINIT_DATA)  gSecurityDelayTimeOnBootFlag; 

#if(DCM_SERVICE_27_ENABLED == STD_ON)
extern VAR(uint32, DCM_NOINIT_DATA) 
                         gSecurityAcessDelayTimeOnBoot[KIND_OF_SECURITY_LEVEL];
extern VAR(uint8, DCM_NOINIT_DATA)  
                            gSecurityAcessAttempNumber[KIND_OF_SECURITY_LEVEL];   
extern VAR(uint8, DCM_NOINIT_DATA)  
                    gSecurityAcessAttempNumberExceeded[KIND_OF_SECURITY_LEVEL];
extern VAR(uint32, DCM_NOINIT_DATA) 
                               gSecurityAcessDelayTime[KIND_OF_SECURITY_LEVEL];      
extern VAR(uint8, DCM_NOINIT_DATA)  
                                gSecurityAcessSequence[KIND_OF_SECURITY_LEVEL];
#endif

#if(DCM_SERVICE_2A_ENABLED == STD_ON)
extern VAR(uint32, DCM_NOINIT_DATA) 
                                 gPeriodicResponseRate[NUMBER_OF_PERIODIC_DID];
extern VAR(uint32, DCM_NOINIT_DATA) 
                                gPeriodicResponseTimer[NUMBER_OF_PERIODIC_DID];
extern VAR(uint8, DCM_NOINIT_DATA)  PeriodicDidCounter; 
#endif

#if(DCM_SERVICE_2C_ENABLED == STD_ON)
extern VAR(DynamicalDidRelatedInfo, DCM_NOINIT_DATA)
                                   gDynamicalDidRelatedInfo[NUMBER_OF_DYN_DID];
extern VAR(uint8, DCM_NOINIT_DATA)  gDynamicalDidCounter;
#endif

#define DCM_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/******************************************************************************* 
*  Macro function 
*******************************************************************************/        
#define Set_SecLevel(x)  (gSecLevelType = (x))
#define Set_SesCtrl(x)  (gSesCtrlType = (x))  //用来设置三种会话服务
#define Set_ActiveProtocol(x)  (gActiveProtocol = (x))
#define Clr_ActiveProtocol()  (gActiveProtocol = (0))
#define Set_PduId(x)  (gMsgContextType.dcmRxPduId = (x))
#define Reset_PduId()  (gMsgContextType.dcmRxPduId = DCM_INVALID_HANDLE_OR_ID)
#define Set_P2_Server_Timer(x)  (gP2ServerTimer = (uint32)((x)/DCM_TASK_TIME))
#define Dec_P2_Server_Timer()  (gP2ServerTimer--)
#define Clr_P2_Server_Timer()  (gP2ServerTimer = 0)
#define Set_S3_Server_Timer(x)  (gS3ServerTimer = (uint32)((x)/DCM_TASK_TIME))
#define Dec_S3_Server_Timer()  (gS3ServerTimer--)
#define Clr_S3_Server_Timer()  (gS3ServerTimer = 0)
#define Set_DiagState(x)  (gDiagState |= (x))
#define Clr_DiagState(x)  (gDiagState &= (~(x)))
#define Reset_DiagState()  (gDiagState = DIAG_IDLE)
#define GetSuppressPosResponseBit()  (gMsgContextType.msgAddInfo.suppressPosResponse)
#define ClrSuppressPosResponseBit()  (gMsgContextType.msgAddInfo.suppressPosResponse = 0)
#define SetNegativeResponseCode(x)  (gNegativeResponseCode = (x))
#define ClrNegativeResponseCode()  (gNegativeResponseCode = 0)
#define Dec_Max_Number_Of_RCRRP()  (gMaxNumberOfRCRRP--)
#define Reset_Max_Number_Of_RCRRP()  (gMaxNumberOfRCRRP = DCM_NUM_MAX_RESPPEND)
#define Make16Bit(HiByte,LoByte)  ((uint16)((((uint16)(HiByte)) << 8)|((uint16)(LoByte))))
#define Make32Bit(HiByte,MiByte,LoByte)  ((uint32)((((uint32)(HiByte)) << 16)|((uint32)(MiByte) << 8)|((uint32)(LoByte))))

/*******************************************************************************
* extern Function
*******************************************************************************/
/* Defined in AUTOSAR */
#define DCM_START_SEC_PUBLIC_CODE
#include "MemMap.h"

extern FUNC(void,DCM_PUBLIC_CODE) Dcm_Init(P2VAR(Dcm_ConfigType,AUTOMATIC,DCM_APPL_DATA) ConfigPtr);
extern FUNC(void,DCM_PUBLIC_CODE) Dsd_InteralInit(void);
extern FUNC(void,DCM_PUBLIC_CODE) Dcm_GetVersionInfo(P2VAR(Std_VersionInfoType,AUTOMATIC,DCM_APPL_DATA) versionInfo);
extern FUNC(Std_ReturnType,DCM_PUBLIC_CODE) Dcm_GetSecurityLevel(P2VAR(Dcm_SecLevelType,AUTOMATIC,DCM_APPL_DATA) SecLevel);
extern FUNC(Std_ReturnType,DCM_PUBLIC_CODE) Dcm_GetSesCtrlType(P2VAR(Dcm_SesCtrlType,AUTOMATIC,DCM_APPL_DATA) SesCtrlType);
extern FUNC(Std_ReturnType,DCM_PUBLIC_CODE) Dcm_GetActiveProtocol(P2VAR(Dcm_ProtocolType,AUTOMATIC,DCM_APPL_DATA) ActiveProtocol);
extern FUNC(void,DCM_PUBLIC_CODE) Dcm_MainFunction(void);

#if(DCM_DSL_DIAG_RESP_FORCE_RESP_PEND_EN == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DslInternal_RCRResponsePending(uint8 ForceRCR_RP);
#else
STATIC FUNC(void,DCM_PRIVATE_CODE) DslInternal_RCRResponsePending(uint8 ForceRCR_RP);
#endif

extern FUNC(void,DCM_PUBLIC_CODE) DsdInternal_ProcessingDone(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_PUBLIC_CODE) DsdInternal_ProcessingDoneNoResponse(void);

#if(DCM_SERVICE_22_COMBINED_DID == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DsdInternal_DidProcessingDone(void);
#endif

extern void DsdInternal_SetNegResponse(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext,uint8 ErrorCode);
/* UDS diagnostic service */
#if(DCM_SERVICE_10_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_DiagnosticSessionControl(void);
extern FUNC(void,DCM_PUBLIC_CODE) App_DefaultPost(Std_ReturnType Result);
#endif

extern uint8 Dsd_GetSessionMapId(Dcm_SesCtrlType DcmSessionType) ;

#if(DCM_SERVICE_11_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_EcuReset(void);
#endif

#if(DCM_SERVICE_14_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ClearDiagnosticInformation(void);
extern FUNC(void,DCM_PUBLIC_CODE) App_ClearDiagnosticInformation(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
#endif

#if(DCM_SERVICE_19_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadDTCInformation(void);
#endif

#if(DCM_SERVICE_22_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadDataByIdentifier(void);
#endif

#if(DCM_SERVICE_23_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadMemoryByAddress(void);
#endif

#if(DCM_SERVICE_24_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadScalingDataByIdentifier(void);
#endif

#if(DCM_SERVICE_27_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_SecurityAccess(void);
extern void  DsdInternal_SecurityAccessKeyCompared(uint8 level, DcmDspSecurityAccessKey keyIsValid);
#endif

#if(DCM_SERVICE_28_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_CommunicationControl(void);
#endif

#if(DCM_SERVICE_2A_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadDataByPeriodicIdentifier(void);
#endif

#if(DCM_SERVICE_2C_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_DynamicallyDefinedDataIdentifier(void);
#endif

#if(DCM_SERVICE_2E_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_WriteDataByIdentifier(void);
#endif

#if(DCM_SERVICE_2F_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_InputOutputControlByIdentifier(void);
#endif

#if(DCM_SERVICE_31_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE)  DspInternal_Uds_RoutineControl(void);
extern FUNC(void,DCM_PUBLIC_CODE)  DsdInternal_RoutineStarted(void);
extern FUNC(void,DCM_PUBLIC_CODE)  DsdInternal_RoutineStopped(void);
extern FUNC(void,DCM_PUBLIC_CODE)  DsdInternal_RequestRoutineResults(void);
#endif

#if(DCM_SERVICE_3D_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_WriteMemoryByAddress(void);
#endif

#if(DCM_SERVICE_3E_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_TesterPresent(void);
#endif

#if(DCM_SERVICE_85_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ControlDTCSetting(void);
#endif

#if(DCM_SERVICE_86_ENABLED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ResponseOnEvent(void);
#endif

#if(DCM_COSTOMIZED_SERVCIE_SUPPORTED == STD_ON)
extern FUNC(void,DCM_PUBLIC_CODE) App_CustomizedServcie(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
#endif

#define DCM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

#endif
