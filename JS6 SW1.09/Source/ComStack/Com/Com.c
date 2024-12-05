/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       Com.c
********************************************************************************
*   Project/Product : COM programme
*   Title:          : Com.c
*   Author:         : bo.liu       2010-2011
*                     donger.yang  2012-
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
*   01.00.00  10/08/2010  bo.liu        N/A     Original
*   01.00.01  04/07/2011  bo.liu        N/A     HrAsrCom110407-01
*   01.00.02  05/08/2011  bo.liu        N/A     HrAsrCom110508-01
*   01.00.03  05/20/2011  bo.liu        N/A     HrAsrCom110520-01
*   01.01.00  03/05/2012  donger.yang   N/A     HrAsrCom120305-01
*   02.00.00  05/03/2012  donger.yang   N/A     HrAsrCom120503-01
*   02.01.00  07/01/2012  donger.yang   N/A     HrAsrCom120701-01
*   02.01.01  07/12/2012  donger.yang   N/A     HrAsrCom120712-01
*   02.01.02  07/22/2012  donger.yang   N/A     HrAsrCom120722-01
*   02.01.03  08/01/2012  donger.yang   N/A     HrAsrCom120801-01
*   02.01.04  08/10/2012  donger.yang   N/A     HrAsrCom120810-01
*   02.01.05  08/14/2012  donger.yang   N/A     HrAsrCom120814-01
*   02.01.06  08/17/2012  donger.yang   N/A     HrAsrCom120817-01
*   02.01.07  08/22/2012  donger.yang   N/A     HrAsrCom120822-01
*   02.01.08  08/25/2012  donger.yang   N/A     HrAsrCom120825-01
*   02.01.09  09/04/2012  donger.yang   N/A     HrAsrCom120904-01
*   02.02.00  09/11/2012  donger.yang   N/A     HrAsrCom120911-01
*   02.02.01  10/10/2012  donger.yang   N/A     HrAsrCom121010-01
*   02.02.02  10/17/2012  donger.yang   N/A     HrAsrCom121017-01
*   02.02.03  10/19/2012  donger.yang   N/A     HrAsrCom121019-01
*   02.02.04  10/22/2012  donger.yang   N/A     HrAsrCom121022-01
*   03.00.00  11/08/2012  donger.yang   N/A     HrAsrCom121108-01
*   03.00.01  01/06/2013  donger.yang   N/A     HrAsrCom130106-01
*   03.00.02  03/25/2013  donger.yang   N/A     HrAsrCom130325-01
*   03.00.03  05/03/2013  donger.yang   N/A     HrAsrCom130503-01
*   03.00.04  06/08/2013  donger.yang   N/A     HrAsrCom130608-01 
*   03.00.05  07/30/2013  donger.yang   N/A     HrAsrCom130730-01 
*   03.00.06  08/26/2013  donger.yang   N/A     HrAsrCom130826-01
********************************************************************************
*END_FILE_HDR*/

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "fm33lg0xx_fl.h"
#include "Com.h"

/*******************************************************************************
*   Macro definitions 
*******************************************************************************/
#define COM_SINGLEFRAMEMAXLENGTH   (8u)
#define COM_ALLRUNNING             ((uint8)0xFF)
#define COM_ERROROPERATION         ((PduLengthType)0xFFFF)
/*******************************************************************************
*   Local Function Declaration 
*******************************************************************************/
#define COM_START_SEC_PRIVATE_CODE
#include "MemMap.h"

STATIC FUNC(void, COM_PRIVATE_CODE) Com_DataCopy
( 
    P2VAR(uint8, COM_APPL_DATA, COM_AUTOMOTIVE) dest, 
    P2VAR(uint8, COM_APPL_DATA, COM_AUTOMOTIVE) src,
    PduLengthType cnt
);


#if (COM_RXIPDUNUM >= 1u)

STATIC FUNC(void, COM_PRIVATE_CODE) Com_CopyRxPDU
(
    PduIdType ComRxPduId,
    P2CONST(PduInfoType, COM_APPL_DATA, COM_AUTOMOTIVE) PduInfo
);

STATIC FUNC(void, COM_PRIVATE_CODE) Com_RxInit
(
    void 
);

STATIC FUNC(void, COM_PRIVATE_CODE) Com_InitRxIpdu
(
    PduIdType ComRxIpduId
);

/*#if (COM_RXIPDUNUM >= 1u)*/
#endif

#if (COM_TXIPDUNUM >= 1u)

STATIC FUNC(void, COM_PRIVATE_CODE) Com_TxInit
(
    void
);

STATIC FUNC(void, COM_PRIVATE_CODE) Com_InitTxIpdu
(
    PduIdType ComTxIpduId
);

/*#if (COM_TXIPDUNUM >= 1u)*/
#endif


#define COM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

#define COM_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
/*******************************************************************************
*   Global Data Define 
*******************************************************************************/
#if (COM_RXIPDUNUM >= 1u)

/* Message Indication Flag */
#ifdef COM_ENABLE_RX_INDICATIONFLAG
VAR(boolean, COM_APPL_DATA) Com_RxIpduIndicationFlag[COM_RXIPDUNUM];
#endif

#ifdef COM_ENABLE_RX_TIMEOUTINDICATIONFLAG
VAR(boolean, COM_APPL_DATA) Com_RxIpduTimeOutFlag[COM_RXIPDUNUM];
#endif

#endif

#if (COM_TXIPDUNUM >= 1u)

VAR(PduInfoType, COM_APPL_DATA) COM_TxDataPtr[COM_TXIPDUNUM];
/* Ipdu timeout flag */
#ifdef COM_ENABLE_TX_TO_INDFLAG
VAR(boolean, COM_APPL_DATA) Com_TxIpduTimeOutFlag[COM_TXIPDUNUM];
#endif

/* Ipdu tx confirmation flag */
#ifdef COM_ENABLE_TX_CONFIRMATIONFLAG
VAR(boolean, COM_APPL_DATA) Com_TxIdpuConfFlag[COM_TXIPDUNUM];
#endif

#endif

/*******************************************************************************
*   Local Data Define 
*******************************************************************************/
#if (COM_RXIPDUNUM >= 1u)

#define COM_RXIPDUFLAGSIZE  \
        ((uint16)(((COM_RXIPDUNUM - 1u) >> 3u) + 1u))

/* This variable contains a Rx timeout flag for each Rx timeout counter to 
indicate the reception of the timeout supervised message. */
STATIC VAR(uint8, COM_PRIVATE_DATA) Com_IpduReceivedFlag[COM_RXIPDUNUM];
STATIC VAR(uint8, COM_PRIVATE_DATA) Com_RxIpduPreStatus[COM_RXIPDUNUM];

/* This variable contains the Rx timeout counter */
STATIC VAR(Com_TimerType, COM_PRIVATE_DATA) 
    Com_IpduRxTimeoutCounter[COM_RXIPDUNUM];

#define Com_GetRxTimeoutTime(IpduId) ( (Com_TimerType)\
    (COM_IpduRxTOTimerTable[IpduId].RxTOTimer / COM_RXMAIN_FUNCTION_PERIOD) )

#define Com_ResetRxTimeoutTimer(IpduId) ( Com_IpduRxTimeoutCounter[IpduId] = \
    Com_GetRxTimeoutTime(IpduId) ) 

STATIC VAR(PduLengthType, COM_PRIVATE_DATA) 
    Com_TpRxIpduAvailableBufferSize[COM_RXIPDUNUM];

STATIC VAR(PduInfoType, COM_PRIVATE_DATA) COM_RxDataPtr[COM_RXIPDUNUM];//111111111111111111111111111111111111111



STATIC VAR(uint8, COM_PRIVATE_DATA) Com_RxIpduStartedFlag[COM_RXIPDUFLAGSIZE];

/* for Tp */
STATIC P2VAR(uint8, COM_APPL_DATA, COM_AUTOMOTIVE) 
    Com_TpRxIpduPtr[COM_RXIPDUNUM];

#endif
#if (COM_TXIPDUNUM >= 1u)

#define COM_TXIPDUFLAGSIZE \
        ((uint16)(((COM_TXIPDUNUM - 1u) >> 3u) + 1u))

STATIC VAR(uint8, COM_PRIVATE_DATA) Com_TxIpduPreStatus[COM_TXIPDUNUM];

#ifdef COM_ENABLE_RETRANSMIT
STATIC VAR(boolean, COM_PRIVATE_DATA) Com_ReTransmit[COM_TXIPDUNUM];
#endif

STATIC VAR(uint8, COM_PRIVATE_DATA) Com_TxState[COM_TXIPDUNUM];

STATIC VAR(Com_TimerType, COM_PRIVATE_DATA) 
    Com_TxIpduCycleCntBackup[COM_TXIPDUNUM];

STATIC VAR(uint8, COM_PRIVATE_DATA) Com_TxIpduStartedFlag[COM_TXIPDUFLAGSIZE];

/* Cyclic Tx counter */
STATIC VAR(Com_TimerType, COM_PRIVATE_DATA) Com_IpduTxCycCounter[COM_TXIPDUNUM];

#define Com_GetTxCycle(IpduId)  ( (Com_TimerType)\
    (COM_IdpuTxCycCntTable[IpduId] / COM_TXMAIN_FUNCTION_PERIOD) )

#define Com_ResetTxCycleTimer(IpduId) ( Com_IpduTxCycCounter[IpduId] = \
    Com_GetTxCycle(IpduId) ) 

#define Com_GetTxRepCycle(IpduId)  ( (Com_TimerType)\
    (COM_IpduRepetitionTxCycCntTable[IpduId] / COM_TXMAIN_FUNCTION_PERIOD) )

#define Com_ResetTxRepCycleTimer(IpduId) ( Com_IpduTxCycCounter[IpduId] = \
    Com_GetTxRepCycle(IpduId) ) 

#define Com_LoadTxBackupTimer(IpduId) ( Com_TxIpduCycleCntBackup[IpduId] = \
    Com_IpduTxCycCounter[IpduId] )

#define Com_ResetTxBackupTimer(IpduId)  ( Com_TxIpduCycleCntBackup[IpduId] = \
    Com_GetTxCycle(IpduId) )

/* Ipdu and Signal Active property */
#ifdef COM_ENABLE_ACTIVEPORPERTY
#define Com_GetTxFastCycle(IpduId)  ( (Com_TimerType)\
    (COM_IdpuTxFastCycleTable[IpduId] / COM_TXMAIN_FUNCTION_PERIOD) )

#define Com_ResetTxFsatCycleTimer(IpduId) ( Com_IpduTxCycCounter[IpduId]= \
    Com_GetTxFastCycle(IpduId) ) 


#define Com_ResetTxCycleFromFastCycle(IpduId) ( Com_IpduTxCycCounter[IpduId] =\
    Com_GetTxCycle(IpduId) - Com_GetTxFastCycle(IpduId) + \
    Com_IpduTxCycCounter[IpduId] )

#define COM_TXSIGNALFLAGSIZE   \
         ((uint16)(((COM_TXSIGNUM - 1u) >> 3u) + 1u))

STATIC VAR(uint8, COM_PRIVATE_DATA) Com_SigActivedFlag[COM_TXSIGNALFLAGSIZE];
STATIC VAR(uint8, COM_PRIVATE_DATA) Com_IpduActiveFlag[COM_TXIPDUFLAGSIZE];

#endif


/* This variable contains the Tx delay counter. If the value is !=0, the 
transmission of messages is not allowed. */
STATIC VAR(Com_TimerType, COM_PRIVATE_DATA) 
    Com_IpduTxDelayCounter[COM_TXIPDUNUM];

#define Com_GetTxDelayTime(IpduId) ( (Com_TimerType)\
    (COM_IpduTxDelayCntTable[IpduId] / COM_TXMAIN_FUNCTION_PERIOD) )

#define Com_ResetTxDelayTimer(IpduId) ( Com_IpduTxDelayCounter[IpduId] = \
    Com_GetTxDelayTime(IpduId) ) 

STATIC VAR(boolean, COM_PRIVATE_DATA) Com_IpduRepetitionFlag[COM_TXIPDUNUM];

#define Com_SetIpduRepetionFlag(IpduId) \
        (Com_IpduRepetitionFlag[IpduId] = COM_YES)

#define Com_ClearIpduRepetionFlag(IpduId) \
        (Com_IpduRepetitionFlag[IpduId] = COM_NO)

/* Message Repetition counter */
STATIC VAR(uint8, COM_PRIVATE_DATA) Com_NumOfTxRepetition[COM_TXIPDUNUM];

#define Com_SetTxRepNumber(IpduId)  ( Com_NumOfTxRepetition[IpduId] = \
    COM_IpduRepetitionTxNumTable [IpduId] )

/* Tx Ipdu timeout counter */
STATIC VAR(Com_TimerType, COM_PRIVATE_DATA) Com_IpduTxTimeOutCnt[COM_TXIPDUNUM];

#define Com_GetTxTimeoutTime(IpduId)  ( (Com_TimerType)\
    (COM_IpduTxTimeOutCntTable[IpduId] / COM_TXMAIN_FUNCTION_PERIOD) )

#define Com_ResetTxTimeoutTimer(IpduId) ( Com_IpduTxTimeOutCnt[IpduId] = \
    Com_GetTxTimeoutTime(IpduId) )


STATIC VAR(Com_TimerType, COM_PRIVATE_DATA) 
    Com_IpduTxOffSetCounter[COM_TXIPDUNUM];

#define Com_GetTxOffsetTime(IpduId) ( (Com_TimerType)\
    (COM_IpduTxOffsetTimerTable[IpduId] / COM_TXMAIN_FUNCTION_PERIOD) )

#define Com_ResetTxOffsetTimer(IpduId) ( Com_IpduTxOffSetCounter[IpduId] = \
    Com_GetTxOffsetTime(IpduId) ) 


#ifdef COM_CALLBACK_POLLING
STATIC VAR(boolean, COM_PRIVATE_DATA) Com_TxConfirmationFlag[COM_TXIPDUNUM];
#endif

STATIC VAR(boolean, COM_PRIVATE_DATA) Com_IpduBeingSentFlag[COM_TXIPDUNUM];

#define Com_SetIpduBeingSentFlag(IpduId)    \
        (Com_IpduBeingSentFlag[IpduId]=COM_YES)

#define Com_ClearIpduBeingSentFlag(IpduId)  \
        (Com_IpduBeingSentFlag[IpduId]=COM_NO)

/* for Tp */
STATIC VAR(Com_LpduInfoType, COM_PRIVATE_DATA) Com_TxIpduInfo[COM_TXIPDUNUM];

#endif


#ifdef COM_ENABLE_ROUTESIGNAL
#if( (COM_TXIPDUNUM >= 1u) && (COM_RXIPDUNUM >= 1u) && \
     (COM_GWIPDUNUM >= 1u) && (COM_GWSIGNUM >= 1u) )

STATIC VAR(boolean, COM_PRIVATE_DATA) 
    COM_SignalGatewayUpdateFlag[COM_GWIPDUNUM];

#define Com_GetSigPosStart(IpduId) \
    (COM_SignalGatewayIpduInfo[IpduId].OffsetInSignalGatewayInfo)

#define Com_GetSigPosEnd(IpduId)   (Com_GetSigPosStart(IpduId) + \
    COM_SignalGatewayIpduInfo[IpduId].SigGWNum)

#define Com_GetSigRxId(SigId)   (COM_SignalGatewayInfo[SigId].RxSignalId)

#define Com_GetSigTxId(SigId)   (COM_SignalGatewayInfo[SigId].TxSignalId)

#endif

#endif

STATIC VAR(uint8, COM_PRIVATE_DATA) ComRxTimeoutMoniFlag = 0U;
STATIC VAR(uint16, COM_PRIVATE_DATA) ComRxTimeoutMoniCnts = 0U;


#define COM_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

#if (COM_RXIPDUNUM >= 1u)

#define COM_START_SEC_PRIVATE_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_RxInit
*                
* Description:   rx initialize.
*                
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_RxInit
(
    void 
)
{

    PduIdType IpduIndex = 0u;

    for(IpduIndex = 0u; IpduIndex < COM_RXIPDUNUM; IpduIndex++)
    {
        Com_InitRxIpdu(IpduIndex);
    }

    for(IpduIndex = 0u; IpduIndex < COM_RXIPDUFLAGSIZE; IpduIndex++)
    {
        Com_RxIpduStartedFlag[IpduIndex] = COM_STOP; 
    }
  
}

/* BEGIN_FUNCTION_HDR
*******************************************************************************
* Function Name: Com_CopyRxPDU
*                  
* Description:   copy pdu data when data received.
*                  
* Inputs:        ComRxPduId,aubCanSduPtr
*                    
* Outputs:       None
* 
* Limitations:   None
*******************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_CopyRxPDU
(
    PduIdType ComRxPduId,
    P2CONST(PduInfoType, COM_APPL_DATA, COM_AUTOMOTIVE) PduInfo
)
{

    uint8 Index = 0u;
    PduLengthType PduLength = 0u;

    if(ComRxPduId < COM_RXIPDUNUM)
    {
        if(PduInfo->SduLength <= COM_TpRxIpduBufferSize[ComRxPduId]) 
        {
            PduLength = PduInfo->SduLength;
  
            if(COM_RxDataPtr[ComRxPduId].SduDataPtr != COM_NULL)
            {
                for (Index = 0u; Index < PduLength; Index++)
                {
                    /* copy message data from Can register */
                    COM_RxDataPtr[ComRxPduId].SduDataPtr[Index] = 
                    PduInfo->SduDataPtr[Index] ;     
                }     
            }
            else
            {
            /* do nothing */
            }        
        } 
        else
        {
        /* report error */
        }         
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_TpRxInfoInit
*                 
* Description:   Init Rx Buferr
*                 
* Inputs:        PduId: Receive PduId 
*                
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_TpRxInfoInit
(
    PduIdType PduId
)    
{

    Com_TpRxIpduPtr[PduId] = COM_RxDataPtr[PduId].SduDataPtr;
    Com_TpRxIpduAvailableBufferSize[PduId] = COM_TpRxIpduBufferSize[PduId]; 

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_InitRxIpdu
*                 
* Description:   used to Setup or shutdown a Ipdu specified by  ComIpduId 
*                 
* Inputs:        ComRxIpduId: Ipdu Id 
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_InitRxIpdu
(
    PduIdType ComRxIpduId
)
{

    uint8 DataIndex = 0u;
    
    if(ComRxIpduId<COM_RXIPDUNUM)
    {
        /* timeout timer reset */
        Com_ResetRxTimeoutTimer(ComRxIpduId);
        COM_RxDataPtr[ComRxIpduId].SduDataPtr = 
        COM_RxDataInitInfo[ComRxIpduId].SduDataPtr;

        COM_RxDataPtr[ComRxIpduId].SduLength = 
        COM_RxDataInitInfo[ComRxIpduId].SduLength;
    
        Com_TpRxIpduPtr[ComRxIpduId] = 
        COM_RxDataPtr[ComRxIpduId].SduDataPtr;

        Com_TpRxIpduAvailableBufferSize[ComRxIpduId] = 
        COM_TpRxIpduBufferSize[ComRxIpduId];
                
        /* Idpu Default Value */
        if( (COM_RxIpduDefaultValue[ComRxIpduId] != COM_NULL) && 
            (COM_RxDataPtr[ComRxIpduId].SduDataPtr != COM_NULL) )
        {
            for(DataIndex = 0u; 
                DataIndex < COM_TpRxIpduBufferSize[ComRxIpduId]; 
                DataIndex++)
            {
                COM_RxDataPtr[ComRxIpduId].SduDataPtr[DataIndex] = 
                COM_RxIpduDefaultValue[ComRxIpduId][DataIndex]; 
            }      
        }
        else
        {
        /* do nothing */
        }

        Com_RxIpduPreStatus[ComRxIpduId] = COM_STOP;
 
        Com_StopRxIpdu(ComRxIpduId);

        /* message Receive flag initialize; */
        Com_IpduReceivedFlag[ComRxIpduId] = COM_NO;
      
        #ifdef COM_ENABLE_RX_TIMEOUTINDICATIONFLAG
      
        /* timeout flag initialize; */
        Com_RxIpduTimeOutFlag[ComRxIpduId] = COM_NO;    
    
        #endif

        #ifdef COM_ENABLE_RX_INDICATIONFLAG

        /* indicationflag initialize; */
        Com_RxIpduIndicationFlag[ComRxIpduId] = COM_NO;

        #endif  
    }
    else
    {  
    /* do nothing */
    }
}

#define COM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

#define COM_START_SEC_PUBLIC_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_StartRxIpdu
*                
* Description:   Setup the Rx message which is specific by IpduIde
*                
* Inputs:        ComRxPduId, Rx message handle
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_StartRxIpdu
(
    PduIdType ComRxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u;

    if(ComRxPduId < COM_RXIPDUNUM)  
    {
        Position_Byte = (uint8)(ComRxPduId >> 3u);
        Position_Bit = (uint8)(ComRxPduId % 8u); 
        Com_RxIpduStartedFlag[Position_Byte] |= (1u << Position_Bit);          
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_StopRxIpdu
*                
* Description:   Stop the Rx message which is specific by IpduIde
*                
* Inputs:        ComRxPduId, Rx message handle
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_StopRxIpdu
(
    PduIdType ComRxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u; 

    if(ComRxPduId < COM_RXIPDUNUM)  
    {
        Position_Byte = (uint8)(ComRxPduId >> 3u);
        Position_Bit = (uint8)(ComRxPduId % 8u); 
        Com_RxIpduStartedFlag[Position_Byte] &= ((uint8)(~(1u << Position_Bit)));        
    } 
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_GetRxIpduStatus
*                
* Description:   Get the Rx message status which is specific by IpduIde
*                
* Inputs:        ComRxPduId, Rx message handle
*                   
* Outputs:       COM_STOP:    message is stopped
*                COM_RUNNING: message is started
*                COM_ERROR:   Error Happens
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(uint8, COM_PUBLIC_CODE) Com_GetRxIpduStatus
(
    PduIdType ComRxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u; 
    uint8 Result = COM_ERROR;

    if(ComRxPduId < COM_RXIPDUNUM)  
    {
        Position_Byte = (uint8)(ComRxPduId >> 3u);
        Position_Bit = (uint8)(ComRxPduId % 8u); 
        if( Com_RxIpduStartedFlag[Position_Byte] & (1u << Position_Bit) ) 
        {
            Result = COM_RUNNING;
        }
        else
        {
            Result = COM_STOP; 
        }
    }
    else
    {
    /* report error */
    }

    return  Result;

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_GetRxIpduLength
*                
* Description:   Get the Rx message's Length which is specific by IpduIde
*                
* Inputs:        ComRxPduId, Rx message handle
*                   
* Outputs:       If COM_ERROROPERATION returned, Rx Ipdu length's access failed
                 else the value returned is RxIpdu Length
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(uint16, COM_PUBLIC_CODE) Com_GetRxIpduLength
(
    PduIdType ComRxPduId
)
{

    uint16 RxIpduLength = COM_ERROROPERATION;

    if(ComRxPduId < COM_RXIPDUNUM)  
    {
        if(Com_GetRxIpduStatus(ComRxPduId) == COM_RUNNING)
        {
            RxIpduLength = COM_RxDataPtr[ComRxPduId].SduLength;
        }
        else
        {
            RxIpduLength = COM_ERROROPERATION;
        }
    }
    else
    {
        RxIpduLength = COM_ERROROPERATION;
        /* Report Error */
    }

    return  RxIpduLength;

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_RxStart
*                
* Description:   rx initialize.
*                
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE)  Com_RxStart
(
    void
)
{

    PduIdType IpduIndex = 0u;
    
    for(IpduIndex = 0u; IpduIndex < COM_RXIPDUFLAGSIZE; IpduIndex++)
    {
        Com_RxIpduStartedFlag[IpduIndex] = COM_ALLRUNNING; 
    }   

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_RxStop
*                
* Description:   stop rx.
*                
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_RxStop
( 
    void 
)
{

    PduIdType IpduIndex = 0u;
    
    for(IpduIndex = 0u; IpduIndex < COM_RXIPDUFLAGSIZE; IpduIndex++)
    {
        Com_RxIpduStartedFlag[IpduIndex] = COM_STOP; 
    } 

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_MainFunctionRx
*                
* Description:   rx task for Ipdu timeout monitor.
*                
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_MainFunctionRx
(
    void
)
{

    PduIdType PduIndex = 0u;
    uint32_t primask;
    #ifdef COM_ENABLE_RX_TIMEOUTVAULE    
    uint8 DataIndex = 0u;   
    #endif 

    for(PduIndex = 0u; PduIndex < COM_RXIPDUNUM; PduIndex++)
    {

        if(Com_GetRxIpduStatus(PduIndex) == COM_RUNNING)
        {
            if(Com_IpduReceivedFlag[PduIndex] != COM_NO)
            {    
                Com_ResetRxTimeoutTimer(PduIndex);

                Com_IpduReceivedFlag[PduIndex] = COM_NO;

                  #ifdef COM_CALLBACK_POLLING

                #ifdef COM_ENABLE_RX_INDICATIONFUN

                if(COM_IpduRxIndicationFunPtr[PduIndex] != COM_NULL)
                {
                    COM_IpduRxIndicationFunPtr[PduIndex]();
                }
                else
                {
                    /* do nothing */
                }

                #endif      
              
                #endif
            }
            else if(COM_IpduRxTOTimerTable[PduIndex].TOMonitored == 
                    COM_RXTIMEOUTMONITOR_YES)
            {   
				if(ComRxTimeoutMoniFlag == COM_BUSY)
				{
	                if(0u != Com_IpduRxTimeoutCounter[PduIndex])
	                {
	                    Com_IpduRxTimeoutCounter[PduIndex]--;
	                    if(0u == Com_IpduRxTimeoutCounter[PduIndex])
	                    {
	                        /* timeout timer reset */
	                        Com_ResetRxTimeoutTimer(PduIndex);

	                        /* call Ipdu timeout indication function */
	                        #ifdef COM_ENABLE_RX_TIMEOUTINDICATIONFUN


							
	                        if(COM_IpduRxTimeoutFunPtr[PduIndex] != COM_NULL) 
	                        {
	                            COM_IpduRxTimeoutFunPtr[PduIndex]();                        
	                        }
	                        else
	                        {
	                        /* do nothing */
	                        } 

	                        #endif

	                        #ifdef COM_ENABLE_RX_TIMEOUTINDICATIONFLAG

	                        Com_RxIpduTimeOutFlag[PduIndex] = COM_YES;

	                        #endif

	                        #ifdef COM_ENABLE_RX_TIMEOUTVAULE

	                        /* set default value for timeout signal/Ipdu */                        
	                        if(COM_IpduRxTimeoutValue[PduIndex] != COM_NULL && 
	                            (COM_RxDataPtr[PduIndex].SduDataPtr != COM_NULL))
	                        {
	                            for(DataIndex = 0u; 
	                                DataIndex < COM_TpRxIpduBufferSize[PduIndex]; 
	                                DataIndex++)
	                            {
	                                COM_RxDataPtr[PduIndex].SduDataPtr[DataIndex] =
	                                COM_IpduRxTimeoutValue[PduIndex][DataIndex]; 
	                            }                         
	                        }
	                        else
	                        {
	                        /* do nothing */
	                        }

	                        #endif  
							
	                    }
	                    else
	                    {
	                    /* do nothing */
	                    }
	                }
	                else
	                {
	                /* do nothing */
	                }
				}
            }           
            else
            {
            /*do nothing*/
            }                    
        }
        else
        {
        /* do nothing */
        }
    }
 
	Can_DisableGlobalInterrupts();
	if(ComRxTimeoutMoniFlag == COM_YES)
	{
		if(ComRxTimeoutMoniCnts != 0U) //500/COM_TXMAIN_FUNCTION_PERIOD;
		{
			ComRxTimeoutMoniCnts--;
			if(ComRxTimeoutMoniCnts == 0U) //500/COM_TXMAIN_FUNCTION_PERIOD;
			{
				ComRxTimeoutMoniFlag = COM_BUSY;
				for(PduIndex = 0u; PduIndex < COM_RXIPDUNUM; PduIndex++)
				{
					Com_ResetRxTimeoutTimer(PduIndex);
				}
			}
		}
		else
		{}
	}
	else
	{}
	Can_EnableGlobalInterrupts() ;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_RxIndication
*                
* Description:   rx indication when rx Ipdu.
*                
* Inputs:        ComRxPduId,aubData_p
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_RxIndication
(
    PduIdType RxPduId,
    P2CONST(PduInfoType, COM_APPL_DATA, COM_AUTOMOTIVE) PduInfoPtr
)
{
    #ifdef COM_ENABLE_ROUTESIGNAL

    #if( (COM_TXIPDUNUM >= 1) && (COM_RXIPDUNUM >= 1) && \
        (COM_GWIPDUNUM >= 1) && (COM_GWSIGNUM >= 1) )
    PduIdType PduIndex = 0u;
    #endif
    #endif
    /* Received Ipdu data copy */
    if(RxPduId < COM_RXIPDUNUM)
    {
        if(Com_GetRxIpduStatus(RxPduId) == COM_RUNNING)
        {
            Com_CopyRxPDU(RxPduId,PduInfoPtr);
     
            COM_RxDataPtr[RxPduId].SduLength = PduInfoPtr->SduLength;
            Com_RxIpduCallout(RxPduId);

            /* Ipdu received */
            Com_IpduReceivedFlag[RxPduId] = COM_YES;

            #ifdef COM_ENABLE_ROUTESIGNAL

            #if( (COM_TXIPDUNUM >= 1) && (COM_RXIPDUNUM >= 1) && \
                (COM_GWIPDUNUM >= 1) && (COM_GWSIGNUM >= 1) )

            /* set the SignalGatewayUpdateFlag */
            for(PduIndex = 0; PduIndex < COM_GWIPDUNUM; PduIndex++)
            {
                if(COM_SignalGatewayIpduInfo[PduIndex].RxPduId == RxPduId)
                {
                    COM_SignalGatewayUpdateFlag[PduIndex] = COM_YES;
                    break;
                }
                else
                {
                /* nothing to do */
                }
            }

            #endif

            #endif

            #ifndef COM_CALLBACK_POLLING

            #ifdef COM_ENABLE_RX_INDICATIONFUN

            if(COM_IpduRxIndicationFunPtr[RxPduId] != COM_NULL)
            {
                COM_IpduRxIndicationFunPtr[RxPduId]();
            }
            else
            {
            /* do nothing */
            }

            #endif

            #endif


            #ifdef COM_ENABLE_RX_INDICATIONFLAG

            /* set Rx indication flag */
            Com_RxIpduIndicationFlag[RxPduId] = COM_YES;    

            #endif      
        }
        else
        {
        /* do nothing */
        }
    
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_StartOfReception
*                 
* Description:   Callbak function Com provided by Com to J1939Tp
*                 
* Inputs:        ComRxPduId:      Receive PduId
*                TpSduLength:     Length in Single Frame or Fist Frame
*                RxBufferSizePtr: Avaliable buffer size Com can provided
*                   
* Outputs:       BUFREQ_E_NOT_OK, BUFREQ_E_BUSY and BUFREQ_E_OVFL
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(BufReq_ReturnType, COM_PUBLIC_CODE) Com_StartOfReception
(
    PduIdType ComRxPduId,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, COM_APPL_DATA, COM_AUTOMOTIVE) RxBufferSizePtr
)
{

    BufReq_ReturnType result = BUFREQ_OK;
  
    if(ComRxPduId < COM_RXIPDUNUM)    
    {
        if(Com_GetRxIpduStatus(ComRxPduId) == COM_RUNNING)  
        {
            if(TpSduLength > Com_TpRxIpduAvailableBufferSize[ComRxPduId])
            {
                result = BUFREQ_E_OVFL;
            }
            else
            {
                Com_TpRxIpduPtr[ComRxPduId] = 
                COM_RxDataPtr[ComRxPduId].SduDataPtr;

                COM_RxDataPtr[ComRxPduId].SduLength=TpSduLength;

                if(RxBufferSizePtr!=COM_NULL)
                {
                    *RxBufferSizePtr = 
                    (PduLengthType)Com_TpRxIpduAvailableBufferSize[ComRxPduId];
                }
                else
                {
                /* do nothing */
                }          
            }
        
            #ifdef COM_ENABLE_LPDURXSTART_INDICATIONFUN

            if(COM_IpduRxStartIndicationFunPtr[ComRxPduId] != COM_NULL)
            {
                COM_IpduRxStartIndicationFunPtr[ComRxPduId](result);
            }
            else
            {
            /* do nothing */
            }

            #endif                 
        }
        else
        {
            result = BUFREQ_E_NOT_OK;
        }   
    }
    else
    {
        /* report error */
        result = BUFREQ_E_NOT_OK;
    }
      
    return result;

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_CopyRxData
*                 
* Description:   Callbak function Comm provided by Comm to J1939Tp 
*                 
* Inputs:        CommRxPduId:     Receive PduId
*                PduInfoPointer:  Pointer to a PduInfoType which indicates
*                                 the number of bytes to be copied (SduLength) 
*                                 and the location of the source data 
*                                 (SduDataPtr).
*                RxBufferSizePtr: Remaining free place in receive buffer after
*                                 completion of this call.
*                  
* Outputs:       BUFREQ_OK, BUFREQ_E_BUSY
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(BufReq_ReturnType, COM_PUBLIC_CODE) Com_CopyRxData
(
    PduIdType PduId,
    P2CONST(PduInfoType, COM_APPL_DATA, COM_AUTOMOTIVE) PduInfoPointer,
    P2VAR(PduLengthType, COM_APPL_DATA, COM_AUTOMOTIVE) RxBufferSizePtr
)
{

    BufReq_ReturnType result = BUFREQ_OK;

    if(PduId<COM_RXIPDUNUM)
    {
        if(Com_GetRxIpduStatus(PduId) == COM_RUNNING) 
        {
            if( PduInfoPointer->SduLength <=  
                Com_TpRxIpduAvailableBufferSize[PduId] )
            {
                Com_DataCopy(Com_TpRxIpduPtr[PduId], 
                                PduInfoPointer->SduDataPtr, 
                                PduInfoPointer->SduLength);
          
                Com_TpRxIpduPtr[PduId] += 
                (uint8)(PduInfoPointer->SduLength); 
          
                  Com_TpRxIpduAvailableBufferSize[PduId] -= 
                (PduInfoPointer->SduLength);
          
                  if(RxBufferSizePtr != COM_NULL)
                  {    
                    *RxBufferSizePtr = 
                    (PduLengthType)Com_TpRxIpduAvailableBufferSize[PduId];    
                }
                else
                {
                /* do nothing */
                } 
            }
            else
            {
                result = BUFREQ_E_OVFL;
            }    
        }
        else
        {
            result = BUFREQ_E_NOT_OK;
        }
 
    }
    else
    {
        /* report error */
        result = BUFREQ_E_NOT_OK;    
    }
        
    return result;

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_TpRxIndication
*                 
* Description:   Callbak function Com provided by Com to J1939Tp 
*                 
* Inputs:        PduId:  Receive PduId
*                Result: NTFRSLT_OK means the complete N-PDU has been eceived
*                        and is stored in the receive buffer.Any other value 
*                        means the N_PDU has not been received,
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_TpRxIndication
(
    PduIdType PduId,
    NotifResultType Result
)
{

    PduIdType PduIndex = 0u;

    if( (PduId < COM_RXIPDUNUM) && (Com_GetRxIpduStatus(PduId) == COM_RUNNING) )
    {
        if(Result == NTFRSLT_OK)
        {
            Com_TpRxInfoInit(PduId);

            for(PduIndex = 0; PduIndex < COM_RXIPDUNUM; PduIndex++)
            {
                if(COM_IpduRxTOTimerTable[PduIndex].RxPduId == PduId)
                {
                    /* Ipdu received */
                    Com_IpduReceivedFlag[PduIndex] = COM_YES;
                    break;
                } 
                else
                {
                /* do nothing */
                }
            }
          
            #ifndef COM_CALLBACK_POLLING
          
            #ifdef COM_ENABLE_RX_INDICATIONFUN

            if(COM_IpduRxIndicationFunPtr[PduId] != COM_NULL)
            {
                COM_IpduRxIndicationFunPtr[PduId]();
            }
            else
            {
            /* do nothing */
            }

            #endif

            #endif
        
            #ifdef COM_ENABLE_RX_INDICATIONFLAG

            /* set Rx indication flag */
            Com_RxIpduIndicationFlag[PduId] = COM_YES;    

            #endif

            #ifdef COM_ENABLE_ROUTESIGNAL

            #if((COM_TXIPDUNUM >= 1u) && (COM_RXIPDUNUM >= 1u) && \
                (COM_GWIPDUNUM >= 1u) && (COM_GWSIGNUM >= 1u))

            /* set the SignalGatewayUpdateFlag */
            for(PduIndex = 0u; PduIndex < COM_GWIPDUNUM; PduIndex++)
            {
                if(COM_SignalGatewayIpduInfo[PduIndex].RxPduId == PduId)
                {
                    COM_SignalGatewayUpdateFlag[PduIndex] = COM_YES;
                    break;
                }
                else
                {
                /* do nothing */
                }
            }

            #endif
        
            #endif
        
        } 
        else
        {
            #ifdef COM_ENABLE_LPDURX_ERRORINDICATIONFUN
            if(COM_IpduRxErrorIndicationFunPtr[PduId] != COM_NULL)
            {
                COM_IpduRxErrorIndicationFunPtr[PduId](Result);
            }
            else
            {
            /* do nothing */
            }
            #endif 
        }     
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_ReceivedSignal
*                 
* Description:   This service copies the data of signal identified by SignalId
*                to the location specified by SignalDataPtr  
*                 
* Inputs:        ComRxSignalId: ignal handle
*                SignalDataPtr: Reference to the location where the received
*                               signal data shall be stored
*       
* Outputs:       E_OK:                      service has been accepted
*                COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was 
*                                           stopped
*                COM_BUSY:                  in case the TP-Buffer is lock for
                                            large data type handling
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(uint8, COM_PUBLIC_CODE) Com_ReceiveSignal
(
    Com_SignalIdType ComRxSignalId, 
    P2VAR(void, COM_APPL_DATA, COM_AUTOMOTIVE) SignalDataPtr
)
{

    uint8 result = COM_SERVICE_NOT_AVAILABLE; 
    PduIdType ComRxPduId = 0u;                      

    if(ComRxSignalId < COM_RXSIGNUM)  
    {
        ComRxPduId = COM_RxSigTable[ComRxSignalId].RxPduId;

        if(Com_GetRxIpduStatus(ComRxPduId) == COM_RUNNING)
        {
            if(COM_ReceivedSignalFunPrt[ComRxSignalId] != COM_NULL)
            {
                COM_ReceivedSignalFunPrt[ComRxSignalId](SignalDataPtr); 
                result = E_OK;
            }
            else
            {
                result = COM_NONE_FUNCTION;
            }      
        }
        else
        {
            result = COM_SERVICE_NOT_AVAILABLE;  
        }   
    }
    else
    {
        result = COM_OUT_OF_RANGE;
    }

    return  result;

}

#define COM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

/*#if (COM_RXIPDUNUM >= 1u)*/
#endif

#if (COM_TXIPDUNUM >= 1u)

#ifdef COM_ENABLE_ACTIVEPORPERTY 

#define COM_START_SEC_PRIVATE_CODE
#include "MemMap.h"
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_ActiveTxIpdu
*                
* Description:   Set Ipdu  Active Flag 
*                             
* Inputs:        ComTxPduId, Tx Ipdu handle
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_ActiveTxIpdu
(
    PduIdType ComTxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u; 

    if(ComTxPduId < COM_TXIPDUNUM)
    {
        Position_Byte = (uint8)(ComTxPduId >> 3u);

        Position_Bit = (uint8)(ComTxPduId % 8u); 

        Com_IpduActiveFlag[Position_Byte] |= (1u << Position_Bit);    
    }
    else
    {
    /* report error */
    }

}
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_DisactiveTxIpdu
*                
* Description:   Reset Ipdu  Active Flag 
*                             
* Inputs:        ComTxPduId, Tx Ipdu handle
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_DisactiveTxIpdu
(
    PduIdType ComTxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u; 

  
    if(ComTxPduId < COM_TXIPDUNUM)
    {
        Position_Byte = (uint8)(ComTxPduId >> 3u);
        Position_Bit = (uint8)(ComTxPduId % 8u); 
        Com_IpduActiveFlag[Position_Byte] &= ( (uint8)(~(1u << Position_Bit)) );  
    }
    else
    {
    /* report error */
    }    
 
}
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_TxIpduIsActived
*                
* Description:   Get the Tx Ipdu  Active Flag 
*                             
* Inputs:        ComTxPduId, Tx Ipdu handle
*             
* Outputs:       COM_NO:      Ipdu is Disactived
*                COM_RUNNING: Ipdu is Actived
*                COM_ERROR:   Error Happens
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(uint8, COM_PRIVATE_CODE) Com_TxIpduIsActived
(
    PduIdType ComTxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u; 
    uint8 Result = COM_ERROR;

    if(ComTxPduId<COM_TXIPDUNUM)
    {
        Position_Byte = (uint8)(ComTxPduId >> 3u);
        Position_Bit = (uint8)(ComTxPduId % 8u); 
        if( Com_IpduActiveFlag[Position_Byte] & (1u << Position_Bit) ) 
        {
            Result = COM_YES;
        }
        else
        {
            Result = COM_NO; 
        }    
    } 
    else
    {
    /* report error */
    }
    
    return  Result;

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_SigIsActived
*                
* Description:   Get Signal  Active Flag
*                
* Inputs:        SignalId, Tx Signal handle
*                   
* Outputs:       COM_NO:    Signal is Disactived
*                COM_YES:   Signal is Actived
*                COM_ERROR: Error Happens
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(uint8, COM_PRIVATE_CODE) Com_SigIsActived
(
    Com_SignalIdType SignalId
)
{

    uint8 Position_Bit = 0u;
    uint16 Position_Byte = 0u; 
    uint8 Result = COM_ERROR;

    if(SignalId < COM_TXSIGNUM)
    {
        Position_Byte = (uint16)(SignalId >> 3u);

        Position_Bit = (uint8)(SignalId % 8u); 
        if( Com_SigActivedFlag[Position_Byte] & (1u << Position_Bit) ) 
        {
            Result = COM_YES;
        }
        else
        {
            Result = COM_NO; 
        };
     
    }
    else
    {
    /* report error */
    }

    return Result;    
 
}


#define COM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

#define COM_START_SEC_PUBLIC_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_ActiveSig
*                
* Description:   Set Signal  Active Flag 
*                             
* Inputs:        SignalId, Tx Signal handle
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_ActiveSig
(
    Com_SignalIdType SignalId
)
{

    uint8 Position_Bit = 0u;
    uint16 Position_Byte = 0u; 

    if(SignalId < COM_TXSIGNUM)
    {
        Position_Byte = (uint16)(SignalId >> 3u);

        Position_Bit = (uint8)(SignalId % 8u); 

        Com_SigActivedFlag[Position_Byte] |= (1u << Position_Bit);
    
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_DisactiveSig
*                
* Description:   Reset Signal  Active Flag
*                
* Inputs:        SignalId, Tx Signal handle
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_DisactiveSig
(
    Com_SignalIdType SignalId
)
{

    uint8 Position_Bit = 0u;
    uint16 Position_Byte = 0u; 

    if(SignalId < COM_TXSIGNUM)
    {
        Position_Byte = (uint16)(SignalId >> 3u);

        Position_Bit = (uint8)(SignalId % 8u); 

        Com_SigActivedFlag[Position_Byte] &= ((uint8)(~(1u << Position_Bit)));
     
    }
    else
    {
    /* report error */
    }    
 
}

#define COM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

/*#ifdef COM_ENABLE_ACTIVEPORPERTY*/
#endif

#define COM_START_SEC_PRIVATE_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_TransmitIpdu
*                
* Description:   transmit Ipdu.
*                
* Inputs:        TxPduId
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_TransmitIpdu
(
    PduIdType TxPduId
)
{
    
#ifdef COM_ENABLE_RETRANSMIT
  
    //BufReq_ReturnType result = E_NOT_OK;
    Std_ReturnType result = E_NOT_OK;
    if( COM_TxDataPtr[TxPduId].SduLength <= COM_SINGLEFRAMEMAXLENGTH)
    {
        Com_TxIpduCallout(TxPduId);
        /* Transmit Ipdu */
        result = Com_Transmit(TxPduId,&COM_TxDataPtr[TxPduId]);  
    }
    else
    {
        Com_TxIpduCallout(TxPduId);
        /* Transmit Tp Ipdu */
        result = Com_TransmitLPDU(TxPduId, &COM_TxDataPtr[TxPduId]);
    }
    if(result == E_OK)
    {
        Com_ReTransmit[TxPduId]=COM_NO;
        /* clear transmit request */
        Com_TxState[TxPduId] &= COM_TXNOREQUEST;        

        Com_ResetTxDelayTimer(TxPduId);     
    }
    else
    {
        Com_ReTransmit[TxPduId] = COM_YES;
    }
    
    #else

    if( COM_TxDataPtr[TxPduId].SduLength <= COM_SINGLEFRAMEMAXLENGTH)
    {
        Com_TxIpduCallout(TxPduId);    
        /* Transmit Ipdu */
        (void)Com_Transmit(TxPduId, &COM_TxDataPtr[TxPduId]);  
    }
    else
    {
        Com_TxIpduCallout(TxPduId);    
        /* Transmit Tp Ipdu */
        (void)Com_TransmitLPDU(TxPduId, &COM_TxDataPtr[TxPduId]);
    }

    /* clear transmit request */
    Com_TxState[TxPduId] &= COM_TXNOREQUEST;

    Com_ResetTxDelayTimer(TxPduId);

    #endif
  
}

 /* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_TxInit
*                
* Description:   rx initialize.
*                
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_TxInit
(
    void
)
{

    PduIdType IpduIndex = 0u;

    for(IpduIndex = 0u; IpduIndex < COM_TXIPDUNUM; IpduIndex++)
    {
        Com_InitTxIpdu(IpduIndex);
    }

    for(IpduIndex = 0u; IpduIndex < COM_TXIPDUFLAGSIZE; IpduIndex++)
    {
        Com_TxIpduStartedFlag[IpduIndex] = COM_STOP; 
    }
    
 }

/* BEGIN_FUNCTION_HDR
*******************************************************************************
* Function Name: Com_ManageCyclicIpdu
*                  
* Description:   Manage the Cyclic message
*                  
* Inputs:        TxPduId: message's handle in Com
*                    
* Outputs:       None
* 
* Limitations:   None
*******************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_ManageCyclicIpdu
(
    PduIdType TxPduId 
) 
{  

    if(Com_IpduTxOffSetCounter[TxPduId] == 0u)
    {
        /* Check if Cyclic send is active for this message */
        if( ((COM_IpduTxTypeTable[TxPduId] & COM_SENDTYPECYCLIC) != COM_NO) || 

            #ifdef COM_ENABLE_ACTIVEPORPERTY 

            (Com_TxIpduIsActived(TxPduId) == COM_YES)  || 

            #endif

            (Com_IpduRepetitionFlag[TxPduId] == COM_YES) ) 
        {
            if(Com_TxIpduCycleCntBackup[TxPduId] > 0u) 
            {
                Com_TxIpduCycleCntBackup[TxPduId]--;
                if(Com_TxIpduCycleCntBackup[TxPduId] == 0u) 
                {
                    Com_TxIpduCycleCntBackup[TxPduId] = Com_GetTxCycle(TxPduId);

                       /* set send Request */
                    Com_TxState[TxPduId] |= COM_TXREQUEST;
                } 
                else
                {
                /* do nothing */
                }
            } 
            else
            {
            /* do nothing */
            }
            if(Com_IpduTxCycCounter[TxPduId] != 0u)
            {
                /* decrement timer counter */
                Com_IpduTxCycCounter[TxPduId]--;    
            } 
            else
            {
            /* do nothing */
            }
            /* Tx timer expired -> request to send message */
            if(Com_IpduTxCycCounter[TxPduId] == 0u)
            {
                #ifdef COM_ENABLE_ACTIVEPORPERTY 

                if(Com_TxIpduIsActived(TxPduId) == COM_YES) 
                {
                    /* road fast cycle counter */
                    Com_ResetTxFsatCycleTimer(TxPduId);
                } 
                else 
                   
                #endif

                if(Com_IpduRepetitionFlag[TxPduId] == COM_YES)
                {
                    if( ((Com_NumOfTxRepetition[TxPduId] - 1u) != 0u) && 
                        (Com_NumOfTxRepetition[TxPduId] != 0u) ) 
                    {
                        Com_NumOfTxRepetition[TxPduId]--;
                        /* road the time between repetition */
                        Com_ResetTxRepCycleTimer(TxPduId);
                    } 
                    else
                    {
                        Com_ClearIpduRepetionFlag(TxPduId); 

                        if(Com_TxIpduCycleCntBackup[TxPduId] > 0u)
                        {
                            Com_IpduTxCycCounter[TxPduId] = 
                            Com_TxIpduCycleCntBackup[TxPduId];                            
                        }
                        else
                        {
                            Com_ResetTxCycleTimer(TxPduId);
                        }
  
                        Com_TxIpduCycleCntBackup[TxPduId] = 0u;                     
                    }
                }
                else
                {    
                    /* Reload the cycle counter for the cyclic message */                             
                    Com_ResetTxCycleTimer(TxPduId);
                }

                /* set send Request */
                Com_TxState[TxPduId] |= COM_TXREQUEST;    
            } 
            else
            {
            /* do nothing */            
            }
        }
        else
        {
        /* do nothing */
        }
    }
    else
    {
        Com_IpduTxOffSetCounter[TxPduId]--;
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_InitTxIpdu
*                 
* Description:   used to Setup or shutdown a Ipdu specified by  ComIpduId 
*                 
* Inputs:        ComTxIpduId: Ipdu Id 
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_InitTxIpdu
(
    PduIdType ComTxIpduId
)
{     

    uint16 DataIndex = 0u;
    
    #ifdef COM_ENABLE_ACTIVEPORPERTY
    Com_SignalIdType SigId = 0u;
    #endif

    if(ComTxIpduId < COM_TXIPDUNUM)
    {

        #ifdef COM_ENABLE_RETRANSMIT

        Com_ReTransmit[ComTxIpduId] = COM_NO;

        #endif
      
        COM_TxDataPtr[ComTxIpduId].SduDataPtr = 
        COM_TxDataInitInfo[ComTxIpduId].SduDataPtr;

        COM_TxDataPtr[ComTxIpduId].SduLength = 
        COM_TxDataInitInfo[ComTxIpduId].SduLength;
    
        /* initialize tx state */
        Com_TxState[ComTxIpduId] = COM_IpduTxTypeTable[ComTxIpduId];    

        /* cyclic transmit counter clear for transmit immediately */
        Com_IpduTxCycCounter[ComTxIpduId] = 0u;    

        /* clear tx delay counter */
        Com_IpduTxDelayCounter[ComTxIpduId] = 0u;

        /* clear repetition transmit counter */
        Com_NumOfTxRepetition[ComTxIpduId] = 0u;

        Com_ClearIpduBeingSentFlag(ComTxIpduId);

        /* Ipdu tx timeout counter initialize */
        Com_IpduTxTimeOutCnt[ComTxIpduId] = COM_NO;

        #ifdef COM_ENABLE_TX_TO_INDFLAG

        /* Ipdu tx timeout flag initialize */
        Com_TxIpduTimeOutFlag[ComTxIpduId] = COM_NO;
    
        #endif

        #ifdef COM_ENABLE_TX_CONFIRMATIONFLAG

        Com_TxIdpuConfFlag[ComTxIpduId] = COM_NO;

        #endif

        Com_ResetTxOffsetTimer(ComTxIpduId);

        Com_TxIpduInfo[ComTxIpduId].Data_p = 
        COM_TxDataPtr[ComTxIpduId].SduDataPtr;

        Com_TxIpduInfo[ComTxIpduId].Length = 
        COM_TxDataPtr[ComTxIpduId].SduLength;

        Com_ClearIpduRepetionFlag(ComTxIpduId);
        Com_TxIpduCycleCntBackup[ComTxIpduId] = 0u;
      
        #ifdef COM_CALLBACK_POLLING

        Com_TxConfirmationFlag[ComTxIpduId] = COM_NO;

        #endif
        /* Ipdu Default Value */
        if( (COM_TxIpduDefaultValue[ComTxIpduId] != COM_NULL) && 
            (COM_TxDataPtr[ComTxIpduId].SduDataPtr != COM_NULL) )
        {
            for(DataIndex = 0u; 
                DataIndex < COM_TxDataPtr[ComTxIpduId].SduLength; 
                DataIndex++)
            {
                COM_TxDataPtr[ComTxIpduId].SduDataPtr[DataIndex] = 
                COM_TxIpduDefaultValue[ComTxIpduId][DataIndex];
            }        
        }
        else
        {
        /* do nothing */
        }   

        #ifdef COM_ENABLE_ACTIVEPORPERTY 

        Com_DisactiveTxIpdu(ComTxIpduId);

        for(SigId = COM_TxIpduOffsetInTxSigTable[ComTxIpduId]; 
            SigId < (COM_TxIpduOffsetInTxSigTable[ComTxIpduId] + 
            COM_TxIpduSigNum[ComTxIpduId]); SigId++) 
        { 
            Com_DisactiveSig(SigId);
        }

        #endif

        Com_TxIpduPreStatus[ComTxIpduId] = COM_STOP;        

        Com_StopTxIpdu(ComTxIpduId);    
    }
    else
    {
    /* do nothing */
    }

}

#define COM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

#define COM_START_SEC_PUBLIC_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_StartTxIpdu
*                
* Description:   Setup the Tx message which is specific by IpduIde
*                
* Inputs:        ComTxPduId, Tx message handle
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_StartTxIpdu
(
    PduIdType ComTxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u; 

    if(ComTxPduId < COM_TXIPDUNUM)
    {
        Position_Byte = (uint8)(ComTxPduId >> 3u);

        Position_Bit = (uint8)(ComTxPduId % 8u); 

        Com_TxIpduStartedFlag[Position_Byte] |= (1u << Position_Bit);    
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_StopTxIpdu
*                
* Description:   Stop the Tx message which is specific by IpduIde
*                
* Inputs:        ComTxPduId, Tx message handle
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_StopTxIpdu
(
    PduIdType ComTxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u;

    if(ComTxPduId < COM_TXIPDUNUM)
    {
        Position_Byte = (uint8)(ComTxPduId >> 3u);
        Position_Bit = (uint8)(ComTxPduId % 8u); 
        Com_TxIpduStartedFlag[Position_Byte] &= ((uint8)(~(1u << Position_Bit)));     
    }
    else
    {
    /* report error */
    }    
 
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_GetTxIpduStatus
*                
* Description:   Get the Tx message status which is specific by IpduIde
*                
* Inputs:        ComTxPduId, Tx message handle
*                   
* Outputs:       COM_STOP:    message is stopped
*                COM_RUNNING: message is started
*                COM_ERROR:   Error Happens
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(uint8, COM_PUBLIC_CODE) Com_GetTxIpduStatus
(
    PduIdType ComTxPduId
)
{

    uint8 Position_Bit = 0u;
    uint8 Position_Byte = 0u;
    uint8 Result = COM_ERROR;

    if(ComTxPduId<COM_TXIPDUNUM)
    {
        Position_Byte = (uint8)(ComTxPduId >> 3u);
        Position_Bit = (uint8)(ComTxPduId % 8u); 
        if( Com_TxIpduStartedFlag[Position_Byte] & (1u << Position_Bit) ) 
        {
            Result = COM_RUNNING;
        }
        else
        {
            Result = COM_STOP; 
        }    
    } 
    else
    {
    /* report error */
    }
    
    return  Result;

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_TriggerIPDUSend
*                
* Description:   By a call to Com_TriggerIPDUSend the I-PDU with the given ID 
*                is triggered for transmission
*                
* Inputs:        PduId: the I-PDU-ID of the I-PDU that shall be triggered for 
*                       sending
*                   
* Outputs:       None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_TriggerIPDUSend
(
    PduIdType PduId
)
{

    if(PduId < COM_TXIPDUNUM) 
    {
        /* set send request */
        Com_TxState[PduId] |= COM_TXREQUEST;    
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
*******************************************************************************
* Function Name: Com_TxStart
*                  
* Description:   tx initialize.
*                  
* Inputs:        None
*                    
* Outputs:       None
* 
* Limitations:   None
*******************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_TxStart
( 
    void 
)
{
    PduIdType IpduIndex = 0u;

    for(IpduIndex = 0u; IpduIndex < COM_TXIPDUFLAGSIZE; IpduIndex++)
    {
        Com_TxIpduStartedFlag[IpduIndex] = COM_ALLRUNNING; 
    }
	if(ComRxTimeoutMoniFlag == COM_NO)
	{
		ComRxTimeoutMoniCnts = 500/COM_TXMAIN_FUNCTION_PERIOD;
		ComRxTimeoutMoniFlag = COM_YES;
	}
}
 
/* BEGIN_FUNCTION_HDR
*******************************************************************************
* Function Name: Com_TxStop
*                  
* Description:   stop tx.
*                  
* Inputs:        None
*                    
* Outputs:       None
* 
* Limitations:   None
*******************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_TxStop
( 
    void 
)
{
    PduIdType IpduIndex = 0u;

    for(IpduIndex = 0u; IpduIndex < COM_TXIPDUFLAGSIZE; IpduIndex++)
    {
        Com_TxIpduStartedFlag[IpduIndex] = COM_NO; 
    }
	ComRxTimeoutMoniFlag = COM_NO;
}





/* BEGIN_FUNCTION_HDR
*******************************************************************************
* Function Name: Com_MainFunctionTx
*                  
* Description:   tx task for sending Ipdu.
*                  
* Inputs:        None
*                    
* Outputs:       None
* 
* Limitations:   None
*******************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_MainFunctionTx
( 
    void 
)
{

    PduIdType TxPduId = 0u;

    for(TxPduId = 0u; TxPduId < COM_TXIPDUNUM; TxPduId++)
    {
        /* check if Cyclic send is active for this message */
        if(Com_GetTxIpduStatus(TxPduId) == COM_RUNNING)
        {
            Com_ManageCyclicIpdu(TxPduId) ;
            /* decrement Transmit delay counter */
            if(Com_IpduTxDelayCounter[TxPduId] > 0u )
            {
                Com_IpduTxDelayCounter[TxPduId]--;
            }
            else
            {
            /* do nothing */
            }

            #ifdef COM_CALLBACK_POLLING

            #ifdef COM_ENABLE_TX_CONFIRMATIONFUN

            if(Com_TxConfirmationFlag[TxPduId] == COM_YES) 
            {
                if(COM_IpduTxConfirmFunPtr[TxPduId] != COM_NULL)
                {
                    /* call tx confirmation function */
                    COM_IpduTxConfirmFunPtr[TxPduId]();    
                } 
                else
                {
                /* do nothing */
                }
 
                Com_TxConfirmationFlag[TxPduId]=COM_NO;    
                         
            }
            else
            {
            /* do nothing */
            }

            #endif

            #endif
            
            /* send message if send request is set */
            #ifdef COM_ENABLE_RETRANSMIT

            if( (((Com_TxState[TxPduId]&COM_TXREQUEST) != 0u) && 
                (Com_IpduTxDelayCounter[TxPduId] == 0u)) || 
                (Com_ReTransmit[TxPduId] == COM_YES) )

            #else

            if( ((Com_TxState[TxPduId]&COM_TXREQUEST) != 0u) && 
                (Com_IpduTxDelayCounter[TxPduId] == 0u) )

            #endif

            {
                #ifdef COM_ENABLE_RETRANSMIT

                if( (Com_ReTransmit[TxPduId] == COM_NO) && 
                    (Com_IpduBeingSentFlag[TxPduId] == COM_NO) )

                #else 

                if(Com_IpduBeingSentFlag[TxPduId]==COM_NO )

                #endif

                {                  
                    Com_ResetTxTimeoutTimer(TxPduId);                  
                }            
                /* transmit message */
                Com_TransmitIpdu(TxPduId);    
                Com_SetIpduBeingSentFlag(TxPduId);
                
            }
            else
            {
            /* do nothing */
            }
            
            /* Ipdu Tx timeout check */
            if( ((COM_IpduTxTypeTable[TxPduId] & COM_SENDTYPETIMEOUTMONITOR) 
                != 0u) && (Com_IpduBeingSentFlag[TxPduId] == COM_YES) )
            {
                if(Com_IpduTxTimeOutCnt[TxPduId] == 0u)
                {
                    /* Message transmission timeout notification:
                    call timeout indicaiton function or set 
                    timeout indication flag */
                    /* set timeout flag */
                    #ifdef COM_ENABLE_TX_TO_INDFLAG

                    Com_TxIpduTimeOutFlag[TxPduId] = COM_YES;

                    #endif

                    #ifdef COM_ENABLE_TX_TO_INDFUN

                    if(COM_IpduTxTimeOutFunPtr[TxPduId] != COM_NULL)
                    {
                        COM_IpduTxTimeOutFunPtr[TxPduId]();
                    }
                    else
                    {
                    /* do nothing */
                    }

                    #endif 
                   
                    /* clear Sending flag */
                    Com_ClearIpduBeingSentFlag(TxPduId);
                }
                else
                {
                    Com_IpduTxTimeOutCnt[TxPduId]--;
                }    
            }
            else
            {
            /* do nothing */
            }
        }
        else
        {
        /* do nothing */
        }                     
    }

}

/* BEGIN_FUNCTION_HDR
*******************************************************************************
* Function Name: Com_TxConfirmation
*                  
* Description:   tx confirmation when transmit successful.
*                  
* Inputs:        IpduId
*                    
* Outputs:       None
* 
* Limitations:   None
*******************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_TxConfirmation
(
    PduIdType TxPduId
)
{

    /* For   Com_004   Reset Delay Timer */
    if(TxPduId < COM_TXIPDUNUM)
    {
        if(Com_GetTxIpduStatus(TxPduId) == COM_RUNNING)
        {
            Com_ResetTxDelayTimer(TxPduId);
            Com_ClearIpduBeingSentFlag(TxPduId);
 
            #ifndef COM_CALLBACK_POLLING

            #ifdef COM_ENABLE_TX_CONFIRMATIONFUN
            
            if(COM_IpduTxConfirmFunPtr[TxPduId] != COM_NULL)
            {
                /* call tx confirmation function */
                COM_IpduTxConfirmFunPtr[TxPduId]();    
            } 
            else
            {
            /* do nothing */
            }

            #endif

            #else

            Com_TxConfirmationFlag[TxPduId] = COM_YES;

            #endif
        
            #ifdef COM_ENABLE_TX_CONFIRMATIONFLAG

            Com_TxIdpuConfFlag[TxPduId] = COM_YES;

            #endif       
        }
        else
        {
        /* do nothing */
        }   
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_SetSendRequest
*                 
* Description:   Set send request when need to send signal.
*                 
* Inputs:        ComTxSignalId
*                   
* Outputs:       none
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_SetSendRequest
(
    Com_SignalIdType ComTxSignalId
)
{

    PduIdType TxPduId = 0u;
    #ifdef COM_ENABLE_ACTIVEPORPERTY    
    Com_SignalIdType SigID = 0u;
    #endif
    /* get Ipdu Id from signal Id */

    TxPduId = COM_TxSigTable[ComTxSignalId].TxPduId;
    
    if(Com_GetTxIpduStatus(TxPduId) == COM_RUNNING)
    {
        
        #ifdef COM_ENABLE_ACTIVEPORPERTY

        if( (COM_TxSigTable[ComTxSignalId].SigTxProperty & 
            COM_SENDTYPEIFACTIVE) != 0u ) 
        {
            if(Com_TxIpduIsActived(TxPduId) == COM_YES) 
            {
                if(Com_SigIsActived(ComTxSignalId) == COM_YES) 
                {
                    Com_ActiveTxIpdu(TxPduId) ;
                } 
                else
                {
                    for(SigID = COM_TxIpduOffsetInTxSigTable[TxPduId]; 
                        SigID < (COM_TxIpduOffsetInTxSigTable[TxPduId] + 
                        COM_TxIpduSigNum[TxPduId]); SigID++) 
                    {
                        if(Com_SigIsActived(SigID) == COM_YES) 
                        {
                            Com_ActiveTxIpdu(TxPduId) ;
                            break; 
                        } 
                        else if( SigID == 
                                (COM_TxIpduOffsetInTxSigTable[TxPduId] + 
                                COM_TxIpduSigNum[TxPduId] - 1) )
                        {
                            Com_DisactiveTxIpdu(TxPduId) ;
                            if( (COM_TxSigTable[ComTxSignalId].SigTxProperty & 
                             COM_SENDTYPEREPETITION) != 0u ) 
                            {   
                                if(COM_IpduRepetitionTxNumTable[TxPduId] != 0u)
                                {
                                    Com_SetIpduRepetionFlag(TxPduId) ;
                                    Com_SetTxRepNumber(TxPduId);
                                }
                                else if((COM_IpduTxTypeTable[TxPduId] & 
                                        COM_SENDTYPECYCLIC) != 0u)
                                {
                                    Com_ResetTxCycleFromFastCycle(TxPduId);
                                }
                                else
                                {
                                /* do nothing */
                                }
                            }
                            else
                            {
                                if((COM_IpduTxTypeTable[TxPduId] & 
                                    COM_SENDTYPECYCLIC) != 0u)
                                {
                                    Com_ResetTxCycleFromFastCycle(TxPduId);
                                }
                                else
                                {
                                /* do nothing */
                                }
                            }                                        
                        }
                        else
                        {
                        /* do nothing */
                        }                 
                    }
                }        
            } 
            else 
            {
                if(Com_SigIsActived(ComTxSignalId) == COM_YES) 
                {
                    Com_ActiveTxIpdu(TxPduId) ;
                    Com_IpduTxCycCounter[TxPduId] = 0u;
                    Com_IpduTxOffSetCounter[TxPduId] = 0u;
                } 
                else
                {
                /* do nothing */
                }
            }            
        } 
        else  
        
        #endif

        if( (COM_TxSigTable[ComTxSignalId].SigTxProperty & 
                    COM_SENDTYPEONEVENT) != 0u )
        {           
            if( (COM_TxSigTable[ComTxSignalId].SigTxProperty & 
                COM_SENDTYPEREPETITION) != 0u )
            {
                Com_SetIpduRepetionFlag(TxPduId) ;
                Com_SetTxRepNumber(TxPduId);

                if( (COM_IpduTxTypeTable[TxPduId] & COM_SENDTYPECYCLIC) 
                    == COM_SENDTYPECYCLIC )
                {
                    if(Com_IpduTxCycCounter[TxPduId] == 0u) 
                    {
                        Com_ResetTxBackupTimer(TxPduId);
                    } 
                    else
                    {
                        Com_LoadTxBackupTimer(TxPduId);
                    }  

                    Com_IpduTxCycCounter[TxPduId] = 0u;

                    Com_TxIpduCycleCntBackup[TxPduId] += 
                    Com_IpduTxOffSetCounter[TxPduId];

                }
                else
                {
                    Com_IpduTxCycCounter[TxPduId] = 0u;

                }  

                Com_IpduTxOffSetCounter[TxPduId] = 0u;
    
            }     
            else
            {
                /* set send request */
                Com_TriggerIPDUSend(TxPduId);
            }
        } 
        else
        {
        /* do nothing */
        }     
    }
    else
    {
    /* do nothing */
    }
 
}


/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_CopyTxData
*                 
* Description:   Callbak function Com provided by Com to J1939Tp
*                 
* Inputs:        ComTxPduId:   Transmitt PduId
*                PduInfoPtr:   Pointer to a PduInfoType, which indicates the 
*                              number of bytes to be copied (SduLength) and the 
*                              location where the data have to be copied to 
*                              SduDataPtr).
*                RetryInfoPtr: None used
*                TxDataCntPtr: Remaining Tx data after completion of this call
*                
* Outputs:       BUFREQ_OK, BUFREQ_E_NOT_OK
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(BufReq_ReturnType, COM_PUBLIC_CODE) Com_CopyTxData
(
    PduIdType PduId, 
    P2VAR(PduInfoType, COM_APPL_DATA, COM_AUTOMOTIVE) PduInfoPtr,
    P2VAR(RetryInfoType, COM_APPL_DATA, COM_AUTOMOTIVE) RetryInfoPtr,
    P2VAR(PduLengthType, COM_APPL_DATA, COM_AUTOMOTIVE) TxDataCntPtr
)
{

    BufReq_ReturnType result = BUFREQ_OK;
    (void)RetryInfoPtr;

    if(PduId < COM_TXIPDUNUM)
    {
        if(Com_GetTxIpduStatus(PduId) == COM_RUNNING)
        {
            Com_DataCopy(PduInfoPtr->SduDataPtr, 
                        Com_TxIpduInfo[PduId].Data_p, 
                        PduInfoPtr->SduLength);

            Com_TxIpduInfo[PduId].Data_p += (PduInfoPtr->SduLength);

            if(Com_TxIpduInfo[PduId].Length >= PduInfoPtr->SduLength)
            {
                Com_TxIpduInfo[PduId].Length -= (PduInfoPtr->SduLength);
            }
            else
            {
                /* last frame */
                Com_TxIpduInfo[PduId].Length = 0u;   
            }

            if(TxDataCntPtr != COM_NULL)
            {
                *TxDataCntPtr =  Com_TxIpduInfo[PduId].Length;    
            }
            else
            {
            }    
        }
        else
        {
            result = BUFREQ_E_NOT_OK;
        }
    }
    else
    {
        /* report error */
        result = BUFREQ_E_NOT_OK;     
    }
    
    return result;
    
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_TpTxConfirmation
*                 
* Description:   Callbak function Com provided by Com to J1939Tp  
*                 
* Inputs:        PduId:  Receive PduId
*                Result: NTFRSLT_OK means the complete N-PDU has been eceived
*                        and is stored in the receive buffer.Any other value 
*                        means the N_PDU has not been received,
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_TpTxConfirmation
(
    PduIdType PduId,
    NotifResultType Result
)
{

    if( (PduId < COM_TXIPDUNUM) && (Com_GetTxIpduStatus(PduId) == COM_RUNNING) )
    {
        Com_TxIpduInfo[PduId].Data_p = COM_TxDataPtr[PduId].SduDataPtr;
        Com_TxIpduInfo[PduId].Length = COM_TxDataPtr[PduId].SduLength;
        
        Com_ResetTxDelayTimer(PduId);
        Com_ClearIpduBeingSentFlag(PduId);
      
        if(Result == NTFRSLT_OK)
        {
            #ifndef COM_CALLBACK_POLLING

            #ifdef COM_ENABLE_TX_CONFIRMATIONFUN

            if(COM_IpduTxConfirmFunPtr[PduId] != COM_NULL)
            {
                /* call tx confirmation function */
                COM_IpduTxConfirmFunPtr[PduId]();    
            } 
            else
            {
            /* do nothing */
            }

            #endif

            #else

            Com_TxConfirmationFlag[PduId] = COM_YES;

            #endif

            #ifdef COM_ENABLE_TX_CONFIRMATIONFLAG

            Com_TxIdpuConfFlag[PduId] = COM_YES;

            #endif       
        }
        else
        {
            #ifdef COM_ENABLE_LPDUTX_ERRORINDICATIONFUN

            if(COM_IpduTxErrorIndicationFunPtr[PduId] != COM_NULL)
            {
                COM_IpduTxErrorIndicationFunPtr[PduId](Result);
            }
            else
            {
            /* do nothing */
            }

            #endif               
        }          
    }
    else
    {
    /* report error */
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_SendSignal
*                 
* Description:   This service updates the signal object identified by 
*                SignalId with the signal referenced by the SignalDataPtr 
*                parameter  
*                 
* Inputs:        ComTxSignalId: Signal handle
*                SignalDataPtr: Reference to the Signal data to be transmitted
*       
* Outputs:       E_OK:                      service has been accepted
*                COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was 
*                                           stopped
*                COM_BUSY:                  in case the TP-Buffer is lock for
                                            large data type handling
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(uint8, COM_PUBLIC_CODE) Com_SendSignalExt
(
    Com_SignalIdType ComTxSignalId,
    P2CONST(void, COM_APPL_DATA, COM_AUTOMOTIVE) SignalDataPtr
)
{

   PduIdType ComTxPduId = 0u;                      
    uint8 result = COM_SERVICE_NOT_AVAILABLE;

    if(ComTxSignalId < COM_TXSIGNUM)
    {
        ComTxPduId = COM_TxSigTable[ComTxSignalId].TxPduId;

        if(COM_SendSignalFunPrt[ComTxSignalId] != COM_NULL)
        {
            COM_SendSignalFunPrt[ComTxSignalId](SignalDataPtr);  
            result = E_OK;        
        }
        else
        {
            result = COM_NONE_FUNCTION;
        }
    }
    else
    {
        result = COM_OUT_OF_RANGE;
    }

    return  result;

}
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_SendSignal
*                 
* Description:   This service updates the signal object identified by 
*                SignalId with the signal referenced by the SignalDataPtr 
*                parameter  
*                 
* Inputs:        ComTxSignalId: Signal handle
*                SignalDataPtr: Reference to the Signal data to be transmitted
*       
* Outputs:       E_OK:                      service has been accepted
*                COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was 
*                                           stopped
*                COM_BUSY:                  in case the TP-Buffer is lock for
                                            large data type handling
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(uint8, COM_PUBLIC_CODE) Com_SendSignal
(
    Com_SignalIdType ComTxSignalId,
    P2CONST(void, COM_APPL_DATA, COM_AUTOMOTIVE) SignalDataPtr
)
{

    PduIdType ComTxPduId = 0u;                      
    uint8 result = COM_SERVICE_NOT_AVAILABLE;

    if(ComTxSignalId < COM_TXSIGNUM)
    {
        ComTxPduId = COM_TxSigTable[ComTxSignalId].TxPduId;

        if(Com_GetTxIpduStatus(ComTxPduId) == COM_RUNNING)
        {
            if(COM_SendSignalFunPrt[ComTxSignalId] != COM_NULL)
            {
                COM_SendSignalFunPrt[ComTxSignalId](SignalDataPtr);  
                result = E_OK;        
            }
            else
            {
                result = COM_NONE_FUNCTION;
            }
        }
        else
        {
            result = COM_SERVICE_NOT_AVAILABLE;  
        }   
    }
    else
    {
        result = COM_OUT_OF_RANGE;
    }

    return  result;

}


#define COM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

/*#if (COM_TXIPDUNUM >= 1u)*/
#endif

#define COM_START_SEC_PRIVATE_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: ComDataCopy
*                 
* Description:   Copy data from source to destination.
*                 
* Inputs:        dest:pointing to the destination where data will be copied to
*                src: pointing to the source where data should be copied frome
*                cnt: the total length of the data should be copied
*                   
* Outputs:       none
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_DataCopy
( 
    P2VAR(uint8, COM_APPL_DATA, COM_AUTOMOTIVE) dest, 
    P2VAR(uint8, COM_APPL_DATA, COM_AUTOMOTIVE) src,
    PduLengthType cnt
)
{

    PduLengthType DataIndex = 0u;
    
    for(DataIndex = 0u; DataIndex < cnt; DataIndex++)
    {
        dest[DataIndex] = src[DataIndex];
    }

}

#if(COM_IPDUGROUPNUM >= 1u)

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_IpduGroupControl
*                 
* Description:   used to Setup or shutdown a Ipdu specified by  ComIpduId 
*                 
* Inputs:        IpduDirection: Indicates the Ipdu is sent by com or 
*                               received by com
*                ComIpduId:     Ipdu Id 
*                IpduControlComman: 
*                                   COM_IPDUCONTROLCOMMAND_START
*                                   COM_IPDUCONTROLCOMMAND_STOP
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE) Com_IpduControl
(
    boolean IpduDirection, 
    PduIdType ComIpduId, 
    boolean  IpduControlComman, 
    boolean Initialize
)
{

    #if(COM_TXIPDUNUM >= 1u)

    if(IpduDirection == COM_DIRECTION_TX)
    {
        if(ComIpduId < COM_TXIPDUNUM)
        {      
            if(IpduControlComman == COM_IPDUCONTROLCOMMAND_START)
            {
                if( (Initialize == COM_YES) && 
                    (Com_TxIpduPreStatus[ComIpduId] == COM_STOP) )
                {
                    Com_InitTxIpdu(ComIpduId);  
                }
                else
                { 
                /* do nothing */
                }

                Com_StartTxIpdu(ComIpduId);    
            }
            else if(IpduControlComman == COM_IPDUCONTROLCOMMAND_STOP)
            {
                Com_StopTxIpdu(ComIpduId);
            }
            else
            {   
            /* do nothing */
            }    
        }
        else
        {
        /* do nothing */
        }
    }    
    else 

    #endif  

    {
        #if(COM_RXIPDUNUM >= 1u)
 
        if(IpduDirection == COM_DIRECTION_RX)
        {
            if(ComIpduId < COM_RXIPDUNUM)
            {
                if(IpduControlComman == COM_IPDUCONTROLCOMMAND_START)
                {
                    if( (Initialize == COM_YES) && 
                        (Com_RxIpduPreStatus[ComIpduId] == COM_STOP) )
                    {
                        Com_InitRxIpdu(ComIpduId);  
                    }
                    else
                    {  
                    /* do nothing */
                    }

                    Com_StartRxIpdu(ComIpduId);      
                }
                else if(IpduControlComman == COM_IPDUCONTROLCOMMAND_STOP)
                {
                    Com_StopRxIpdu(ComIpduId);
                }
                else
                {  
                /* do nothing */
                }        
            }
            else
            {
            /* do nothing */
            }
        }
        else
        {
        /* do nothing */
        }

        #endif

    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_IpduGroupStop
*                 
* Description:   This service stops I-PDU group.  
*                 
* Inputs:        IpduGroupVector: Contains the activation state
*                                 (stopped=0/Started=1) for all I-PDU Groups
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE)  Com_IpduGroupStop
(
    Com_IpduGroupVector IpduGroupVector
)
{

    uint8 IpduGroupIndex = 0u;
    PduIdType IpduIndex = 0u; 
    PduIdType IpduStart = 0u;
    PduIdType IpduEnd = 0u;
      
    for(IpduGroupIndex = 0u; 
        IpduGroupIndex < COM_IPDUGROUPNUM; 
        IpduGroupIndex++)
    {
        if( ( IpduGroupVector & 
            ((Com_IpduGroupVector)(1u << IpduGroupIndex)) ) == 0u)
        {
            IpduStart = COM_IpduGroupInfo[IpduGroupIndex].IpduGroupPosition;

            IpduEnd = IpduStart + 
            COM_IpduGroupInfo[IpduGroupIndex].IpduNumInGroup;

            for(IpduIndex = IpduStart; IpduIndex < IpduEnd; IpduIndex++)
            {          
                Com_IpduControl(COM_GroupIpduInfo[IpduIndex].IpduDirecttion, 
                                COM_GroupIpduInfo[IpduIndex].IpduId, 
                                COM_IPDUCONTROLCOMMAND_STOP, 
                                COM_NO);   
            }      
        }
        else 
        {       
        /* do nothing */  
        }   
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_IpduGroupStart
*                 
* Description:   This service starts I-PDU group.  
*                 
* Inputs:        IpduGroupVector: Contains the activation state
*                                 (stopped=0/Started=1) for all I-PDU Groups
*                initialize:      flag to request initialization of the I-PUDs 
*                                 which are newly started
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, COM_PRIVATE_CODE)  Com_IpduGroupStart
(
    Com_IpduGroupVector IpduGroupVector, 
    boolean Initialize
)
{

    uint8 IpduGroupIndex = 0u;
    PduIdType IpduIndex = 0u; 
    PduIdType IpduStart = 0u;
    PduIdType IpduEnd = 0u;  
    
    for(IpduGroupIndex = 0u; 
        IpduGroupIndex < COM_IPDUGROUPNUM; 
        IpduGroupIndex++)
    {
        if( ( IpduGroupVector & 
            ((Com_IpduGroupVector)(1u << IpduGroupIndex)) ) != 0u )
        {
            IpduStart = COM_IpduGroupInfo[IpduGroupIndex].IpduGroupPosition;

            IpduEnd = IpduStart + 
            COM_IpduGroupInfo[IpduGroupIndex].IpduNumInGroup;

            for(IpduIndex = IpduStart; IpduIndex < IpduEnd; IpduIndex++)
            {
                Com_IpduControl(COM_GroupIpduInfo[IpduIndex].IpduDirecttion, 
                                COM_GroupIpduInfo[IpduIndex].IpduId, 
                                COM_IPDUCONTROLCOMMAND_START, 
                                Initialize);   
            }      
        }
        else
        {           
        /* do nothing */  
        }   
    }

}

/*#if(COM_IPDUGROUPNUM >= 1u)*/
#endif

#define COM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

#define COM_START_SEC_PUBLIC_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_Init
*                
* Description:   tx/rx initialize.
*                
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_Init
(
    void
)
{

    #if (COM_RXIPDUNUM >= 1u)

    Com_RxInit();

    #endif

    #if (COM_TXIPDUNUM >= 1u)

    Com_TxInit();

    #endif

	ComRxTimeoutMoniFlag = COM_NO;
}

#if(COM_IPDUGROUPNUM >= 1u)
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_IpduGroupControl
*                 
* Description:   This service starts I-PDU group.  
*                 
* Inputs:        IpduGroupVector: Contains the activation state
*                                 (stopped=0/Started=1) for all I-PDU Groups
*                initialize:      flag to request initialization of the I-PUDs 
*                                 which are newly started
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE) Com_IpduGroupControl
(
    Com_IpduGroupVector IpduGroupVector, 
    boolean Initialize
)
{

    PduIdType IpduIndex = 0u;
  
    #if(COM_TXIPDUNUM > 0u)

    for(IpduIndex = 0u; IpduIndex < COM_TXIPDUNUM; IpduIndex++)
    {
        Com_TxIpduPreStatus[IpduIndex] = Com_GetTxIpduStatus(IpduIndex);
    }

    #endif
  
    #if(COM_RXIPDUNUM  > 0u)

    for(IpduIndex = 0u; IpduIndex < COM_RXIPDUNUM; IpduIndex++)
    {
        Com_RxIpduPreStatus[IpduIndex] = Com_GetRxIpduStatus(IpduIndex);
    } 

    #endif
   
    Com_IpduGroupStop(IpduGroupVector); 
    Com_IpduGroupStart(IpduGroupVector, Initialize);  
}

/*#if(COM_IPDUGROUPNUM >= 1u)*/
#endif

#ifdef COM_ENABLE_ROUTESIGNAL

#if( (COM_TXIPDUNUM >= 1u) && (COM_RXIPDUNUM >= 1u) && \
    (COM_GWIPDUNUM >= 1u) && (COM_GWSIGNUM >= 1u) )

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Com_MainFunctionRouteSignals
*                 
* Description:   Calls the signal gateway part of the AUTOSAR COM module to
*                forward received signals to be routed.  
*                 
* Inputs:        None
*       
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, COM_PUBLIC_CODE)  Com_MainFunctionRouteSignals
(
    void
)
{

    PduIdType PduId = 0u;
    Com_SignalIdType SigId = 0u;
    uint8 SignalValue[8u];

    for(PduId = 0u; PduId < COM_GWIPDUNUM; PduId++)
    {
        if(COM_SignalGatewayUpdateFlag[PduId] == COM_YES)
        {
            COM_SignalGatewayUpdateFlag[PduId] = COM_NO;
            for(SigId = Com_GetSigPosStart(PduId); 
                SigId < Com_GetSigPosEnd(PduId); SigId++)
            {
                if( Com_ReceiveSignal(Com_GetSigRxId(SigId),
                                      SignalValue) == E_OK )
                {
                    (void)Com_SendSignal(Com_GetSigTxId(SigId), SignalValue);
                }
                else
                {
                /* do nothing */
                }
            }
        }
        else
        {
        /* do nothing */
        }
    }  
  
}

/*
#if( (COM_TXIPDUNUM >= 1u) && (COM_RXIPDUNUM >= 1u) && \
    (COM_GWIPDUNUM >= 1u) && (COM_GWSIGNUM >= 1u) )
*/
#endif 

/*#ifdef COM_ENABLE_ROUTESIGNAL*/
#endif

#if 0
void Com_RxTimeOutMonitoringDisable(void)
{
	ComRxTimeoutMoniFlag = COM_NO;
}

void Com_RxTimeOutMonitoringEnable(void)
{
    PduIdType IpduIndex = 0u;
	if(ComRxTimeoutMoniFlag == FALSE)
	{
	    for(IpduIndex = 0u; IpduIndex < COM_RXIPDUNUM; IpduIndex++)
	    {
			Com_ResetRxTimeoutTimer(IpduIndex);
	    }
		ComRxTimeoutMoniFlag = TRUE;
	}
}
#endif

#define COM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

