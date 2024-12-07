/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Dcm_Type.h
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

#ifndef _DCM_TYPES_H_
#define _DCM_TYPES_H_

/**************************************************************************************************
* Include files
**************************************************************************************************/
#include "Platform_Types.h"
#include "ComStack_Types.h"

/**************************************************************************************************
* Macros and Tyedef
**************************************************************************************************/
typedef uint8 Dcm_StatusType;
typedef uint8 Dcm_SecLevelType;
typedef uint8 Dcm_SesCtrlType;
typedef uint8 Dcm_ProtocolType;
typedef uint8 Dcm_NegativeResponseCodeType;
typedef uint8 Dcm_CommunicationModeType;
typedef struct
{
    Dcm_ProtocolType protocolType;
}Dcm_ConfigType;  /* Need change */
typedef uint8 Dcm_ConfirmationStatusType;
typedef uint8 Dcm_OpStatusType;
typedef uint8 Dcm_ReturnReadMemoryType;
typedef uint8 Dcm_ReturnWriteMemoryType;
typedef uint8 Dcm_RoeStateType;
typedef uint8 Dcm_EcuStartModeType;
typedef struct
{
    uint8 ProtocolId; 
    uint8 TesterSourceAddr;
    uint8 Sid;
    uint8 SubFncId;
    Dcm_EcuStartModeType EcuStartMode;
}Dcm_ProgConditionsType;
typedef uint8 Dcm_MsgItemType;
typedef uint8 CurrentSidMapType;
typedef uint8 CurrentOBDSidMapType;
typedef Dcm_MsgItemType *Dcm_MsgType;
typedef uint16 Dcm_DspMaxDidToReadType;
typedef uint32 Dcm_MsgLenType;
typedef uint32 Dcm_CurentDcmDsdServiceTableSizeType;
typedef uint32 Dcm_CurentDcmOBDServiceTableSizeType;
typedef uint32 Dcm_DspNonDefaultSessionS3ServerType;
typedef uint8 Dcm_IdContextType; /* Need change */
typedef struct
{
    uint8 reqType; /* 0=physical request; 1=functional request */
    uint8 suppressPosResponse;/* 0=no不抑制正响应; 1=yes抑制正响应;*/
    uint8 cancelOperation;/* 0=no不取消挂起的操作;1=cancel pending operation 取消挂起的操作 */
}Dcm_MsgAddInfoType;

typedef struct 
{
    Dcm_MsgType reqData; //请求数据
    Dcm_MsgLenType reqDataLen;
    Dcm_MsgType resData;  //响应数据
    Dcm_MsgLenType resDataLen;
    Dcm_MsgAddInfoType msgAddInfo; //用于存储与消息相关的附加信息，如会话类型、安全级别等。
    Dcm_MsgLenType resMaxDataLen;//响应数据的最大长度
    Dcm_IdContextType idContext;//用于存储与消息 ID 相关的上下文信息，如会话 ID、事务 ID 等
    PduIdType dcmRxPduId;  //接收pdu的id
}Dcm_MsgContextType;
typedef uint8 Dcm_ResetModeType;

typedef enum
{
    DCM_UNINITIALIZED = 0,
    DCM_INITIALIZED = 1  
}DCM_InitType;

/* Dcm_StatusType */
#define DCM_E_OK                             (0x00u)
#define DCM_E_COMPARE_KEY_FAILED             (0x01u)
#define DCM_E_TI_PREPARE_LIMITS              (0x02u)
#define DCM_E_TI_PREPARE_INCONSTENT          (0x03u)
#define DCM_E_SESSION_NOT_ALLOWED            (0x04u)
#define DCM_E_PROTOCOL_NOT_ALLOWED           (0x05u)
#define DCM_E_ROE_NOT_ACCEPTED               (0x06u)
#define DCM_E_PERIODICID_NOT_ACCEPTED        (0x07u)
#define DCM_E_REQUEST_NOT_ACCEPTED           (0x08u)
#define DCM_E_REQUEST_ENV_NOK                (0x09u)
/* Dcm_ProtocolType */
#define DCM_OBD_ON_CAN                       (0x00u)
#define DCM_OBD_ON_FLEXRAY                   (0x01u)
#define DCM_OBD_ON_IP                        (0x02u)
#define DCM_UDS_ON_CAN                       (0x03u)
#define DCM_UDS_ON_FLEXRAY                   (0x04u)
#define DCM_UDS_ON_IP                        (0x05u)
#define DCM_ROE_ON_CAN                       (0x06u) 
#define DCM_ROE_ON_FLEXRAY                   (0x07u)
#define DCM_ROE_ON_IP                        (0x08u)
#define DCM_PERIODICTRANS_ON_CAN             (0x09u)
#define DCM_PERIODICTRANS_ON_FLEXRAY         (0x0Au)
#define DCM_PERIODICTRANS_ON_IP              (0x0Bu)
/* Dcm_SesCtrlType */
#define DCM_DEFAULT_SESSION                  (0x01u)
#define DCM_PROGRAMMING_SESSION              (0x02u)
#define DCM_EXTENDED_DIAGNOSTIC_SESSION      (0x03u)
#define DCM_SAFETY_SYSTEM_DIAGNOSTIC_SESSION (0x04u)
/* Dcm_SessionType*/
#define DCM_SESSION_DEFAULT                                 (0x01u)
#define DCM_SESSION_PROGRAMMING                             (0x02u)        
#define DCM_SESSION_EXTENDED_DIAGNOSTIC                     (0x04u)        
#define DCM_SESSION_USERDEFINED1                            (0x08u)
#define DCM_SESSION_USERDEFINED2                            (0x10u)
#define DCM_SESSION_USERDEFINED3                            (0x20u)
#define DCM_SESSION_USERDEFINED4                            (0x40u)
#define DCM_SESSION_USERDEFINED5                            (0x80u)

#define DCM_SESSION_EXTENDED					DCM_SESSION_EXTENDED_DIAGNOSTIC
/* Dcm Flag status */
#define DCM_FLAG_ACTIVE                                     (0x01u)  //启动
#define DCM_FLAG_DISACTIVE                                  (0x00u)  //停止

/* Dcm_SecLevelType */
#define DCM_SEC_LEV_LOCK      (0x01u)
#define DCM_SEC_LEV_L1        (0x02u) 
#define DCM_SEC_LEV_L2        (0x04u) 
#define DCM_SEC_LEV_L3        (0x08u)
#define DCM_SEC_LEV_L4        (0x10u) 
#define DCM_SEC_LEV_L5        (0x20u) 
#define DCM_SEC_LEV_L6        (0x40u) 
#define DCM_SEC_LEV_L7        (0x80u)
#define DCM_SEC_LEV_ALL       (0xffu)
/* invalid handle or received id*/
#define DCM_INVALID_HANDLE_OR_ID  ((uint8)0xFF)
/* Negative Response Service Id */
#define DCM_NEGATIVE_RES_SERVICE_ID                    (0x7Fu)
/* Dcm_NegativeResponseCodeType  负响应码*/
#define DCM_E_POSITIVERESPONSE                         (0x00u)  //请求处理成功
#define DCM_E_GENERALREJECT                            (0x10u)  ///请求被拒绝 
#define DCM_E_SERVICENOTSUPPORTED                      (0x11u) //请求的服务不支持
#define DCM_E_SUBFUNCTIONNOTSUPPORTED                  (0x12u)//请求的子功能不支持
#define DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT    (0x13u)//消息长度不正确或格式不正确
#define DCM_E_BUSYREPEATREQUEST                        (0x21u)//系统忙，重复请求
#define DCM_E_CONDITIONSNOTCORRECT                     (0x22u)//条件不匹配
#define DCM_E_REQUESTSEQUENCEERROR                     (0x24u)//请求顺序错误
#define DCM_E_REQUESTOUTOFRANGE                        (0x31u)//请求超出范围
#define DCM_E_SECURITYACCESSDENIED                     (0x33u)//安全访问被拒绝
#define DCM_E_INVALIDKEY                               (0x35u)//密钥无效
#define DCM_E_EXCEEDNUMBEROFATTEMPTS                   (0x36u)//尝试次数过多
#define DCM_E_REQUIREDTIMEDELAYNOTEXPIRED              (0x37u)//必需时间未过期
#define DCM_E_UPLOADDOWNLOADNOTACCEPTED                (0x70u)//上传/下载不支持
#define DCM_E_TRANSFERDATASUSPENDED                    (0x71u)//传输数据暂停
#define DCM_E_GENERALPROGRAMMINGFAILURE                (0x72u)//程序化失败
#define DCM_E_WRONGBLOCKSEQUENCECOUNTER                (0x73u)//块序列计数器错误
#define DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING (0x78u)//请求正确接收，响应正在处理 
#define DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION   (0x7Eu)//子功能不支持
#define DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION       (0x7Fu)//服务不支持
#define DCM_E_RPMTOOHIGH                               (0x81u)//RPM 过高
#define DCM_E_RPMTOOLOW                                (0x82u)//RPM 过低
#define DCM_E_ENGINEISRUNNING                          (0x83u)//引擎在运行
#define DCM_E_ENGINEISNOTRUNNING                       (0x84u)//引擎不在运行
#define DCM_E_ENGINERUNTIMETOOLOW                      (0x85u)//引擎运行时间太短
#define DCM_E_TEMPERATURETOOHIGH                       (0x86u)//温度过高
#define DCM_E_TEMPERATURETOOLOW                        (0x87u)//温度过低
#define DCM_E_VEHICLESPEEDTOOHIGH                      (0x88u)//速度过高
#define DCM_E_VEHICLESPEEDTOOLOW                       (0x89u)//速度过低
#define DCM_E_THROTTLE_PEDALTOOHIGH                    (0x8Au)// 油门踏板过高
#define DCM_E_THROTTLE_PEDALTOOLOW                     (0x8Bu)// 油门踏板过低
#define DCM_E_TRANSMISSIONRANGENOTINNEUTRAL            (0x8Cu)// 变速箱不在中档
#define DCM_E_TRANSMISSIONRANGENOTINGEAR               (0x8Du)// 变速箱不在档位
#define DCM_E_BRAKESWITCH_NOTCLOSED                    (0x8Fu)// 刹车开关未关闭
#define DCM_E_SHIFTERLEVERNOTINPARK                    (0x90u)// 档位开关未置入P档
#define DCM_E_TORQUECONVERTERCLUTCHLOCKED              (0x91u)// verterter 离合器锁定
#define DCM_E_VOLTAGETOOHIGH                           (0x92u)// 电压太高
#define DCM_E_VOLTAGETOOLOW                            (0x93u)// 电压太低
/* Dcm_CommunicationModeType */                        
#define DCM_ENABLE_RX_TX_NORM            (0x00u)
#define DCM_ENABLE_RX_DISABLE_TX_NORM    (0x01u)
#define DCM_DISABLE_RX_ENABLE_TX_NORM    (0x02u)
#define DCM_DISABLE_RX_TX_NORMAL         (0x03u)
#define DCM_ENABLE_RX_TX_NM              (0x04u)
#define DCM_ENABLE_RX_DISABLE_TX_NM      (0x05u)
#define DCM_DISABLE_RX_ENABLE_TX_NM      (0x06u)
#define DCM_DISABLE_RX_TX_NM             (0x07u)
#define DCM_ENABLE_RX_TX_NORM_NM         (0x08u)
#define DCM_ENABLE_RX_DISABLE_TX_NORM_NM (0x09u)
#define DCM_DISABLE_RX_ENABLE_TX_NORM_NM (0x0Au)
#define DCM_DISABLE_RX_TX_NORM_NM        (0x0Bu)
/* Dcm_ConfirmationStatusType */
#define DCM_RES_POS_OK     (0x00u)
#define DCM_RES_POS_NOT_OK (0x01u)
#define DCM_RES_NEG_OK     (0x02u)
#define DCM_RES_NEG_NOT_OK (0x03u)
/* Dcm_OpStatusType */
#define DCM_INITIAL        (0x00u)
#define DCM_PENDING        (0x01u)
#define DCM_CANCEL         (0x02u)
#define DCM_FORCE_RCRRP_OK (0x03u)
/* Dcm_ReturnReadMemoryType */
#define DCM_READ_OK        (0x00u)
#define DCM_READ_PENDIN    (0x01u)
#define DCM_READ_FAILED    (0x02u)
/* Dcm_ReturnWriteMemoryType */
#define DCM_WRITE_OK       (0x00u)
#define DCM_WRITE_PENDIN   (0x01u)
#define DCM_WRITE_FAILED   (0x02u)
/* Dcm_RoeStateType */
#define DCM_ROE_ACTIVE     (0x00u)
#define DCM_ROE_UNACTIV    (0x01u)
/* Dcm_EcuStartModeType */
#define DCM_COLD_START     (0x00u)
#define DCM_WARM_START     (0x01u)
/* Dcm_ResetModeType */
#define DCM_NO_RESET                            (0x00u)
#define DCM_HARD_RESET                          (0x01u)
#define DCM_KEY_ON_OFF_RESET                    (0x02u)
#define DCM_SOFT_RESET                          (0x03u)
#define DCM_ENABLE_RAPID_POWER_SHUTDOWN_RESET   (0x04u)
#define DCM_DISABLE_RAPID_POWER_SHUTDOWN_RESET  (0x05u)
#define DCM_BOOTLOADER_RESET                    (0x06u)
#define DCM_SS_BOOTLOADER_RESET                 (0x07u)
#define DCM_RESET_EXECUTION                     (0x08u)



#endif
