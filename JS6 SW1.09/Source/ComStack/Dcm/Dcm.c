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
*   Title           : Dcm module Source File 
*   Author          : Hirain
********************************************************************************
*   Description     : Main source file of the AUTOSAR Diagnositc communcation 
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

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "Dcm.h"
#include "Demm.h"
/*******************************************************************************
* Local Macro and Tyedef
*******************************************************************************/
/* General Define */
#define DCM_HAVE_ERRORS  (0x01u) //存在错误
#define DCM_HAVE_NO_ERRORS  (0x00u)//不存在错误
/*sid偏移量*/
#define DCM_SID_OFF_SET  (0x40u)//表示服务 ID 的偏移量，通常用于将请求的 SID 转换为响应的 SID。
/*除法和时间相关*/
#define DCM_DIVIDE_TOW  (0x01u) //表示除以 2                
#define DCM_TIME_EXPIRED  (0x00u)//表示时间已过期
/*sid位置*/
#define DCM_SID_POSITION  (0x00u)//表示 SID 在数据帧中的位置
/*放大倍数*/
#define DCM_AMPLIFY_8_TIMES  (0x08u)//表示放大 8 倍
/*偏移数据*/
#define THE_OFFSET_DATA  ((uint8)0x40)  //表示通用定义任务的偏移量
/*任务时间*/
#define DCM_COMMON_DEFINED_TASK_TIME  (0x10u)  //定义的任务时间
/*数据长度要求*/
#define DCM_REQUIRE_DATA_LENGTH_1_BYTE  (0x01u)
#define DCM_REQUIRE_DATA_LENGTH_2_BYTE  (0x02u)
#define DCM_REQUIRE_DATA_LENGTH_3_BYTE  (0x03u)
#define DCM_REQUIRE_DATA_LENGTH_4_BYTE  (0x04u)
#define DCM_REQUIRE_DATA_LENGTH_5_BYTE  (0x05u)
#define DCM_REQUIRE_DATA_LENGTH_6_BYTE  (0x06u)
#define DCM_REQUIRE_DATA_LENGTH_7_BYTE  (0x07u)
/*响应数据长度*/
#define DCM_RESPONSE_DATA_LENGTH_1_BYTE  (0x01u)
#define DCM_RESPONSE_DATA_LENGTH_2_BYTE  (0x02u)
#define DCM_RESPONSE_DATA_LENGTH_3_BYTE  (0x03u)
#define DCM_RESPONSE_DATA_LENGTH_4_BYTE  (0x04u)
#define DCM_RESPONSE_DATA_LENGTH_5_BYTE  (0x05u)
/*通道缓冲区数量*/
#define DCM_NUMBER_OF_CHANNEL_BUFFER_OBD_INCLUDE  (0x04u)//包含OBD通道
#define DCM_NUMBER_OF_CHANNEL_BUFFER_WITHOUT_OBD  (0x02u)//不包含OBD通道
/*服务和子服务长度*/
#define DCM_SERVICE_AND_SUB_SERVICE_REQUIRE_LENGTH  (0x02u)//表示服务和子服务的请求长度
#define DCM_SERVICE_AND_SUB_SERVICE_RESPONSE_LENGTH  (0x02u)//示服务和子服务的响应长度
#define SETSUPPRESSPOSRESPONSEBITMASK  ((uint8)0x80)//表示设置抑制正响应的位掩码
#define SUPPRESS_POS_RSP_BYTE  ((uint8)0x80)//表示抑制正响应的字节
#define THE_LARGEST_SERVICE_ID  ((uint8)0x85)//表示最大的服务 ID
#define RIGHT_ROLLING_SUPPRESS_POS_RSP_BIT  (0x07u)//表示右移抑制正响应位
#define GET_THE_SECURITY_LEVEL  (0xFEu)///表示获取安全级别的值
/*会话类型、安全级别和请求地址不正确*/
#define DCM_SESSION_TYPE_NOT_CORRECT  (0x00u)//会话类型不正确
#define DCM_SECURITY_LEVEL_NOT_CORRECT  (0x00u)//表示安全级别不正确
#define DCM_REQUEST_ADDRESS_NOT_CORRECT  (0x00u)//表示请求地址不正确
/* for negative response 0x78 */
#define FORCERCR_RP  (0x01u)
#define UNFORCERCR_RP  (0x00u)
/* for 0x10 */
#define DCM_SERVICE_10_SESSION_TYPE_0  ((uint8)0x00)
#define DCM_SERVICE_10_SESSION_TYPE_1  ((uint8)0x01)
#define DCM_SERVICE_10_SESSION_TYPE_2  ((uint8)0x02)
#define DCM_SERVICE_10_SESSION_TYPE_3  ((uint8)0x03)
#define DCM_SERVICE_10_SESSION_TYPE_4  ((uint8)0x04)
#define DCM_SERVICE_10_SESSION_TYPE_5  ((uint8)0x05)
#define DCM_SERVICE_10_SESSION_TYPE_6  ((uint8)0x06)
#define DCM_SERVICE_10_SESSION_TYPE_7  ((uint8)0x07)
#define DCM_SERVICE_10_POSITIVE_RESPONSE_DATA  (0x50u)
/* for 0x14 */
#define DCM_SERVICE_14_REQ_LENGTH  (0x4u)
/* for 0x22 */
#define DCM_SERVICE_22_NO_DID_TO_STORE  (0x00u)
#define DCM_SERVICE_22_DID_DATA_LENGTH  (0x02u)
#define DCM_SERVICE_22_RIGHT_ROLLING_8_BIT (0x08u)
#define DCM_SERVICE_22_INVALID_DID_POSITION  (0xFFu)
#define DCM_SERVICE_22_NO_DIDPOSITION_TO_STORE  (0x00u)
/* for 0x23 */
#define DCM_SERVICE_23_GET_MEMORY_AND_ADDRESS_BYTE_LOW_NIBBLE  ((uint8)0x0F)
#define DCM_SERVICE_23_GET_MEMORY_AND_ADDRESS_BYTE_HIGH_NIBBLE  ((uint8)0x04)
/* for 0x24 */
#define DCM_SERVICE_24_REQ_LENGTH  (0x3u)
/* for 0x27 */
#define DCM_SERVICE_27_SECURITY_MAP_LOCK  (0x00u)
#define DCM_SERVICE_27_SECURITY_MAP_L1  (0x01u)
#define DCM_SERVICE_27_SECURITY_MAP_L2  (0x02u)
#define DCM_SERVICE_27_SECURITY_MAP_L3  (0x03u)
#define DCM_SERVICE_27_SECURITY_MAP_L4  (0x04u)
#define DCM_SERVICE_27_SECURITY_MAP_L5  (0x05u)
#define DCM_SERVICE_27_SECURITY_MAP_L6  (0x06u)
#define DCM_SERVICE_27_SECURITY_MAP_L7  (0x07u)
#define DCM_SERVICE_27_SECURITY_MAP_L8  (0x08u)
#define DCM_SERVICE_27_SEND_KEEY  (0x00u)
/* for 0x28 */
#define DCM_SERVICE_28_REQ_LENGTH  (0x3u)
/* for 0x2A */
#define DCM_SERVICE_2A_NO_PERIODIC_DID_TO_STORE  (0x00u)
#define DCM_SERVICE_2A_HIGH_SUB_SERVICE_NUMBER  (0x4u)
#define DCM_SERVICE_2A_LOW_SUB_SERVICE_NUMBER  (0x1u)
#define DCM_SERVICE_2A_SEND_AT_SLOW_RATE  (0x1u)
#define DCM_SERVICE_2A_SEND_AT_MEDIUM_RATE  (0x2u)
#define DCM_SERVICE_2A_SEND_AT_FAST_RATE  (0x3u)
#define DCM_SERVICE_2A_STOP_SENDING  (0x4u)
#define DCM_SERVICE_2A_POSITIVE_RESPONSE  (0x6Au)
#define DCM_SERVICE_2A_NO_PERIODIC_DIDPOSITION_TO_STORE  (0x00u)
/* for 0x2C */
#define DCM_SERVICE_2C_JUDGE_THE_LENGTH_IS_MULTIPLE_OF_4  (0x04u)
#define DCM_SERVICE_2C_LEAST_REQ_LENGTH  (0x4u)
#define DCM_SERVICE_2C_DEFINE_BY_MEMORY_ADDRESS  (0x2u)
#define DCM_SERVICE_2C_SUB_SERVICE_03_REQ_LENGTH  (0x4u)
/* for 0x2F */
#define DCM_SERVICE_2F_RETURN_CONTROL_TO_ECU_REQ_LENGTH  (0x4u)
#define DCM_SERVICE_2F_RESET_TO_DEFAULT_REQ_LENGTH  (0x4u)
#define DCM_SERVICE_2F_FREEZE_CURRENT_STATE_REQ_LENGTH  (0x4u)  
/* for 0x31 */
#define HAVE_GOTTON_SEED  (0x01u)
#define HAVE_STARTED_ROUNTINE  (0x01u)
#define HAVE_STOPPED_ROUNTINE  (0x02u)
#define HAVE_REQ_ROUNTINE_RESULT  (0x04u)
#define INVALID_SECURITY_LEVEL  (0xFFu)
#define INVALID_SESSION_TYPE  (0xFFu)
#define INVALID_SUB_SERVICE_POSITION  (0xFFu)
/* for 0x3D */
#define DCM_SERVICE_3D_GET_MEMORY_AND_ADDRESS_BYTE_LOW_NIBBLE  ((uint8)0x0F)
#define DCM_SERVICE_3D_GET_MEMORY_AND_ADDRESS_BYTE_HIGH_NIBBLE  ((uint8)0x04)
/* for 0x3E */
#define DCM_SERVICE_3E_RESPONSE_DATA_LENGTH  (0x02u)
#define DCM_SERVICE_3E_REQUIRE_DATA_LENGTH  (0x02u) 
/* for 0x10 0x11 0x85 */
#define DCM_SERVICE_10_11_85_REQUIRE_DATA_LENGTH  (0x02u) 

/*******************************************************************************
* Global variables(Scope:global)
*******************************************************************************/
#define DCM_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

VAR(Dcm_MsgItemType, DCM_NOINIT_DATA) 
                                gUDS_Physical_DiagBuffer[UDS_PHYS_BUFFER_SIZE];
              
VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)   
                              gUDS_Functional_DiagBuffer[UDS_FUNC_BUFFER_SIZE];

#if(ISO_15031_5_MultiChannel == STD_ON)
VAR(DcmDslBufferSize, DCM_NOINIT_DATA)  
          gDcmDslAvailableBufferSize[DCM_NUMBER_OF_CHANNEL_BUFFER_OBD_INCLUDE];
#else
VAR(DcmDslBufferSize, DCM_NOINIT_DATA)  
          gDcmDslAvailableBufferSize[DCM_NUMBER_OF_CHANNEL_BUFFER_WITHOUT_OBD];
#endif
 
#if(ISO_15031_5 == STD_ON)
#if(ISO_15031_5_MultiChannel == STD_ON)
VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)   
                                gOBD_Physical_DiagBuffer[OBD_PHYS_BUFFER_SIZE];
VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)   
                              gOBD_Functional_DiagBuffer[OBD_FUNC_BUFFER_SIZE];
#endif
#endif

VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                                 gNegativeResponseBuffer[NEG_RESP_BUFFER_SIZE];
VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)  
                          gNegativeResponseBufferForRRCP[NEG_RESP_BUFFER_SIZE];

#if(DCM_SERVICE_2A_ENABLED == STD_ON)
VAR(Dcm_MsgItemType, DCM_NOINIT_DATA)   
                             gPeriodicResponseBuffer[PERODIC_RESP_BUFFER_SIZE];
#endif

#if(DCM_SERVICE_86_ENABLED == STD_ON)
VAR(Dcm_MsgItemType, DCM_NOINIT_DATA) 
                                gResponseOnEventBuffer[EVENT_RESP_BUFFER_SIZE];
#endif

VAR(uint16, DCM_NOINIT_DATA)  gResponseLength;   
VAR(uint8, DCM_NOINIT_DATA)  gCurrentSessionType;

#if(DCM_SERVICE_27_ENABLED == STD_ON)
VAR(Dcm_SecLevelType, DCM_NOINIT_DATA)  gSecLevelType;
VAR(Dcm_SecLevelType, DCM_NOINIT_DATA)   
                                 gSecLevelForCurrentSecurityAccessRequestLevel;
#endif

VAR(Dcm_SesCtrlType, DCM_NOINIT_DATA)  gSesCtrlType;
VAR(Dcm_ProtocolType, DCM_NOINIT_DATA)  gActiveProtocol;
VAR(Dcm_MsgContextType, DCM_NOINIT_DATA) gMsgContextType;
VAR(Dcm_NegativeResponseCodeType, DCM_NOINIT_DATA)  gNegativeResponseCode;
VAR(uint8, DCM_NOINIT_DATA)  gDiagState;
VAR(uint32, DCM_NOINIT_DATA)  gP2ServerTimer;
VAR(uint32, DCM_NOINIT_DATA)  gS3ServerTimer;
VAR(uint8, DCM_NOINIT_DATA)  gP2ServerTimerStartFlag;
VAR(uint8, DCM_NOINIT_DATA)  gS3ServerTimerStartFlag;

#if(DCM_SERVICE_27_ENABLED == STD_ON)
/* Default value,used for Delay time on boot for 0x27 */
VAR(uint8, DCM_NOINIT_DATA)  gCurrentSecurityAccessRequestLevel;
VAR(uint8, DCM_NOINIT_DATA)  gSecurityDelayTimeOnBootFlag;
VAR(uint32, DCM_NOINIT_DATA) 
                         gSecurityAcessDelayTimeOnBoot[KIND_OF_SECURITY_LEVEL];
VAR(uint8, DCM_NOINIT_DATA)  
                            gSecurityAcessAttempNumber[KIND_OF_SECURITY_LEVEL];
VAR(uint8, DCM_NOINIT_DATA)  
                    gSecurityAcessAttempNumberExceeded[KIND_OF_SECURITY_LEVEL];
VAR(uint32, DCM_NOINIT_DATA)  gSecurityAcessDelayTime[KIND_OF_SECURITY_LEVEL]; 
VAR(uint8, DCM_NOINIT_DATA)  gSecurityAcessSequence[KIND_OF_SECURITY_LEVEL];
#endif

#if(DCM_SERVICE_2A_ENABLED == STD_ON)
VAR(uint32, DCM_NOINIT_DATA)  gPeriodicResponseRate[NUMBER_OF_PERIODIC_DID];
VAR(uint32, DCM_NOINIT_DATA)  gPeriodicResponseTimer[NUMBER_OF_PERIODIC_DID]; 
VAR(uint8, DCM_NOINIT_DATA)  PeriodicDidCounter;
#endif

#if(DCM_SERVICE_2C_ENABLED == STD_ON)
VAR(DynamicalDidRelatedInfo, DCM_NOINIT_DATA)   
                                   gDynamicalDidRelatedInfo[NUMBER_OF_DYN_DID];
VAR(uint8, DCM_NOINIT_DATA)  gDynamicalDidCounter;
#endif

#if(DCM_SERVICE_31_ENABLED == STD_ON)
VAR(uint8, DCM_NOINIT_DATA)  gRountineControlSequence[NUMBER_OF_RID];
VAR(uint8, DCM_NOINIT_DATA)  gRountineControlDidHandle;
#endif

VAR(EcucFunctionNameDef, DCM_NOINIT_DATA)  gFunctionHandler;
VAR(VoidEcucSubFunctionNameDef, DCM_NOINIT_DATA)  gSubFunctionHandler;
 
/*******************************************************************************
* Global variables(Scope:local)
*******************************************************************************/
STATIC VAR(DCM_InitType, DCM_CONFIG_DATA) DCM_InitStatus = DCM_UNINITIALIZED;
STATIC VAR(DcmDsdServiceTable, DCM_PRIVATE_CODE) service;
STATIC VAR(EcucPostSubFunctionNameDef, DCM_PRIVATE_CODE)  
                                                       gSubFunctionPostHandler;
STATIC VAR(uint32, DCM_PRIVATE_CODE)  gMaxNumberOfRCRRP;
STATIC VAR(uint8, DCM_PRIVATE_CODE)  gSubServicePosition;

#if(DCM_SERVICE_22_ENABLED == STD_ON)
STATIC VAR(uint8, DCM_PRIVATE_CODE)  gDidPosition;
STATIC VAR(DidlistType, DCM_PRIVATE_CODE)  DidList[MAX_NUM_OF_DID_TO_READ];
#if(DCM_SERVICE_22_COMBINED_DID == STD_ON)
STATIC VAR(Dcm_MsgType, DCM_PRIVATE_CODE)  gDidResData;
#endif
STATIC VAR(Dcm_MsgLenType, DCM_PRIVATE_CODE)  gDidResDataLen;
#endif 

#if(DCM_SERVICE_24_ENABLED == STD_ON)
STATIC VAR(uint8, DCM_PRIVATE_CODE)  gNumberOfReadDID_24;
#endif

#if(DCM_SERVICE_2A_ENABLED == STD_ON)
STATIC VAR(PeriodicDidlistType, DCM_PRIVATE_CODE)  
                                       PeriodicDidList[NUMBER_OF_PERIODIC_DID];
STATIC VAR(PeriodicDidlistType, DCM_PRIVATE_CODE)  CurrentPeriodicDid;
STATIC VAR(uint16, DCM_PRIVATE_CODE)  CurrentPeriodicDidIndx;
STATIC VAR(uint16, DCM_PRIVATE_CODE) 
                                 gPeriodicResponseFlag[NUMBER_OF_PERIODIC_DID];
#endif

#if(DCM_SERVICE_22_ENABLED == STD_ON) 
STATIC VAR(uint16, DCM_PRIVATE_CODE)  DidCounter;
#endif 

#if(DCM_SERVICE_2E_ENABLED == STD_ON) 
STATIC VAR(uint8, DCM_PRIVATE_CODE)  gNumberOfWriteDID;
#endif

#if(DCM_SERVICE_2F_ENABLED == STD_ON)
STATIC VAR(uint8, DCM_PRIVATE_CODE)  gNumberOfIOCtrlDID;
#endif

STATIC VAR(uint8, DCM_PRIVATE_CODE)  gDcm_RepeatTransmitFlag;
STATIC VAR(uint8, DCM_PRIVATE_CODE)  gDcm_CurrentServiceSubfuncAvail;

#define DCM_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/*******************************************************************************
* Local Function Prototype
*******************************************************************************/
/* Self-defined */
#define DCM_START_SEC_PRIVATE_CODE
#include "MemMap.h"

STATIC FUNC(uint8,DCM_PRIVATE_CODE)  FindUdsSid(uint8 RequestSid);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_UdsSidDispatcher(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_RepeatTransmitCheck(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_Periodic_Transmit(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_DidDispatcher(void);

#if(DCM_DSL_DIAG_RESP_FORCE_RESP_PEND_EN == STD_ON)
extern FUNC(void,DCM_PRIVATE_CODE)  
                             DslInternal_RCRResponsePending(uint8 ForceRCR_RP);
#else
STATIC FUNC(void,DCM_PRIVATE_CODE)  
                             DslInternal_RCRResponsePending(uint8 ForceRCR_RP);
#endif

STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_10_11_85(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_AccessAttemptNumberDec(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_UdsRepeatServiceCall(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_UdsPostServiceProcessing(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DslInternal_Timer_CallbackTask(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_Uds_Common(void);
STATIC FUNC(void,DCM_PUBLIC_CODE)  Dcm_ResetToDefaultSession(void);
STATIC FUNC(void,DCM_PRIVATE_CODE)  
                               DsdInternal_SessionTransferPostProcessing(void);

#define DCM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"
 
/*******************************************************************************
* Global Function Implementation
*******************************************************************************/

#define DCM_START_SEC_PUBLIC_CODE
#include "MemMap.h"
volatile uint8_t Flag_diag_session=0x01;//初始值为默认会话模式
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dcm_Init
*                
* Description:   Initial Dcm module's global variable 
*                             
* Inputs:        ConfigPtr: Pointer pointing to the configure parameter of Dcm
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE) Dcm_Init
(
    P2VAR(Dcm_ConfigType,AUTOMATIC,DCM_CONFIG_DATA) ConfigPtr
)
{
    uint16 i = 0;
    (void) ConfigPtr;
    
    #if(ISO_15031_5 == STD_ON)
    {
        Init_SupportedPID();
    } 
    #endif

    Dsd_InteralInit();
    Set_SesCtrl(DCM_DEFAULT_SESSION);
    Flag_diag_session=0x01;
    gCurrentSessionType = DCM_SESSION_DEFAULT;
    Clr_S3_Server_Timer();
    gSecurityDelayTimeOnBootFlag = DCM_FLAG_ACTIVE;
    gDcm_CurrentServiceSubfuncAvail = UDS_SERVICE_WITHOUT_SUB_FUNCTION;
    
    #if(ISO_15031_5_MultiChannel == STD_ON)
    gDcmDslAvailableBufferSize[0] = UDS_PHYS_BUFFER_SIZE;
    gDcmDslAvailableBufferSize[1] = UDS_FUNC_BUFFER_SIZE;
    gDcmDslAvailableBufferSize[2] = OBD_PHYS_BUFFER_SIZE;
    gDcmDslAvailableBufferSize[3] = OBD_FUNC_BUFFER_SIZE;
    #else
    gDcmDslAvailableBufferSize[0] = UDS_PHYS_BUFFER_SIZE;
    gDcmDslAvailableBufferSize[1] = UDS_FUNC_BUFFER_SIZE;
    #endif
    
    for(i = 0; i < UDS_PHYS_BUFFER_SIZE; i++)
    {
        gUDS_Physical_DiagBuffer[i] = 0;
    }

    for(i = 0; i < UDS_FUNC_BUFFER_SIZE; i++)
    {
        gUDS_Functional_DiagBuffer[i] = 0;
    }

    service = gCurrentDcmDsdServiceTable[0];
    
    #if(DCM_SERVICE_22_ENABLED == STD_ON)
    for(i = 0; i < MAX_NUM_OF_DID_TO_READ; i++)
    {
       DidList[i].Did = DCM_SERVICE_22_NO_DID_TO_STORE;
       DidList[i].DidPosition = DCM_SERVICE_22_NO_DIDPOSITION_TO_STORE;  
    }
    gDidPosition = 0; 
    DidCounter = 0;

    #if(DCM_SERVICE_22_COMBINED_DID == STD_ON)
    #endif
    gDidResDataLen = 0;
    #endif  

    #if(DCM_SERVICE_24_ENABLED == STD_ON)
    gNumberOfReadDID_24 = NUMBER_OF_READ_DID_24;
    #endif
    
    #if(DCM_SERVICE_27_ENABLED == STD_ON)

    for(i = 0;i < KIND_OF_SECURITY_LEVEL;i++)
    {
        gSecurityAcessDelayTimeOnBoot[i] 
           = gDcmDspSecurityRow[i].DcmDspSecurityDelayTimeOnBoot/DCM_TASK_TIME;
        gSecurityAcessAttempNumber[i] = gDcmDspSecurityRow[i].DcmDspSecurityNumAttDelay;
        gSecurityAcessDelayTime[i] = gDcmDspSecurityRow[i].DcmDspSecurityDelayTime/DCM_TASK_TIME;
    }
    gSecLevelType = DCM_SEC_LEV_LOCK;
    gCurrentSecurityAccessRequestLevel = DCM_SEC_LEV_LOCK;
    gSecLevelForCurrentSecurityAccessRequestLevel = 0;

    for(i = 0; i < KIND_OF_SECURITY_LEVEL; i++)
    {
        gSecurityAcessSequence[i] = 0;
    }
    #endif
    
    #if(DCM_SERVICE_2A_ENABLED == STD_ON)
    for(i = 0; i < PERODIC_RESP_BUFFER_SIZE;i++)
    {
        gPeriodicResponseBuffer[i] = 0;
    }
    
    for(i = 0; i < NUMBER_OF_PERIODIC_DID; i++)
    {
        gPeriodicResponseRate[i] = 0;
        gPeriodicResponseTimer[i] = 0;
        gPeriodicResponseFlag[i] = DCM_FLAG_DISACTIVE;
    }
    
    PeriodicDidCounter = 0;
    #endif

    #if(DCM_SERVICE_2C_ENABLED == STD_ON)
    for(i = 0; i < NUMBER_OF_DYN_DID; i++)
    {
        gDynamicalDidRelatedInfo[i].DynamicalDid = 0xFF;
        gDynamicalDidRelatedInfo[i].DynamicalDidPosion = 0;
        gDynamicalDidRelatedInfo[i].SourceDidCounter = 0;
        gDynamicalDidRelatedInfo[i].AddessCounter = 0;
        gDynamicalDidRelatedInfo[i].TotalDataSize = 0;
        gDynamicalDidRelatedInfo[i].DefinedTime = 0;
    }
    gDynamicalDidCounter = 0u;
    #endif
    
    #if(DCM_SERVICE_2E_ENABLED == STD_ON) 
    gNumberOfWriteDID = NUMBER_OF_WRITE_DID;
    #endif

    #if(DCM_SERVICE_2F_ENABLED == STD_ON)
    gNumberOfIOCtrlDID = NUMBER_OF_CONTROL_DID;
    #endif

    #if(DCM_SERVICE_31_ENABLED == STD_ON)
    #if(NUMBER_OF_RID > 0) 
    gRountineControlDidHandle = DCM_INVALID_HANDLE_OR_ID; 
    for(i = 0; i < NUMBER_OF_RID; i++)
    {
        gRountineControlSequence[i] = 0;
    }
    #endif
    #endif 
    
    #if(DCM_SERVICE_86_ENABLED == STD_ON)
    for(i = 0;i < EVENT_RESP_BUFFER_SIZE;i++)
    {
        gResponseOnEventBuffer[i] = 0;
    }
    #endif

    gResponseLength = 0;   
    gNegativeResponseCode = DCM_E_POSITIVERESPONSE;
    gS3ServerTimerStartFlag = DCM_FLAG_DISACTIVE;
    gSubFunctionPostHandler = DCM_NULL;
    gMaxNumberOfRCRRP = DCM_NUM_MAX_RESPPEND;
    gSubServicePosition = 0;
    gDcm_RepeatTransmitFlag = DCM_FLAG_DISACTIVE;
  
    DCM_InitStatus = DCM_INITIALIZED;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dcm_GetVersionInfo
*                
* Description:   Get Dcm's version 
*                             
* Inputs:        versionInfo: Pointer pointing to the version info of Dcm
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE) Dcm_GetVersionInfo
(
    P2VAR(Std_VersionInfoType,AUTOMATIC,DCM_APPL_DATA) versionInfo
)
{
    versionInfo->vendorID = DCM_VENDOR_ID;
    versionInfo->moduleID = DCM_MODULE_ID;
    versionInfo->instanceID = 0;
    versionInfo->sw_major_version = DCM_SW_MAJOR_VERSION;
    versionInfo->sw_minor_version = DCM_SW_MINOR_VERSION;
    versionInfo->sw_patch_version = DCM_SW_PATCH_VERSION;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dcm_GetSecurityLevel
*                
* Description:   Get Dcm's Security Level
*                             
* Inputs:        SecLevel: Active Security Level value
*                
* Outputs:       result: Always be OK
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_27_ENABLED == STD_ON)
FUNC(Std_ReturnType,DCM_PUBLIC_CODE) Dcm_GetSecurityLevel
(
    P2VAR(Dcm_SecLevelType,AUTOMATIC,DCM_APPL_DATA) SecLevel
)
{   
    Std_ReturnType result = E_OK;

    *SecLevel = gCurrentSecurityAccessRequestLevel;
    return result;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dcm_GetSecurityLevelMap
*                
* Description:   Map Request_seed_Lx and Send_key_lx to the same result
*                             
* Inputs:        ReqSecLevel: Request security sub Function data
*                
* Outputs:       result: The according Security Level
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(Std_ReturnType,DCM_PUBLIC_CODE) Dcm_GetSecurityLevelMap
(
    P2VAR(Dcm_SecLevelType,AUTOMATIC,DCM_APPL_DATA) ReqSecLevel
)
{
    uint8 SecLevel = *ReqSecLevel;
    uint8 SecLevel_Int = 0;   
    Std_ReturnType result;
    
    switch(SecLevel)
    {                                 
        case  REQUEST_SEED_L1:                                 
        case  SEND_KEY_L1:

              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_L1;
        break;                      
                                          
        case  REQUEST_SEED_L11:                                
        case  SEND_KEY_L12:
                  
              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_L2;
        break; 
                                                
        case  REQUEST_SEED_L3:                                  
        case  SEND_KEY_L3:
                  
              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_L3;
        break; 
                                              
        case  REQUEST_SEED_L4:                               
        case  SEND_KEY_L4:

              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_L4;
        break;          
                                               
        case  REQUEST_SEED_L5:                                
        case  SEND_KEY_L5:

              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_L5;
        break;           
                                
        case  REQUEST_SEED_L6:                               
        case  SEND_KEY_L6:

              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_L6;
        break;          
                                                
        case  REQUEST_SEED_L7:                                 
        case  SEND_KEY_L7:

              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_L7;
        break;         
                                                
        case  REQUEST_SEED_L8:                                   
        case  SEND_KEY_L8:

              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_L8;
        break;  
            
        default: 
              SecLevel_Int = DCM_SERVICE_27_SECURITY_MAP_LOCK; 
        break;

    }

    gSecLevelForCurrentSecurityAccessRequestLevel = SecLevel_Int;
    result = ((0x01 << SecLevel_Int) & GET_THE_SECURITY_LEVEL);   
    return result;
}

#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dcm_GetSesCtrlType
*                
* Description:   Get current session type
*                             
* Inputs:        SesCtrlType: Active session type
*                
* Outputs:       result: Always be OK
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(Std_ReturnType,DCM_PUBLIC_CODE) Dcm_GetSesCtrlType            
(
    P2VAR(Dcm_SesCtrlType,AUTOMATIC,DCM_APPL_DATA) SesCtrlType
)
{
    *SesCtrlType = gCurrentSessionType;
    return E_OK;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dcm_GetActiveProtocol
*                
* Description:   Get current active protocol
*                             
* Inputs:        SesCtrlType: Active portocol type
*                
* Outputs:       result: The active protocol
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(Std_ReturnType,DCM_PUBLIC_CODE) Dcm_GetActiveProtocol
(
    P2VAR(Dcm_ProtocolType,AUTOMATIC,DCM_APPL_DATA) ActiveProtocol
)
{
    return *ActiveProtocol;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dcm_ResetToDefaultSession
*                
* Description:   Turn current session mode into default mode
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PUBLIC_CODE) Dcm_ResetToDefaultSession 
(
    void
)
{
    #if(DCM_SERVICE_2A_ENABLED==STD_ON)
    uint16 i;
    {
        /* Clear all periodicDid info in PeriodicDidList */ 
        for(i=0;i<NUMBER_OF_PERIODIC_DID;i++)
        {
            PeriodicDidList[i].Did=0;
            PeriodicDidList[i].DidPosition=0;
            gPeriodicResponseFlag[i]=0;
            gPeriodicResponseRate[i]=0;
            gPeriodicResponseTimer[i]=0;
        }
        PeriodicDidCounter = 0;
    }
    #endif
    Set_SesCtrl(DCM_SESSION_DEFAULT);
     Flag_diag_session=0x01;
    gCurrentSessionType = DCM_SESSION_DEFAULT;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dcm_MainFunction
*                
* Description:   The main processing Function
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE) Dcm_MainFunction
(
    void
)
{   
    #if 0
    if(DCM_UNINITIALIZED = DCM_InitStatus) 
    {
       Det_ReportError(DCM_MODULE_ID_VALUE,
                       DCM_INSTANCE_ID_VALUE,
                       DCM_INIT_APIID,
                       DCM_E_TRANSITION);      
    }
    #endif

    DsdInternal_UdsPostServiceProcessing();
    DsdInternal_UdsRepeatServiceCall();    //state=processing
    DslInternal_Timer_CallbackTask();    //state=done
    
    #if(ISO_15031_5 == STD_ON)
    DsdInternal_ObdSidDispatcher();
    #endif
    
    DsdInternal_UdsSidDispatcher();
    DsdInternal_RepeatTransmitCheck();
     
    #if(DCM_SERVICE_2A_ENABLED == STD_ON)
    DsdInternal_Periodic_Transmit();  
    #endif
}

#define DCM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"


#define DCM_START_SEC_PRIVATE_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DslInternal_Timer_CallbackTask
*                
* Description:   The timer processing function
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE) DslInternal_Timer_CallbackTask(void) 
{
    uint8 SessionId = 0;
    
    #if((DCM_SERVICE_2A_ENABLED == STD_ON) || (DCM_SERVICE_27_ENABLED == STD_ON))
    uint8 i;
    #endif
    

    /* for 0x27, access delay time */
    #if (DCM_SERVICE_27_ENABLED == STD_ON)                           
    if (gSecurityAcessAttempNumber[gCurrentSecurityAccessRequestLevel - 1] == 0) 
    {
        if (gSecurityAcessDelayTime[gCurrentSecurityAccessRequestLevel - 1] != DCM_TIME_EXPIRED) 
        {
            gSecurityAcessDelayTime[gCurrentSecurityAccessRequestLevel - 1]--;
        } 
        else 
        {
            gSecurityAcessAttempNumber[gCurrentSecurityAccessRequestLevel - 1] = gDcmDspSecurityRow[gCurrentSecurityAccessRequestLevel - 1].DcmDspSecurityNumAttDelay;
            gSecurityAcessDelayTime[gCurrentSecurityAccessRequestLevel - 1] = gDcmDspSecurityRow[gCurrentSecurityAccessRequestLevel - 1].DcmDspSecurityDelayTime/DCM_TASK_TIME;
            for (i = 0; i < KIND_OF_SECURITY_LEVEL; i++) 
            {
                gSecurityAcessSequence[i] = 0;
            }
        }
    } 
    else 
    {
    }

    #endif
    /* for 0x27, delay time on boot */
    if (gSecurityDelayTimeOnBootFlag == DCM_FLAG_ACTIVE) 
    {
        if (gSecurityAcessDelayTimeOnBoot[0] != DCM_TIME_EXPIRED) 
        {
            gSecurityAcessDelayTimeOnBoot[0]--;
        } 
        else 
        {
            gSecurityDelayTimeOnBootFlag = DCM_FLAG_DISACTIVE;
            gSecurityAcessDelayTimeOnBoot[0] = gDcmDspSecurityRow[0].DcmDspSecurityDelayTimeOnBoot/DCM_TASK_TIME;
        }
    } 
    else
    {
    }
    /* S3 Timer */
    if (gS3ServerTimerStartFlag == DCM_FLAG_ACTIVE) 
    {
        if (gS3ServerTimer != DCM_TIME_EXPIRED) 
        {
            Dec_S3_Server_Timer();
            if (gS3ServerTimer == DCM_TIME_EXPIRED) 
            {
                dem_enable_dtc_setting();
                Dcm_ResetToDefaultSession();
                gS3ServerTimerStartFlag = DCM_FLAG_DISACTIVE;
                Dcm_DefaultSessionPost(E_OK);
                DsdInternal_SessionTransferPostProcessing();
                
                #if(DCM_SERVICE_27_ENABLED == STD_ON)
                for (i = 0; i < KIND_OF_SECURITY_LEVEL; i++) 
                {
                    gSecurityAcessSequence[i] = 0;
                }
                gSecLevelType = DCM_SEC_LEV_LOCK;
                #endif
                
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
     /* P2 and P2* timer  */
    if (gP2ServerTimerStartFlag == DCM_FLAG_ACTIVE)
    {
        if (gMaxNumberOfRCRRP != 0)
        {
            Dec_P2_Server_Timer();
            /* If P2 timeout,send first 0x78 negative response and change P2 to P2Ext */
            DslInternal_RCRResponsePending(UNFORCERCR_RP);
            /* If P2Ext timeout,send following 0x78 negative response */
            SessionId = Dsd_GetSessionMapId(gSesCtrlType);
            if ((gP2ServerTimer > (uint32) (gDcmDspSessionRow[SessionId].DcmDspSessionP2ServerMax/ DCM_TASK_TIME)) 
            &&  (gP2ServerTimer < (((uint32) (gDcmDspSessionRow[SessionId].DcmDspSessionP2StarServerMax/DCM_TASK_TIME)) >> DCM_DIVIDE_TOW)))
            {
                DslInternal_RCRResponsePending(FORCERCR_RP);
            }
            else
            {
            }
        } 
        else
        {
            DsdInternal_SetNegResponse(&gMsgContextType, DCM_E_GENERALREJECT);
            DsdInternal_ProcessingDone(&gMsgContextType);
            Set_DiagState(DIAG_UDS_RCRP_DONE);
        }
    } 
    else 
    {
    }
    /* For 0x2A */
    #if(DCM_SERVICE_2A_ENABLED == STD_ON)
    if(gDiagState == DIAG_IDLE)
    {
        for(i = 0; i < PeriodicDidCounter; i++)
        {
            if(gPeriodicResponseTimer[PeriodicDidList[i].DidPosition] != 0)
            {
                gPeriodicResponseTimer[PeriodicDidList[i].DidPosition]--;
            }
            else
            {
            }
        }
    }
    else
    {
    }
    #endif
}


/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DslInternal_RCRResponsePending
*                
* Description:   used for 0x78 negative response
*                             
* Inputs:        ForceRCR_RP: Parameter to decide the 0x78 send method
*                
* Outputs:       None
* 
* Limitations:   None  
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE) DslInternal_RCRResponsePending
(
    uint8 ForceRCR_RP
)
{
    PduInfoType temp = { DCM_NULL, 0};
                     
    if((gP2ServerTimer == 0)  ||  (ForceRCR_RP == FORCERCR_RP))  
    {
        Set_DiagState(DIAG_UDS_RCRP);
       
        gNegativeResponseBufferForRRCP[0] = DCM_NEGATIVE_RES_SERVICE_ID;
        gNegativeResponseBufferForRRCP[1]
                                  = gUDS_Physical_DiagBuffer[DCM_SID_POSITION];
        gNegativeResponseBufferForRRCP[2] 
                              = DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING;
        
        temp.SduDataPtr = gNegativeResponseBufferForRRCP;
        temp.SduLength = (uint16)NEG_RESP_BUFFER_SIZE;

        /* Transmite 0x78 negative response(NR) */
        (void)DcmTransmit(gMsgContextType.dcmRxPduId,&temp);
        
         /* Decrease the allowed maximum number of 0x78 NR */
        Dec_Max_Number_Of_RCRRP();
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_DiagnosticSessionControl
*                
* Description:   Implementation of Uds servie 0x10
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_10_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_DiagnosticSessionControl
(
    void
)
{
    DsdInternal_10_11_85();
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_EcuReset
*                
* Description:   Implementation of Uds servie 0x11
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_11_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_EcuReset
(
    void
) 
{   
    DsdInternal_10_11_85();
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_ClearDiagnosticInformation
*                
* Description:   Implementation of Uds servie 0x14
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_14_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ClearDiagnosticInformation
(
    void
)
{
    /* Check message format */
    if(gMsgContextType.reqDataLen != DCM_SERVICE_14_REQ_LENGTH)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    else
    {
        gSubFunctionHandler = App_ClearDiagnosticInformation;
        (void)(*gSubFunctionHandler)(&gMsgContextType);
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_ReadDTCInformation
*                
* Description:   Implementation of Uds servie 0x19 
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_19_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadDTCInformation
(
    void
)
{
    switch(gMsgContextType.reqData[1])
    {
        #if(reportNumberOfDTCByStatusMask == STD_ON)
        case DCM_REPORTNUMBEROFDTCBYSTATUSMASK:
        #endif
        
        #if(reportDTCByStatusMask == STD_ON)
        case DCM_REPORTDTCBYSTATUSMASK:
        #endif
        
        #if(reportDTCSnapshotRecordByRecordNumber == STD_ON)
        case DCM_REPORTDTCSNAPSHOTRECORDBYRECORDNUMBER:
        #endif
        
        #if(reportMirrorMemoryDTCByStatusMask == STD_ON)
        case DCM_REPORTMIRRORMEMORYDTCBYSTATUSMASK:
        #endif
        
        #if(reportNumberOfMirrorMemoryDTCByStatusMask == STD_ON)
        case DCM_REPORTNUMBEROFMIRRORMEMORYDTCBYSTATUSMASK:
        #endif
        
        #if(reportNumberOfEmissionsRelatedOBDDTCByStatusMask == STD_ON)
        case DCM_REPORTNUMBEROFEMISSIONSRELATEDOBDDTCBYSTATUSMASK:
        #endif
        #if(reportEmissionsRelatedOBDDTCByStatusMask == STD_ON)
        case DCM_REPORTEMISSIONSRELATEDOBDDTCBYSTATUSMASK:
        #endif
        
        #if((reportNumberOfDTCByStatusMask == STD_ON) \
           || (reportDTCByStatusMask == STD_ON) \
           || (reportDTCSnapshotRecordByRecordNumber == STD_ON) \
           || (reportMirrorMemoryDTCByStatusMask == STD_ON) \
           || (reportNumberOfMirrorMemoryDTCByStatusMask == STD_ON) \
           || (reportNumberOfEmissionsRelatedOBDDTCByStatusMask == STD_ON) \
           || (reportEmissionsRelatedOBDDTCByStatusMask == STD_ON))
        if(gMsgContextType.reqDataLen != DCM_REQUIRE_DATA_LENGTH_3_BYTE)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        }
        else
        {
        }
        break;
        #endif
        
        #if(reportDTCSnapshotIdentification == STD_ON) 
        case DCM_REPORTDTCSNAPSHOTIDENTIFICATION:
        #endif
        #if (reportDTCSnapshotIdentification == STD_ON) 
        if(gMsgContextType.reqDataLen != DCM_REQUIRE_DATA_LENGTH_2_BYTE)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        }
        else
        {
        }        
        break;
        #endif
        #if(reportDTCSnapshotRecordByDTCNumber == STD_ON) 
        case DCM_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER:
        #endif
        
        #if(reportDTCExtendedDataRecordByDTCNumber == STD_ON) 
        case DCM_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER:
        #endif
        #if(reportMirrorMemoryDTCExtendedDataRecordByDTCNumber == STD_ON) 
        case DCM_REPORTMIRRORMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER:
        #endif
        
        #if((reportDTCSnapshotRecordByDTCNumber == STD_ON) \
           || (reportDTCExtendedDataRecordByDTCNumber == STD_ON) \
           || (reportMirrorMemoryDTCExtendedDataRecordByDTCNumber == STD_ON))
        if(gMsgContextType.reqDataLen != DCM_REQUIRE_DATA_LENGTH_6_BYTE)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        }
        else
        {
        }
        break;
        #endif 
        
        #if(reportSupportedDTC == STD_ON)
        case DCM_REPORTSUPPORTEDDTC:
        #endif
        
        #if(reportFirstTestFailedDTC == STD_ON)
        case DCM_REPORTFIRSTTESTFAILEDDTC:
        #endif
        
        #if(reportFirstConfirmedDTC == STD_ON)
        case DCM_REPORTFIRSTCONFIRMEDDTC:
        #endif
        
        #if(reportMostRecentTestFailedDTC == STD_ON)
        case DCM_REPORTMOSTRECENTTESTFAILEDDTC:
        #endif
        
        #if(reportMostRecentConfirmedDTC == STD_ON)
        case DCM_REPORTMOSTRECENTCONFIRMEDDTC:
        #endif
        
        #if((reportSupportedDTC == STD_ON) \
           || (reportFirstTestFailedDTC == STD_ON) \
           || (reportFirstConfirmedDTC == STD_ON) \
           || (reportMostRecentTestFailedDTC == STD_ON) \
           || (reportMostRecentConfirmedDTC == STD_ON))
        if(gMsgContextType.reqDataLen != DCM_REQUIRE_DATA_LENGTH_2_BYTE)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        }
        else
        {
        }
        break;
        #endif
        
        #if(reportNumberOfDTCBySeverityMaskRecord == STD_ON)
        case DCM_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD:
        #endif
        
        #if(reportDTCBySeverityMaskRecord == STD_ON)
        case DCM_REPORTDTCBYSEVERITYMASKRECORD:
        #endif
        
        #if((reportNumberOfDTCBySeverityMaskRecord == STD_ON) \
           || (reportDTCBySeverityMaskRecord == STD_ON))
        if(gMsgContextType.reqDataLen != DCM_REQUIRE_DATA_LENGTH_4_BYTE)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        }
        else
        {
        }
        break;
        #endif
        
        #if(reportSeverityInformationOfDTC == STD_ON)
        case DCM_REPORTSEVERITYINFORMATIONOFDTC:
        if(gMsgContextType.reqDataLen != DCM_REQUIRE_DATA_LENGTH_5_BYTE)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        }
        else
        {
        }
        break;
        #endif
        
        default:
        break;
    }
    if(gNegativeResponseCode == DCM_E_POSITIVERESPONSE)
    {
        DsdInternal_Uds_Common();
    }
    else
    {
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_ReadDataByIdentifier
*                
* Description:   Implementation of Uds servie 0x22
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_22_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE)DspInternal_Uds_ReadDataByIdentifier
(
    void
)
{
    /* the correct request length should be even number and bigger than 0 */
    gMsgContextType.reqDataLen -= 1u; 
     
    /* Check message format */
    if((gMsgContextType.reqDataLen&0x01) != 0)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    else if((gMsgContextType.reqDataLen >> DCM_DIVIDE_TOW) > MAX_NUM_OF_DID_TO_READ)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
    }
    else
    {
        DsdInternal_DidDispatcher();
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_ReadMemoryByAddress
*                
* Description:   Implementation of Uds servie 0x23
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_23_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadMemoryByAddress
(
    void
)
{
    uint8 securitylevel = 0;
    uint8 sessiontype = 0;
    uint8 addressFormat = 0;
    uint8 sizeFormat = 0;
    uint8 i = 0;
    uint32 startAddress = 0;
    uint32 memorySize = 0;
    uint8  addresssegment = 0;
    uint8 error = DCM_HAVE_NO_ERRORS;
    addressFormat = gMsgContextType.reqData[1]  
                        & DCM_SERVICE_23_GET_MEMORY_AND_ADDRESS_BYTE_LOW_NIBBLE;
    sizeFormat = gMsgContextType.reqData[1] 
                      >> DCM_SERVICE_23_GET_MEMORY_AND_ADDRESS_BYTE_HIGH_NIBBLE;
                       
    /* Check addressAndLengthFormatIdentifier */
    if((addressFormat != gMemoryAddressFormat) 
                                          || (sizeFormat != gMemorySizeFormat))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
    }
    
    /* Check message format */ 
    else if(gMsgContextType.reqDataLen != (DCM_SERVICE_AND_SUB_SERVICE_REQUIRE_LENGTH 
    + gMemoryAddressFormat + gMemorySizeFormat))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    else
    {
        /* Calculate start address */
        for(i = 0;i < gMemoryAddressFormat;i++)
        {
            startAddress |= (uint32)(gMsgContextType.reqData[DCM_SERVICE_AND_SUB_SERVICE_REQUIRE_LENGTH + i])  
                    << (uint8)(DCM_AMPLIFY_8_TIMES*(gMemoryAddressFormat-1-i));      
        }
        
        /* Calculate size  */
        for(i = 0;i < gMemorySizeFormat;i++)
        {
            memorySize |= (uint32)(gMsgContextType.reqData[DCM_SERVICE_AND_SUB_SERVICE_REQUIRE_LENGTH + gMemoryAddressFormat+i])
                       << (uint8)(DCM_AMPLIFY_8_TIMES*(gMemorySizeFormat-1-i)); 
        }
        
        /* Check the scope of the address in requested message */
        for(i = 0;i < NUMBER_OF_READ_ADDRESS_SEG;i++)
        {
            if((startAddress >= gDcmDspReadMemoryRangeInfo_23[i].DcmDspReadMemoryRangeLow)
            && ((startAddress + memorySize)
            <= gDcmDspReadMemoryRangeInfo_23[i].DcmDspReadMemoryRangeHigh))
            {
                addresssegment = i;
                
                /* Check session condition */
                sessiontype = gDcmDspReadMemoryRangeInfo_23[addresssegment].DcmDspReadMemoryRangeSessionRef;
                if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    error = DCM_HAVE_ERRORS; 
                }
                
                /* Check adressing mode */
                else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) 
                && (gDcmDspReadMemoryRangeInfo_23[addresssegment].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1))
                {
                    /* Service is not supported under functional request,but do
                                 not send 0x11 negative response */
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    error = DCM_HAVE_ERRORS; 
                }
                else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX)
                && ((gDcmDspReadMemoryRangeInfo_23[addresssegment].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                {
                    /* Service is not supported under physical request, send 
                                 negative response */ 
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    error = DCM_HAVE_ERRORS;  
                }
                else
                {
                    /* Check security access conditon */
                    securitylevel = gDcmDspReadMemoryRangeInfo_23[addresssegment].DcmDspReadMemoryRangeSecurityLevelRef;
                    #if(DCM_SERVICE_27_ENABLED == STD_ON)
                    if(securitylevel != 0)
                    {
                        if((securitylevel & gSecLevelType ) == DCM_SECURITY_LEVEL_NOT_CORRECT)                
                        {
                             DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                             error = DCM_HAVE_ERRORS; 
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                    }
                    #endif
                }
                i = NUMBER_OF_READ_ADDRESS_SEG;
            }
            else
            {
                if(i == (NUMBER_OF_READ_ADDRESS_SEG-1))
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                    error = DCM_HAVE_ERRORS;
                }
            }
        }
        
        /* If no error happened, give positive response */
        if(error == DCM_HAVE_NO_ERRORS)
        {
            gSubFunctionHandler = App_ReadAddress;
            (void)(*gSubFunctionHandler)(&gMsgContextType);
        }
        else
        {
        }
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_ReadScalingDataByIdentifier
*                
* Description:   Implementation of Uds servie 0x24
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_24_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadScalingDataByIdentifier
(
    void
)
{
    uint8  i = 0;
    uint8  error = DCM_HAVE_NO_ERRORS;
    uint16 tempdid = 0;
    uint8  didposition = 0;
    uint8  securitylevel = 0;
    uint8  sessiontype = 0;
    
    /* Check message format */
    if(gMsgContextType.reqDataLen != DCM_SERVICE_24_REQ_LENGTH)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        error = DCM_HAVE_ERRORS;
    }
    else
    {
        tempdid = Make16Bit(gMsgContextType.reqData[1],gMsgContextType.reqData[2]);
        for(i = 0; i < gNumberOfReadDID_24; i++)
        {
            if(tempdid == gDcmDsdSubService_24[i].DcmDspDidIdentifier)
            {
                securitylevel
                = gDcmDsdSubService_24[didposition].DcmDspDidReadSecurityLevelRef;
                sessiontype = gDcmDsdSubService_24[didposition].DcmDspDidReadSessionRef;
                
                /* Check if the requested DID supported */
                if(gDcmDsdSubService_24[didposition].DcmDspDidUsed != 1)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                    error = DCM_HAVE_ERRORS;
                }
                
                /* Check Session condition */
                else if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    error = DCM_HAVE_ERRORS; 
                }
                
                /* Check adressing mode */
                else if((gMsgContextType.msgAddInfo.reqType
                == UDS_FUNCTIONAL_ON_CAN_RX) 
                && (gDcmDsdSubService_24[didposition].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1))
                {
                    /* Service is not supported under functional request,but do
                                 not send 0x31 negative response */
                     
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    error = DCM_HAVE_ERRORS; 
                }
                else if((gMsgContextType.msgAddInfo.reqType
                == UDS_PHYSICAL_ON_CAN_RX) && ((gDcmDsdSubService_24[didposition].AddressingMode&PHYSICAL_REQ_SUPPORTED) == 0))
                {
                    /* Service is not supported under physical request,
                                 send negative response */ 
                     
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    error = DCM_HAVE_ERRORS;  
                }
                
                /* Check security acess condition */
                #if(DCM_SERVICE_27_ENABLED == STD_ON)
                else if(securitylevel != 0)
                {
                    if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT)
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                        error = DCM_HAVE_ERRORS; 
                    }
                    else
                    {
                    }
                }
                #endif
                else
                {
                }
                didposition = i;
                i = gNumberOfReadDID_24;
            }
            else
            { 
                if(i == gNumberOfReadDID_24 - 1)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE); 
                }
                else
                {
                }
            }
        }
    }
    
    if(error == DCM_HAVE_NO_ERRORS)
    {
        /* If no error happend,give positive response */
        gSubFunctionHandler = gDcmDsdSubService_24[didposition].DcmDspDataGetScalingInfoFnc;
        (void)(*gSubFunctionHandler)(&gMsgContextType);
    }
    else
    {
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_SecurityAccess
*                
* Description:   Implementation of Uds servie 0x27
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
/* there is only one kind of security access level at a time */
#if(DCM_SERVICE_27_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_SecurityAccess
(
    void
)
{
    uint8 i = 0;
    uint8 level = 0;
    
    level = Dcm_GetSecurityLevelMap(gMsgContextType.reqData + 1);
    
    /* Check whether the delay time on-boot has elapsed */
    if(gSecurityDelayTimeOnBootFlag != DCM_FLAG_DISACTIVE)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
    }
    
    /* If attempt number is over the limit, check whether the delay time has elapsed */
    else if(gSecurityAcessAttempNumber[gCurrentSecurityAccessRequestLevel - 1] == 0)
    {
        if(gSecurityAcessAttempNumberExceeded[gCurrentSecurityAccessRequestLevel - 1] == 1)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_EXCEEDNUMBEROFATTEMPTS);
            gSecurityAcessAttempNumberExceeded[gCurrentSecurityAccessRequestLevel - 1] = 0;
        }
        else
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUIREDTIMEDELAYNOTEXPIRED);
        }
    }

    /*   Get Seed   */
    else if((gMsgContextType.reqData[1]&1) != DCM_SERVICE_27_SEND_KEEY) 
    {
        /* Check message format */
        if (gMsgContextType.reqDataLen != DCM_SERVICE_AND_SUB_SERVICE_REQUIRE_LENGTH)  
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
            DsdInternal_AccessAttemptNumberDec();
        }
        
        /* Check the requested security leve has already been cracked, if so, send zero seeds */
        else if (gSecLevelType == level)
        {               
            gMsgContextType.resData[1] = gMsgContextType.reqData[1];
            for (i = 0; i < gDcmDspSecurityRow[gSecLevelForCurrentSecurityAccessRequestLevel-1].DcmDspSecuritySeedSize; i++)
            {
                gMsgContextType.resData[DCM_SERVICE_AND_SUB_SERVICE_RESPONSE_LENGTH+i] = 0x00;
            }

            /* Record latest request's level */
            gCurrentSecurityAccessRequestLevel = gSecLevelForCurrentSecurityAccessRequestLevel;
            gSecurityAcessSequence[gCurrentSecurityAccessRequestLevel - 1] = HAVE_GOTTON_SEED;
            gMsgContextType.resDataLen
            = gDcmDspSecurityRow[gCurrentSecurityAccessRequestLevel - 1].DcmDspSecuritySeedSize 
            + DCM_SERVICE_AND_SUB_SERVICE_RESPONSE_LENGTH;    
            DsdInternal_ProcessingDone(&gMsgContextType);
        }
        
        /* If not, call related appliction callback function to get the seeds */
        else 
        {
            gCurrentSecurityAccessRequestLevel = gSecLevelForCurrentSecurityAccessRequestLevel;
            gSecurityAcessSequence[gSecLevelForCurrentSecurityAccessRequestLevel-1] = HAVE_GOTTON_SEED;
            gMsgContextType.resDataLen = gDcmDspSecurityRow[gCurrentSecurityAccessRequestLevel - 1].DcmDspSecuritySeedSize 
            + DCM_SERVICE_AND_SUB_SERVICE_RESPONSE_LENGTH; 
            gSubFunctionHandler = gDcmDsdSubService[gSubServicePosition].DcmDsdSubFunction;
            (void)(*gSubFunctionHandler)(&gMsgContextType);
        }
    }

    /*   Send Key  */
    else
    {
        /* Check whether the client has already gotten the seeds */
        if((gSecurityAcessSequence[gCurrentSecurityAccessRequestLevel - 1]&HAVE_GOTTON_SEED) != HAVE_GOTTON_SEED)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTSEQUENCEERROR);
            DsdInternal_AccessAttemptNumberDec();
        }
        else
        {   /* Check whether the send-key request's level is equal to latest get-seed request */
            if(gCurrentSecurityAccessRequestLevel != gSecLevelForCurrentSecurityAccessRequestLevel)
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTSEQUENCEERROR);

                /*If not,decrease allowed attmept number */
                DsdInternal_AccessAttemptNumberDec(); 
            }
            
            /* Check message format */
            else if (gMsgContextType.reqDataLen 
            != (gDcmDspSecurityRow[gSecLevelForCurrentSecurityAccessRequestLevel-1].DcmDspSecurityKeySize
            + DCM_SERVICE_AND_SUB_SERVICE_REQUIRE_LENGTH))
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                
                /*If not,decrease allowed attmept number */
                DsdInternal_AccessAttemptNumberDec();
            }
            else
            {
                /* Call related application callback funtion to compare the key */
                gSubFunctionHandler = gDcmDsdSubService[gSubServicePosition].DcmDsdSubFunction;
                (void)(*gSubFunctionHandler)(&gMsgContextType);
            }
        }
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_CommunicationControl
*                
* Description:   Implementation of Uds servie 0x28
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_28_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_CommunicationControl
(
    void
)
{
    /* Check message format */
    if(gMsgContextType.reqDataLen != DCM_SERVICE_28_REQ_LENGTH)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    
    /* Check communication type */
    else if(((gMsgContextType.reqData[2]&BOTH_NORMAL_AND_NM_MSG) == 0)
           || (gMsgContextType.reqData[2] > DCM_REQUIRE_DATA_LENGTH_3_BYTE))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
    }
    else
    {
        DsdInternal_Uds_Common();
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_ReadDataByPeriodicIdentifier
*                
* Description:   Implementation of Uds servie 0x2A
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if((DCM_SERVICE_2A_ENABLED == STD_ON) && (DCM_SERVICE_22_ENABLED == STD_ON))
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ReadDataByPeriodicIdentifier
(
    void
)
{
    uint32 i = 0;
    uint32 j = 0;
    uint8  securitylevel = 0;
    uint8  sessiontype = 0;
    PeriodicDidlistType periodicdidlist[MAX_NUM_OF_PERDID_TO_READ] = {0,0};
    uint8 counter = 0;
    uint8 temp = 0;
    uint8 error = DCM_HAVE_NO_ERRORS;
    uint8 SupportedDIDNum = 0;

    /* is number request DIDs   < = MAXNUMDIDs */
    /* Check transfer mode */
    if(((gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_2_BYTE ) > MAX_NUM_OF_PERDID_TO_READ) 
       || (gMsgContextType.reqData[1] > DCM_SERVICE_2A_HIGH_SUB_SERVICE_NUMBER)
       || (gMsgContextType.reqData[1] < DCM_SERVICE_2A_LOW_SUB_SERVICE_NUMBER))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE); 
        error = DCM_HAVE_ERRORS;
    }
    else
    {
        if(gMsgContextType.reqData[1] == DCM_SERVICE_2A_STOP_SENDING)
        {
            if(gMsgContextType.reqDataLen < DCM_REQUIRE_DATA_LENGTH_2_BYTE)
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                error = DCM_HAVE_ERRORS;
            }
            else if(gMsgContextType.reqDataLen == DCM_REQUIRE_DATA_LENGTH_2_BYTE)
            {
                /* Clear all periodicDid info in PeriodicDidList */ 
                for(i = 0; i < NUMBER_OF_PERIODIC_DID; i++)
                {
                    PeriodicDidList[i].Did = DCM_SERVICE_2A_NO_PERIODIC_DID_TO_STORE;
                    PeriodicDidList[i].DidPosition = DCM_SERVICE_2A_NO_PERIODIC_DIDPOSITION_TO_STORE;
                    gPeriodicResponseFlag[i] = DCM_FLAG_DISACTIVE;
                    gPeriodicResponseRate[i] = 0;
                    gPeriodicResponseTimer[i] = 0;
                }
               PeriodicDidCounter = 0;
               gMsgContextType.resDataLen = 1u;
               DsdInternal_ProcessingDone(&gMsgContextType);
               error = DCM_HAVE_ERRORS;
            }
            else
            {
            }
        }
        else if((gMsgContextType.reqData[1] == DCM_SERVICE_2A_SEND_AT_FAST_RATE)
               || (gMsgContextType.reqData[1] == DCM_SERVICE_2A_SEND_AT_MEDIUM_RATE)
               || (gMsgContextType.reqData[1] == DCM_SERVICE_2A_SEND_AT_SLOW_RATE))
        {
            if(gMsgContextType.reqDataLen < DCM_REQUIRE_DATA_LENGTH_3_BYTE)
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                error = DCM_HAVE_ERRORS;
            }
        }
        else
        {
        }

        if(error == DCM_HAVE_NO_ERRORS)
        {
            if(gMsgContextType.reqDataLen > DCM_REQUIRE_DATA_LENGTH_2_BYTE)
            {
                 /* Check whether a particular periodicDataIdentifier multiple times within a single request message */
                 for(i = 0; i < gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_3_BYTE; i++)
                 {
                     for(j = i+1u; j < gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_3_BYTE; j++)
                     {
                         if(gMsgContextType.reqData[i + DCM_REQUIRE_DATA_LENGTH_2_BYTE] 
                            == gMsgContextType.reqData[j + DCM_REQUIRE_DATA_LENGTH_2_BYTE])
                         {
                             DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                             error = DCM_HAVE_ERRORS;
                             i = gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_3_BYTE;
                             j = gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_3_BYTE;
                         }
                         else
                         {
                         }
                     }
                 }
             }
        }
        if(error == DCM_HAVE_NO_ERRORS)
        {
            /* Find the supported DID */
            counter = 0;
            for(i = 0; i < gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_2_BYTE; i++)
            {
                for(j = 0; j < NUMBER_OF_PERIODIC_DID; j++)
                {
                    if(gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_2_BYTE + i] == gDcmDsdSubService_2A[j].DcmDspDidIdentifier)
                    {
                        /* Check session type */
                        SupportedDIDNum++;
                        sessiontype = gDcmDsdSubService_2A[j].DcmDspDidReadSessionRef;
                        securitylevel = gDcmDsdSubService_2A[j].DcmDspDidReadSecurityLevelRef;
                        
                        /*if parameter supported in session */
                        if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
                        {
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                            error = DCM_HAVE_ERRORS;
                            j = NUMBER_OF_PERIODIC_DID;
                            i = gMsgContextType.reqDataLen;
                        }
                        
                       /*if parameter security access allow */
                       #if(DCM_SERVICE_27_ENABLED == STD_ON)
                       else if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT)
                       {                
                           DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                           error = DCM_HAVE_ERRORS;
                           j = NUMBER_OF_PERIODIC_DID;
                           i = gMsgContextType.reqDataLen;
                       }
                       
                       /* if other condication fullfilled */
                       /* if service security access allowed */
                       else if(service.ServiceTable.DcmDsdSidTabSecurityLevelRef & gSecLevelType == 0)
                       {
                           DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                           error = DCM_HAVE_ERRORS;
                           j = NUMBER_OF_PERIODIC_DID;
                           i = gMsgContextType.reqDataLen;
                       }
                       #endif
                       
                       else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX)
                       && (service.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                       {
                           /* AddressingMode is not supportted */
                       }
                       else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX)
                       && ((service.AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                       {
                           /* AddressingMode is not supportted */
                           DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                           error = DCM_HAVE_ERRORS;
                           j = NUMBER_OF_PERIODIC_DID;
                           i = gMsgContextType.reqDataLen;
                       }
                       
                       /* Check DID adressing mode */
                      else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX)
                      && (gDcmDsdSubService_2A[j].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                      {
                      }
                      else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX)
                      && ((gDcmDsdSubService_2A[j].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                      {
                          DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                          error = DCM_HAVE_ERRORS;
                          j = NUMBER_OF_PERIODIC_DID;
                          i = gMsgContextType.reqDataLen;
                      }
                      else
                      { 
                          periodicdidlist[counter].Did
                          = gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_2_BYTE + i];
                          periodicdidlist[counter].DidPosition = (uint16)j;                  
                          counter++; 
                          j = NUMBER_OF_PERIODIC_DID;
                      }
                    }
                    else
                    {
                    }
                }
            }

            if(gNegativeResponseCode != DCM_E_POSITIVERESPONSE)
            {
                /* do nothing */
            }
            else if(SupportedDIDNum == 0)
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                error = DCM_HAVE_ERRORS;
            }
            /* check other state of the supported DID  */ 
            else if(counter == 0)
            {
                /* all DIDs' AddressingMode not OK ,under UDS_FUNCTIONAL addressing */
                DsdInternal_ProcessingDoneNoResponse();
                error = DCM_HAVE_ERRORS;
            }
            else
            {
                if(gMsgContextType.reqData[1] < DCM_REQUIRE_DATA_LENGTH_4_BYTE)
                {
                    /* Update periodic DID ,If one periodicDid is already in PeriodicDidList,use new transmission rate */
                    for(i = 0; i < counter; i++)
                    {
                        if(gMsgContextType.reqData[1] == DCM_REQUIRE_DATA_LENGTH_1_BYTE)
                        {
                            gPeriodicResponseRate[periodicdidlist[i].DidPosition]
                            = SEND_AT_SLOW_RATE/DCM_TASK_TIME;
                        }
                        else if(gMsgContextType.reqData[1] == DCM_REQUIRE_DATA_LENGTH_2_BYTE)
                        {
                            gPeriodicResponseRate[periodicdidlist[i].DidPosition]
                            = SEND_AT_MEDIUM_RATE/DCM_TASK_TIME;
                        }
                        else
                        {
                            gPeriodicResponseRate[periodicdidlist[i].DidPosition]
                            = SEND_AT_FAST_RATE/DCM_TASK_TIME;
                        }
                        if(gPeriodicResponseFlag[periodicdidlist[i].DidPosition] == DCM_FLAG_ACTIVE)
                        {
                            /* change rate frome next periodic ,so delet */
                        }
                        else
                        {
                            gPeriodicResponseFlag[periodicdidlist[i].DidPosition] = DCM_FLAG_ACTIVE;
                            
                            /*Send as soon as possible*/
                            gPeriodicResponseTimer[periodicdidlist[i].DidPosition] = 0;
                        }
                    }
                    
                    /*Updata PeriodicDidList */
                    PeriodicDidCounter = 0;
                    for(j = 0; j < NUMBER_OF_PERIODIC_DID; j++)
                    {
                        if(gPeriodicResponseFlag[j] == DCM_FLAG_ACTIVE)
                        {
                            PeriodicDidList[PeriodicDidCounter].Did
                            = gDcmDsdSubService_2A[j].DcmDspDidIdentifier;
                            PeriodicDidList[PeriodicDidCounter].DidPosition = (uint16)j;
                            PeriodicDidCounter++;
                        }
                    }
                    
                    /* restart from the first  PeriodicDidList*/
                    CurrentPeriodicDidIndx  = 0;
                }
                else
                { 
                    /* dele DID from  Periodic Tx table (PeriodicDidList)*/
                    temp = PeriodicDidCounter;
                    
                    /* Clear periodicDid info in PeriodicDidList */ 
                    for(i = 0; i < counter; i++)
                    {
                        for(j = 0; j < temp; j++)
                        {
                            if((periodicdidlist[i].Did == PeriodicDidList[j].Did) 
                            && (gPeriodicResponseFlag[periodicdidlist[i].DidPosition] == DCM_FLAG_ACTIVE))
                            {
                                gPeriodicResponseFlag[periodicdidlist[i].DidPosition] = DCM_FLAG_DISACTIVE;
                                gPeriodicResponseRate[periodicdidlist[i].DidPosition] = 0;
                                gPeriodicResponseTimer[periodicdidlist[i].DidPosition] = 0;
                                j = temp;
                            }
                            else
                            {
                                if(j == (temp-1u))
                                {
                                    /* 
                                     * If periodicDid to clear is not in PeriodicDidList, 
                                     * give negative response 
                                     */
                                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                                    error = DCM_HAVE_ERRORS; 
                                }
                                else
                                {
                                }
                            }
                        }
                    }
                   if(error == DCM_HAVE_NO_ERRORS)
                   {
                       /*Updata PeriodicDidList */
                       PeriodicDidCounter = 0;
                       for(j = 0; j < NUMBER_OF_PERIODIC_DID; j++)
                       {
                           if(gPeriodicResponseFlag[j] == DCM_FLAG_ACTIVE)
                           {
                               PeriodicDidList[PeriodicDidCounter].Did
                               = gDcmDsdSubService_2A[j].DcmDspDidIdentifier; 
                               PeriodicDidList[PeriodicDidCounter].DidPosition = (uint16)j;
                               PeriodicDidCounter++;
                           }
                       }
                       for(j = 0; j < PeriodicDidCounter; j++)
                       {
                           if(CurrentPeriodicDid.DidPosition
                           == PeriodicDidList[j].DidPosition)
                           {
                               /*
                                * remember the current did'index in new list,
                                * if the current did is not dele 
                                */
                               CurrentPeriodicDidIndx = (uint16)j;
                               j = PeriodicDidCounter;
                           }
                           else
                           {   
                               /*Tx DID from the first*/
                               CurrentPeriodicDidIndx  = 0;
                           }
                      }
                   }
                }
            }
        }
    }
    
    /* No error happend,postive response */
    if(error == DCM_HAVE_NO_ERRORS) 
    {
        gMsgContextType.resDataLen = 1u;
        DsdInternal_ProcessingDone(&gMsgContextType);
    }
    else
    {
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_DynamicallyDefinedDataIdentifier
*                
* Description:   Implementation of Uds servie 0x2C
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/

/* Asumption: the addresssAndLength format is the same as 0x23 and 0x3d */
#if((DCM_SERVICE_2C_ENABLED == STD_ON) && (DCM_SERVICE_22_ENABLED == STD_ON) && (DCM_SERVICE_23_ENABLED == STD_ON))
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_DynamicallyDefinedDataIdentifier
(
    void
)
{
    uint8  i = 0;
    uint8  j = 0;
    uint8  error = 1;
    uint16 tempdid = 0;
    uint16 dynamicaldid = 0xFF;
    uint8  dynamicaldidposition = 0;
    uint8  securitylevel = 0;
    uint8  sessiontype = 0;
    DynamicalDidRelatedInfo  dynamicaldidrealatedinfo = {0,0,0,{ 0 },{ 0 },0,{ 0 },0,0};
    uint32 sum = 0;
    uint8  sourcedidcounter = 0;
    uint8  addressFormat = 0;
    uint32 startAddress = 0;
    uint8  sizeFormat = 0;
    uint32 memorySize = 0; 
    uint8  addresssegment = 0;
    uint8  addressandmemorylength = 0;
    uint8  addresscounter = 0;
    
    /* Check message format */
    if(gMsgContextType.reqDataLen < DCM_SERVICE_2C_LEAST_REQ_LENGTH)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    else
    {
        tempdid = Make16Bit(gMsgContextType.reqData[2],gMsgContextType.reqData[3]);
        for(i = 0; i < NUMBER_OF_DYN_DID; i++)
        {
            if(tempdid == gDcmDsdSubService_2C[i].DcmDspDidIdentifier)
            {
                /* Check the security access condition of dynamical did */
                securitylevel = gDcmDsdSubService_2C[dynamicaldidposition].DcmDspDidReadSecurityLevelRef;
                sessiontype = gDcmDsdSubService_2C[dynamicaldidposition].DcmDspDidReadSessionRef;
                if(securitylevel != 0)
                {
                    #if(DCM_SERVICE_27_ENABLED == STD_ON)
                    if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT)
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                        error = 1; 
                    }
                    else
                    #endif
                    {
                        error = 0;
                        dynamicaldidposition = i;
                        i = NUMBER_OF_DYN_DID;
                    }
                }
                else
                {
                    error = 0;
                    dynamicaldidposition = i;
                    i = NUMBER_OF_DYN_DID;
                }
            }
            else
            {
                 if(i == (NUMBER_OF_DYN_DID-1))
                 {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                    error = 1;
                 }
                 else
                 {
                 }
            }
        }
        if(error == 1)
        { 
            /* do nothing */
        }
        
        /* Check session condition */
        else if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
        }
        
        /* Check adressing mode */
        else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) \
        && (gDcmDsdSubService_2C[dynamicaldidposition].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1))
        {
            /* Service is not supported under functional request,but do not send 0x11 negative response */
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
        }
        else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) && ((gDcmDsdSubService_2C[dynamicaldidposition].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
        {
            /* Service is not supported under physical request, send negative response */ 
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
        }
        else
        {       
            dynamicaldid = tempdid;
            if(gMsgContextType.reqData[1] == 0x01) 
            {
                dynamicaldidrealatedinfo.SourceDidCounter = 0;
                dynamicaldidrealatedinfo.TotalDataSize = 0;
                
                /* Check message format. correct length should be multiple of 4 */
                if((gMsgContextType.reqDataLen % DCM_SERVICE_2C_JUDGE_THE_LENGTH_IS_MULTIPLE_OF_4) != 0)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                }
                else
                {
                    for(i = 0; i < (gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_4_BYTE); i +=  DCM_REQUIRE_DATA_LENGTH_4_BYTE)
                    {
                        tempdid = Make16Bit(gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_4_BYTE + i],gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_5_BYTE + i]);
                        for(j = 0; j < NUMBER_OF_READ_DID; j++)                                          
                        {
                            if(tempdid != gDcmDsdSubService_22[j].DcmDspDidIdentifier)
                            {
                                /* Check whether all the source is supported */
                                if(j == (NUMBER_OF_READ_DID-1))
                                {
                                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                                    error = 1;
                                }
                                else
                                {
                                }
                            }
                            else
                            {
                                /* Check whether the PositionInSourceDataRecord is valid */
                                if((gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_6_BYTE+i] < 1)
                                  || (gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_6_BYTE+i] > gDcmDsdSubService_22[j].DcmDspDataSize))
                                {
                                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                                    error = 1;
                                }
                                else
                                {
                                    /* Check security access condition of the source did */
                                    securitylevel = gDcmDsdSubService_22[j].DcmDspDidReadSecurityLevelRef;
                                    if(securitylevel != 0)
                                    {
                                        #if(DCM_SERVICE_27_ENABLED == STD_ON)

                                        if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT)
                                        {
                                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                                            error = 1;
                                        }
                                        else
                                        {
                                            /* Record related info of the supported source did and check the next source did */
                                            dynamicaldidrealatedinfo.ByDid[dynamicaldidrealatedinfo.SourceDidCounter].SourceDid = tempdid;
                                            dynamicaldidrealatedinfo.ByDid[dynamicaldidrealatedinfo.SourceDidCounter].SourceDidPosition = j;
                                            dynamicaldidrealatedinfo.ByDid[dynamicaldidrealatedinfo.SourceDidCounter].PositionInSourceDataRecord 
                                            = gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_6_BYTE + i];
                                            dynamicaldidrealatedinfo.ByDid[dynamicaldidrealatedinfo.SourceDidCounter].MemorySize 
                                            = gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_7_BYTE + i];
                                            dynamicaldidrealatedinfo.SourceDidCounter++; 

                                            /* Redudce cycle times */
                                            j = NUMBER_OF_READ_DID;
                                        }
                                        #endif
                                    }
                                    else
                                    {
                                        /* Record related info of the supported source did and check the next source did */
                                        dynamicaldidrealatedinfo.ByDid[dynamicaldidrealatedinfo.SourceDidCounter].SourceDid = tempdid;
                                        dynamicaldidrealatedinfo.ByDid[dynamicaldidrealatedinfo.SourceDidCounter].SourceDidPosition = j;
                                        dynamicaldidrealatedinfo.ByDid[dynamicaldidrealatedinfo.SourceDidCounter].PositionInSourceDataRecord = gMsgContextType.reqData[6 + i];
                                        dynamicaldidrealatedinfo.ByDid[dynamicaldidrealatedinfo.SourceDidCounter].MemorySize = gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_7_BYTE + i];
                                        dynamicaldidrealatedinfo.SourceDidCounter++;

                                        /* Redudce cycle times */
                                        j = NUMBER_OF_READ_DID;
                                    }
                                    
                                }
                            }
                        }
                    }
                    if(error == 0)
                    {
                        /* Check whether the amount of data to be packed into the dynamic data identifier exceeds the maximum allowed by server */
                        dynamicaldidrealatedinfo.TotalDataSize = gDynamicalDidRelatedInfo[dynamicaldidposition].TotalDataSize;
                        for(i = 0; i < dynamicaldidrealatedinfo.SourceDidCounter; i++)
                        {
                            sum +=  dynamicaldidrealatedinfo.ByDid[i].MemorySize;
                        }
                        dynamicaldidrealatedinfo.TotalDataSize += sum;
                        if(dynamicaldidrealatedinfo.TotalDataSize > gDcmDsdSubService_2C[dynamicaldidposition].DcmDspDataSize)
                        {
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                        }
                        else
                        {
                            /* No error happended,move all the source Did info in to global variable gDynamicalDidRelatedInfo[8] */
                            if(gDynamicalDidRelatedInfo[dynamicaldidposition].DynamicalDid != dynamicaldid)
                            {
                                gDynamicalDidRelatedInfo[dynamicaldidposition].DynamicalDid = dynamicaldid;
                                gDynamicalDidRelatedInfo[dynamicaldidposition].DynamicalDidPosion = dynamicaldidposition;
                                gDynamicalDidCounter++;
                            }
                            sourcedidcounter = gDynamicalDidRelatedInfo[dynamicaldidposition].SourceDidCounter;
                            for(i = 0;i < dynamicaldidrealatedinfo.SourceDidCounter;i++)
                            {
                                gDynamicalDidRelatedInfo[dynamicaldidposition].ByDid[i + sourcedidcounter].SourceDid = dynamicaldidrealatedinfo.ByDid[i].SourceDid;
                                gDynamicalDidRelatedInfo[dynamicaldidposition].ByDid[i + sourcedidcounter].SourceDidPosition = dynamicaldidrealatedinfo.ByDid[i].SourceDidPosition;
                                gDynamicalDidRelatedInfo[dynamicaldidposition].ByDid[i + sourcedidcounter].PositionInSourceDataRecord = dynamicaldidrealatedinfo.ByDid[i].PositionInSourceDataRecord;
                                gDynamicalDidRelatedInfo[dynamicaldidposition].ByDid[i + sourcedidcounter].MemorySize = dynamicaldidrealatedinfo.ByDid[i].MemorySize;
                            }
                            gDynamicalDidRelatedInfo[dynamicaldidposition].SourceDidCounter += dynamicaldidrealatedinfo.SourceDidCounter; 
                            gDynamicalDidRelatedInfo[dynamicaldidposition].TotalDataSize = dynamicaldidrealatedinfo.TotalDataSize;
                            i = gDynamicalDidRelatedInfo[dynamicaldidposition].DefinedTime;
                            gDynamicalDidRelatedInfo[dynamicaldidposition].Sequence[i].ByDidOrAddress = 0;
                            gDynamicalDidRelatedInfo[dynamicaldidposition].Sequence[i].DidOrAddressCounter = dynamicaldidrealatedinfo.SourceDidCounter;
                            gDynamicalDidRelatedInfo[dynamicaldidposition].DefinedTime++;
                            gMsgContextType.resDataLen = DCM_REQUIRE_DATA_LENGTH_4_BYTE;
                            DsdInternal_ProcessingDone(&gMsgContextType);
                        }
                    }
                    else
                    {
                    }
                }
            }
            else if(gMsgContextType.reqData[1] == DCM_SERVICE_2C_DEFINE_BY_MEMORY_ADDRESS)
            {
                dynamicaldidrealatedinfo.AddessCounter = 0;
                dynamicaldidrealatedinfo.TotalDataSize = 0;
                addressFormat = gMsgContextType.reqData[4] & DCM_SERVICE_23_GET_MEMORY_AND_ADDRESS_BYTE_LOW_NIBBLE;
                sizeFormat = gMsgContextType.reqData[4] >> DCM_REQUIRE_DATA_LENGTH_4_BYTE; 
                addressandmemorylength = addressFormat + sizeFormat;
                
                /* Check addressAndLengthFormatIdentifier */
                if((addressFormat != gMemoryAddressFormat) || (sizeFormat != gMemorySizeFormat))
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                }

                /* Check message format */
                else if((gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_5_BYTE)%addressandmemorylength != 0)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                }
                else
                {   
                    for(i = 0; i < (gMsgContextType.reqDataLen-DCM_REQUIRE_DATA_LENGTH_5_BYTE);i += addressandmemorylength)
                    {
                        /* Calculate start address */
                        for(j = 0; j < gMemoryAddressFormat; j++)
                        {
                            startAddress |= (uint32)(gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_5_BYTE + j + i]) << (uint8)(DCM_AMPLIFY_8_TIMES*(gMemoryAddressFormat - 1 - j));      
                        }
                        
                        /* Calculate size */
                        for(j = 0; j < gMemorySizeFormat; j++)
                        {
                            memorySize |= (uint32)(gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_5_BYTE + gMemoryAddressFormat + j + i]) << (uint8)(DCM_AMPLIFY_8_TIMES*(gMemorySizeFormat - 1 - j)); 
                        }
                        
                        /* Check address */
                        for(j = 0; j < NUMBER_OF_READ_ADDRESS_SEG; j++)
                        {
                            if((startAddress >= gDcmDspReadMemoryRangeInfo_23[j].DcmDspReadMemoryRangeLow) && ((startAddress + memorySize)  <= gDcmDspReadMemoryRangeInfo_23[j].DcmDspReadMemoryRangeHigh))
                            {
                                addresssegment = j;
                                j = NUMBER_OF_READ_ADDRESS_SEG;
                            }
                            else
                            {
                                if(j == (NUMBER_OF_READ_ADDRESS_SEG-1))
                                {
                                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                                    error = 1;
                                }
                            }
                        }
                        if(error == 0)
                        {
                            /* Check security access conditon */
                            securitylevel = gDcmDspReadMemoryRangeInfo_23[addresssegment].DcmDspReadMemoryRangeSecurityLevelRef;
                            if(securitylevel != 0)
                            {
                                #if(DCM_SERVICE_27_ENABLED == STD_ON)
                                if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT) 
                                {
                                     DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                                     error = 1; 
                                }
                                else
                                {
                                    /* Record related info of the supported source did and check the next source did */
                                    dynamicaldidrealatedinfo.ByAddress[dynamicaldidrealatedinfo.AddessCounter].MemoryAddress = startAddress;
                                    dynamicaldidrealatedinfo.ByAddress[dynamicaldidrealatedinfo.AddessCounter].AddressSegment = addresssegment;
                                    dynamicaldidrealatedinfo.ByAddress[dynamicaldidrealatedinfo.AddessCounter].MemorySize = memorySize;
                                    dynamicaldidrealatedinfo.AddessCounter++;
                                    
                                    /* Clear the print of last cycle */
                                    startAddress = 0;

                                     /* Clear the print of last cycle */
                                    memorySize = 0; 
                                }
                                #endif
                            }
                            else
                            {
                                /* Record related info of the supported source did and check the next source did */
                                dynamicaldidrealatedinfo.ByAddress[dynamicaldidrealatedinfo.AddessCounter].MemoryAddress = startAddress;
                                dynamicaldidrealatedinfo.ByAddress[dynamicaldidrealatedinfo.AddessCounter].AddressSegment = addresssegment;
                                dynamicaldidrealatedinfo.ByAddress[dynamicaldidrealatedinfo.AddessCounter].MemorySize = memorySize;
                                dynamicaldidrealatedinfo.AddessCounter++;
                                
                                /* Clear the print of last cycle */
                                startAddress = 0;

                                /* Clear the print of last cycle */
                                memorySize = 0;  
                            }
                        }
                        else
                        {
                        }
                    }
                    if(error == 0)
                    {
                        /* 
                         * Check whether the amount of data to be packed into the dynamic data 
                         * identifier exceeds the maximum allowed by server 
                         */
                        dynamicaldidrealatedinfo.TotalDataSize = gDynamicalDidRelatedInfo[dynamicaldidposition].TotalDataSize;
                        for(i = 0; i < dynamicaldidrealatedinfo.AddessCounter; i++)
                        {
                            sum += dynamicaldidrealatedinfo.ByAddress[i].MemorySize;
                        }
                        dynamicaldidrealatedinfo.TotalDataSize += sum;
                        if(dynamicaldidrealatedinfo.TotalDataSize > gDcmDsdSubService_2C[dynamicaldidposition].DcmDspDataSize)
                        {
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                        }
                        else
                        {
                            /* 
                             * No error happended,move all the source did info in to global 
                             * variable gDynamicalDidRelatedInfo[8] 
                             */
                            if(gDynamicalDidRelatedInfo[dynamicaldidposition].DynamicalDid != dynamicaldid)
                            {
                                gDynamicalDidRelatedInfo[dynamicaldidposition].DynamicalDid = dynamicaldid;
                                gDynamicalDidRelatedInfo[dynamicaldidposition].DynamicalDidPosion = dynamicaldidposition;
                                gDynamicalDidCounter++;
                            }
                            addresscounter = gDynamicalDidRelatedInfo[dynamicaldidposition].AddessCounter;
                            for(i = 0; i < dynamicaldidrealatedinfo.AddessCounter; i++)
                            {
                                gDynamicalDidRelatedInfo[dynamicaldidposition].ByAddress[i + addresscounter].MemoryAddress 
                                = dynamicaldidrealatedinfo.ByAddress[i].MemoryAddress;
                                gDynamicalDidRelatedInfo[dynamicaldidposition].ByAddress[i + addresscounter].AddressSegment 
                                = dynamicaldidrealatedinfo.ByAddress[i].AddressSegment;
                                gDynamicalDidRelatedInfo[dynamicaldidposition].ByAddress[i + addresscounter].MemorySize 
                                = dynamicaldidrealatedinfo.ByAddress[i].MemorySize;
                            }
                            gDynamicalDidRelatedInfo[dynamicaldidposition].AddessCounter += dynamicaldidrealatedinfo.AddessCounter; 
                            gDynamicalDidRelatedInfo[dynamicaldidposition].TotalDataSize = dynamicaldidrealatedinfo.TotalDataSize;
                            i = gDynamicalDidRelatedInfo[dynamicaldidposition].DefinedTime;
                            gDynamicalDidRelatedInfo[dynamicaldidposition].Sequence[i].ByDidOrAddress = 1;
                            gDynamicalDidRelatedInfo[dynamicaldidposition].Sequence[i].DidOrAddressCounter = dynamicaldidrealatedinfo.AddessCounter;
                            gDynamicalDidRelatedInfo[dynamicaldidposition].DefinedTime++;
                            
                            /* Positive response */ 
                            gMsgContextType.resDataLen = DCM_RESPONSE_DATA_LENGTH_4_BYTE;
                            DsdInternal_ProcessingDone(&gMsgContextType);
                        }
                    }
                    else
                    {
                    }
                }
            }
            else
            {
                if(gMsgContextType.reqDataLen != DCM_SERVICE_2C_SUB_SERVICE_03_REQ_LENGTH)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                }
                else
                {
                    /* Clear dynamical Did info */
                    gDynamicalDidRelatedInfo[dynamicaldidposition].DynamicalDid = 0;
                    gDynamicalDidRelatedInfo[dynamicaldidposition].DynamicalDidPosion = 0;
                    gDynamicalDidRelatedInfo[dynamicaldidposition].DefinedTime = 0;
                    gDynamicalDidRelatedInfo[dynamicaldidposition].SourceDidCounter = 0;
                    gDynamicalDidRelatedInfo[dynamicaldidposition].AddessCounter = 0;
                    gDynamicalDidRelatedInfo[dynamicaldidposition].TotalDataSize = 0;
                    gDynamicalDidCounter--;
                    
                    /* No error happended,positive response */
                    gMsgContextType.resDataLen = DCM_RESPONSE_DATA_LENGTH_4_BYTE;
                    DsdInternal_ProcessingDone(&gMsgContextType);
                }
            }
        }
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_WriteDataByIdentifier
*                
* Description:   Implementation of Uds servie 0x2E
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_2E_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_WriteDataByIdentifier
(
    void
)
{
    uint8  i = 0;
    uint8  error = 1;
    uint16 tempdid = 0;
  
    uint8  didposition = 0;
    uint8  securitylevel = 0;
    uint8  sessiontype = 0;
  
    tempdid = Make16Bit(gMsgContextType.reqData[1],gMsgContextType.reqData[2]);
    for(i = 0; i < gNumberOfWriteDID; i++)
    {
        if(tempdid == gDcmDsdSubService_2E[i].DcmDspDidIdentifier)
        {
            error = 0;
            didposition = i;
            i = gNumberOfWriteDID;
        }
        else
        {
        }
    }
    sessiontype = gDcmDsdSubService_2E[didposition].DcmDspDidWriteSessionRef;
    
    /* Check whether the requested DID is defined in gDcmDsdSubService_2E */
    if(error == 1)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
    }
    
    /* Check if the requested DID supported */
    else if(gDcmDsdSubService_2E[didposition].DcmDspDidUsed != 1)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
    }
    
    /* Check message format */
    else if(gMsgContextType.reqDataLen != (3 + gDcmDsdSubService_2E[didposition].DcmDspDataSize))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    
    /* Check session condition */
    else if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
    }
    
    /* Check adressing mode */
    else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) 
    && (gDcmDsdSubService_2E[didposition].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
    }
    else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX)
    && ((gDcmDsdSubService_2E[didposition].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
    {
        /* Service is not supported under physical request, send negative response */ 
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
    }
    else
    {
        /* Check whether the security condition is met */
        securitylevel = gDcmDsdSubService_2E[didposition].DcmDspDidWriteSecurityLevelRef;
        if(securitylevel != 0)
        {
            #if(DCM_SERVICE_27_ENABLED == STD_ON)
            if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT)               
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
            }
            #endif
            #if(DCM_SERVICE_27_ENABLED == STD_ON)
            else
            {
            #endif
            
                /* No error happened,give positive response */
                gMsgContextType.resData += 1;
                gSubFunctionHandler = gDcmDsdSubService_2E[didposition].DcmDspDataWriteFnc;
                (void)(*gSubFunctionHandler)(&gMsgContextType);

            #if(DCM_SERVICE_27_ENABLED == STD_ON)
            }
            #endif
        }
        else
        {
            /* No error happened,give positive response */
            gMsgContextType.resData += 1;
            gSubFunctionHandler = gDcmDsdSubService_2E[didposition].DcmDspDataWriteFnc;
            (void)(*gSubFunctionHandler)(&gMsgContextType);
        }
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_InputOutputControlByIdentifier
*                
* Description:   Implementation of Uds servie 0x2F
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_2F_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_InputOutputControlByIdentifier
(
    void
)
{
    uint8  i = 0;
    uint8  error = DCM_HAVE_ERRORS;
    uint16 tempdid = 0;
   
    uint8  didposition = 0;
    uint8  securitylevel = 0;
    uint8  sessiontype = 0;

    
   // DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
   // return;


    tempdid = Make16Bit(gMsgContextType.reqData[1],gMsgContextType.reqData[2]);
    for(i = 0; i < gNumberOfIOCtrlDID; i++)
    {
        if(tempdid == gDcmDsdSubService_2F[i].DcmDspDidIdentifier)
        {
            error = DCM_HAVE_NO_ERRORS;
            didposition = i;
            sessiontype = gDcmDsdSubService_2F[didposition].DcmDspDidControlSessionRef;
            i = gNumberOfIOCtrlDID;
        }
    }
    
    /* Check whether the requested DID is defined in gDcmDsdSubService_2F */
    if(error != DCM_HAVE_NO_ERRORS)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
    }
    
    /* Check session condition */
    else if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
    }
    
    /* Check adressing mode */
    else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) 
    && (gDcmDsdSubService_2F[didposition].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
    }
    else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX)
    && ((gDcmDsdSubService_2F[didposition].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
    {
        /* Service is not supported under physical request, send negative response */ 
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
    }
    else
    {
        /* Check security condition */
        securitylevel = gDcmDsdSubService_2F[didposition].DcmDspDidControlSecurityLevelRef;
        if(securitylevel != 0)
        {
            #if(DCM_SERVICE_27_ENABLED == STD_ON)
            if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT)             
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                error = DCM_HAVE_ERRORS;
            }
            else
            {
            }
            #endif
        }
        else
        {
        }
        if(error == DCM_HAVE_NO_ERRORS)
        {
            
            switch(gMsgContextType.reqData[3])
            {
                case RETURN_CONTROL_TO_ECU:
                {
                    /* Check message format */
                    if(gMsgContextType.reqDataLen != DCM_SERVICE_2F_RETURN_CONTROL_TO_ECU_REQ_LENGTH)
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                    }
                    else
                    {
                        /* No error happened,give positive response */
                        gSubFunctionHandler = gDcmDsdSubService_2F[didposition].DcmDspDataReturnControlToEcuFnc;
                        if(gSubFunctionHandler == DCM_NULL)
                        {                                           
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                        
                        }
                        else
                        {
                            (void)(*gSubFunctionHandler)(&gMsgContextType);
                        }
                    }
                }
                break;
                
                case RESET_TO_DEFAULT:
                {
                    /* Check message format */
                    if(gMsgContextType.reqDataLen != DCM_SERVICE_2F_RESET_TO_DEFAULT_REQ_LENGTH)
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                    }
                    else
                    {
                        /* Call related appliction callbacek function */
                        gSubFunctionHandler = gDcmDsdSubService_2F[didposition].DcmDspDataResetToDefaultFnc;
                        
                        if(gSubFunctionHandler == DCM_NULL)
                        {                                        
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                        
                        }
                        else
                        {
                            (void)(*gSubFunctionHandler)(&gMsgContextType);
                        }
                    }
                }
                break;
                
                case FREEZE_CURRENT_STATE:
                {
                    /* Check message format */ 
                    if(gMsgContextType.reqDataLen != DCM_SERVICE_2F_FREEZE_CURRENT_STATE_REQ_LENGTH)
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                    }
                    else
                    {
                        /* Call related appliction callbacek function */
                        gSubFunctionHandler = gDcmDsdSubService_2F[didposition].DcmDspDataFreezeCurrentStateFnc;
                        
                        if(gSubFunctionHandler == DCM_NULL)
                        {                                            
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                        
                        }
                        else
                        {
                            (void)(*gSubFunctionHandler)(&gMsgContextType);
                        }
                    }
                }
                break;
                
                case SHORT_TERM_ADJUSTMENT:
                {
                    /* Check message format */
                    if(gMsgContextType.reqDataLen 
                    != DCM_REQUIRE_DATA_LENGTH_4_BYTE + gDcmDsdSubService_2F[didposition].RecordSizes.DcmDspDidControlEnableMaskRecordSize)
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                    }
                    else
                    {
                        /* Call related appliction callbacek function */
                        gSubFunctionHandler = gDcmDsdSubService_2F[didposition].DcmDspDataShortTermAdjustmentFnc;
                        
                        if(gSubFunctionHandler == DCM_NULL)
                        {                                         
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                        }
                        else
                        {
                            (void)(*gSubFunctionHandler)(&gMsgContextType);            
                        }
                    }
                }
                break;
                
                default:
                {
                    /* Check message format*/
                    if(gMsgContextType.reqDataLen != (DCM_REQUIRE_DATA_LENGTH_3_BYTE 
                      +  gDcmDsdSubService_2F[didposition].RecordSizes.DcmDspDidControlOptionRecordSize))
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                    }
                    else
                    {
                        gSubFunctionHandler = gDcmDsdSubService_2F[didposition].OtherFnc;
                       
                        if(gSubFunctionHandler == DCM_NULL)
                        {
                                                                        
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                        
                        }
                        else
                        {
                            (void)(*gSubFunctionHandler)(&gMsgContextType);
                        }
                    }
                }
                break;
            }
        }
        else
        {
        }
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_RoutineControl
*                
* Description:   Implementation of Uds servie 0x31
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_31_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE)DspInternal_Uds_RoutineControl
(
    void
)
{
    uint8 i = 0;
    uint16 routineId = 0; 
    uint8 postion = NUMBER_OF_RID;
    uint8 securitylevel = INVALID_SECURITY_LEVEL;
    uint8 sessiontype = INVALID_SESSION_TYPE;
    uint8 error = DCM_HAVE_ERRORS;

    routineId = Make16Bit(gMsgContextType.reqData[2],gMsgContextType.reqData[3]);
    
    /* Check whether the requested RID is supported */
    for(i = 0; i < NUMBER_OF_RID; i++)
    {
        if(routineId == DcmDspRoutineIdentifierTable_31[i].DcmDspRoutineIdentifier)
        {
            postion = i;
            sessiontype = DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.RoutineAuthorization.DcmDspRoutineSessionRef;
            securitylevel = DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.RoutineAuthorization.DcmDspRoutineSecurityLevelRef; 
            i = NUMBER_OF_RID;
        }
        else
        {
        }
    }

    /* No supported RID found */ 
    if(postion == NUMBER_OF_RID) 
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE); 
    }
    
    /* parameter execuation state check */
    /* is parameter supported in session */
    else if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
    }
    
    /* is parameter security access allowed ?*/
    else if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
    }

    /* security ok */
    else
    {
        error = DCM_HAVE_NO_ERRORS;
    }
    
    /* no NRC registered */
    if(error == DCM_HAVE_NO_ERRORS)
    {   
        error = DCM_HAVE_ERRORS;
      
        /* RID supported SubFun */
        if(gMsgContextType.reqData[1] == START_ROUTINE)
        {
            /*  Is RID supports sub-function  */
            if(DcmDspRoutineIdentifierTable_31[postion].DcmDspStartRoutineFnc == DCM_NULL)
            { 
                 /*  sub not cfg to support  */
                 DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
            }
            else
            {
                /*  if req lenght valid  */
                /*  If routine control signal is fixed,check message format  */
                if(DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineFixedLength == 1u)
                {
                    if(gMsgContextType.reqDataLen != (DCM_REQUIRE_DATA_LENGTH_4_BYTE + 
                    DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.DcmDspStartRoutineIn.DcmDspRoutineSignalLength))
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                    }
                    else
                    {  
                       /*  lenght valid  */
                        error = DCM_HAVE_NO_ERRORS;
                    }
                }
                else
                {
                    /*  lenght valid  */
                    error = DCM_HAVE_NO_ERRORS;
                }
                if(error == DCM_HAVE_NO_ERRORS)
                { 
                    error = DCM_HAVE_ERRORS;
                    
                    /*  0x31's Sub fun is found  in DsdInternal_UdsSidDispatcher  */
                    if(gSubServicePosition != INVALID_SUB_SERVICE_POSITION)
                    {    
                        /* Sub-Function Exection state check */
                        /* is  Sub-Function supported in session  */
                        /* all condication have checked and no error*/
                        /* Check service id adressing mode  */
                        if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) 
                        && (service.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                        {
                            /* AddressingMode is not supportted */
                            DsdInternal_ProcessingDoneNoResponse();
                        }
                        else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) 
                        && ((service.AddressingMode & 0x01) == 0))
                        {
                            /* AddressingMode is not supportted */
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT); 
                        } 
                                   
                        /* Check sub-function adressing mode */
                        else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) 
                        && (gDcmDsdSubService[gSubServicePosition].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                        {
                            DsdInternal_ProcessingDoneNoResponse();
                        }
                        else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) 
                        && ((gDcmDsdSubService[gSubServicePosition].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                        {
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT); 
                        }
                           
                        /* Check RID adressing mode */                                                                    
                        else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX)
                        && (DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.RoutineAuthorization.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                        {
                            DsdInternal_ProcessingDoneNoResponse();
                        }
                        else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) 
                        && ((DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.RoutineAuthorization.AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                        {
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT); 
                        }
                        else            
                        {
                            gRountineControlDidHandle = postion;

                            /* reset  sequence */
                            gRountineControlSequence[postion] = 0;
                            gSubFunctionHandler = DcmDspRoutineIdentifierTable_31[postion].DcmDspStartRoutineFnc;
                            (*gSubFunctionHandler)(&gMsgContextType); 
                        }
                    }
                    else
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
                    }
                }
                else
                {
                }
            }
        }
        else if(gMsgContextType.reqData[1] == STOP_ROUTINE)
        { 
             /*  If RID supports sub-function  */
             if(DcmDspRoutineIdentifierTable_31[postion].DcmDspStopRoutineFnc == DCM_NULL)
             { 
                 /*sub = 02,sub not cfg to support */
                 DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
             }
             else
             { 
                 /*is req lenght valid?*/
                 /* If routine control signal is fixed,check message format */
                 if(DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineFixedLength == 1u)
                 {
                     if(gMsgContextType.reqDataLen != (DCM_REQUIRE_DATA_LENGTH_4_BYTE + 
                     DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.DcmDspRoutineStopIn.DcmDspRoutineSignalLength))
                     {
                         DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                     }
                     else
                     {
                         /*  lenght valid  */
                         error = DCM_HAVE_NO_ERRORS; 
                     }
                 }
                 else
                 {
                     /*  lenght valid  */
                     error = DCM_HAVE_NO_ERRORS; 
                 }
                 if(error == DCM_HAVE_NO_ERRORS)
                 {      
                     /*  0x31's Sub fun is found  in DsdInternal_UdsSidDispatcher  */
                     if(gSubServicePosition != INVALID_SUB_SERVICE_POSITION)
                     {  
                         /* Sub-Function Exection state check */
                         /* is  Sub-Function supported in session  */
                         /* all condication have checked and no error*/
                         /* Check service id adressing mode  */
                         if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) 
                            && (service.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                         {
                             /* AddressingMode is not supportted */
                             DsdInternal_ProcessingDoneNoResponse();
                         }
                         else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) 
                                && ((service.AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                         {
                             /* AddressingMode is not supportted */
                             DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);    
                         }            
                         /* Check sub-function adressing mode */
                         else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) 
                                && (gDcmDsdSubService[gSubServicePosition].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                         {
                             DsdInternal_ProcessingDoneNoResponse();
                         }
                         else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) 
                                && ((gDcmDsdSubService[gSubServicePosition].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                         {
                             DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT); 
                         }   
                         /* Check RID adressing mode */                                                                    
                         else if((gMsgContextType.msgAddInfo.reqType 
                         == UDS_FUNCTIONAL_ON_CAN_RX) && (DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.RoutineAuthorization.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                         {
                             DsdInternal_ProcessingDoneNoResponse();
                         }
                         else if((gMsgContextType.msgAddInfo.reqType 
                         == UDS_PHYSICAL_ON_CAN_RX) && ((DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.RoutineAuthorization.AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                         {
                             DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT); 
                         } 
                         else if(((gRountineControlSequence[postion]&HAVE_STARTED_ROUNTINE) == 0) && ((gRountineControlSequence[postion]&HAVE_REQ_ROUNTINE_RESULT) == 0)) 
                         {
                             DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTSEQUENCEERROR);
                         } 
                         else
                         {
                             gRountineControlDidHandle = postion;
                             gSubFunctionHandler = DcmDspRoutineIdentifierTable_31[postion].DcmDspStopRoutineFnc;
                             (*gSubFunctionHandler)(&gMsgContextType);

                             if((gRountineControlSequence[postion] & HAVE_STOPPED_ROUNTINE) != 0)
                             {
                                 gRountineControlSequence[postion] &= (uint8)(~((uint8)HAVE_STARTED_ROUNTINE));
                                 if(gRountineControlSequence[postion] == (uint8)(HAVE_REQ_ROUNTINE_RESULT|HAVE_STOPPED_ROUNTINE))
                                 {
                                     gRountineControlSequence[postion] = 0;
                                 }
                             }
                         }
                     }
                     else
                     {       
                         /* Not cfg in gDcmDsdSubService Table */
                         DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
                     }
                 }
                 else
                 {
                 }
             }
        }
        else if(gMsgContextType.reqData[1] == REQUEST_ROUTINE_RESULT)  
        { 
            /*  If RID supports sub-function  */
            if(DcmDspRoutineIdentifierTable_31[postion].DcmDspRequestResultsRoutineFnc == DCM_NULL)
            { 
                /*  sub = 03,sub not cfg to support  */
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
            }
            else
            { 
               /*  if req lenght valid  */
               /*  If routine control signal is fixed,check message format */
               if(DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineFixedLength == 1u)
               {             
                   if(gMsgContextType.reqDataLen != DCM_REQUIRE_DATA_LENGTH_4_BYTE)
                   {
                       DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                   }
                   else
                   {
                       /*  lenght valid  */
                       error = DCM_HAVE_NO_ERRORS;
                   }
               }
               else
               {
                   /*  lenght valid  */
                   error = DCM_HAVE_NO_ERRORS;
               }
               if(error == DCM_HAVE_NO_ERRORS)
               { 
                   /*  0x31's Sub fun is found  in DsdInternal_UdsSidDispatcher  */
                   if(gSubServicePosition != 0xFF)
                   {  
                       /*  all condication have checked and no error  */
                       /*  Check service id adressing mode  */
                       if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) && (service.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                       {
                           /* AddressingMode is not supportted */
                           DsdInternal_ProcessingDoneNoResponse();
                       }
                       else if((gMsgContextType.msgAddInfo.reqType 
                       == UDS_PHYSICAL_ON_CAN_RX) && ((service.AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                       {
                           /* AddressingMode is not supportted */
                           DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);    
                       } 
                                
                       /* Check sub-function adressing mode */
                       else if((gMsgContextType.msgAddInfo.reqType 
                       == UDS_FUNCTIONAL_ON_CAN_RX) && (gDcmDsdSubService[gSubServicePosition].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                       {
                           DsdInternal_ProcessingDoneNoResponse();
                       }
                       else if((gMsgContextType.msgAddInfo.reqType 
                       == UDS_PHYSICAL_ON_CAN_RX) && ((gDcmDsdSubService[gSubServicePosition].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                       {
                           DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT); 
                       }
                        
                       /* Check RID adressing mode */                                    
                       else if((gMsgContextType.msgAddInfo.reqType 
                       == UDS_FUNCTIONAL_ON_CAN_RX) && (DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.RoutineAuthorization.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                       {
                           DsdInternal_ProcessingDoneNoResponse();
                       }
                       else if((gMsgContextType.msgAddInfo.reqType 
                       == UDS_PHYSICAL_ON_CAN_RX) && ((DcmDspRoutineIdentifierTable_31[postion].DcmDspRoutineInfoRef.RoutineAuthorization.AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                       {
                           DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT); 
                       }
                       else if(((gRountineControlSequence[postion]&HAVE_STARTED_ROUNTINE) == 0) && 
                       ((gRountineControlSequence[postion]&HAVE_STOPPED_ROUNTINE) == 0)) 
                       {
                           DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTSEQUENCEERROR);
                       }
                       else
                       {
                           gRountineControlDidHandle = postion;
                           gSubFunctionHandler = DcmDspRoutineIdentifierTable_31[postion].DcmDspRequestResultsRoutineFnc;
                           (*gSubFunctionHandler)(&gMsgContextType);

                           /* (HAVE_REQ_ROUNTINE_RESULT|HAVE_STOPPED_ROUNTINE|HAVE_STARTED_ROUNTINE)) */
                           if((gRountineControlSequence[postion] & HAVE_REQ_ROUNTINE_RESULT) != 0)
                           {
                               gRountineControlSequence[postion] &= (uint8)(~((uint8)HAVE_STARTED_ROUNTINE));
                               if(gRountineControlSequence[postion] == (uint8)(HAVE_REQ_ROUNTINE_RESULT|HAVE_STOPPED_ROUNTINE))
                               {
                                   gRountineControlSequence[postion] = 0;
                               }
                           }
                       }
                   }
                   else
                   {  
                       DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
                   }
               }
               else
               {
               }
            }
        }
        else
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
        }
    }
    else
    {
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_WriteMemoryByAddress
*                
* Description:   Implementation of Uds servie 0x3D
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_3D_ENABLED==STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_WriteMemoryByAddress
(
    void
)
{
    uint8 securitylevel = 0;
    uint8 sessiontype = 0;
    uint8 addressFormat = 0;
    uint8 sizeFormat = 0;
    uint8 i = 0;
    uint32 startAddress = 0;
    uint32 memorySize = 0;
    uint8  addresssegment = 0;
    uint8 error = E_OK;
    /*uint8 Possion = 0;*/
    addressFormat = gMsgContextType.reqData[1] & DCM_SERVICE_3D_GET_MEMORY_AND_ADDRESS_BYTE_LOW_NIBBLE;
    sizeFormat = gMsgContextType.reqData[1] >> DCM_SERVICE_3D_GET_MEMORY_AND_ADDRESS_BYTE_HIGH_NIBBLE;


    /* Calculate start address */
    for(i=0;i<gMemoryAddressFormat;i++)
    {
        startAddress |= (uint32)(gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_2_BYTE+i])<<(uint8)(DCM_AMPLIFY_8_TIMES*(gMemoryAddressFormat-1-i));        
    }
    /* Calculate size  */
    for(i=0;i<gMemorySizeFormat;i++)
    {
        memorySize |= (uint32)(gMsgContextType.reqData[DCM_REQUIRE_DATA_LENGTH_2_BYTE+gMemoryAddressFormat+i])<<(uint8)(DCM_AMPLIFY_8_TIMES*(gMemorySizeFormat-1-i)); 
    }

    /* Check the scope of the address in requested message */
    for(i=0;i<NUMBER_OF_WRITE_ADDRESS_SEG;i++)
    {

        if((startAddress>=gDcmDspWriteMemoryRangeInfo_3D[i].DcmDspWriteMemoryRangeLow)&&((startAddress+memorySize)<=gDcmDspWriteMemoryRangeInfo_3D[i].DcmDspWriteMemoryRangeHigh))
        {
            addresssegment = i;
            i = NUMBER_OF_WRITE_ADDRESS_SEG;
        }
        else
        {
            if(i==(NUMBER_OF_WRITE_ADDRESS_SEG - 1))
            {
                error=E_NOT_OK;
            }
        }
    }

    /* Check addressAndLengthFormatIdentifier */
    /* is parametre supported ?*/
    if((addressFormat!=gMemoryAddressFormat) || (sizeFormat!=gMemorySizeFormat) || (error ==E_NOT_OK))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
    }
    /* Check message format */ 
    /* is request length valid ? */ 
    else if(gMsgContextType.reqDataLen != (DCM_REQUIRE_DATA_LENGTH_2_BYTE + gMemoryAddressFormat+gMemorySizeFormat+memorySize))
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }        
    else
    {         
        /*error =E_NOT_OK;*/
        /* parameter execution state check */
        sessiontype=gDcmDspWriteMemoryRangeInfo_3D[addresssegment].DcmDspWriteMemoryRangeSessionRef;
        securitylevel=gDcmDspWriteMemoryRangeInfo_3D[addresssegment].DcmDspWriteMemoryRangeSecurityLevelRef;

        /*is parameter supported in session ?*/
        if((sessiontype & gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
        }

        /* Check security access conditon */
        /*is parameter security access allowed ?*/
        else if((securitylevel & gSecLevelType )== DCM_SECURITY_LEVEL_NOT_CORRECT)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
        }

        /* is service security access allowed? */
        else if((service.ServiceTable.DcmDsdSidTabSecurityLevelRef & gSecLevelType) == 0)
        {
            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
        } 
        else
        {
            if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) && (service.AddressingMode - gMsgContextType.msgAddInfo.reqType < 1u))
            {
                /* AddressingMode is not supportted */
                DsdInternal_ProcessingDoneNoResponse();
            } 
            else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) && ((service.AddressingMode & PHYSICAL_REQ_SUPPORTED)==DCM_REQUEST_ADDRESS_NOT_CORRECT))
            {
                /* AddressingMode is not supportted */
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
            }                     
            /* Check par adressing mode */
            else if((gMsgContextType.msgAddInfo.reqType==UDS_FUNCTIONAL_ON_CAN_RX) && (gDcmDspWriteMemoryRangeInfo_3D[addresssegment].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
            {
                DsdInternal_ProcessingDoneNoResponse();
            }
            else if((gMsgContextType.msgAddInfo.reqType==UDS_PHYSICAL_ON_CAN_RX) && ((gDcmDspWriteMemoryRangeInfo_3D[addresssegment].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
            }
            else
            {
                gSubFunctionHandler=App_WriteAddress;
                (void)(*gSubFunctionHandler)(&gMsgContextType);
            }
        }
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_TesterPresent
*                
* Description:   Implementation of Uds servie 0x3E
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_3E_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_TesterPresent
(
    void
)
{
    /* Check message format */
    if (gMsgContextType.reqDataLen != DCM_SERVICE_3E_REQUIRE_DATA_LENGTH)
    {
        DsdInternal_SetNegResponse(&gMsgContextType, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    else
    {
        if (GetSuppressPosResponseBit() == 0) /* If positive response is supressed, dont' need make it */
        {
            gMsgContextType.resDataLen = DCM_SERVICE_3E_RESPONSE_DATA_LENGTH;
            DsdInternal_ProcessingDone(&gMsgContextType);
        }
        else
        {
            DsdInternal_ProcessingDoneNoResponse();
        }
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_ControlDTCSetting
*                
* Description:   Implementation of Uds servie 0x85
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_85_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ControlDTCSetting
(
    void
)
{
    DsdInternal_10_11_85();
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DspInternal_Uds_ResponseOnEvent
*                
* Description:   Implementation of Uds servie 0x86
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_86_ENABLED == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DspInternal_Uds_ResponseOnEvent
(
    void
)
{
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_SetNegResponse
*                
* Description:   Set negative response
*                             
* Inputs:        pMsgContext:Pointer pointed to the buffer having NR
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE) DsdInternal_SetNegResponse
(
    P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext,
    uint8 ErrorCode 
)
{
    
    if(gNegativeResponseCode == DCM_E_POSITIVERESPONSE)
    {
        SetNegativeResponseCode(ErrorCode);
    }
    else
    {
    }
    gNegativeResponseBuffer[0] = DCM_NEGATIVE_RES_SERVICE_ID;
    gNegativeResponseBuffer[1] = gUDS_Physical_DiagBuffer[DCM_SID_POSITION];
    gNegativeResponseBuffer[2] = gNegativeResponseCode;

    /* Make global pointer point to gNegativeResponseBuffer */
    pMsgContext->resData = gNegativeResponseBuffer;
    pMsgContext->resDataLen = NEG_RESP_BUFFER_SIZE;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_ProcessingDone
*                
* Description:   Preocessing the response result of each Uds service.Transmit 
*                negative or positive respose(if not suppressed)
*                             
* Inputs:        pMsgContext: Pointer pointed to the buffer having response
*                
* Outputs:       None
* 
* Limitations:   None
是处理诊断请求的响应，并根据不同的条件决定是否发送正响应或负响应
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE) DsdInternal_ProcessingDone
(
    P2VAR(Dcm_MsgContextType,AUTOMATIC,DCM_APPL_DATA) pMsgContext
)
{
    PduInfoType temp = { DCM_NULL,0 }; //初始化一个 PduInfoType 结构体
    Std_ReturnType result = E_OK;  //初始化返回结果为成功

    if((gDiagState&DIAG_UDS_RCRP_DONE) != DIAG_UDS_RCRP_DONE)
    {
        /* Suppress positive response or fobidden 0x11 and 0x12 NRC under the 
           condition of functionally-addressing */
           // 如果诊断状态不是 RCRP_DONE
        if(((GetSuppressPosResponseBit() == 1)&& (gNegativeResponseCode == DCM_E_POSITIVERESPONSE)) //抑制正响应&请求成功
          || (gMsgContextType.resDataLen == 0))
        {
            // 如果抑制了正响应或响应数据长度为 0
            /* Start S3 Timer */ 
            if(gSesCtrlType != DCM_SESSION_DEFAULT)
            {
                 // 如果当前会话不是默认会话
                gS3ServerTimerStartFlag = DCM_FLAG_ACTIVE; // 启动 S3 定时器
                Set_S3_Server_Timer(gDcmDspNonDefaultSessionS3Server);  // 设置 S3 定时器 5000ms
            }
            else
            {
            }
            Dsd_InteralInit();  // 初始化内部状态
        }
        else
        {
            /* Check whether the response length is over the related buffer's 
             * buffersize.
             * Response is always put in physical buffer 
             */
            if(gMsgContextType.resDataLen > UDS_PHYS_BUFFER_SIZE) 
            {
                DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE); // 设置请求超出范围的负响应
            }
            else
            {
                /* Under the situation of functionally-addressing,not send 0x31 NR */
                // 在功能寻址且没有子功能的情况下，发送 0x31 负响应
                if((gMsgContextType.msgAddInfo.reqType == 1)
                && (gNegativeResponseCode == DCM_E_REQUESTOUTOFRANGE) 
                && (gDcm_CurrentServiceSubfuncAvail == UDS_SERVICE_WITHOUT_SUB_FUNCTION)) 
                {
                    /* Start S3 Timer */
                    // 如果当前会话不是默认会话
                    if(gSesCtrlType != DCM_SESSION_DEFAULT)
                    {
                        /*在返回 NRC 31 后重置 S3 定时器是为了确保系统能够继续监控请求的处理状态，
                        并在必要时采取进一步的措施。通过重置定时器，可以防止无限等待，提高系统的稳定性和可靠性。*/
                        gS3ServerTimerStartFlag = DCM_FLAG_ACTIVE;
                        Set_S3_Server_Timer(gDcmDspNonDefaultSessionS3Server);
                    }
                    else
                    {
                    
                    }
                    Dsd_InteralInit();
                }
                else
                {
                }
            }
            if((gNegativeResponseCode != DCM_E_POSITIVERESPONSE) 
            && (gNegativeResponseCode != DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING))             
            {
                #if(DCM_SERVICE_22_COMBINED_DID == STD_OFF)
                #else
                pMsgContext->resData = gNegativeResponseBuffer;  //// 设置响应数据为负响应缓冲区
                pMsgContext->resDataLen = NEG_RESP_BUFFER_SIZE;  // 设置响应数据的第一个字节
                #endif 
            }
            #if(DCM_SERVICE_2A_ENABLED == STD_ON)  
            else if((gDiagState & DIAG_UDS_PERIODIC_TX) == DIAG_UDS_PERIODIC_TX)
            {
                pMsgContext->resData = gPeriodicResponseBuffer;
            }
            #endif
            else  //回应NRC=0X78,且请求处理成功
            {
                pMsgContext->resData = gUDS_Physical_DiagBuffer; // 后续的响应数据将被存储在 gUDS_Physical_DiagBuffer 中
                pMsgContext->resData[0] = pMsgContext->reqData[0] + DCM_SID_OFF_SET;  // 正响应sid+40
            }
            
            /* Positive response or negative response */
            temp.SduDataPtr = pMsgContext->resData;  // 设置数据指针
            temp.SduLength  = (uint16)pMsgContext->resDataLen;  // 设置数据长度
            gResponseLength = (uint16)pMsgContext->resDataLen; // 设置全局响应长度
            if(pMsgContext->dcmRxPduId != DCM_INVALID_HANDLE_OR_ID)
            {
                result = DcmTransmit(pMsgContext->dcmRxPduId,&temp);  // 发送响应
            }
            else
            {
            }
            
            /* FF has been correctly transmitted */
            // 如果传输成功
            if(result == E_OK)  
            {
                gDcm_RepeatTransmitFlag = DCM_FLAG_DISACTIVE;  // 停止重复传输
   
                /* Stop P2 Timer */ 
                gP2ServerTimerStartFlag = DCM_FLAG_DISACTIVE;  // 停止 P2 定时器
                Clr_P2_Server_Timer();
                ClrSuppressPosResponseBit();
                gFunctionHandler = DCM_NULL;
                gSubFunctionHandler = DCM_NULL;
                gSubFunctionPostHandler = DCM_NULL;
            }
            else
            {
                gDcm_RepeatTransmitFlag = DCM_FLAG_ACTIVE;//传输错误重复传输
                gFunctionHandler = DCM_NULL;
                gSubFunctionHandler = DCM_NULL;
            }
            #if(DCM_SERVICE_22_COMBINED_DID == STD_OFF)
            ClrNegativeResponseCode();  // 清除负响应代码
            #endif
        }
        Reset_Max_Number_Of_RCRRP();   // 重置最大 RCRP 数量
    }
    else
    {
        Clr_ActiveProtocol();  // 清除活动协议
        Reset_PduId();  // 重置 PDU ID
        Clr_DiagState(DIAG_UDS_RCRP_DONE);  // 清除诊断状态
        gFunctionHandler = DCM_NULL;  // 清除函数处理器
        gSubFunctionHandler = DCM_NULL;  // 清除子函数处理器
        gSubFunctionPostHandler = DCM_NULL;  // 清除子函数后处理器
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_ProcessingDoneNoResponse
*                
* Description:   The processing of request has been finished, but do not send
*                any response including positive or negative for other reason 
*                but not be set of SuppressPosResponseBit.
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE) DsdInternal_ProcessingDoneNoResponse
(
    void
)
{
    gMsgContextType.resDataLen = 0;//0 表示没有响应数据需要发送
    DsdInternal_ProcessingDone(&gMsgContextType);
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_DidProcessingDone
*                
* Description:   This function will be call by 0x22 service's callbackfunction
*                once one of the did processing is finished when 
*                DCM_SERVICE_22_COMBINED_DID is enabled.
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_22_COMBINED_DID == STD_ON)
FUNC(void,DCM_PUBLIC_CODE) DsdInternal_DidProcessingDone
(
    void
)
{
    if(gDidPosition != DCM_SERVICE_22_INVALID_DID_POSITION)
    {
        gDidResData[0] = (uint8)(DidList[gDidPosition].Did >> DCM_SERVICE_22_RIGHT_ROLLING_8_BIT);
        gDidResData[1] = (uint8)(DidList[gDidPosition].Did);
        gDidResData += (gDcmDsdSubService_22[DidList[gDidPosition].DidPosition].DcmDspDataSize 
                       + DCM_RESPONSE_DATA_LENGTH_2_BYTE);
        gDidResDataLen += (gDcmDsdSubService_22[DidList[gDidPosition].DidPosition].DcmDspDataSize 
                          + DCM_RESPONSE_DATA_LENGTH_2_BYTE);
        gMsgContextType.resData = gDidResData;
        gMsgContextType.resDataLen = gDidResDataLen;
        gDidPosition++;

        if(gDidPosition < DidCounter)
        {
            gSubFunctionHandler = gDcmDsdSubService_22[DidList[gDidPosition].DidPosition].DcmDspDataReadFnc;
        }
        else
        {
            gMsgContextType.resData = gUDS_Physical_DiagBuffer; 
            gMsgContextType.resDataLen = gDidResDataLen;
            gDidPosition = 0;
            DidCounter = 0;
            gDidResData = 0;
            gDidResDataLen = 0;
            DsdInternal_ProcessingDone(&gMsgContextType);
        }
    }
    else
    {
        DsdInternal_ProcessingDone(&gMsgContextType);
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: FindSid
*                
* Description:   The timer processing function
*                             
* Inputs:        RequestSid:service Id in requested message
*                
* Outputs:       result: The service Id's positon in gCurrentSidMap
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(uint8,DCM_PRIVATE_CODE) FindUdsSid
(
    uint8 RequestSid
)
{
    uint8 result = DCM_INVALID_HANDLE_OR_ID;
    uint8 offset = 0;
    if(((RequestSid & THE_OFFSET_DATA) == 0) 
      && (RequestSid  <= THE_LARGEST_SERVICE_ID))
    {
        if((RequestSid & SUPPRESS_POS_RSP_BYTE) != 0)
        {
          offset = THE_OFFSET_DATA;
        }
        else
        {
        }
        result = gCurrentSidMap[RequestSid-offset];
    }
    else
    {
    }
    return result;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_Dispatcher
*                
* Description:   Uds service dispatcher.
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE) DsdInternal_UdsSidDispatcher
(
    void
)
{
    uint8 i = 0;
    uint8 sid = 0;
    DcmDsdServiceTable service = { 0,{ DCM_NULL, 0, 0, 0 }, 0, 0, 0};
    uint8 securitylevel = 0;
    uint8 sessiontype = 0;
    uint8 error = DCM_HAVE_ERRORS;
    #if(DCM_GENERAL_DIAGNOSTIC_REQUEST_CTL == STD_ON)
    Std_ReturnType result = E_OK;
    #endif
    
    if((gDiagState&DIAG_UDS_INDICATION) == DIAG_UDS_INDICATION)
    {
        Clr_DiagState(DIAG_UDS_INDICATION);
        gSubServicePosition = 0;

        #if(DCM_GENERAL_DIAGNOSTIC_REQUEST_CTL == STD_ON)
        result = App_DiagnosticActive();
        if(result != E_OK) 
        {
            CanTp_Init(DCM_NULL);  
        }
        else
        {
        #endif

            Set_DiagState(DIAG_UDS_PROCESSING);
            sid = FindUdsSid(gMsgContextType.reqData[0]);
            service = gCurrentDcmDsdServiceTable[sid];
            
            /* If it is functional request, check whether it is supported */
            if(sid < gCurentDcmDsdServiceTableSize)
            {
                /* Check adressing mode */                                      
                if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX)
                && (service.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1))
                {
                    DsdInternal_ProcessingDoneNoResponse();
                    /* Service is not supported under functional request,but do not
                       send 0x11 negative response */
                }
                else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) && ((service.AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                {
                    /* Service is not supported under physical request, send negative response */ 
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SERVICENOTSUPPORTED); 
                }
                else
                {
                    /* Check minimum request length */
                    if(gMsgContextType.reqDataLen < DCM_REQUIRE_DATA_LENGTH_2_BYTE)
                    {
                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                    }
                    else
                    {
                        sessiontype = service.ServiceTable.DcmDsdSidTabSessionLevelRef;
                        securitylevel = service.ServiceTable.DcmDsdSidTabSecurityLevelRef;
                        
                        /* Check service session condition */
                        if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
                        { 

            			/*D50 ʹ��,�������󣬻Ựģʽ����ȷ������Ӧ����*/
				#if 1
                        	if(gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX)
                          {
                            DsdInternal_ProcessingDoneNoResponse();
                          }
				                  else
				#endif
				                  {
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION);
				                  }
                        }
                        
                        /* Check service security access conditon */
                        else if((securitylevel != 0) && ((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT))
                        {
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                        }
                        else
                        {
                            /* If the requested service has sub-function */
                            if((service.ServiceTable.DcmDsdSidTabSubfuncAvail == 1))
                            {
                                /* Set suprresPostResponseBit if needed */
                                gMsgContextType.msgAddInfo.suppressPosResponse = (gMsgContextType.reqData[1] & SETSUPPRESSPOSRESPONSEBITMASK) >> RIGHT_ROLLING_SUPPRESS_POS_RSP_BIT;
                                gMsgContextType.reqData[1] &= ~SETSUPPRESSPOSRESPONSEBITMASK;
                                
                                /* Check if the sub-function is defined in the gDcmDsdSubService */
                                for(i = service.FirstSubService; i < (service.LastSubService+1); i++)
                                {
                                    if((gMsgContextType.reqData[1] == gDcmDsdSubService[i].DcmDsdSubServiceId))
                                    {
                                        error = DCM_HAVE_NO_ERRORS;
                                        gSubServicePosition = i;
                                        i = service.LastSubService + 1; 
                                    }
                                    else
                                    {
                                    }
                                }
                                if(error == DCM_HAVE_ERRORS)
                                {
                                    /* Under the situation of functionally-addressing,not send 0x12 NR */
                                    if(gMsgContextType.msgAddInfo.reqType == 0)
                                    {
                                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
                                    }
                                    else
                                    {
                                        DsdInternal_ProcessingDoneNoResponse();
                                    }
                                }
                                else
                                {
                                    sessiontype = gDcmDsdSubService[gSubServicePosition].DcmDsdSubServiceSessionLevelRef;
                                    securitylevel = gDcmDsdSubService[gSubServicePosition].DcmDsdSubServiceSecurityLevelRef;
                                    
                                    /* Check if the sub-function supported */
                                    if(gDcmDsdSubService[gSubServicePosition].DcmDsdSubServiceIdSupported != SUB_FUNCTION_SUPPORTED)
                                    {
                                        /* Under the situation of functionally-addressing,not send 0x12 NR */
                                        if(gMsgContextType.msgAddInfo.reqType == 0) 
                                        {
                                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED);
                                        }
                                        else
                                        {
                                            DsdInternal_ProcessingDoneNoResponse();
                                        }
                                    }
                                    
                                    /* Check sub-function adressing mode */
                                    else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) && (gDcmDsdSubService[gSubServicePosition].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1))
                                    {
                                        DsdInternal_ProcessingDoneNoResponse();
                                        /* sub-function is not supported under functional request,but do not send 0x12 negative response */
                                    }
                                    else if((gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX) && ((gDcmDsdSubService[gSubServicePosition].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                                    {
                                        /* sub-function is not supported under physical request, send negative response */ 
                                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTED); 
                                    }
                                    
                                    /* Check sub-function session conditon */
                                    else if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT) 
                                    {
                                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION);
                                    }
                                    
                                    /* Check sub-function security conditon */
                                    else if((securitylevel != 0) && ((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT))
                                    {
                                        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                                    }
                                    else
                                    {
                                        /* Call corresponding service function */                                                    
                                        
                                        gDcm_CurrentServiceSubfuncAvail = UDS_SERVICE_WITH_SUB_FUNCTION;
                                        gFunctionHandler = service.ServiceTable.DcmDsdSidTabFnc;
                                        (*gFunctionHandler)(); 
                                    }
                                }
                            }
                            else
                            {
                                /* Call corresponding service function */
                                gDcm_CurrentServiceSubfuncAvail = UDS_SERVICE_WITHOUT_SUB_FUNCTION;
                                gFunctionHandler = service.ServiceTable.DcmDsdSidTabFnc;
                                (*gFunctionHandler)();
                            }
                        }
                    }
                }
            }
            else
            {
                #if(DCM_COSTOMIZED_SERVCIE_SUPPORTED == STD_ON)
                gSubFunctionHandler = App_CustomizedServcie;
                (void)(*gSubFunctionHandler)(&gMsgContextType);
                #else
                if(gMsgContextType.msgAddInfo.reqType == UDS_PHYSICAL_ON_CAN_RX)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SERVICENOTSUPPORTED);
                }
                else
                {
                    /* Under the situation of functionally-addressing,not send 0x11 NR */
                    DsdInternal_ProcessingDoneNoResponse();
                }
                #endif
            }
        #if(DCM_GENERAL_DIAGNOSTIC_REQUEST_CTL == STD_ON)
        }
        #endif
        if((gNegativeResponseCode != DCM_E_POSITIVERESPONSE) 
          &&(gNegativeResponseCode != DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING))
        {
            DsdInternal_ProcessingDone(&gMsgContextType);
        }
        else
        {
            
        }
    }
    else
    {
        
    }
}

#define DCM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

#define DCM_START_SEC_PRIVATE_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_RepeatTransmitCheck
*                
* Description:   Uds service repeat transmit
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE) DsdInternal_RepeatTransmitCheck
(
    void
)
{
    PduInfoType temp = { DCM_NULL, 0};
    Std_ReturnType result = 0; 
    
    if(gDcm_RepeatTransmitFlag == DCM_FLAG_ACTIVE)
    {
        if((gP2ServerTimer > 0) || ((gP2ServerTimer == 0)  
           && ((gDiagState&DIAG_UDS_RCRP) == DIAG_UDS_RCRP)))
      
        {
            temp.SduDataPtr = gMsgContextType.resData;
            temp.SduLength = (uint16)gMsgContextType.resDataLen;
            gResponseLength = (uint16)gMsgContextType.resDataLen;
            
            if(gMsgContextType.dcmRxPduId != DCM_INVALID_HANDLE_OR_ID)
            {
                result = DcmTransmit(gMsgContextType.dcmRxPduId,&temp);
            }
            else
            {
                result = E_NOT_OK;
            }
            
            /* FF has been correctly transmitted */
            if(result == E_OK)
            {
                gDcm_RepeatTransmitFlag = DCM_FLAG_DISACTIVE;

                /* Stop P2 Timrer */ 
                gP2ServerTimerStartFlag = DCM_FLAG_DISACTIVE;
                Clr_P2_Server_Timer();
                gFunctionHandler = DCM_NULL;
                gSubFunctionHandler = DCM_NULL;
                gSubFunctionPostHandler = DCM_NULL;
            }
            else
            {
                gDcm_RepeatTransmitFlag = DCM_FLAG_ACTIVE;
            }
        }
        else
        {                                                                                                    
            gDcm_RepeatTransmitFlag = DCM_FLAG_DISACTIVE;
            Dsd_InteralInit();
        }
    }
    else
    {
    }
}
#if(DCM_SERVICE_2A_ENABLED == STD_ON)

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_Periodic_Transmit
*                
* Description:   Uds service 2A periodic transmit
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE) DsdInternal_Periodic_Transmit
(
    void
)
{   
    uint8 i = 0;
    
    if(gDiagState == DIAG_IDLE)
    {
        for(i = 0; i < PeriodicDidCounter; i++)
        {
            if(gPeriodicResponseTimer[PeriodicDidList[CurrentPeriodicDidIndx].DidPosition] == 0)
            { 
                Set_DiagState(DIAG_UDS_PERIODIC_TX);
                
                /* Type1,use the same CAN id as normal request message, 
                change to types2 may be better */   
                gMsgContextType.dcmRxPduId = UDS_FUNCTIONAL_ON_CAN_RX; 
                gDcm_CurrentServiceSubfuncAvail == UDS_SERVICE_WITHOUT_SUB_FUNCTION;
                gMsgContextType.resData = gPeriodicResponseBuffer;
                gMsgContextType.resData[0] = DCM_SERVICE_2A_POSITIVE_RESPONSE;
                gMsgContextType.resData[1]
                = gDcmDsdSubService_2A[PeriodicDidList[CurrentPeriodicDidIndx].DidPosition].DcmDspDidIdentifier;
                gMsgContextType.resDataLen
                = 2u + gDcmDsdSubService_2A[PeriodicDidList[CurrentPeriodicDidIndx].DidPosition].DcmDspDataSize;
                gDidPosition = 0xFF;
                gSubFunctionHandler
                = gDcmDsdSubService_2A[PeriodicDidList[CurrentPeriodicDidIndx].DidPosition].DcmDspDataReadFnc;
                (*gSubFunctionHandler)(&gMsgContextType);
                gPeriodicResponseTimer[PeriodicDidList[CurrentPeriodicDidIndx].DidPosition]
                = gPeriodicResponseRate[PeriodicDidList[CurrentPeriodicDidIndx].DidPosition];
                i = PeriodicDidCounter;
                CurrentPeriodicDidIndx++;
                if(CurrentPeriodicDidIndx == PeriodicDidCounter)
                {
                    CurrentPeriodicDidIndx = 0;
                }
                else
                {    
                }
            } 
            else
            {
                if(CurrentPeriodicDidIndx + 1u == PeriodicDidCounter)
                {
                    CurrentPeriodicDidIndx = 0;
                }
                else
                {
                    CurrentPeriodicDidIndx++;
                }
            }

        }
    }
    else
    {
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_DidDispatcher
*                
* Description:   Implementation of Uds service 0x22
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_22_ENABLED == STD_ON)
STATIC FUNC(void,DCM_PRIVATE_CODE) DsdInternal_DidDispatcher
(
    void
)
{
    uint16 tempdid = 0;
    uint8* tempreqdata = DCM_NULL;
    uint32 i = 0;
    uint16 j = 0;
   /// uint16 k = 0;
    uint8  securitylevel = 0;
    uint8  sessiontype = 0;
    uint8 SupportedDIDNum = 0;

    tempreqdata = gMsgContextType.reqData + 1u;
    gDidPosition = 0;
    DidCounter = 0;
    
    /* Check whether the request DIDs are supported and record total number of DID */
    /* search for all supported DID and record them */
    for(i = 0; i < gMsgContextType.reqDataLen; i += DCM_REQUIRE_DATA_LENGTH_2_BYTE)
    {
        tempdid = Make16Bit(*tempreqdata,*(tempreqdata + 1u));
        for(j = 0; j < NUMBER_OF_READ_DID; j++)
        {
            if(tempdid == gDcmDsdSubService_22[j].DcmDspDidIdentifier)
            {
                /* Check session type */
                SupportedDIDNum++;
                sessiontype = gDcmDsdSubService_22[j].DcmDspDidReadSessionRef;
                securitylevel = gDcmDsdSubService_22[j].DcmDspDidReadSecurityLevelRef;
                
                /* parameter supported is in session or not*/
                if((sessiontype&gSesCtrlType) == DCM_SESSION_TYPE_NOT_CORRECT)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    j = NUMBER_OF_READ_DID;
                    i = gMsgContextType.reqDataLen;
                }
                
                /* parameter security is allow access or not*/
                #if(DCM_SERVICE_27_ENABLED == STD_ON)
                else if((securitylevel & gSecLevelType) == DCM_SECURITY_LEVEL_NOT_CORRECT)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                    j = NUMBER_OF_READ_DID;
                    i = gMsgContextType.reqDataLen;
                }
                
                /* check if other condication are fullfilled */
                /* check if service security access is allowed */
                else if(service.ServiceTable.DcmDsdSidTabSecurityLevelRef & gSecLevelType == 0)
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_SECURITYACCESSDENIED);
                    j = NUMBER_OF_READ_DID;
                    i = gMsgContextType.reqDataLen;
                }
                #endif
                else if((gMsgContextType.msgAddInfo.reqType 
                == UDS_FUNCTIONAL_ON_CAN_RX) && (service.AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                {
                    /* AddressingMode is not supportted */
                }
                else if((gMsgContextType.msgAddInfo.reqType 
                == UDS_PHYSICAL_ON_CAN_RX) && ((service.AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                {           
                    /* AddressingMode is not supportted */
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    j = NUMBER_OF_READ_DID;
                    i = gMsgContextType.reqDataLen;
                }
                          
                /* Check DID adressing mode */
                else if((gMsgContextType.msgAddInfo.reqType == UDS_FUNCTIONAL_ON_CAN_RX) 
                       && (gDcmDsdSubService_22[j].AddressingMode-gMsgContextType.msgAddInfo.reqType < 1u))
                {
                }
                else if((gMsgContextType.msgAddInfo.reqType
                == UDS_PHYSICAL_ON_CAN_RX) && ((gDcmDsdSubService_22[j].AddressingMode&PHYSICAL_REQ_SUPPORTED) == DCM_REQUEST_ADDRESS_NOT_CORRECT))
                {
                    DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_CONDITIONSNOTCORRECT);
                    j = NUMBER_OF_READ_DID;
                    i = gMsgContextType.reqDataLen;
                }
                else
                {
                    DidList[DidCounter].Did = tempdid;
                    DidList[DidCounter].DidPosition = j;
                    DidCounter++;
                    j = NUMBER_OF_READ_DID; 
                }           
            }
            else 
            {
            }
        }
        tempreqdata += DCM_SERVICE_22_DID_DATA_LENGTH;
        #if (DCM_SERVICE_2C_ENABLED==STD_ON)
        for(j = 0; j < NUMBER_OF_DYN_DID; j++)
        {
            if(tempdid == gDcmDsdSubService_2C[j].DcmDspDidIdentifier)
            {
                for(k = 0; k < 16; k++) 
                {
                    for(i = 0; i < NUMBER_OF_READ_DID; i++) 
                    {
                        if(gDynamicalDidRelatedInfo[j].DynamicalDid == 0) 
                        {
                            gDynamicalDidRelatedInfo[j].DynamicalDid = 0xFF;
                            DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
                            DsdInternal_ProcessingDone(&gMsgContextType);    
                        }
                        else
                        {
                            (gMsgContextType.resData) += 1u; 
                            gMsgContextType.resDataLen = 1u;
                            gDidResDataLen = gMsgContextType.resDataLen;
                            gMsgContextType.resData[0] = (uint8)(gDcmDsdSubService_2C[j].DcmDspDidIdentifier >> 0x08);
                            gMsgContextType.resData[1] = (uint8)gDcmDsdSubService_2C[j].DcmDspDidIdentifier;
                            gSubFunctionHandler = gDcmDsdSubService_22[i].DcmDspDataReadFnc;
                            (void)(*gSubFunctionHandler)(&gMsgContextType);           
                        } 
                    }
                }
            }
            else 
            {
            }
        }
        #endif
    }
    if(gNegativeResponseCode != DCM_E_POSITIVERESPONSE)
    {
    }
    else if(SupportedDIDNum == 0)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_REQUESTOUTOFRANGE);
    }
    
    /* check other state of the supported DID  */ 
    else if(DidCounter == 0)
    {
      DsdInternal_ProcessingDoneNoResponse();
    }
    else
    {
        /* Call realated did processing function */
        (gMsgContextType.resData) += 1u; 
        gMsgContextType.resDataLen = 1u;

        #if(DCM_SERVICE_22_COMBINED_DID == STD_ON)
        gDidResData = gMsgContextType.resData;
        #endif

        gDidResDataLen = gMsgContextType.resDataLen;
        gSubFunctionHandler = gDcmDsdSubService_22[DidList[gDidPosition].DidPosition].DcmDspDataReadFnc;
        (void)(*gSubFunctionHandler)(&gMsgContextType);
    }

}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_10_11_28_85
*                
* Description:   If no error happened, give positive response if needed, and
*                call related application callback function
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_10_11_85
(
    void
)
{
    if(gMsgContextType.reqDataLen != DCM_SERVICE_10_11_85_REQUIRE_DATA_LENGTH)
    {
        DsdInternal_SetNegResponse(&gMsgContextType,DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    }
    else
    {
        DsdInternal_Uds_Common();
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_AccessAttemptNumberDec
*                
* Description:   Decrease allowed security access attempt number
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(DCM_SERVICE_27_ENABLED == STD_ON)
STATIC FUNC(void,DCM_PRIVATE_CODE) DsdInternal_AccessAttemptNumberDec
(
    void
)
{
    if(gSecurityAcessAttempNumber[gCurrentSecurityAccessRequestLevel - 1] != 0)
    {
        gSecurityAcessAttempNumber[gCurrentSecurityAccessRequestLevel - 1]--;
        if(gSecurityAcessAttempNumber[gCurrentSecurityAccessRequestLevel - 1] == 0)
        {
            gSecurityAcessAttempNumberExceeded[gCurrentSecurityAccessRequestLevel - 1] = 1;
        }
    }   
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_UdsRepeatServiceCall
*                
* Description:   Repeat ServiceCall
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE) DsdInternal_UdsRepeatServiceCall(void)
{
    if((gDiagState&DIAG_UDS_PROCESSING) == DIAG_UDS_PROCESSING) 
    {
        if(gSubFunctionHandler != DCM_NULL)
        {
            (void)(*gSubFunctionHandler)(&gMsgContextType);
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
* Function Name: DsdInternal_UdsPostServiceProcessing
*                
* Description:   Postprocessing of request."Post" means positive response has
*                been sent and confirmed.If negative response has been sent,this
*                function will not be called.
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_UdsPostServiceProcessing
(
    void
)
{
    if((gDiagState&DIAG_UDS_PROCESSING) != DIAG_UDS_PROCESSING)
    {
        if(gSubFunctionPostHandler != DCM_NULL)
        {
            (*gSubFunctionPostHandler)(E_OK);
            gSubFunctionPostHandler = DCM_NULL;
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
* Function Name: DsdInternal_SessionTransferPostProcessing
*                
* Description:   Reset security access and rountine control releated  variables 
*               
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_SessionTransferPostProcessing
(
    void
)
{
    uint8 i = 0;

    #if(DCM_SERVICE_31_ENABLED == STD_ON)
    for(i = 0; i < NUMBER_OF_RID; i++)
    {
        gRountineControlSequence[i] = 0u;
    }
    gRountineControlDidHandle = DCM_INVALID_HANDLE_OR_ID;
    #endif

    #if(DCM_SERVICE_27_ENABLED == STD_ON)
    for(i = 0; i < KIND_OF_SECURITY_LEVEL; i++)
    {
        gSecurityAcessSequence[i] = 0;
    }
    gSecLevelType = DCM_SEC_LEV_LOCK;  
    #endif

    if(gSecurityDelayTimeOnBootFlag == DCM_FLAG_DISACTIVE)
    {
        #if(DCM_SERVICE_27_ENABLED == STD_ON)
        for(i = 0; i < KIND_OF_SECURITY_LEVEL; i++)
        {
            if(gSecurityAcessDelayTime[i]
            == gDcmDspSecurityRow[i].DcmDspSecurityDelayTime/DCM_TASK_TIME)
            {
                gSecurityAcessAttempNumber[i] = gDcmDspSecurityRow[i].DcmDspSecurityNumAttDelay;
            }
        }
        #endif
    }
    else
    {
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_Uds_Common
*                
* Description:   Abstraction of Common code of Uds Service function
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void,DCM_PRIVATE_CODE)  DsdInternal_Uds_Common
(
    void
)
{
    /* Check and call sub-function main hander */
    gSubFunctionHandler = gDcmDsdSubService[gSubServicePosition].DcmDsdSubFunction;
    if(gSubFunctionHandler != DCM_NULL)
    {
        (void)(*gSubFunctionHandler)(&gMsgContextType);
    }
    else
    {
    }
    
    /* If the there is no error found in main handler,check and call 
       sub-function post hanler */
    if((gNegativeResponseCode == DCM_E_POSITIVERESPONSE) || (gNegativeResponseCode == DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING))
    {        
        if(gUDS_Physical_DiagBuffer[0] == DCM_SERVICE_10_POSITIVE_RESPONSE_DATA) 
        {
           gCurrentSessionType = gUDS_Physical_DiagBuffer[1];
        }
                                                          
        if(gDcmDsdSubService[gSubServicePosition].DcmDsdSubFunctionPost != DCM_NULL)
        {
            gSubFunctionPostHandler = gDcmDsdSubService[gSubServicePosition].DcmDsdSubFunctionPost; 
        }
        else
        {
        }
    }
    else
    {
    }
}

#define DCM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

#define DCM_START_SEC_PUBLIC_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_SecurityAccessKeyCompared
*                
* Description:   Post pocessing after application has finished key-checking
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE)  DsdInternal_SecurityAccessKeyCompared
(
    uint8 level, DcmDspSecurityAccessKey keyIsValid
)
{
    uint8 i = 0;

    if (keyIsValid == KEY_IS_NOT_VALID)
    {   
        /*If key is not valid,decrease allowed attmept number */
        DsdInternal_AccessAttemptNumberDec();
    }
    
    /* If the key is valid,unlock realated security level and reset related variable */
    else
    {
        for(i = 0; i < KIND_OF_SECURITY_LEVEL; i++)
        {
            gSecurityAcessSequence[i] = 0;
        }    
        gSecurityAcessAttempNumber[gCurrentSecurityAccessRequestLevel - 1] = gDcmDspSecurityRow[gCurrentSecurityAccessRequestLevel - 1].DcmDspSecurityNumAttDelay;
        gSecurityAcessDelayTime[gCurrentSecurityAccessRequestLevel - 1] = gDcmDspSecurityRow[gCurrentSecurityAccessRequestLevel - 1].DcmDspSecurityDelayTime/DCM_TASK_TIME;
        Set_SecLevel(level);
    }
}

#if(DCM_SERVICE_31_ENABLED == STD_ON) 
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_RoutineStarted
*                
* Description:   Set rountine started sequence flag
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE)  DsdInternal_RoutineStarted
(
    void
)
{
    gRountineControlSequence[gRountineControlDidHandle]  |= HAVE_STARTED_ROUNTINE;
    gRountineControlDidHandle = DCM_INVALID_HANDLE_OR_ID;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_RoutineStopped
*                
* Description:   Set rountine stopped sequence flag
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE)  DsdInternal_RoutineStopped
(
    void
)
{
    gRountineControlSequence[gRountineControlDidHandle] |= HAVE_STOPPED_ROUNTINE;
    /* Reset */
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: DsdInternal_RequestRoutineResults
*                
* Description:   Set RequestRoutineResults sequence flag
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void,DCM_PUBLIC_CODE)  DsdInternal_RequestRoutineResults
(
    void
)
{
    gRountineControlSequence[gRountineControlDidHandle] |= HAVE_REQ_ROUNTINE_RESULT;
    /* Reset */
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dsd_InteralInit
*                
* Description:   Init common global paramters
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/

FUNC(void,DCM_PUBLIC_CODE) Dsd_InteralInit(void)
{
    gP2ServerTimerStartFlag = DCM_FLAG_DISACTIVE;
    Clr_P2_Server_Timer();
    ClrSuppressPosResponseBit();
    ClrNegativeResponseCode();
    Clr_ActiveProtocol();
    Reset_DiagState();
    Reset_PduId();
    gFunctionHandler = DCM_NULL;
    gSubFunctionHandler = DCM_NULL;
    gMsgContextType.resDataLen = 0;
    Reset_Max_Number_Of_RCRRP() ;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Dsd_GetSessionMapId
*                
* Description:   Map self define session to the real session level
*                             
* Inputs:        None
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(uint8,DCM_PUBLIC_CODE) Dsd_GetSessionMapId(Dcm_SesCtrlType DcmSessionType) 
{
    uint8 SessionId = 0;
    switch(DcmSessionType)   
    {
        case  DCM_SESSION_DEFAULT:                         
              SessionId = DCM_SERVICE_10_SESSION_TYPE_0;
        break;
        case  DCM_SESSION_PROGRAMMING:                     
              SessionId = DCM_SERVICE_10_SESSION_TYPE_1; 
        break;
        case  DCM_SESSION_EXTENDED_DIAGNOSTIC:             
              SessionId = DCM_SERVICE_10_SESSION_TYPE_2;
        break;  
        case  DCM_SESSION_USERDEFINED1:                    
              SessionId = DCM_SERVICE_10_SESSION_TYPE_3; 
        break;
        case  DCM_SESSION_USERDEFINED2:                    
              SessionId = DCM_SERVICE_10_SESSION_TYPE_4; 
        break;
        case  DCM_SESSION_USERDEFINED3:                    
              SessionId = DCM_SERVICE_10_SESSION_TYPE_5; 
        break;  
        case  DCM_SESSION_USERDEFINED4:                    
              SessionId = DCM_SERVICE_10_SESSION_TYPE_6; 
        break;
        case  DCM_SESSION_USERDEFINED5:                    
              SessionId = DCM_SERVICE_10_SESSION_TYPE_7; 
        break;
        default:                                           
              SessionId = 0; 

        break;            
    }
    return SessionId;
}

#define DCM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

