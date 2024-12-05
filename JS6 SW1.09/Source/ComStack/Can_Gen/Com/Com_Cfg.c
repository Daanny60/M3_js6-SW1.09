/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE
*   This software is the property of HiRain Technologies. Any information
*   contained in this doc should not be reproduced, or used, or disclosed
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       Com_Cfg.c
********************************************************************************
*   Project/Product: COM programme
*   Title:           Com_Cfg.c
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

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "Com.h"
#include "Com_Cfg.h"
#include "keyboard_config.h"

#if(COM_TXIPDUNUM>=1)
Ipdu_TxMessage1_bufType  Ipdu_TxMessage1;

COM_CONST Ipdu_TxMessage1_bufType        Ipdu_TxMessage1_DefualtValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0,
    }
};


COM_CONST PduInfoType COM_TxDataInitInfo[COM_TXIPDUNUM] = {
    {Ipdu_TxMessage1._c, 8u},
};
/*M3 配置不同帧上的信号数量*/
COM_CONST COM_UINT8 COM_TxIpduSigNum[COM_TXIPDUNUM] = {
    5u,
};

/*M3 信号所在的消息ID 信号发送方式*/
COM_CONST   COM_TxSigStruct COM_TxSigTable[COM_TXSIGNUM] = {
    { 0u, COM_SENDTYPECYCLIC, (Com_SigInActiveType *)0},
    { 0u, COM_SENDTYPECYCLIC, (Com_SigInActiveType *)0},
    { 0u, COM_SENDTYPECYCLIC, (Com_SigInActiveType *)0},
    { 0u, COM_SENDTYPECYCLIC, (Com_SigInActiveType *)0},
    { 0u, COM_SENDTYPECYCLIC, (Com_SigInActiveType *)0},
    
};

COM_CONST COM_UINT8 *COM_TxIpduDefaultValue[COM_TXIPDUNUM] = {
    Ipdu_TxMessage1_DefualtValue._c,
};

COM_CONST COM_UINT8 COM_IpduTxTypeTable[COM_TXIPDUNUM] = {
    (COM_SENDTYPECYCLIC | COM_SENDTYPETIMEOUTMONITOR),
};

COM_CONST COM_UINT16 COM_IdpuTxCycCntTable[COM_TXIPDUNUM] = {
    100u,
};
/*这个一直没理解啥意思*/
COM_CONST COM_UINT16 COM_IdpuTxFastCycleTable[COM_TXIPDUNUM] = {
    0u,
    
};

COM_CONST COM_UINT16 COM_TxIpduOffsetInTxSigTable[COM_TXIPDUNUM] = {
    0u,
    
};
COM_CONST COM_UINT16 COM_IpduTxDelayCntTable[COM_TXIPDUNUM] = {
    0,
    
};
COM_CONST COM_UINT16 COM_IpduRepetitionTxCycCntTable[COM_TXIPDUNUM] = {
    40,
};
COM_CONST COM_UINT8 COM_IpduRepetitionTxNumTable[COM_TXIPDUNUM] = {
    3,
};
COM_CONST COM_UINT16 COM_IpduTxTimeOutCntTable[COM_TXIPDUNUM] = {
    500,
};
COM_CONST COM_UINT16 COM_IpduTxOffsetTimerTable[COM_TXIPDUNUM] = {
    0,
};

#ifdef COM_ENABLE_TX_TO_INDFUN
COM_CONST COM_TxTimeOutFun COM_IpduTxTimeOutFunPtr[COM_TXIPDUNUM] = {
    AppIpdu_Message1_TxTOIndication,
};
#endif

#ifdef COM_ENABLE_TX_CONFIRMATIONFUN
COM_CONST COM_TxConfirmationFun COM_IpduTxConfirmFunPtr[COM_TXIPDUNUM] = {
    AppTxIpdu_Message1_Conf,
};
#endif

COM_CONST COM_UINT16 COM_UnderlyHandle[COM_TXIPDUNUM] = {
    0u
};

#endif /*#if(COM_TXIPDUNUM>=1)*/

#if(COM_RXIPDUNUM>=1)
RxIpdu_Message1_bufType  RxIpdu_Message1;

RxIpdu_Message2_bufType RxIpdu_Message2;

//193
Ipdu_193_bufType RxIpdu_193;

//1F4
Ipdu_1F4_bufType RxIpdu_1F4;

//23C
Ipdu_23C_bufType RxIpdu_23C;


COM_CONST RxIpdu_Message1_bufType     RxIpdu_Message1_DefualtValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0,0x0, 0x0, 0x0, 0x0,
    }
};

COM_CONST RxIpdu_Message2_bufType     RxIpdu_Message2_DefualtValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0,
    }
};


COM_CONST Ipdu_193_bufType     RxIpdu_193DefualtValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    }
};

COM_CONST Ipdu_1F4_bufType     RxIpdu_1F4DefualtValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    }
};

COM_CONST Ipdu_23C_bufType     RxIpdu_23CDefualtValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    }
};





#ifdef COM_ENABLE_RX_TIMEOUTVAULE



COM_CONST RxIpdu_Message1_bufType        RxIpdu_Message1_TimeoutValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0,
    }
};

COM_CONST RxIpdu_Message2_bufType          RxIpdu_Message2_TimeoutValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0
    }
};

//193 TimeoutValue
COM_CONST Ipdu_193_bufType          RxIpdu_193TimeoutValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
    }
};

//1F4 TimeoutValue
COM_CONST Ipdu_1F4_bufType          RxIpdu_1F4TimeoutValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    }
};

//23C TimeoutValue
COM_CONST Ipdu_23C_bufType          RxIpdu_23CTimeoutValue = {
    {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    }
};


#endif
COM_CONST PduInfoType COM_RxDataInitInfo[COM_RXIPDUNUM] = {
    {RxIpdu_Message1._c, 8u},
    {RxIpdu_Message2._c, 8u},
};

COM_CONST COM_UINT8 *COM_RxIpduDefaultValue[COM_RXIPDUNUM] = {
    RxIpdu_Message1_DefualtValue._c,
    RxIpdu_Message2_DefualtValue._c,
};

//#ifdef COM_ENABLE_RX_TIMEOUTVAULE
COM_CONST COM_UINT8 *COM_IpduRxTimeoutValue[COM_RXIPDUNUM] = {
    RxIpdu_Message1_TimeoutValue._c,
    RxIpdu_Message2_TimeoutValue._c,
};
//#endif

COM_CONST ComTpBufferSize COM_TpRxIpduBufferSize[COM_RXIPDUNUM] = {
    COM_RXIPDU_Message1_BUFFER_SIZE,
    COM_RXIPDU_Message2_BUFFER_SIZE,  
};

COM_CONST COM_UINT8 COM_RxIpduSigNum[COM_RXIPDUNUM] = {
    1u,
    1u,
};

COM_CONST COM_RxSigStruct COM_RxSigTable[COM_RXSIGNUM] = {
    {0u, 0u},
    {1u, 0u},
};

/*the offset of the signal in the ipdu*/
COM_CONST COM_UINT16 COM_RxIpduOffsetInRxSigTable[COM_RXIPDUNUM] = {
    0u,
    1u,
};

COM_CONST COM_IpduRxTimeOut_Struct COM_IpduRxTOTimerTable[COM_RXIPDUNUM] = {
    {0u, COM_RXTIMEOUTMONITOR_YES, 500u},     //560   no timeout
    {1u, COM_RXTIMEOUTMONITOR_YES, 100u},  //117   10ms   10*cycle=100
};


#ifdef COM_ENABLE_RX_INDICATIONFUN

COM_CONST COM_RxIndicationFun COM_IpduRxIndicationFunPtr[COM_RXIPDUNUM] = {
    AppIpdu_Message1_Ind,//HeatCommand
    AppIpdu_Message2_Ind,//EngineState
};
#endif

#ifdef COM_ENABLE_RX_TIMEOUTINDICATIONFUN

COM_CONST COM_TimeoutIndicationFun COM_IpduRxTimeoutFunPtr[COM_RXIPDUNUM] = {
    AppRxIpdu_Message1_TimeOutInd,
    AppRxIpdu_Message2_TimeOutInd,
};
#endif


#endif /*#if(COM_RXIPDUNUM>=1)*/

#ifdef COM_ENABLE_ROUTESIGNAL
#if((COM_TXIPDUNUM>=1) && (COM_RXIPDUNUM>=1) && (COM_GWIPDUNUM>=1) && (COM_GWSIGNUM>=1))
COM_CONST COM_SignalGatewayInfoType COM_SignalGatewayInfo[COM_GWSIGNUM] = {
};

COM_CONST COM_SignalGatewayIpduInfoType COM_SignalGatewayIpduInfo[COM_GWIPDUNUM] = {
};
#endif/*#if((COM_TXIPDUNUM>=1) && (COM_RXIPDUNUM>=1) && (COM_GWIPDUNUM>=1) && (COM_GWSIGNUM>=1))*/
#endif/*#ifdef COM_ENABLE_ROUTESIGNAL6*/

#if(COM_IPDUGROUPNUM>=1)
COM_CONST COM_GroupIpduInfoType COM_GroupIpduInfo[] = {
    {0, COM_DIRECTION_TX},
    
    {0, COM_DIRECTION_RX},
    {1, COM_DIRECTION_RX},
};
COM_CONST COM_IpduGroupInfoType COM_IpduGroupInfo[COM_IPDUGROUPNUM] = {
    {0, 1},
    {1, 2},
};
#endif
