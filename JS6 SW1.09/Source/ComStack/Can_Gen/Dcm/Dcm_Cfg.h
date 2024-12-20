/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Dcm_Cfg.h
********************************************************************************
*   Project/Product : Dcm module
*   Title           : Dcm module Header File 
*   Author          : Hirain
********************************************************************************
*   Description     : Main header file of the AUTOSAR Diagnositc communcation 
*   manager, according to:AUTOSAR_SWS_DiagnosticCommunicationManager.pdf (Release
*   4.0) and ISO14229-1.pdf.
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
*   02.01.00    03/09/2013    jinbiao.li    N/A          HrAsrDcm130903-01
********************************************************************************
* END_FILE_HDR*/
#ifndef _DCM_CFG_H_
#define _DCM_CFG_H_

/*******************************************************************************
* Include files
********************************************************************************/
//#include "Dcm.h"
#include "Dcm_Types.h"
#include "PduR_Dcm.h"
#include "keyboard_config.h"
/*******************************************************************************
* Macros and Tyedef
********************************************************************************/

#ifdef  SWSCONFIG_HIGH
#define NUM_OF_DTC        10u
#endif

#ifdef  SWSCONFIG_MID
#define NUM_OF_DTC       19u
#endif

#ifdef  SWSCONFIG_LOW
#define NUM_OF_DTC       15u
#endif
 /******************************************************************************\ 
|                            The following macros are non-configurable                |
 \*****************************************************************************/
#define  UseBlockId                                                 0x01u
#define  UseDataClientServer                                        0x02u
#define  UseDataSenderReceiver                                      0x03u
#define  UseEcuSignal                                               0x04u
#define  UseFnc                                                     0x05u
#define  DCM_NULL                                                   ((void*)0)
#define  SUB_FUNCTION_NOT_SUPPORTED                                 0x00u
#define  SUB_FUNCTION_SUPPORTED                                     0x01u
#define  PHYSICAL_REQ_SUPPORTED                                     0x01u
#define  FUNCTIONAL_REQ_SUPPORTED                                   0x02u
#define  PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED                      0x03u
/* For 0x19 */
#define DCM_REPORTNUMBEROFDTCBYSTATUSMASK                               0x01u
#define DCM_REPORTDTCBYSTATUSMASK                                       0x02u
#define DCM_REPORTDTCSNAPSHOTIDENTIFICATION                             0x03u
#define DCM_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER                          0x04u
#define DCM_REPORTDTCSNAPSHOTRECORDBYRECORDNUMBER                       0x05u
#define DCM_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER                      0x06u
#define DCM_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD                       0x07u
#define DCM_REPORTDTCBYSEVERITYMASKRECORD                               0x08u
#define DCM_REPORTSEVERITYINFORMATIONOFDTC                              0x09u
#define DCM_REPORTSUPPORTEDDTC                                          0x0Au
#define DCM_REPORTFIRSTTESTFAILEDDTC                                    0x0Bu
#define DCM_REPORTFIRSTCONFIRMEDDTC                                     0x0Cu
#define DCM_REPORTMOSTRECENTTESTFAILEDDTC                               0x0Du
#define DCM_REPORTMOSTRECENTCONFIRMEDDTC                                0x0Eu
#define DCM_REPORTMIRRORMEMORYDTCBYSTATUSMASK                           0x0Fu
#define DCM_REPORTMIRRORMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER          0x10u
#define DCM_REPORTNUMBEROFMIRRORMEMORYDTCBYSTATUSMASK                   0x11u
#define DCM_REPORTNUMBEROFEMISSIONSRELATEDOBDDTCBYSTATUSMASK            0x12u
#define DCM_REPORTEMISSIONSRELATEDOBDDTCBYSTATUSMASK                    0x13u

/* For 0x27 */
#define REQUEST_SEED_L1                                         0x01u
#define SEND_KEY_L1                                             0x02u
#define REQUEST_SEED_L11                                        0x11u
#define SEND_KEY_L12                                            0x12u

#define REQUEST_SEED_L3                                         0xfdu
#define SEND_KEY_L3                                         0xfcu
#define REQUEST_SEED_L4                                         0xfbu
#define SEND_KEY_L4                                         0xfau
#define REQUEST_SEED_L5                                         0xf9u
#define SEND_KEY_L5                                         0xf8u
#define REQUEST_SEED_L6                                         0xf7u
#define SEND_KEY_L6                                         0xf6u
#define REQUEST_SEED_L7                                         0xf5u
#define SEND_KEY_L7                                         0xf4u
#define REQUEST_SEED_L8                                         0xf3u
#define SEND_KEY_L8                                         0xf2u
/* For 0x28 */
#define ENABLE_RX_AND_TX                                            0x00u
#define ENBALE_RX_AND_DISABLE_TX                                    0x01u
#define DISABLE_RX_AND_ENBALE_TX                                    0x02u
#define DISABLE_RX_AND_TX                                           0x03u
#define NORMAL_MSG                                                  0x01u
#define NM_MSG                                                      0x02u
#define BOTH_NORMAL_AND_NM_MSG                                      0x03u
/* For 0x2C */
#define DEFINEBYIDENTIFIER                                          0x01u
#define DEFINEBYMEMORYADDRESS                                       0x02u
#define CLEARDYNAMICALLYDEFINEDDATAIDENTIFIER                       0x03u
/* For 0x2F */
#define RETURN_CONTROL_TO_ECU                                       0x00u
#define RESET_TO_DEFAULT                                            0x01u
#define FREEZE_CURRENT_STATE                                        0x02u
#define SHORT_TERM_ADJUSTMENT                                       0x03u
/* For 0x31 */
#define START_ROUTINE                                               0x01u
#define STOP_ROUTINE                                                0x02u
#define REQUEST_ROUTINE_RESULT                                      0x03u
/* For 0x3E */
#define ZERO_SUB_FUNCTION                                           0x00u         
/* For 0x85 控制dtc开/关*/
#define DTC_RECORD_ON                                               0x01u
#define DTC_RECORD_OFF                                              0x02u
#define DcmTransmit(DcmTxPduId,PduInfoPtr)                          PduR_DcmTransmit(DcmTxPduId,PduInfoPtr) 
#define Dcm_DefaultSessionPost(result)                              App_DefaultPost(result)

#define DCM_VENDOR_ID             (uint16)(0xFFFFu)  /* Supplier ID */
#define DCM_MODULE_ID             (uint16)(0x0023u)  /* Dcm Moudle ID */
/* CanTp Component release Version  */
#define DCM_SW_MAJOR_VERSION      (uint8)(0x01u)
#define DCM_SW_MINOR_VERSION      (uint8)(0x01u)
#define DCM_SW_PATCH_VERSION      (uint8)(0x00u)
/* Autosar release version */
#define DCM_AR_MAJOR_VERSION      (uint8)(0x01u)
#define DCM_AR_MINOR_VERSION      (uint8)(0x00u)
#define DCM_AR_PATCH_VERSION      (uint8)(0x00u)

 /***********************************************************************************\ 
|                           The following macros are configurable                     |
 \***********************************************************************************/
#define DCM_DSP_DATA_USE_PORT                                        UseFnc
#define DCM_DEV_ERROR_DETECT                                         STD_OFF
#define DCM_OEM_INDICATION                                           STD_OFF
#define DCM_SUPPLIER_INDICATION                                      STD_OFF
#define DCM_RESPONSE_ALL_REQ                                         STD_OFF
#define DCM_VERSION_INFO_API                                         STD_ON
/* For 15031-5 */
#define ISO_15031_5                                                  STD_OFF
#define ISO_15031_5_MultiChannel                                     STD_OFF
/* Related to Dcm calling cycle */
#define DCM_TASK_TIME                                                (uint32)5u
/* Related to 0x78 negative response */
#define DCM_DSL_DIAG_RESP_FORCE_RESP_PEND_EN                         STD_ON
#define DCM_NUM_MAX_RESPPEND                                         4294967295u
/* Related to customized service */
#define DCM_COSTOMIZED_SERVCIE_SUPPORTED                             STD_OFF
/* Related to Obd service */
/* Related to Uds service */
#define DCM_NUM_OF_SERVICE                                           11u+1u 
#define DCM_NUM_OF_SUB_SERVICE                                       (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+\
                                                                      NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION+\
                                                                      NUM_OF_28_SUB_FUNCTION+NUM_OF_2C_SUB_FUNCTION+\
                                                                      NUM_OF_31_SUB_FUNCTION+NUM_OF_3E_SUB_FUNCTION+\
                                                                      NUM_OF_85_SUB_FUNCTION+NUM_OF_86_SUB_FUNCTION+1u)
#define UDS_SERVICE_WITHOUT_SUB_FUNCTION                             0u //无子功能的UDS服务
#define UDS_SERVICE_WITH_SUB_FUNCTION                                1u //有子功能
#define DCM_DSDSIDTABID                                              1u//用于标识 DCM（Diagnostic Communication Manager）中的某个表或 ID
#define DCM_SUBSERVICESTART                                          0u//用于标识子服务编号的起始值
#define DCM_SERVICE_10_ENABLED                                       STD_ON
#if(DCM_SERVICE_10_ENABLED==STD_ON)
/*For user defined session control subfunction*/
/*start*/
#define DCM_Kuwe_Session_SESSION                            0x04
#define DCM_SESSION_Kuwe_Session                          DCM_SESSION_USERDEFINED1 
#define DCM_SERVICE_10_Default                              STD_ON
#define DCM_SERVICE_10_Programming                              STD_ON
#define DCM_SERVICE_10_Extended_Diagnostic                              STD_ON
#define DCM_SERVICE_10_UserDefined1                                STD_OFF
#define DCM_SERVICE_10_UserDefined2                                STD_OFF
#define DCM_SERVICE_10_UserDefined3                                STD_OFF
#define DCM_SERVICE_10_UserDefined4                                STD_OFF
#define DCM_SERVICE_10_UserDefined5                                STD_OFF
#define KIND_OF_SESSION                                              3u
#define NUM_OF_10_SUB_FUNCTION                                       3u
#define DEFAULT_AND_EXTENDED_SEESION_HANDLE             (DCM_SESSION_DEFAULT  | DCM_SESSION_EXTENDED_DIAGNOSTIC)
#define EXTENDED_SEESION_HANDLE                                      DCM_SESSION_EXTENDED_DIAGNOSTIC  
/*定时器用于检测诊断请求的响应超时。当客户端发送一个诊断请求后，P2 定时器开始计时。
如果在 P2 定时器到期前没有收到响应，客户端会认为服务器未能及时处理请求，可能会重新发送请求或采取其他措施。*/
#define RESPONSE_WITH_P2TIMER                                        STD_ON 
#else
#define NUM_OF_10_SUB_FUNCTION                                       0u
#endif
#define DCM_SERVICE_11_ENABLED                                       STD_ON
#if(DCM_SERVICE_11_ENABLED==STD_ON)
#define DCM_SERVICE_11_HardReset                                     STD_ON
#define DCM_SERVICE_11_EnableRapidPowerShutDown                                     STD_OFF
#define DCM_SERVICE_11_DisableRapidPowerShutDown                                     STD_OFF
#define DCM_SERVICE_11_UserDefined1                                     STD_OFF
#define DCM_SERVICE_11_UserDefined2                                     STD_OFF
#define DCM_SERVICE_11_KeyOffOnReset                                     STD_OFF
#define DCM_SERVICE_11_SoftReset                                     STD_OFF
#define NUM_OF_11_SUB_FUNCTION                                       1u 
#else
#define NUM_OF_11_SUB_FUNCTION                                       0u
#endif
#define DCM_SERVICE_14_ENABLED                                       STD_ON
#define DCM_SERVICE_19_ENABLED                                       STD_ON
#if(DCM_SERVICE_19_ENABLED==STD_ON) 
#define reportNumberOfDTCByStatusMask                                STD_ON 
#define reportDTCByStatusMask                                STD_ON 
#define reportSupportedDTC                                STD_ON 
#define reportDTCBySeverityMaskRecord                                     STD_OFF
#define reportSeverityInformationOfDTC                                     STD_OFF
#define reportDTCSnapshotRecordByDTCNumber                                     STD_ON
#define reportDTCSnapshotRecordByRecordNumber                                     STD_OFF
#define reportDTCExtendedDataRecordByDTCNumber                                     STD_ON
#define reportNumberOfDTCBySeverityMaskRecord                                     STD_OFF
#define reportEmissionsRelatedOBDDTCByStatusMask                                     STD_OFF
#define reportNumberOfMirrorMemoryDTCByStatusMask                                     STD_OFF
#define reportNumberOfEmissionsRelatedOBDDTCByStatusMask                                     STD_OFF
#define reportDTCSnapshotIdentification                                     STD_OFF
#define reportMirrorMemoryDTCExtendedDataRecordByDTCNumber                                     STD_OFF
#define reportMostRecentConfirmedDTC                                     STD_OFF
#define reportMostRecentTestFailedDTC                                     STD_OFF
#define reportMirrorMemoryDTCByStatusMask                                     STD_OFF
#define reportFirstConfirmedDTC                                     STD_OFF
#define reportFirstTestFailedDTC                                     STD_OFF
#define NUM_OF_19_SUB_FUNCTION                                       5u
#else
#define NUM_OF_19_SUB_FUNCTION                                       0u
#endif
  #define DCM_SERVICE_22_ENABLED                                       STD_ON
#define DCM_SERVICE_22_COMBINED_DID                                  STD_OFF
#if(DCM_SERVICE_22_COMBINED_DID == STD_ON)
  #define MAX_NUM_OF_DID_TO_READ                                       1u
#else
  #define MAX_NUM_OF_DID_TO_READ                                       1u
#endif
#if(DCM_SERVICE_22_ENABLED==STD_ON)
#define NUMBER_OF_READ_DID                                           20u//17u
#endif
#define DCM_SERVICE_23_ENABLED                                       STD_OFF
#define DCM_SERVICE_24_ENABLED                                       STD_OFF
#define DCM_SERVICE_27_ENABLED                                       STD_ON
#if(DCM_SERVICE_27_ENABLED==STD_ON)
#define DCM_SERVICE_27_Request_Seed_L1                                STD_ON
#define DCM_SERVICE_27_Send_Key_L1                                    STD_ON

#define KIND_OF_SECURITY_LEVEL                                       7u
#define NUM_OF_27_SUB_FUNCTION                                       2u
#define DISABLE_SECURITY_ACCESS_ATTEMPT_NUMBER_RESET                  STD_OFF
#else
#define NUM_OF_27_SUB_FUNCTION                                       0u
#endif

#define DCM_SERVICE_28_ENABLED                                       STD_ON
#if(DCM_SERVICE_28_ENABLED==STD_ON)
#define DCM_SERVICE_28_EnableRxAndTx                                 STD_ON
#define DCM_SERVICE_28_DisableRxAndTx                                 STD_ON
#define DCM_SERVICE_28_EnableRxAndDisableTx                                     STD_OFF
#define DCM_SERVICE_28_DisableRxAndEnableTx                                     STD_OFF
#define NUM_OF_28_SUB_FUNCTION                                       2u
#else
#define NUM_OF_28_SUB_FUNCTION                                       0u
#endif

#define DCM_SERVICE_2A_ENABLED                                       STD_OFF
#define NUMBER_OF_PERIODIC_DID                                          1u
#define DCM_SERVICE_2C_ENABLED                                       STD_OFF
#define NUM_OF_2C_SUB_FUNCTION                                         0u
#define DCM_SERVICE_2E_ENABLED                                       STD_ON
#if(DCM_SERVICE_2E_ENABLED==STD_ON)
#define NUMBER_OF_WRITE_DID                                        4u // 1u
#endif

#define DCM_SERVICE_2F_ENABLED                                       STD_OFF
#if(DCM_SERVICE_2F_ENABLED==STD_ON)
#define NUMBER_OF_CONTROL_DID                                        5u
#endif

#define DCM_SERVICE_31_ENABLED                                       STD_ON
#if(DCM_SERVICE_31_ENABLED==STD_ON)
#define DCM_SERVICE_31_startRoutine                                  STD_ON
#define DCM_SERVICE_31_stopRoutine                                  STD_ON
#define DCM_SERVICE_31_requestRoutineResults                                  STD_ON
#define NUMBER_OF_RID                                                2u
#define NUM_OF_31_SUB_FUNCTION                                       3u
#else
#define NUM_OF_31_SUB_FUNCTION                                       0u
#endif

#define DCM_SERVICE_3D_ENABLED                                       STD_OFF
#define DCM_SERVICE_3E_ENABLED                                        STD_ON
#if(DCM_SERVICE_3E_ENABLED==STD_ON)
#define NUM_OF_3E_SUB_FUNCTION                                        1u
#else
#define NUM_OF_3E_SUB_FUNCTION                                       0u
#endif
#define DCM_SERVICE_85_ENABLED                                       STD_ON
#if(DCM_SERVICE_85_ENABLED==STD_ON)
#define DCM_SERVICE_85_DTCRecordOn                                   STD_ON
#define DCM_SERVICE_85_DTCRecordOff                                   STD_ON
#define NUM_OF_85_SUB_FUNCTION                                       2u
#else
#define NUM_OF_85_SUB_FUNCTION                                       0u
#endif
#define DCM_SERVICE_86_ENABLED                                       STD_OFF
#define NUM_OF_86_SUB_FUNCTION                                         0u
/* Related to diagnostic buffer */
#define DCM_PAGEDBUFFER_ENABLED                                      STD_OFF
#define UDS_FUNC_BUFFER_SIZE                                         8u
#define UDS_PHYS_BUFFER_SIZE                                         440u
#define NEG_RESP_BUFFER_SIZE                                         3u
#if(DCM_SERVICE_2A_ENABLED==STD_ON)
#define PERODIC_RESP_BUFFER_SIZE     8
#endif
/* Additional applicaiton callback */
#define DCM_GENERAL_DIAGNOSTIC_REQUEST_CTL                           STD_OFF
typedef uint32 DcmDslBufferSize;
typedef void   (*EcucFunctionNameDef)(void);
typedef uint8  (*EcucSubFunctionNameDef)(Dcm_MsgContextType* Function);
typedef void   (*VoidEcucSubFunctionNameDef)(Dcm_MsgContextType* Function);
typedef void   (*EcucPostSubFunctionNameDef)(Std_ReturnType Result);

/* For Uds service table */
typedef struct
{
    EcucFunctionNameDef DcmDsdSidTabFnc;
    uint8               DcmDsdSidTabSubfuncAvail;
    uint8               DcmDsdSidTabSecurityLevelRef; 
    uint8               DcmDsdSidTabSessionLevelRef;
}DcmDsdService;
typedef struct
{
    uint8         DcmDsdSidTabId;
    DcmDsdService ServiceTable;
    uint8         FirstSubService; /* self-defined */
    uint8         LastSubService;  /* self-defined */
    uint8         AddressingMode;
}DcmDsdServiceTable;

/* For Uds sub-service table */
typedef struct
{
    VoidEcucSubFunctionNameDef  DcmDsdSubFunction;/* self-defined */
    EcucPostSubFunctionNameDef  DcmDsdSubFunctionPost;/* self-defined */
    uint8                       DcmDsdSubServiceId;
    uint8                       DcmDsdSubServiceIdSupported;/* self-defined */
    uint8                       DcmDsdSubServiceSecurityLevelRef;//安全等级
    uint8                       DcmDsdSubServiceSessionLevelRef;
    uint8                       AddressingMode;/* self-defined */
}DcmDsdSubService;

/* For security access */
typedef struct
{
    /* uint32 DcmDspSecurityADRSize;*/
    uint32 DcmDspSecurityDelayTime;     
    uint32 DcmDspSecurityDelayTimeOnBoot;
    uint32 DcmDspSecurityKeySize;
    uint8  DcmDspSecurityLevel;
    uint8  DcmDspSecurityNumAttDelay;
    uint32 DcmDspSecuritySeedSize;
}DcmDspSecurityRow;
typedef enum
{
    KEY_IS_NOT_VALID=0x00u,
    KEY_IS_VALID=0x01u
}DcmDspSecurityAccessKey;
/* For session control */
typedef enum
{
    DCM_NO_BOOT,
    DCM_OEM_BOOT,
    DCM_SYS_BOOT
}DcmDspSessionForBoot;
typedef struct 
{
    DcmDspSessionForBoot DcmDspSession;
    uint8                DcmDspSessionLevel;
    uint32               DcmDspSessionP2ServerMax;
    uint32               DcmDspSessionP2StarServerMax;
}DcmDspSessionRow;

/* For 0x22 */
typedef struct
{
    uint32                 DcmDspDidIdentifier;    
    boolean                DcmDspDidUsed;
    uint16                 DcmDspDataSize;
    #if(DCM_DSP_DATA_USE_PORT==UseFnc)
    VoidEcucSubFunctionNameDef DcmDspDataReadFnc;
    #endif
    #if(DCM_DSP_DATA_USE_PORT==UseEcuSignal)
    VoidEcucSubFunctionNameDef DcmDspDataEcuSignal;
    VoidEcucSubFunctionNameDef DcmDspDataReadEcuSignal;
    #endif
    uint8                  DcmDspDidReadSecurityLevelRef;
    uint8                  DcmDspDidReadSessionRef;
    uint8                  AddressingMode;
}Dcm_22_ServiceInfoType;

/* For 0x23 */
typedef struct
{
  uint32 DcmDspReadMemoryRangeHigh;
  uint32 DcmDspReadMemoryRangeLow;
  uint8  DcmDspReadMemoryRangeSecurityLevelRef;
  uint8  DcmDspReadMemoryRangeSessionRef;
  uint8  AddressingMode;
}DcmDspReadMemoryRangeInfo;

typedef uint8 Dcm_MemoryAddressFormatType;
typedef uint8 Dcm_MemorySizeFormatType;

/* For 0x24 */
typedef struct
{
    uint32                 DcmDspDidIdentifier;    
    boolean                DcmDspDidUsed;
    uint16                 DcmDspDataSize;
    #if(DCM_DSP_DATA_USE_PORT==UseFnc)
    VoidEcucSubFunctionNameDef DcmDspDataGetScalingInfoFnc;
    #endif
    #if(DCM_DSP_DATA_USE_PORT==UseEcuSignal)
    VoidEcucSubFunctionNameDef DcmDspDataEcuSignal;
    VoidEcucSubFunctionNameDef DcmDspDataReadEcuSignal;
    #endif
    uint8                  DcmDspDidReadSecurityLevelRef;
    uint8                  DcmDspDidReadSessionRef;
    uint8                  AddressingMode;
}Dcm_24_ServiceInfoType;

/* For 0x2A */
typedef struct
{
    uint8                  DcmDspDidIdentifier;    
    boolean                DcmDspDidUsed;
    uint16                 DcmDspDataSize;
    #if(DCM_DSP_DATA_USE_PORT==UseFnc)
    VoidEcucSubFunctionNameDef DcmDspDataReadFnc;
    #endif
    uint8                  DcmDspDidReadSecurityLevelRef;
    uint8                  DcmDspDidReadSessionRef;
    uint8                  AddressingMode;
}Dcm_2A_ServiceInfoType;

/* For 0x2C */
typedef struct
{
    uint32                 DcmDspDidIdentifier; 
    uint16                 DcmDspDataSize;
    uint8                  DcmDspDidReadSecurityLevelRef;/* security level must be the same as in 0x22 */
    uint8                  DcmDspDidReadSessionRef;/* session type must be the same as in 0x22 */
    uint8                  AddressingMode;
}Dcm_2C_ServiceInfoType;
/* For 0x2E */
typedef struct
{
    uint32                 DcmDspDidIdentifier;    
    boolean                DcmDspDidUsed;
    uint16                 DcmDspDataSize;
    #if(DCM_DSP_DATA_USE_PORT==UseFnc)
    VoidEcucSubFunctionNameDef DcmDspDataWriteFnc;      
    #endif
    #if(DCM_DSP_DATA_USE_PORT==UseEcuSignal)
    VoidEcucSubFunctionNameDef DcmDspDataEcuSignal;
    #endif
    uint8                  DcmDspDidWriteSecurityLevelRef;
    uint8                  DcmDspDidWriteSessionRef;
    uint8                  AddressingMode;
}Dcm_2E_ServiceInfoType;

/* For 0x2F */
typedef struct
{
    uint32 DcmDspDidControlOptionRecordSize;   /* IOCP */   
    uint32 DcmDspDidControlEnableMaskRecordSize;/* CM */ 
    uint32 DcmDspDidControlStatusRecordSize;    /* CS */
}DcmDspDidControlRecordSizes;
typedef struct
{
    uint32                       DcmDspDidIdentifier;     
    uint8                        DcmDspDidControlSecurityLevelRef;
    uint8                        DcmDspDidControlSessionRef;
    uint8                        AddressingMode;
    DcmDspDidControlRecordSizes  RecordSizes;
    #if(DCM_DSP_DATA_USE_PORT==UseFnc)
    VoidEcucSubFunctionNameDef   DcmDspDataReturnControlToEcuFnc; 
    VoidEcucSubFunctionNameDef   DcmDspDataResetToDefaultFnc;     
    VoidEcucSubFunctionNameDef   DcmDspDataFreezeCurrentStateFnc; 
    VoidEcucSubFunctionNameDef   DcmDspDataShortTermAdjustmentFnc;
    #endif
    VoidEcucSubFunctionNameDef       OtherFnc;/* self-defined */
}Dcm_2F_ServiceInfoType;

/* For 0x31 */   
typedef struct
{
    uint8  DcmDspRoutineSecurityLevelRef;
    uint8  DcmDspRoutineSessionRef;
    uint8  AddressingMode;
}DcmDspRoutineAuthorization;
typedef struct
{
    uint32 DcmDspRoutineSignalLength;
    /* uint32 DcmDspRoutineSignalPos; */ /* Reserved */  
}DcmDspRoutineSignal;
typedef struct
{
    DcmDspRoutineAuthorization  RoutineAuthorization;
    DcmDspRoutineSignal         DcmDspRoutineRequestResOut;/*not used */
    DcmDspRoutineSignal         DcmDspRoutineStopIn;
    DcmDspRoutineSignal         DcmDspRoutineStopOut;  /*not used */
    DcmDspRoutineSignal         DcmDspStartRoutineIn;
    DcmDspRoutineSignal         DcmDspStartRoutineOut; /*not used */
}DcmDspRoutineInfo;
typedef struct
{
    uint16                      DcmDspRoutineIdentifier;
    boolean                     DcmDspRoutineUsed;     /*not used */
    boolean                     DcmDspRoutineUsePort;/*not used */
    boolean                     DcmDspRoutineFixedLength;
    VoidEcucSubFunctionNameDef  DcmDspStartRoutineFnc;
    VoidEcucSubFunctionNameDef  DcmDspStopRoutineFnc;
    VoidEcucSubFunctionNameDef  DcmDspRequestResultsRoutineFnc;
    DcmDspRoutineInfo           DcmDspRoutineInfoRef;
}DcmDspRoutine;

/* For 0x3D */
typedef struct
{
  uint32 DcmDspWriteMemoryRangeHigh;
  uint32 DcmDspWriteMemoryRangeLow;
  uint8  DcmDspWriteMemoryRangeSecurityLevelRef;
  uint8  DcmDspWriteMemoryRangeSessionRef;
  uint8  AddressingMode;
}DcmDspWriteMemoryRangeInfo;

/* For 0x86 */
typedef struct
{
  void(*DcmDspDidRoeActivateFnc)();
  uint32 DcmDspDidRoeEventId;
  boolean DcmDspRoeInitOnDSC;
  uint32  DcmDspRoeInterMessageTime;
  uint16  DcmDspRoeMaxEventLength;
  uint8   DcmDspRoeMaxNumberOfRetry;
  uint16  DcmDspRoeMaxQueueLength;
  boolean DcmDspRoeQueueEnabled;
}DcmDspRoe;
/**************************************************************************************************
* Pre-compile parameter
**************************************************************************************************/
extern CONST(uint8, DCM_VARIABLE) gSidMap_1[72];
extern P2CONST(CurrentSidMapType, DCM_VARIABLE, DCM_VARIABLE) gCurrentSidMap;
extern P2CONST(DcmDsdServiceTable, DCM_VARIABLE, DCM_VARIABLE) gCurrentDcmDsdServiceTable;
extern CONST(uint32, DCM_VARIABLE) gCurentDcmDsdServiceTableSize;
/* For 0x10,0x11,0x19,0x27,0x28,0x2C,0x31,0x3E,0x85 0x86 */
extern CONST(DcmDsdSubService, DCM_VARIABLE) gDcmDsdSubService[DCM_NUM_OF_SUB_SERVICE];

/**************************************************************************************************
* Link-compile parameter
**************************************************************************************************/
extern CONST(uint32, DCM_VARIABLE) gDcmDspNonDefaultSessionS3Server;//5000ms
extern CONST(DcmDspSessionRow, DCM_VARIABLE) gDcmDspSessionRow[KIND_OF_SESSION];

#if(DCM_SERVICE_27_ENABLED==STD_ON)
extern CONST(DcmDspSecurityRow, DCM_VARIABLE) gDcmDspSecurityRow[KIND_OF_SECURITY_LEVEL];
#endif 

#if(DCM_SERVICE_22_ENABLED==STD_ON)
extern CONST(Dcm_22_ServiceInfoType, DCM_VARIABLE) gDcmDsdSubService_22[NUMBER_OF_READ_DID];    /* For 0x22 */
extern CONST(Dcm_DspMaxDidToReadType, DCM_VARIABLE) DcmDspMaxDidToRead;
#endif

#if((DCM_SERVICE_23_ENABLED==STD_ON)|| (DCM_SERVICE_2C_ENABLED==STD_ON)||(DCM_SERVICE_3D_ENABLED==STD_ON))
extern CONST(Dcm_MemoryAddressFormatType, DCM_VARIABLE) gMemoryAddressFormat;
extern CONST(Dcm_MemorySizeFormatType, DCM_VARIABLE) gMemorySizeFormat;
#endif

#if(DCM_SERVICE_23_ENABLED==STD_ON)
extern CONST(DcmDspReadMemoryRangeInfo, DCM_VARIABLE) gDcmDspReadMemoryRangeInfo_23[NUMBER_OF_READ_ADDRESS_SEG];
#endif

#if(DCM_SERVICE_24_ENABLED==STD_ON)
extern CONST(Dcm_24_ServiceInfoType, DCM_VARIABLE) gDcmDsdSubService_24[NUMBER_OF_READ_DID_24]; /* For 0x24 */
#endif

#if(DCM_SERVICE_2A_ENABLED==STD_ON)
extern CONST(Dcm_2A_ServiceInfoType, DCM_VARIABLE) gDcmDsdSubService_2A[NUMBER_OF_PERIODIC_DID];/* For 0x2A */
#endif 

#if(DCM_SERVICE_2C_ENABLED==STD_ON)
extern CONST(Dcm_2C_ServiceInfoType, DCM_VARIABLE) gDcmDsdSubService_2C[NUMBER_OF_DYN_DID];     /* For 0x2C */
#endif

#if(DCM_SERVICE_2E_ENABLED==STD_ON)
extern CONST(Dcm_2E_ServiceInfoType, DCM_VARIABLE)  gDcmDsdSubService_2E[NUMBER_OF_WRITE_DID];   /* For 0x2E */
#endif

#if(DCM_SERVICE_2F_ENABLED==STD_ON)
extern CONST(Dcm_2F_ServiceInfoType, DCM_VARIABLE)  gDcmDsdSubService_2F[NUMBER_OF_CONTROL_DID]; /* For 0x2F */
#endif

#if(DCM_SERVICE_31_ENABLED==STD_ON)
extern CONST(DcmDspRoutine, DCM_VARIABLE)  DcmDspRoutineIdentifierTable_31[NUMBER_OF_RID];       /* For 0x31 */ 
#endif

#if(DCM_SERVICE_3D_ENABLED==STD_ON)
extern CONST(DcmDspWriteMemoryRangeInfo, DCM_VARIABLE)  gDcmDspWriteMemoryRangeInfo_3D[NUMBER_OF_WRITE_ADDRESS_SEG];
#endif

/**************************************************************************************************
* Application Callback Function Prototye
**************************************************************************************************/
#if(DCM_GENERAL_DIAGNOSTIC_REQUEST_CTL==STD_ON)
extern FUNC(Std_ReturnType,DCM_CODE) App_DiagnosticActive(void);
#endif
#if(DCM_COSTOMIZED_SERVCIE_SUPPORTED==STD_ON)
extern FUNC(void,DCM_CODE) App_CustomizedServcie(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
#endif

///////#define NUM_OF_DTC                17u
//#define NUM_OF_DTC  APP_KEYFUN_NUM


/* 10 */
extern FUNC(void,DCM_CODE) App_DefaultPre(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Default(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Programming(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Extended_Diagnostic(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Kuwe_Session(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_DefaultPost(Std_ReturnType Result);
extern FUNC(void,DCM_CODE) App_ProgrammingPost(Std_ReturnType Result);
extern FUNC(void,DCM_CODE) App_Extended_DiagnosticPost(Std_ReturnType Result);
extern FUNC(void,DCM_CODE) App_Kuwe_SessionPost(Std_ReturnType Result);
/* 11 */
extern FUNC(void,DCM_CODE) App_HardReset(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_HardResetPost(Std_ReturnType Result);
extern FUNC(void,DCM_CODE) App_SoftReset(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_SoftResetPost(Std_ReturnType Result);
/* 14 */
extern FUNC(void,DCM_CODE) App_ClearDiagnosticInformation(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
/* 19 */
extern FUNC(void,DCM_CODE) App_ReportNumberOfDTCByStatusMask(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_ReportDTCByStatusMask(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_ReportSupportedDTC(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_ReportDTCSnapshotRecordByDTCNumber(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_ReportDTCExtendedDataRecordByDTCNumber(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_ReportDTCSnapshotIdentification(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
/* 22 */
extern FUNC(void,DCM_CODE) App_Read0xF18C(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF187(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF18A(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF193(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF195(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF194(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0x1540(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0x1541(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF183(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF184(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF190(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0x0004(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0x0005(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF186(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF188(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF189(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF18B(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF192(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF197(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF199(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF19D(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Read0xF1A8(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext); 
extern FUNC(void,DCM_CODE) App_Read0x040E(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext); 
extern FUNC(void,DCM_CODE) App_Read0x040F(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext); 
extern FUNC(void,DCM_CODE) App_Read0x0410(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext); 

/* 23 */
#if(DCM_SERVICE_23_ENABLED==STD_ON)
extern FUNC(void,DCM_CODE) App_ReadAddress(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
#endif
/* 24 */
/* 27 */
extern VAR(uint8, DCM_VARIABLE)   gSecurityAcessSequence[KIND_OF_SECURITY_LEVEL];
extern FUNC(void,DCM_CODE) App_Request_Seed_L1(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Send_Key_L1(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
/* 28 */
extern FUNC(void,DCM_CODE) App_EnableRxAndTx(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_DisableRxAndTx(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_EnableRxAndDisableTx(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_DisableRxAndEnableTx(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
/* 2A */
/* 2E */
extern FUNC(void,DCM_CODE) App_Write0xF190(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Write0xF199(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Write0xF19D(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_Write0xF1A8(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
/* 2F */
#if(DCM_SERVICE_2F_ENABLED==STD_ON)
extern FUNC(void,DCM_CODE) App_ReturnControlToEcu0x3901(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ShortTermAdjustment0x3901(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ReturnControlToEcu0x3902(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ShortTermAdjustment0x3902(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ReturnControlToEcu0x3903(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ShortTermAdjustment0x3903(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ReturnControlToEcu0xFF00(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ShortTermAdjustment0x3904(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ReturnControlToEcu0x3905(Dcm_MsgContextType* pMsgContext);
extern FUNC(void,DCM_CODE) App_ShortTermAdjustment0x2020(Dcm_MsgContextType* pMsgContext);
extern uint8 u8ReadAdjustCmd(void);
#endif
/* 31 */
#if(DCM_SERVICE_31_ENABLED==STD_ON)
extern VAR(uint8, DCM_VARIABLE)  gRountineControlSequence[NUMBER_OF_RID];
extern VAR(uint8, DCM_VARIABLE)  gRountineControlDidHandle;
#endif
extern FUNC(void,DCM_CODE) App_StartRoutine0x0203(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);


extern FUNC(void,DCM_CODE) App_WriteAddress(Dcm_MsgContextType* pMsgContext); 
/* 3D */
#if(DCM_SERVICE_3D_ENABLED==STD_ON)
extern FUNC(void,DCM_CODE) App_WriteAddress(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
#endif
/* 85 */
extern VAR(uint8, DCM_VARIABLE) gDTCSwitch;
extern VAR(uint8, DCM_VARIABLE) DTCStatus[NUM_OF_DTC];
extern FUNC(void,DCM_CODE) App_DTCRecordOn(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_DTCRecordOff(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_DTCRecordOn(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);
extern FUNC(void,DCM_CODE) App_DTCRecordOff(P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext);

#endif
