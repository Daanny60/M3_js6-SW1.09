/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : OsekNm.c
********************************************************************************
*   Project/Product : OsekNm programme
*   Title           : OsekNm.c
*   Author          : yzhai
*                     bo.liu
*                     peng.liu
*                     xiang.li1
*                     jingyun.zhang
********************************************************************************
*   Description     : Header of OsekNm module.                    
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
*   01.00.00    10/10/2009    yzhai         N/A          Original
*   01.00.01    08/08/2010    bo.liu        N/A          HrAsrOsekNm100808-01
*   01.00.02    11/08/2010    bo.liu        N/A          HrAsrOsekNm100811-01
*   01.00.03    11/08/2010    bo.liu        N/A          HrAsrOsekNm100811-02
*   01.00.04    20/01/2011    bo.liu        N/A          HrAsrOsekNm110120-01
*   01.00.05    21/01/2011    bo.liu        N/A          HrAsrOsekNm110121-01
*   01.00.06    24/01/2011    bo.liu        N/A          HrAsrOsekNm110124-01
*   01.00.07    25/01/2011    bo.liu        N/A          HrAsrOsekNm110125-01
*   01.00.08    17/05/2011    bo.liu        N/A          HrAsrOsekNm110517-01
*   01.00.09    17/05/2011    bo.liu        N/A          HrAsrOsekNm110517-02
*   01.00.10    18/05/2011    bo.liu        N/A          HrAsrOsekNm110518-01
*   01.00.11    19/07/2011    bo.liu        N/A          HrAsrOsekNm110719-01
*   01.00.12    20/12/2011    bo.liu        N/A          HrAsrOsekNm111220-01
*   01.00.13    15/01/2012    peng.liu      N/A          HrAsrOsekNm120115-01
*   01.00.14    11/06/2012    xiang.li1     N/A          HrAsrOsekNm120611-01
*   01.00.15    16/07/2012    xiang.li1     N/A          HrAsrOsekNm120716-01
*   01.01.00    18/07/2012    xiang.li1     N/A          HrAsrOsekNm120718-01
*   01.02.00    18/07/2012    xiang.li1     N/A          HrAsrOsekNm120718-02
*   01.02.01    07/04/2013    donger.yang   N/A          HrAsrOsekNm130407-01
*   01.02.02    06/05/2013    peng.liu      N/A          HrAsrOsekNm130506-01
*   01.03.00    30/05/2013    jingyun.zhang N/A          HrAsrOsekNm130530-01
*   01.04.00    08/06/2013    jingyun.zhang N/A          HrAsrOsekNm130608-01
*   01.04.01    09/06/2013    jingyun.zhang N/A          HrAsrOsekNm130609-01
*   01.05.00    18/06/2013    jingyun.zhang N/A          HrAsrOsekNm130618-01
*   01.05.01    21/08/2013    donger.yang   N/A          HrAsrOsekNm130821-01
*   01.05.02    06/09/2013    donger.yang   N/A          HrAsrOsekNm130906-01
*   01.06.01    10/10/2013    jingyun.zhang N/A          HrAsrOsekNm131010-01 
*   01.06.02    06/01/2014    jingyun.zhang N/A          HrAsrOsekNm140116-01 
*   02.00.00    10/03/2014    rui.guo       N/A          HrAsrOsekNm140310-01
********************************************************************************
* END_FILE_HDR*/

#ifndef _OSEKNM_H_
#define _OSEKNM_H_


/*******************************************************************************
*   Includes 
*******************************************************************************/
#include "OsekNm_Cfg.h"
#include "CanIf.h"



/***************************** Program Body ***********************************/


/*******************************************************************************
*   Macro definitions 
*******************************************************************************/
#define DATA_INIT                        ((uint8)0x00u)
#define NM_BIT_SET                       (1)
#define NM_BIT_CLEAR                     (0)

/* configuration needed :relate to Licence */
#define V_DEF_SUPPORTED_PLATFORM
#define C_COMP_DIABDATA_MPC55XX

/* NM State Code Mask */
/* mask: 00011111 */
#define NM_STATE_CODE_MASK                ((uint8)0x1Fu)  
/* mask: 11100000 */
#define NM_STATE_DECODE_MASK              ((uint8)0xE0u)   

/*NM State start*/
#define NM_STATE_CODE_OFF                 ((nmStateType)0x00u)
#define NM_STATE_CODE_LIMPHOME            ((nmStateType)0x01u)
#define NM_STATE_CODE_NORMALPREPSLEEP     ((nmStateType)0x02u)
#define NM_STATE_CODE_LIMPHOMEPREPSLEEP   ((nmStateType)0x03u)
#define NM_STATE_CODE_TWBSNORMAL          ((nmStateType)0x04u)
#define NM_STATE_CODE_TWBSLIMPHOME        ((nmStateType)0x05u)
#define NM_STATE_CODE_BUSSLEEP            ((nmStateType)0x08u)
#define NM_STATE_CODE_NORMAL              ((nmStateType)0x09u)
#define NM_STATE_CODE_SHUTDOWN            ((nmStateType)0x0Au)

/*NM State end*/
#define NM_OWN_STATION     nmChannelNodeID[nmCurChannel]
#define NMTXPDUID          nmTxPduIdAry[nmCurChannel]
#define nmStatus           nmStatusAry[nmCurChannel]
#define nmTxDataFld        nmTxDataFldAry[nmCurChannel]
#define nmRxDataFld        nmRxDataFldAry[nmCurChannel]
#if defined ( NM_FOR_FAW )               
#define nmTxMsgIndFld      nmTxMsgIndFldAry[nmCurChannel]
#define nmRxMsgIndFld      nmRxMsgIndFldAry[nmCurChannel]
#endif
#define nmLogicalSuccessor nmLogicalSuccessorAry[nmCurChannel]
#define nmRxSourceAddr     nmRxSrcAddAry[nmCurChannel]
#define nmRxCount          nmRxCountAry[nmCurChannel]
#define nmTxCount          nmTxCountAry[nmCurChannel]
#define nmTimerCntTtyp     nmTimerCntTtypAry[nmCurChannel]  
#define nmTimerCntTmax     nmTimerCntTmaxAry[nmCurChannel]
#define nmTimerCntTerr     nmTimerCntTerrAry[nmCurChannel]
#define nmTimerCntTwbs     nmTimerCntTwbsAry[nmCurChannel]
#define nmTimerCntTtx      nmTimerCntTtxAry[nmCurChannel]
#define nmTimerCntTbusoff  nmTimerCntTbusoffAry[nmCurChannel]
#if defined ( NM_FOR_FAW )                
#define nmTimerCntTactivemini nmTimerCntTactiveminiAry[nmCurChannel]        
#define nmTimerCntTlimphome   nmTimerCntTlimphomeAry[nmCurChannel]    
#endif    
/*#define nmCurState nmCurStateAry[nmCurChannel]*/
/*#define nmCanBusOffFlg nmCanBusOffFlgAry[nmCurChannel]*/
/*#define nmCanBusOffBeing nmCanBusOffBeingAry[nmCurChannel]*/
#define nubNmBusOffCnt        nubNmBusOffCntAry[nmCurChannel]
/*#define nmConfigNormal(x)     nmConfigNormalAry[nmCurChannel][x]*/

#define NM_PDU_DLC            ((uint8)0x08u)
#define NM_PDU_CONTROL_DLC    ((uint8)0x02u)
#define NM_PDU_BASICID        nmPduBasicIDAry[nmCurChannel]                  
#define NM_PDU_BASICID_MASK   nmPduBasicIDMaskAry[nmCurChannel]             
#define NM_PDU_SOUCEID_MASK   nmPduSouceIDMaskAry[nmCurChannel]              

#define NM_COUNT_TYP            \
    (nmCountTypAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)
#define NM_COUNT_MAX            \
    (nmCountMaxAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)                   
#define NM_COUNT_ERR            \
    (nmCountErrAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)                    
#define NM_COUNT_WBS            \
    (nmCountWbsAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)                    
#define NM_COUNT_TX             \
    (nmCountTxAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)                    
#define NM_COUNT_FAST_TBUSOFF   \
    (nmCountBusoffFastTimerAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)           
#define NM_COUNT_SLOW_TBUSOFF   \
    (nmCountBusoffSlowTimerAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)
#if defined (NM_FOR_FAW)                        
#define NM_COUNT_TACTIVEMINI    \
    (nmCountActMinAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)
#define NM_COUNT_TLIMPHOME      \
    (nmCountTLimpHAry[nmCurChannel]/NM_MAINFUNCTION_PERIOD)
#endif                    
#define NM_BUSOFF_FAST_RECOVERY_TIMES     nmBusOffFastRecoveryAry[nmCurChannel]            
#define NM_BUSOFF_SLOW_RECOVERY_TIMES     nmBusOffSlowRecoveryAry[nmCurChannel]    
#define RX_LIMIT                          nmRxLimitAry[nmCurChannel]    
#define TX_LIMIT                          nmTxLimitAry[nmCurChannel]  
#define NM_NUMBER_OF_CFG_NODE             nmNumberOfCfgNodeAry[nmCurChannel]
#define NM_NORMAL_CFG_RECORD(x)           (*(nmNormalCfgPtrAry[nmCurChannel] + (x)))
#define NM_LIMPHOME_CFG_RECORD(x)         (*(nmLimpHomeCfgPtrAry[nmCurChannel] + (x)))   

/*CAN ID mode : standrd or extended*/
#define NM_CAN_ID_STANDARD_ENABLE     \
    (nmChannelConfigAry[nmCurChannel].B.NMIDStandardEn)
/* remote wakeup signal */
#define NM_REMOTE_WAKEUP_ENABLE       \
    (nmChannelConfigAry[nmCurChannel].B.NMRemoteWakeUpEn)    
/* Congfig functions */
#define NM_CHN_CONFIG_NORMAL              \
    (nmChannelConfigAry[nmCurChannel].B.NMCfgNormalEn) 
#define NM_CHN_CONFIG_LIMPHOME              \
    (nmChannelConfigAry[nmCurChannel].B.NMCfgLimpHEn) 
/* Extended Callback functions */
#define NM_EXTENDED_CALLBACK_ENABLE   \
    (nmChannelConfigAry[nmCurChannel].B.NMExtCallBackEn)    
/*gg*/
#define NM_DEFAULT_ACTIVE    \
    (nmChannelConfigAry[nmCurChannel].B.NMDefaultActive) 
#define NM_DEFAULT_STRATED   \
    (nmChannelConfigAry[nmCurChannel].B.NMDefaultStarted) 
#define NM_USED_SWITCH     \
    (nmChannelConfigAry[nmCurChannel].B.NMUsedSwitch)
/*end*/
/*#if defined ( NM_TYPE_HIRAIN )*/
/* nmStatus */
/*#define nmStatusByteGlobal              (nmStatus.R[0])*/
#define nmRealTimeStateCode             (nmStatus.B.NMStateCode)

/*#define nmStatusRuning                  (nmStatus.R[1])*/
#define nmStatusChnInit                 (nmStatus.B.NMInited)
#define nmStatusNMActive                (nmStatus.B.NMActive)
#define nmStatusNMBusError              (nmStatus.B.NMBusError)
#define nmStatusNMCfgStable             (nmStatus.B.NMCfgStable)
#define nmStatusNMBusOffFlag            (nmStatus.B.NMBusOffFlag)/*nmCanBusOffFlg*/
#define nmStatusErrRestoring            (nmStatus.B.ErrRestoring)/*nmCanBusOffBeing */
#define nmStatusSlowRecovery            (nmStatus.B.SlowRecovery)

/*#define nmStatusByteLocal               (nmStatus.R[2])*/
#define nmStatusLocalSleepInd           (nmStatus.B.LocalSleepInd)
#define nmStatusLocalSleepAck           (nmStatus.B.LocalSleepAck)
#define nmStatusMerkerStable            (nmStatus.B.MerkerStable)
#define nmStatusMerkerLimpHome          (nmStatus.B.MerkerLimpHome)
#define nmStatusImSkipped               (nmStatus.B.ImSkipped)
#define nmStatusImDestined              (nmStatus.B.ImDestined)
#define nmStatusUseRingData             (nmStatus.B.UseRingData)
#define nmStatusTransmitFlag            (nmStatus.B.TransmitFlag)

/*#if defined (NM_FOR_FAW)*/             
/*#define nmStatusByteLocal1              (nmStatus.R[3])*/
#define nmStatusActTimeReach            (nmStatus.B.ActTimeReach)
#define nmStatusAliveSended             (nmStatus.B.AliveSended)
#define nmStatusLHTimeReach             (nmStatus.B.LHTimeReach)
#define nmStatusNortoBusSleep           (nmStatus.B.NortoBusSleep)
#define nmStatusLHtoBusSleep            (nmStatus.B.LHtoBusSleep)
/*#endif*/
#define nmStatusNMModeInd               (nmStatus.B.NMModeInd)

/* About two important algorithms */
/* Skipped in the logical ring & Determine logical successor */
#define nmRxMsgSourceId_S               nmRxSourceAddr 
#define nmRxMsgDestId_D                 (nmRxDataFld.B.DestID)
#define nmReceiverId_R                  NM_OWN_STATION
#define nmLogSucId_L                    nmLogicalSuccessor

/* nmRxDataFld */
#if defined (NM_FOR_FAW)               
#define nmRxDataFldByteOpCode           (nmRxDataFld.B.OpCode)
#else
#define nmRxDataFldOpCodeMsgType        (nmRxDataFld.B.OpCodeMsgType)
#define nmRxDataFldOpCodeSleepInd       (nmRxDataFld.B.OpCodeSleepInd)
#define nmRxDataFldOpCodeSleepAck       (nmRxDataFld.B.OpCodeSleepAck)
#endif
#define nmRxDataFldByteDestID           (nmRxDataFld.B.DestID)
#define nmRxDataFldByte0                (nmRxDataFld.R[0])
#define nmRxDataFldByte1                (nmRxDataFld.R[1])
#define nmRxDataFldByteUser0            (nmRxDataFld.R[2])
#define nmRxDataFldByteUser1            (nmRxDataFld.R[3])
#define nmRxDataFldByteUser2            (nmRxDataFld.R[4])
#define nmRxDataFldByteUser3            (nmRxDataFld.R[5])
#define nmRxDataFldByteUser4            (nmRxDataFld.R[6])
#define nmRxDataFldByteUser5            (nmRxDataFld.R[7])

/* nmTxDataFld */                 
#if defined (NM_FOR_FAW)               
#define nmTxDataFldByteOpCode           (nmTxDataFld.B.OpCode)
#else
#define nmTxDataFldOpCodeMsgType        (nmTxDataFld.B.OpCodeMsgType)
#define nmTxDataFldOpCodeSleepInd       (nmTxDataFld.B.OpCodeSleepInd)
#define nmTxDataFldOpCodeSleepAck       (nmTxDataFld.B.OpCodeSleepAck)
#endif
#define nmTxDataFldByteDestID           (nmTxDataFld.B.DestID)
#define nmTxDataFldByte0                (nmTxDataFld.R[0])
#define nmTxDataFldByte1                (nmTxDataFld.R[1])
#define nmTxDataFldByteUser0            (nmTxDataFld.R[2])
#define nmTxDataFldByteUser1            (nmTxDataFld.R[3])
#define nmTxDataFldByteUser2            (nmTxDataFld.R[4])
#define nmTxDataFldByteUser3            (nmTxDataFld.R[5])
#define nmTxDataFldByteUser4            (nmTxDataFld.R[6])
#define nmTxDataFldByteUser5            (nmTxDataFld.R[7])

#if defined (NM_FOR_FAW)              
#define nmRxDataFldOpCodeMsgType        (nmRxMsgIndFld.B.OpCodeMsgType)
#define nmRxDataFldOpCodeSleepInd       (nmRxMsgIndFld.B.OpCodeSleepInd)
#define nmRxDataFldOpCodeSleepAck       (nmRxMsgIndFld.B.OpCodeSleepAck)
#define nmTxDataFldOpCodeMsgType        (nmTxMsgIndFld.B.OpCodeMsgType)
#define nmTxDataFldOpCodeSleepInd       (nmTxMsgIndFld.B.OpCodeSleepInd)
#define nmTxDataFldOpCodeSleepAck       (nmTxMsgIndFld.B.OpCodeSleepAck)
#endif
 
/*#endif */

/*******************************************************************************
  Local data types and structures
*******************************************************************************/
/*Description: This type represents the various status flags.*/
typedef union
{
    uint8 R[4];
    struct
    {
        uint8     NMStateCode       :8;
        
#if defined ( NM_BYTEORDER_LSB )
        uint8     NMInited           :1;       /* byte0 bit0 */
        uint8     NMActive           :1;       /* byte0 bit1 */
        uint8     NMBusError        :1;       /* byte0 bit2 */
        uint8     NMCfgStable       :1;       /* byte0 bit3 */
        uint8     NMBusOffFlag       :1;       /* byte0 bit4 */
        uint8     ErrRestoring       :1;       /* byte0 bit5 */
        uint8     SlowRecovery       :1;       /* byte0 bit6 */
        uint8     Reserved0           :1;

        uint8     TransmitFlag       :1;       /* byte1 bit0 */
        uint8     UseRingData       :1;       /* byte1 bit1 */
        uint8     ImDestined        :1;       /* byte1 bit2 */
        uint8     ImSkipped           :1;       /* byte1 bit3 */
        uint8     MerkerLimpHome    :1;       /* byte1 bit4 */
        uint8     MerkerStable       :1;       /* byte1 bit5 */
        uint8     LocalSleepAck       :1;       /* byte1 bit6 */  
        uint8     LocalSleepInd       :1;       /* byte1 bit7 */

        /*Indicate whether T[ActiveMini] has reached*/
        uint8     ActTimeReach       :1;       /* byte1 bit0 */ 
        /*Indicate whether The first Alive message has sent after wake up*/
        uint8     AliveSended       :1;       /* byte1 bit1 */ 
        /*Indicate whether T[LimpHome] has reached*/
        uint8     LHTimeReach       :1;       /* byte1 bit2 */ 
        uint8     LHtoBusSleep       :1;       /* byte1 bit3 */
        /*Indicate whether from state Normal to WaitBusSleep is directly*/
        uint8     Reserved1           :1;       /* byte1 bit4 */ 
        /*Indicate whether from state LimpHome to WaitBusSleep is directly*/
        uint8     Reserved2           :1;       /* byte1 bit5 */
        uint8    NMModeInd         :2;     /* byte3 bit6~7 */ 

#else
        uint8     Reserved0           :1;
        uint8     SlowRecovery       :1;       /* byte0 bit1 */
        uint8     ErrRestoring       :1;       /* byte0 bit2 */
        uint8     NMBusOffFlag       :1;       /* byte0 bit3 */
        uint8     NMCfgStable       :1;       /* byte0 bit4 */
        uint8     NMBusError        :1;       /* byte0 bit5 */
        uint8     NMActive           :1;       /* byte0 bit6 */
        uint8     NMInited           :1;       /* byte0 bit7 */

        uint8     LocalSleepInd       :1;       /* byte1 bit0 */
        uint8     LocalSleepAck       :1;       /* byte1 bit1 */
        uint8     MerkerStable       :1;       /* byte1 bit2 */
        uint8     MerkerLimpHome    :1;       /* byte1 bit3 */
        uint8     ImSkipped           :1;       /* byte1 bit4 */
        uint8     ImDestined        :1;       /* byte1 bit5 */
        uint8     UseRingData       :1;       /* byte1 bit6 */
        uint8     TransmitFlag       :1;       /* byte1 bit7 */

        uint8    NMModeInd         :2;     /* byte3 bit0~1 */ 
        uint8     Reserved2           :1;       /* byte1 bit2 */
        uint8     Reserved1           :1;       /* byte1 bit3 */
        uint8     LHtoBusSleep       :1;       /* byte1 bit4 */
        uint8     LHTimeReach       :1;       /* byte1 bit5 */
        uint8     AliveSended       :1;       /* byte1 bit6 */
        uint8     ActTimeReach       :1;       /* byte1 bit7 */
#endif
    }B;
} nmStatusType;


/*******************************************************************************
* Name:    nmAlarmType
* Used by: NmSetAlarm() (parameter) 
*******************************************************************************/

typedef enum
{
    /* nmAlarm=0, Timer TTyp */
    NM_TIMER_TYP = 0 
    /* nmAlarm=1, Timer TMax */
    ,NM_TIMER_MAX = 1 
    /* nmAlarm=2, Timer TErr */
    ,NM_TIMER_ERR = 2
    /* nmAlarm=3, Timer TWbs */
    ,NM_TIMER_WBS = 3 
    /* nmAlarm=4, Timer TTx */
    ,NM_TIMER_TX = 4  
    /* nmAlarm=5, fast Timer Tbusoff,OsekNm_006*/
    ,NM_FAST_TIMER_TBUSOFF = 5 
    /* nmAlarm=6, Slow Timer Tbusoff,OsekNm_008*/
    ,NM_SLOW_TIMER_TBUSOFF = 6      
    #if defined (NM_FOR_FAW) 
    /* nmAlarm=7, Timer Tactivemini */ 
    ,NM_TIMER_ACTIVEMINI  = 7 
    /* nmAlarm=8, Timer TLimpHome */  
    ,NM_TIMER_LIMPHOME    = 8 
    /* nmAlarm=8, All Timer*/
    #endif
    ,NM_TIMER_All = 9
} nmAlarmType;


/*******************************************************************************
* Name:    nmInitModeType
* Used by: NmInit() (parameter) 
*******************************************************************************/
typedef enum
{
    /* SDL Enter0: NM first Init ---> OsekDirNmInit() and NmStart()*/
    NM_D_BUSINIT = 0,  
    /* SDL Enter1: from bus off or normal */
    NM_D_BUSOFF = 1, 
    /* SDL Enter2: NMInitBusSleep to NMTwbsNormal From ---> 
      NMNormal/NMNormalPreSleep to NMTwbsNormal*/
    NM_D_NORMAL_TO_BUSSLEEP = 2, 
    /* SDL Enter3: leave NMBusSleep ---> NM state is sleep and GotoMode(Awake) 
      is called or WakeUp signal is received*/
    NM_D_BUSAWAKE = 3,  
     /* SDL Enter4: do not Init NM */
    NM_D_NOTINIT = 4, 
    /* SDL Enter5: leave NMLimpHome */
    NM_D_LIMPHOME = 5, 
    /* SDL Enter6: leave NMNormal */
    NM_D_NORMAL = 6,  
    /* SDL Enter7: NMInitBusSleep to NMTwbsLimpHome */
    NM_D_LIMPHOME_TO_BUSSLEEP = 7 
} nmInitModeType;

/*******************************************************************************
* Name:    nmSleepAwakeModeType
* Used by: GotoMode() (parameter) 
*******************************************************************************/
typedef enum
{
    GOTO_BUSSLEEP = 1,
    GOTO_AWAKE = 2
} nmSleepAwakeModeType;

/*******************************************************************************
* Name:    nmStateType
* Used by: NmGetCurState() (return type)
*          CanNm_SetNxtState() (parameter)
* Description:
*******************************************************************************/
typedef uint8 nmStateType;
/*******************************************************************************
* Name:        nmIndexType
* Description: This type is used for the Index counter.
* Note:        
*******************************************************************************/
typedef uint16 nmIndexType;
/*******************************************************************************
* Name:    NmReturnType
* Used by: CmpConfig() (return type) 
*******************************************************************************/
/*typedef uint8 nmReturnType;*/

typedef enum
{
    NM_E_OK,
    NM_E_NOT_OK,
    NM_E_NOT_EXECUTED
}NmReturnType;


#if defined(NM_ENABLE_CONFIG_LIMPHOME)||defined(NM_ENABLE_CONFIG_NORMAL)
 #define NODE_OFFLINE   0
 /*#define NODE_OPERABLE  1*/   /* Rx alive or ring */
 /*#define NODE_ERRORLH   2*/
typedef enum
{  
    CfgNormal = 1,
    CfgLimpHome = 2
}ConfigKindName;
#endif

#define OSEKNM_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"/* PRQA S 5087 */


#define OSEKNM_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"/* PRQA S 5087 */



/*******************************************************************************
  Global function prototypes
*******************************************************************************/
/*******************************************************************************
  Global function prototypes
*******************************************************************************/
#define OSEKNM_START_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Init(void);
extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Talk
(
    const nmIndexType nmCurChannel
);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Silent
(
    const nmIndexType nmCurChannel
);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Start
(
    const nmIndexType nmCurChannel
);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Stop
(
    const nmIndexType nmCurChannel
);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_GotoMode
(
    const nmIndexType nmCurChannel,
    const nmSleepAwakeModeType modeSleepAwake
);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_MainFunction(void);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_BusOff
(
    const nmIndexType CanControllerId
);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_RxIndication
(
    const PduIdType canNmPduId,
    const Can_IdType canNmRxId,
    CONSTP2CONST(PduInfoType,AUTOMATIC,OSEKNM_APPL_DATA) pCanNmRxPduPtr
);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_TxConfirmation
(
    const PduIdType canNmTxPduId
);

extern FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_WakeUp
(
    const nmIndexType CanControllerId
);
#if defined NM_ENABLE_DATA_ACCESS
extern FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_GetUserData
(
    const nmIndexType nmCurChannel,
    CONSTP2VAR(uint8, AUTOMATIC, OSEKNM_APPL_DATA) nmUserData
);

extern FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_SetUserData
(
    const nmIndexType nmCurChannel,
    CONSTP2CONST(uint8, AUTOMATIC, OSEKNM_PUBLIC_CODE) nmUserData
);

extern FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_ReadNmData
(
    const nmIndexType nmCurChannel,
    CONSTP2VAR(uint8, AUTOMATIC, OSEKNM_APPL_DATA) nmData
);
#endif
#if defined(NM_ENABLE_CONFIG_LIMPHOME)||defined(NM_ENABLE_CONFIG_NORMAL)

extern FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_GetConfig
(
    const nmIndexType nmCurChannel, 
    const ConfigKindName   CfgKind,
    CONSTP2VAR(uint8, AUTOMATIC, OSEKNM_APPL_DATA) dataPtr
);
#if 0
extern FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_CmpConfig
(
    const nmIndexType nmCurChannel, 
    const uint8 idx
);
#endif
#endif

/*Basic NM CallBack function for the Application*/
/*indicate nm normal*/
extern void ApplNmCanStart(const nmIndexType nmChannelHandle); 
/*This callback sets the communication hardware (CAN controller,bus transceiver) 
* to a state from which they can be woken up by the CAN bus.*/
extern void ApplNmCanSleep(const nmIndexType nmChannelHandle);
/*indicates bus off error occured*/
extern void ApplNmBusOff(const nmIndexType nmChannelHandle);
/*indicates can bus wake up*/
extern void ApplNmCanWakeUp(const nmIndexType nmChannelHandle);
/*Enter LimpHome State,OsekNm_007*/
extern void ApplNmCanLimpHomeStart(const nmIndexType nmChannelHandle);
extern void ApplNmLimpHomeState(const nmIndexType nmChannelHandle);
/*Exit LimpHome State,OsekNm_007*/
extern void ApplNmCanLimpHomeEnd(const nmIndexType nmChannelHandle);
/*Slow Bus Off recovery failed*/
extern void ApplNmCanBusOffRecoveryFail(const nmIndexType nmChannelHandle);
/*indicate end of bus off error */
extern void ApplNmBusOffEnd(const nmIndexType nmChannelHandle); 
/*This callback function is performed when busOff recovery successed.*/
extern void ApplNmBusOffRecoverySuccess(const nmIndexType nmChannelHandle);
extern void ApplNmWaitBusSleepCancel(const nmIndexType nmChannelHandle);
extern void ApplNmWaitBusSleep(const nmIndexType nmChannelHandle);
extern void ApplNmShutDown(const nmIndexType nmChannelHandle);
extern void ApplNmRingIndication(const nmIndexType nmChannelHandle,const nmNetIdType SourceID);
extern void ApplNmLimpHIndication(const nmIndexType nmChannelHandle,const nmNetIdType SourceID);
#define OSEKNM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */


#endif  /* _OSEKNM_H_ */
