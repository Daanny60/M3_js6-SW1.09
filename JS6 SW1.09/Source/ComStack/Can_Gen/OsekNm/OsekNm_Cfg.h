
/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : OsekNm_cfg.h
********************************************************************************
*   Project/Product : OsekNm programme
*   Title           : OsekNm.c
*   Author          : yzhai
*                     bo.liu
*                     peng.liu
*                     xiang.li1
*                     jingyun.zhang
********************************************************************************

*   Description     : The macro configuration of OSEKNM module.                    
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
*   01.05.00    18/06/2013    jingyun.zhang N/A          HrAsrOsekNm130618-01
*   02.00.00    10/03/2014    rui.guo       N/A          HrAsrOsekNm140310-01
********************************************************************************
* END_FILE_HDR*/

#ifndef _OSEKNM_CFG_H_
#define _OSEKNM_CFG_H_

/*******************************************************************************
*   Includes 
*******************************************************************************/

#include "ComStack_Types.h"
/************************************ Program Body ****************************/
/*pragma MESSAGE DISABLE C166 WARNING C1106: Non-standard bitfield type */

/* NM Order Type: LSB or MSB */
//#define NM_FOR_FAW


#define NM_ENABLE_RING_RX_IND
#define NM_ENABLE_LIMPHOME_RX_IND
#define NM_ENABLE_DATA_ACCESS
#define NM_BYTEORDER_LSB             
#define NM_ENABLE_CONFIG_NORMAL  
#define NM_ENABLE_CONFIG_LIMPHOME 

#define NM_BUSOFF_FAST_RECOVERY_ENABLE (STD_ON)

/*Multiply channel NetWork Management*/


#define NM_CANIF_USED_CONTROLLER_NUM        1/* !!!must be equal to CANIF_USED_CONTROLLER_NUM */
#define NM_NUMBER_OF_CHANNELS               1/* the sum of all NM channel config nodes  */
/*#define NM_NUMBER_OF_CONFIG_NODES_MAX       ((uint8)0xFFu)*/
/*Value must be an integer multiple of 8 and must not 
exceed the value of CANdb attribute 'NmMessageCount'.
*/

#define NM_MAINFUNCTION_PERIOD              (5)

#define NM_EXTEND_CAN_ID_MASK               (255UL)/*(0xffU)*/ /*((uint32)(0x000000ffu))*/
#define NmExtendIdToStand(ExtendedCanId)    ((((uint32)(ExtendedCanId)) & NM_EXTEND_CAN_ID_MASK)+ (NM_PDU_BASICID) )
                                            
#define NM_CAN_TXID                         (NM_PDU_BASICID + NM_OWN_STATION)
#define GetNmRxMsgBasic(RxCanID)            ((uint32)((RxCanID) & NM_PDU_BASICID_MASK))/*not used*/
#define GetNmRxMsgSrcID(RxCanID)            ((uint8)((RxCanID) & NM_PDU_SOUCEID_MASK))




/*indicate whether the Can Controller Buffer can be cleared*/
#define CAN_MSGBUFFER_CLRLESS                STD_ON/*not use*/ 

#define T_ACTIVE_MINI_ON                     STD_ON
#define T_LIMPHOME_MINI_ON                   STD_ON
#define NORWBSCANCEL_INDCLR                  STD_OFF/*not use*/
#define LHTWBSCANCEL_INDCLR                  STD_OFF/*not use*/
#define LH_PREPSLEEP_INDCLR                  STD_OFF/*not use*/
#define LEAVE_WBS_INDORACK_CLR               STD_OFF/*not use*/

/*>=1*/
#define NM_CHN0_NUMBER_OF_CONFIG_NODES       128 /*1~256*/
#define NM_CHN1_NUMBER_OF_CONFIG_NODES       32  /*1~256*/

#define NM_CHN0_NUMBER_OF_CFG_BYTE ((uint8)(((NM_CHN0_NUMBER_OF_CONFIG_NODES) / 8) + 1))

#define NM_CHN1_NUMBER_OF_CFG_BYTE ((uint8)(((NM_CHN1_NUMBER_OF_CONFIG_NODES) / 8) + 1))


#if(NM_NUMBER_OF_CHANNELS >= 1)
    #define CN0_ID_STANDARD                  STD_ON
    #define CN0_REMOTE_WAKEUP                STD_ON
    #define CN0_NETWORK_CFG_NORMAL           STD_ON
    #define CN0_NETWORK_CFG_LIMPHOME         STD_ON
    #define CN0_EXT_CALLBACK                 STD_ON
    #define CN0_DEFAULT_ACTIVE               STD_ON
    #define CN0_DEFAULT_STARTED                 STD_ON
    #define CN0_USED_SWITCH                  STD_ON


    
#endif
#if(NM_NUMBER_OF_CHANNELS >= 2)
    #define CN1_ID_STANDARD                  STD_ON
    #define CN1_REMOTE_WAKEUP                STD_ON
    #define CN1_NETWORK_CFG_NORMAL            STD_ON
    #define CN1_NETWORK_CFG_LIMPHOME         STD_ON
    #define CN1_EXT_CALLBACK                 STD_ON
    #define CN1_DEFAULT_ACTIVE                 STD_ON
    #define CN1_DEFAULT_STARTED              STD_ON
    #define CN1_USED_SWITCH                     STD_ON

#endif




#if(NM_NUMBER_OF_CHANNELS >= 1)
    #if defined ( NM_BYTEORDER_LSB )
    #define CN0_CONFIGURATION       (((uint8)CN0_ID_STANDARD          <<0)|\
                                     ((uint8)CN0_REMOTE_WAKEUP        <<1)|\
                                     ((uint8)CN0_NETWORK_CFG_NORMAL    <<2)|\
                                     ((uint8)CN0_NETWORK_CFG_LIMPHOME  <<3)|\
                                     ((uint8)CN0_EXT_CALLBACK         <<4)|\
                                     ((uint8)CN0_DEFAULT_ACTIVE       <<5)|\
                                     ((uint8)CN0_DEFAULT_STARTED      <<6)|\
                                     ((uint8)CN0_USED_SWITCH          <<7)) /*gg*/
                                     
                                     
    #else
    #define CN0_CONFIGURATION       (((uint8)CN0_ID_STANDARD          <<7)|\
                                     ((uint8)CN0_REMOTE_WAKEUP        <<6)|\
                                     ((uint8)CN0_NETWORK_CFG_NORMAL    <<5)|\
                                     ((uint8)CN0_NETWORK_CFG_LIMPHOME  <<4)|\
                                     ((uint8)CN0_EXT_CALLBACK         <<3)|\
                                     ((uint8)CN0_DEFAULT_ACTIVE       <<2)|\
                                     ((uint8)CN0_DEFAULT_STARTED      <<1)|\
                                     ((uint8)CN0_USED_SWITCH          <<0))/*gg*/
                                     
    #endif
#endif

#if(NM_NUMBER_OF_CHANNELS >= 2)
    #if defined ( NM_BYTEORDER_LSB )
    #define CN1_CONFIGURATION       (((uint8)CN1_ID_STANDARD          <<0)|\
                                     ((uint8)CN1_REMOTE_WAKEUP        <<1)|\
                                     ((uint8)CN1_NETWORK_CFG_NORMAL    <<2)|\
                                     ((uint8)CN1_NETWORK_CFG_LIMPHOME  <<3)|\
                                     ((uint8)CN1_EXT_CALLBACK     <<4)|\
                                     ((uint8)CN1_DEFAULT_ACTIVE   <<5)|\
                                     ((uint8)CN1_DEFAULT_STARTED  <<6)|\
                                     ((uint8)CN1_USED_SWITCH      <<7)) /*gg*/
                                     
    #else
    #define CN1_CONFIGURATION       (((uint8)CN1_ID_STANDARD     <<7)|\
                                     ((uint8)CN1_REMOTE_WAKEUP    <<6)|\
                                     ((uint8)CN1_NETWORK_CFG_NORMAL        <<5)|\
                                     ((uint8)CN1_NETWORK_CFG_LIMPHOME   <<4)|\
                                     ((uint8)CN1_EXT_CALLBACK     <<3)|\
                                     ((uint8)CN1_DEFAULT_ACTIVE   <<2)|\
                                     ((uint8)CN1_DEFAULT_STARTED  <<1)|\
                                     ((uint8)CN1_USED_SWITCH     <<0))/*gg*/
                                     
    #endif
#endif


typedef union
{
    uint8 R;
    struct
    {
#if defined ( NM_BYTEORDER_LSB )
        uint8    NMIDStandardEn     :1;     /* byte0 bit0 */
        uint8    NMRemoteWakeUpEn   :1;     /* byte0 bit1 */
        uint8    NMCfgNormalEn    :1;     /* byte0 bit2 */
        uint8    NMCfgLimpHEn         :1;     /* byte0 bit3 */
        uint8    NMExtCallBackEn    :1;     /* byte0 bit4 */
        uint8    NMDefaultActive    :1;     /* byte0 bit5 */
        uint8    NMDefaultStarted   :1;     /* byte0 bit6 */
        uint8    NMUsedSwitch     :1;     /* byte0 bit7 */
#else
        uint8    NMUsedSwitch     :1;     /* byte0 bit0 */
        uint8    NMDefaultStarted   :1;     /* byte0 bit1 */
        uint8    NMDefaultActive    :1;     /* byte0 bit2 */
        uint8    NMExtCallBackEn    :1;     /* byte0 bit3 */
        uint8    NMCfgLimpHEn         :1;     /* byte0 bit4 */
        uint8    NMCfgNormalEn    :1;     /* byte0 bit5 */
        uint8    NMRemoteWakeUpEn   :1;     /* byte0 bit6 */
        uint8    NMIDStandardEn     :1;     /* byte0 bit7 */
#endif
    }B;
} nmChannelConfigUnion;


/*Description: This type represents the Data Field layout of the CAN message.*/
typedef union
{
    uint8 R[8];
    struct
    {
        uint8   DestID;                         /* byte0 */
#ifdef NM_FOR_FAW        
        uint8   OpCode;                         /* byte1 */
#elif defined ( NM_BYTEORDER_LSB )
        uint8   OpCodeMsgType        :3;        /* byte1 bit0..bit2 */
        uint8   unused1              :1;        /* byte1 bit3 Reserved*/
        uint8   OpCodeSleepInd       :1;        /* byte1 bit4 */
        uint8   OpCodeSleepAck       :1;        /* byte1 bit5 */
        uint8   OpCodeRes            :2;        /* byte1 bit6 and bit7 Reserved */
#else
        uint8   OpCodeRes            :2;        /* byte1 bit7 and bit6 Reserved */
        uint8   OpCodeSleepAck       :1;        /* byte1 bit5 */
        uint8   OpCodeSleepInd       :1;        /* byte1 bit4 */
        uint8   unused1              :1;        /* byte1 bit3 Reserved*/
        uint8   OpCodeMsgType        :3;        /* byte1 bit2..bit0 */
#endif
        uint8   unused2 :8;                        /* byte2 */
        uint8   unused3 :8;                        /* byte3 */
        uint8   unused4 :8;                        /* byte4 */
        uint8   unused5 :8;                        /* byte5 */
        uint8   unused6 :8;                        /* byte6 */
        uint8   unused7 :8;                        /* byte7 */

    }B;
} nmDataFldTypeUnion;

#ifdef NM_FOR_FAW       
/*Description: This type represents the Message type and Sleep Indication.*/
typedef union
{
    uint8 R;
    struct
    {
#if defined ( NM_BYTEORDER_LSB )
        uint8   OpCodeMsgType        :3;        /* byte0 bit0..bit2 */
        uint8   unused1              :1;        /* byte0 bit3 Reserved*/
        uint8   OpCodeSleepInd       :1;        /* byte0 bit4 */
        uint8   OpCodeSleepAck       :1;        /* byte0 bit5 */
        uint8   OpCodeRes            :2;        /* byte0 bit6 and bit7 Reserved */
#else
        uint8   OpCodeRes            :2;        /* byte0 bit7 and bit6 Reserved */
        uint8   OpCodeSleepAck       :1;        /* byte0 bit5 */
        uint8   OpCodeSleepInd       :1;        /* byte0 bit4 */
        uint8   unused1              :1;        /* byte0 bit3 Reserved*/
        uint8   OpCodeMsgType        :3;        /* byte0 bit2..bit0 */
#endif
    }B;
}nmMsgIndUnion;
#endif

typedef uint8 nmNetIdType;
typedef uint8 nmRxTxCntType;
typedef uint16 nmTimerCntType;

/* Defines for NM message bits */
#define ALIVE                    (1)
#define RING                     (2)
#define LIMPHOME                 (4)
#define SLEEP_IND                (16)/*Not used*/
#define SLEEP_ACK                (32)/*Not used*/

#ifdef NM_FOR_FAW           
/* Defines for NM message type and indication bits */
#define MSG_ALIVE                (0x02)
#define MSG_ALIVE_IND            (0x06)
#define MSG_RING                 (0x01)
#define MSG_RING_IND             (0x05)
#define MSG_RING_IND_ACK         (0x07)
#define MSG_RING_IND_ACK1        (0x03)
#define MSG_LIMPHOME             (0x00)
#define MSG_LIMPHOME_IND         (0x04)
#endif

/*#define NmNumOfCfgNode(NmChannelId)  NmNumOfCfgNodeAry[NmChannelId]   */
#define NmToLogCanChn(NmChannelId)   NmToLogCanChnAry[NmChannelId]
#define LogCanToNmChn(LogCanCtrId)   LogCanToNmChnAry[LogCanCtrId]

#define NMCanInit(nmCurChannel)   /*Can and CanIf INIT */
#define NMCanTransmit()                      \
    CanIf_Transmit(NMTXPDUID,&NmTxPduInfo)
#define NmCanInterruptRestore()              \
    Can_EnableGlobalInterrupts()
#define NmCanInterruptDisable()              \
    Can_DisableGlobalInterrupts()
#define NmCanTxOffline(NmChanneld)         \
    CanIf_SetPduMode(NmToLogCanChn(NmChanneld),CANIF_SET_TX_OFFLINE)
#define NmCanTxOnline(NmChanneld)            \
    CanIf_SetPduMode(NmToLogCanChn(NmChanneld),CANIF_SET_TX_ONLINE)
#define NmCanSetModeStart(NmChanneld)            \
    (void)CanIf_SetControllerMode(NmToLogCanChn(NmChanneld),CANIF_CS_STARTED)
/*#define NmCanSleep(NmChanneld)            \
    CanIf_SetControllerMode(NmToLogCanChn(NmChanneld),CANIF_CS_SLEEP)*/
#define NmCanClearBuffer(NmChanneld)        \
    CanIf_ClearTransmitBuffer(NmToLogCanChn(NmChanneld))

#define NMBusErrorCanInit(NmChanneld)   \
    Can_InitController((NmToLogCanChn(NmChanneld)),\
    (P2CONST(Can_ControllerBaudrateConfigType, AUTOMATIC, CAN_CONFIG_CONST))(0))
    
#define NMBusErrorCanStart(NmChanneld)   \
    (void)CanIf_SetControllerMode(NmToLogCanChn(NmChanneld),CANIF_CS_STARTED)

/*constant configuration array*/
/*Configuration for each channel*/

#define  OSEKNM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"/* PRQA S 5087 */

extern CONST(nmChannelConfigUnion,OSEKNM_CONFIG_CONST) 
    nmChannelConfigAry[NM_NUMBER_OF_CHANNELS];
extern CONST(uint8,OSEKNM_CONFIG_CONST) 
    NmNumOfCfgNodeAry[NM_NUMBER_OF_CHANNELS];
extern CONST(uint8, OSEKNM_CONFIG_CONST) 
    NmToLogCanChnAry[NM_NUMBER_OF_CHANNELS];
extern CONST(uint8, OSEKNM_CONFIG_CONST) 
    LogCanToNmChnAry[NM_CANIF_USED_CONTROLLER_NUM];
/*extern CONST(uint8, OSEKNM_CONFIG_CONST)
    CanIfToNmChnAry[NM_NUMBER_OF_CHANNELS];*/
/*NM TxPDUID in CanIfTxPduConfig*/
extern CONST(PduIdType, OSEKNM_CONFIG_CONST) nmTxPduIdAry[NM_NUMBER_OF_CHANNELS];
/*the node ID for different channel.*/
extern CONST(nmNetIdType, OSEKNM_CONFIG_CONST) 
    nmChannelNodeID[NM_NUMBER_OF_CHANNELS];
/* NM Basic ID in the network */
extern CONST(uint32, OSEKNM_CONFIG_CONST) 
    nmPduBasicIDAry[NM_NUMBER_OF_CHANNELS];
/* NM Message idbase mask*/
extern CONST(uint32, OSEKNM_CONFIG_CONST) 
    nmPduBasicIDMaskAry[NM_NUMBER_OF_CHANNELS];
/*NM Message source id mask*/
extern CONST(uint32, OSEKNM_CONFIG_CONST) 
    nmPduSouceIDMaskAry[NM_NUMBER_OF_CHANNELS];

extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountTypAry[NM_NUMBER_OF_CHANNELS];
extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountMaxAry[NM_NUMBER_OF_CHANNELS];
extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountErrAry[NM_NUMBER_OF_CHANNELS];
extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountWbsAry[NM_NUMBER_OF_CHANNELS];
extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountTxAry [NM_NUMBER_OF_CHANNELS];
extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountBusoffFastTimerAry[NM_NUMBER_OF_CHANNELS];
extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountBusoffSlowTimerAry[NM_NUMBER_OF_CHANNELS];
#if defined (NM_FOR_FAW) 
#if(T_ACTIVE_MINI_ON == STD_ON)                        
extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountActMinAry[NM_NUMBER_OF_CHANNELS];
#endif
#if(T_LIMPHOME_MINI_ON == STD_ON)
extern CONST(nmTimerCntType, OSEKNM_CONFIG_CONST) 
    nmCountTLimpHAry[NM_NUMBER_OF_CHANNELS];
#endif
#endif
extern CONST(uint16,OSEKNM_CONFIG_CONST) 
    nmBusOffFastRecoveryAry[NM_NUMBER_OF_CHANNELS]; 
extern CONST(uint16, OSEKNM_CONFIG_CONST)
    nmBusOffSlowRecoveryAry[NM_NUMBER_OF_CHANNELS];
extern CONST(nmRxTxCntType, OSEKNM_CONFIG_CONST) 
    nmRxLimitAry[NM_NUMBER_OF_CHANNELS];
extern CONST(nmRxTxCntType, OSEKNM_CONFIG_CONST) 
    nmTxLimitAry[NM_NUMBER_OF_CHANNELS];
    
extern CONST(uint8,OSEKNM_CONFIG_CONST) 
    nmNumberOfCfgNodeAry[NM_NUMBER_OF_CHANNELS];

   

extern CONSTP2VAR(uint8,OSEKNM_CONFIG_DATA,OSEKNM_CONFIG_DATA) 
    nmNormalCfgPtrAry[NM_NUMBER_OF_CHANNELS];
extern CONSTP2VAR(uint8,OSEKNM_CONFIG_DATA,OSEKNM_CONFIG_DATA) 
    nmLimpHomeCfgPtrAry[NM_NUMBER_OF_CHANNELS];


#define  OSEKNM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"/* PRQA S 5087 */

#endif  /* _OSEKNM_CFG_H_ */
