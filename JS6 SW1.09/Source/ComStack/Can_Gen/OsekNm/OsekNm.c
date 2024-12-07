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
*   Description     : Source of OsekNm module.                    
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
    
/*******************************************************************************
*   Includes 
*******************************************************************************/
#include "OsekNm.h"
#include "fm33lg0xx_fl.h"
extern unsigned char fast_sleep_status;
/*******************************************************************************
*   Macro definitions 
*******************************************************************************/

#define OSEKNM_DATA_LENGTH      ((uint16)0x08u)
#define BYTE_LENGTH             ((uint16)0x08u)
#define NmTxPduInfo             NmTxPduInfoAry[nmCurChannel]
#define NmTxBuffer              NmTxBufferAry[nmCurChannel]
/*******************************************************************************
*   Local Data Define 
*******************************************************************************/
/*#define CanNm_GetNmChn(PduId)     CanIfToNmChnAry[PduId]*/
/******************************************************************************* 
* Name:        nmCurChannel
* Description: NM Current deal channel.
* Values:       
*******************************************************************************/
#define OSEKNM_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"/* PRQA S 5087 */

/******************************************************************************* 
* Name:        NmTxPduInfo
* Description: 
* Values:      
*******************************************************************************/
STATIC VAR(PduInfoType, OSEKNM_PRIVATE_DATA) 
    NmTxPduInfoAry[NM_NUMBER_OF_CHANNELS];    

/******************************************************************************* 
* Name:        nmStatus
* Description: 
* Values:      
*******************************************************************************/
STATIC VAR(nmStatusType, OSEKNM_PRIVATE_DATA) 
    nmStatusAry[NM_NUMBER_OF_CHANNELS];

/******************************************************************************* 
* Name:        nmTxDataFld
* Description: NM Transmit Data field.
* Values:            
*******************************************************************************/
STATIC VAR(nmDataFldTypeUnion, OSEKNM_PRIVATE_DATA) 
    nmTxDataFldAry[NM_NUMBER_OF_CHANNELS];

/******************************************************************************* 
* Name:        nmRxDataFld
* Description: NM Receive Data field.
* Values:      
*******************************************************************************/
STATIC VAR(nmDataFldTypeUnion, OSEKNM_PRIVATE_DATA) 
    nmRxDataFldAry[NM_NUMBER_OF_CHANNELS];

#if defined ( NM_FOR_FAW )               
/******************************************************************************* 
* Name:        nmTxMsgInd
* Description: NM Transmit Data field.
* Values:            
*******************************************************************************/
STATIC VAR(nmMsgIndUnion, OSEKNM_PRIVATE_DATA) 
    nmTxMsgIndFldAry[NM_NUMBER_OF_CHANNELS];            

/******************************************************************************* 
* Name:        nmRxMsgInd
* Description: NM Receive Data field.
* Values:      
*******************************************************************************/
STATIC VAR(nmMsgIndUnion, OSEKNM_PRIVATE_DATA) 
    nmRxMsgIndFldAry[NM_NUMBER_OF_CHANNELS];    
#endif

/******************************************************************************* 
* Name:        nmLogicalSuccessor
* Description: the Destination ID of message transmitted.
* Values:      
*******************************************************************************/
STATIC VAR(nmNetIdType, OSEKNM_PRIVATE_DATA) 
    nmLogicalSuccessorAry[NM_NUMBER_OF_CHANNELS];

/*******************************************************************************
* Name:        nmRxMsgId
* Description: Can Buffer Message ID.
* Values:      
*******************************************************************************/
STATIC VAR(nmNetIdType,OSEKNM_PRIVATE_DATA) 
    nmRxSrcAddAry[NM_NUMBER_OF_CHANNELS];
/******************************************************************************* 
* Name:        nmRxCount & nmTxCount
* Description: the receive & transmit counts.
* Values:      [0...RX_LIMIT] & [0...TX_LIMIT]
*******************************************************************************/
STATIC VAR(nmRxTxCntType, OSEKNM_PRIVATE_DATA) 
    nmRxCountAry[NM_NUMBER_OF_CHANNELS];
STATIC VAR(nmRxTxCntType, OSEKNM_PRIVATE_DATA) 
    nmTxCountAry[NM_NUMBER_OF_CHANNELS];

/******************************************************************************* 
* Name:        nmTimerCnt
* Description: NM Timer Counts.
* Values:      [0,0,0,0,0]
*******************************************************************************/
STATIC VAR(nmTimerCntType, OSEKNM_PRIVATE_DATA) 
    nmTimerCntTtypAry[NM_NUMBER_OF_CHANNELS];
STATIC VAR(nmTimerCntType, OSEKNM_PRIVATE_DATA) 
    nmTimerCntTmaxAry[NM_NUMBER_OF_CHANNELS];
STATIC VAR(nmTimerCntType, OSEKNM_PRIVATE_DATA) 
    nmTimerCntTerrAry[NM_NUMBER_OF_CHANNELS];
STATIC VAR(nmTimerCntType, OSEKNM_PRIVATE_DATA) 
    nmTimerCntTwbsAry[NM_NUMBER_OF_CHANNELS];
STATIC VAR(nmTimerCntType, OSEKNM_PRIVATE_DATA) 
    nmTimerCntTtxAry[NM_NUMBER_OF_CHANNELS];
STATIC VAR(nmTimerCntType, OSEKNM_PRIVATE_DATA) 
    nmTimerCntTbusoffAry[NM_NUMBER_OF_CHANNELS];
/*OsekNm_013*/
#if defined ( NM_FOR_FAW )                
STATIC VAR(nmTimerCntType, OSEKNM_PRIVATE_DATA) 
    nmTimerCntTactiveminiAry[NM_NUMBER_OF_CHANNELS];        
STATIC VAR(nmTimerCntType, OSEKNM_PRIVATE_DATA) 
    nmTimerCntTlimphomeAry[NM_NUMBER_OF_CHANNELS];    
#endif

/******************************************************************************* 
* Name:        nmState
* Description: NM State.
* Values:      [0,0]
*******************************************************************************/
STATIC VAR(uint8, OSEKNM_PRIVATE_DATA) 
    NmTxBufferAry[NM_NUMBER_OF_CHANNELS][OSEKNM_DATA_LENGTH];
/*Bus off flag*/
/*STATIC VAR(uint8, OSEKNM_PRIVATE_DATA) nmCanBusOffFlgAry[NM_NUMBER_OF_CHANNELS];*/    
/*Bus off being*/
/*STATIC VAR(uint8, OSEKNM_PRIVATE_DATA) nmCanBusOffBeingAry[NM_NUMBER_OF_CHANNELS]; */
/*OsekNm_008*/
STATIC VAR(uint8, OSEKNM_PRIVATE_DATA) nubNmBusOffCntAry[NM_NUMBER_OF_CHANNELS];

#define OSEKNM_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"/* PRQA S 5087 */

/*******************************************************************************
*   Local Function Declaration 
*******************************************************************************/

#define OSEKNM_START_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

STATIC FUNC(nmStateType, OSEKNM_PRIVATE_CODE) CanNm_GetCurState
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) CanNm_SetNxtState
(
    const nmIndexType nmCurChannel,
    const nmStateType nxtState
);
/*TBusOff check and process function when Tbusoff timeout*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmCanBusOffCheckFun
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmModeCheckFun
(
	const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmGotoModeAwake
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmGotoModeSleep
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmRxIndication
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmBusErrorRecover
(
    const nmIndexType     nmCurChannel
);
STATIC FUNC(nmStateType, OSEKNM_PRIVATE_CODE) NmReadyToBusSleep
(
    const nmIndexType    nmCurChannel,
    const nmInitModeType initMode
);
STATIC FUNC(nmStateType, OSEKNM_PRIVATE_CODE) NmInitReset
(
    const nmIndexType    nmCurChannel,
    const nmInitModeType initMode
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTxPDUCreat
(
    const nmIndexType nmCurChannel
);                    
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTxPDUCopy
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmRxPDUCopy
(
    const nmIndexType nmCurChannel,
    CONSTP2CONST(PduInfoType,AUTOMATIC,OSEKNM_PRIVATE_DATA) canSduPtr
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmSetAlarm
(
    const nmIndexType nmCurChannel,
    const nmAlarmType nmAlarm
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmCancelAlarm
(
    const nmIndexType nmCurChannel,
    const nmAlarmType nmAlarm
);
STATIC FUNC(NmReturnType, OSEKNM_PRIVATE_CODE) NmStandardNormal
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmUpdateLogSuc
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmJudgeSkipped
(
    const nmIndexType nmCurChannel
);

#if defined(NM_ENABLE_CONFIG_LIMPHOME)||defined(NM_ENABLE_CONFIG_NORMAL)
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmSetConfig
(
    const nmIndexType nmCurChannel,
    const nmNetIdType   nmSourceID,
    const ConfigKindName   CfgKind
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmResetConfig
(
    const nmIndexType nmCurChannel
);
#endif

STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTxConfirmation
(
    const nmIndexType nmCurChannel
);

/*TTyp timer count function*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTTypCntFun
(
    const nmIndexType nmCurChannel
);

/*TMax timer count function*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTMaxCntFun
(
    const nmIndexType nmCurChannel
);

/*TErr timer count function*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTErrCntFun
(
    const nmIndexType nmCurChannel
);

/*TTx timer count function*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTTxCntFun
(
    const nmIndexType nmCurChannel
);

/*TWaitBusSleep timer count function*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTWBSCntFun
(
    const nmIndexType nmCurChannel
);

STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTBusOffCntFun
(
    const nmIndexType nmCurChannel
);

/*OsekNm_010*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTBusOffRecoveryTimeOutProc
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTtypTimeOutProc
(
    const nmIndexType nmCurChannel
);
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTmaxTimeOutProc
(
    const nmIndexType nmCurChannel
); 
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTerrTimeOutProc
(
    const nmIndexType nmCurChannel
); 
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTwbsTimeOutProc
(
    const nmIndexType nmCurChannel
); 
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTtxTimeOutProc
(
    const nmIndexType nmCurChannel
); 

/*OsekNm_013*/
#if defined ( NM_FOR_FAW )                    
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmRxMsgDecode
(
    const nmIndexType nmCurChannel
);             
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTxMsgCoding
(
    const nmIndexType nmCurChannel
); 
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTactiveminiTimeOutProc
(
    const nmIndexType nmCurChannel
);  
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTlimphomeTimeOutProc
(
    const nmIndexType nmCurChannel
);

/*Tactivemini timer count function*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTactiveminiCntFun
(
    const nmIndexType nmCurChannel
);

/*Tlimphome timer count function*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTlimphomeCntFun
(
    const nmIndexType nmCurChannel
);  

#endif
#define OSEKNM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNm_Init
*                
* Description:   API function called by Application.Initialise the Osek NM Model.
*                
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE)CanNm_Init(void) 
{
    nmIndexType nmIdx;
    nmIndexType nmCurChannel;
    uint32_t primask;
    /*OsekNm_State = 0;*/
    NmCanInterruptDisable();
    for(nmCurChannel=0; nmCurChannel < (nmIndexType)(NM_NUMBER_OF_CHANNELS); nmCurChannel++)
    {
        /*nmCurChannel = nmChannelIndex; */
        if(STD_ON == NM_USED_SWITCH)
        {
            /* Initialise nmStatus */
            nmStatus.R[0] = DATA_INIT;
            nmStatus.R[1] = DATA_INIT;
            /*#if defined ( NM_FOR_FAW ) */                  
            nmStatus.R[2] = DATA_INIT; 
            nmStatus.R[3] = DATA_INIT;
            /*#endif */     
            /*nmCanBusOffFlg = 0;*/
            /*OsekNm_008*/
            nubNmBusOffCnt = 0;   
            /*nmCanBusOffBeing = 0;*/
            /* Initialise nmTxDataFld & nmRxDataFld & Tx Buffer*/
            for (nmIdx=0; nmIdx < OSEKNM_DATA_LENGTH; nmIdx++)
            {
                nmTxDataFld.R[nmIdx] = DATA_INIT;
                nmRxDataFld.R[nmIdx] = DATA_INIT;
                NmTxBuffer[nmIdx] = DATA_INIT;
            }
           
            NmTxPduInfo.SduDataPtr = &NmTxBuffer[0];
            NmTxPduInfo.SduLength = (PduLengthType)OSEKNM_DATA_LENGTH;

           // CanNm_Stop(nmCurChannel);

            /* Set Active Mode */
            if(STD_ON == NM_DEFAULT_ACTIVE)
            {
                CanNm_Talk(nmCurChannel);
            }
            else
            {

            }

            nmStatusChnInit = NM_BIT_SET;/**/

            if(STD_ON == NM_DEFAULT_STRATED)
            {
                /*NmInitReset(xx,NM_D_BUSINIT) is call in CanNm_Start*/
                CanNm_Start(nmCurChannel);    
            }
            else
            {

            }
        }    
        else
        {
            nmStatusChnInit = NM_BIT_CLEAR;/**/
        }
    }
    /*OsekNm_State = 1;*/
    NmCanInterruptRestore();

}

#define OSEKNM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */


/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: NmBusErrorRecover
*
* Description:   Process the NM when BusOff.
*             
* Inputs:        initMode
* 
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmBusErrorRecover
(
    const nmIndexType nmCurChannel
)
{
    /*nmStateType nmNxtState_Par;*/
    const nmStateType nmNxtState_Par = NM_STATE_CODE_LIMPHOME;

    /*OsekNm_008*/
    NmCancelAlarm(nmCurChannel,NM_TIMER_All);

    nubNmBusOffCnt++;

#if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)
    /*nm config reset*/
    NmResetConfig(nmCurChannel);

#endif


#if defined ( NM_FOR_FAW )                            
    (nmCurChannel,NM_TIMER_LIMPHOME);                
    nmStatusLHTimeReach =  NM_BIT_CLEAR;    
#endif

    nmStatusErrRestoring = NM_BIT_SET;
    
    /*nmNxtState_Par = NM_STATE_CODE_LIMPHOME;*/
    
    CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);
#if ( NM_BUSOFF_FAST_RECOVERY_ENABLE != STD_OFF)

    if((nubNmBusOffCnt <= NM_BUSOFF_FAST_RECOVERY_TIMES)&&(0 == nmStatusSlowRecovery))    
    {
        if(nubNmBusOffCnt == NM_BUSOFF_FAST_RECOVERY_TIMES)
        {
            /* 1:Fast recover will  try last one time,Slow recover will be processed*/
            nmStatusSlowRecovery = NM_BIT_SET;
            nubNmBusOffCnt = 0;
        }
        else
        {

        }
        
        NmSetAlarm(nmCurChannel,NM_FAST_TIMER_TBUSOFF);
        /*CallBack function, inform Application Bus off*/
        ApplNmBusOff(nmCurChannel);    

    }
    else if(nubNmBusOffCnt <= NM_BUSOFF_SLOW_RECOVERY_TIMES)
#else
	if(nubNmBusOffCnt <= NM_BUSOFF_SLOW_RECOVERY_TIMES)
#endif
    {
        /*OsekNm_006*/
        NmSetAlarm(nmCurChannel,NM_SLOW_TIMER_TBUSOFF);
        /*CallBack function, inform Application Bus off*/
        ApplNmBusOff(nmCurChannel);    
    }
    else
    {
        nubNmBusOffCnt = 0;
        /*process fast recover first*/
        nmStatusSlowRecovery = NM_BIT_CLEAR;
        ApplNmCanBusOffRecoveryFail(nmCurChannel);
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: NmReadyToBusSleep
*
* Description:   Process.
*             
* Inputs:        initMode
* 
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(nmStateType, OSEKNM_PRIVATE_CODE) NmReadyToBusSleep
(
    const nmIndexType    nmCurChannel,
    const nmInitModeType initMode
)
{
    nmStateType nmNxtState_Par;

    if(initMode == NM_D_NORMAL_TO_BUSSLEEP)
    {

#if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)
        /*nm config reset*/
        NmResetConfig(nmCurChannel);
#endif        
        /* disable application communication (D_Offline) */
        (void)NmCanTxOffline(nmCurChannel);
        /*NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);*/
        /*NmCancelAlarm(nmCurChannel,NM_TIMER_TYP);*/
        NmCancelAlarm(nmCurChannel,NM_TIMER_All);
        NmSetAlarm(nmCurChannel,NM_TIMER_WBS);  
        nmNxtState_Par = NM_STATE_CODE_TWBSNORMAL;
     
        if(1 == NM_EXTENDED_CALLBACK_ENABLE)
        {
            ApplNmWaitBusSleep(nmCurChannel);
        }
        else
        {
            /* do nothing */
        }
    }
    else if(initMode == NM_D_LIMPHOME_TO_BUSSLEEP)
    {
    #if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)
        /*nm config reset*/
        NmResetConfig(nmCurChannel);
    #endif
        /* disable application communication (D_Offline) */
        (void)NmCanTxOffline(nmCurChannel);
        /*NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);*/
        NmCancelAlarm(nmCurChannel,NM_TIMER_All);
        NmSetAlarm(nmCurChannel,NM_TIMER_WBS);    
        nmNxtState_Par = NM_STATE_CODE_TWBSLIMPHOME;      
        if(1 == NM_EXTENDED_CALLBACK_ENABLE )
        {
            ApplNmWaitBusSleep(nmCurChannel);
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        nmNxtState_Par = CanNm_GetCurState(nmCurChannel);
    }
    return(nmNxtState_Par);
}
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: NmInitReset
*
* Description:   init the NM to the first state when logical ring first setup.
*             
* Inputs:        initMode
* 
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(nmStateType, OSEKNM_PRIVATE_CODE) NmInitReset
(
    const nmIndexType    nmCurChannel,
    const nmInitModeType initMode
)
{
    nmStateType nmNxtState_Par;

    NmCancelAlarm(nmCurChannel,NM_TIMER_All);

    /*Start: SDL Enter0,Enter3,Enter4,Enter5,Enter6*/
    switch(initMode)
    {
        /* initialise the hardware D_init(...;BusInit) */
        case NM_D_BUSINIT:
        {
            NMCanInit(nmCurChannel);
            /* config.limphome:=0*/
            nmRxCount = DATA_INIT;
            /* NMInitReset:NMrxcount:=0 and NMtxcount:=0 */
            nmTxCount = DATA_INIT;            
            ApplNmCanStart(nmCurChannel);
            /* enable application communication (D_Online) */
            (void)NmCanTxOnline(nmCurChannel);
            break;
        }
        case NM_D_BUSAWAKE:
        {
            /* Enter3: Leave NMBusSleep*/
            /* initialise the hardware D_init(...;BusAwake) */
            (void)NmCanSetModeStart(nmCurChannel);
            /* NMInitReset:NMrxcount:=0 and NMtxcount:=0 */
            nmRxCount = DATA_INIT;
            nmTxCount = DATA_INIT;

            /* enable application communication (D_Online) */
            (void)NmCanTxOnline(nmCurChannel);
            break;
        }
        case NM_D_LIMPHOME:
        {   
            /*NmCancelAlarm(nmCurChannel,NM_TIMER_ERR); to be canceled next*/
            /*OsekNm_013*/
            /* to be canceled next
            #if defined ( NM_FOR_FAW )                    
            NmCancelAlarm(nmCurChannel,NM_TIMER_LIMPHOME);            
            #endif
            */
            nmStatusMerkerLimpHome = NM_BIT_CLEAR;

            /* NMInitReset:NMrxcount:=0 and NMtxcount:=0 */
            nmRxCount = DATA_INIT;
            nmTxCount = DATA_INIT;

            ApplNmCanLimpHomeEnd(nmCurChannel);/*OsekNm_007*/
            /* enable application communication (D_Online) */
            (void)NmCanTxOnline(nmCurChannel);
            break;
        }
        case NM_D_NORMAL:
        /*Enter6: Leave NMNormal*/
        /*enable application communication (D_Online) */
        (void)NmCanTxOnline(nmCurChannel);
        break;

        default :
            /*do nothing */
        break;

    }
#if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)

    /*nm config reset*/
    NmResetConfig(nmCurChannel);
    /* config.present:=own station*/
    NmSetConfig(nmCurChannel,NM_OWN_STATION,CfgNormal);
    
#endif
    /* logical successor:=own station */
    nmLogicalSuccessor = NM_OWN_STATION;
    /*Receve message*/ 
    nmRxCount++;       

    if(1 == nmStatusNMActive)
    {
        /*send an alive message*/
        nmStatusTransmitFlag = NM_BIT_SET;
        nmTxDataFldOpCodeMsgType = ALIVE;
        nmTxDataFldOpCodeSleepInd = NM_BIT_CLEAR;
        nmTxDataFldOpCodeSleepAck = NM_BIT_CLEAR;
        nmTxDataFldByteDestID = nmLogicalSuccessor;
        NmTxPDUCreat(nmCurChannel);                          
        nmTxCount++;

        #if defined (NM_FOR_FAW)                    
        /*if alive message has already send*/    
        if(nmStatusAliveSended == 0)                 
        {
        /*if not send,set T[avtive_mini] Alarm*/
            nmStatusAliveSended = NM_BIT_SET;
            nmStatusActTimeReach = NM_BIT_CLEAR;
            NmSetAlarm(nmCurChannel,NM_TIMER_ACTIVEMINI);
        } 
        else 
        {
            /*nothing to do*/
        }
        #endif    
    }
    else
    {
        /*nothing to do*/
    }
    /*OsekNm_001*/
   


    if((nmRxCount <= RX_LIMIT) && (nmTxCount <= TX_LIMIT))      
    {
        NmSetAlarm(nmCurChannel,NM_TIMER_TYP);
        nmNxtState_Par = NM_STATE_CODE_NORMAL;
    }
    else
    {
        NmSetAlarm(nmCurChannel,NM_TIMER_ERR);
        #if defined (NM_FOR_FAW)                    
        NmSetAlarm(nmCurChannel,NM_TIMER_LIMPHOME);        
        nmStatusLHTimeReach =  NM_BIT_CLEAR;
        #endif
        nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
        /*OsekNm_007*/
        ApplNmCanLimpHomeStart(nmCurChannel);
    }
	
	nmStatusUseRingData = NM_BIT_CLEAR;
    return(nmNxtState_Par);
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTxPDUCreat
*
* Description:   coyp nm pdu to tx buffer.
*             
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTxPDUCreat
(
    const nmIndexType nmCurChannel
)                  
{
#if defined (NM_FOR_FAW)               
    NmTxMsgCoding(nmCurChannel);
#endif
    NmTxPDUCopy(nmCurChannel);
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTxPDUCopy
*
* Description:   coyp nm pdu to tx buffer.
*             
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTxPDUCopy
(
    const nmIndexType nmCurChannel
)
{
    nmIndexType nmIdx;
    uint32_t primask;
    NmCanInterruptDisable();
    for (nmIdx = 0; nmIdx < OSEKNM_DATA_LENGTH; nmIdx++)
    {
        /*Copy the NM Data Feild to Can Buffer array*/
        NmTxBuffer[nmIdx] = nmTxDataFld.R[nmIdx];
    }
    NmCanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmRxPDUCopy
*
* Description:   copy rx data to nm buffer.
*             
* Inputs:        canSduPtr
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmRxPDUCopy
(
    const nmIndexType nmCurChannel,
    CONSTP2CONST(PduInfoType, AUTOMATIC, OSEKNM_PRIVATE_DATA) canSduPtr
)
{
    nmIndexType nmIdx;
    uint32_t primask;
    
    NmCanInterruptDisable();
    if(canSduPtr != NULL_PTR)
    {
        if(((canSduPtr->SduLength) > 0)&&((canSduPtr->SduLength) <= OSEKNM_DATA_LENGTH))
        {
            for (nmIdx = 0; nmIdx < (canSduPtr ->SduLength); nmIdx++)
            {
                /*copy message data from Can register*/
                nmRxDataFld.R[nmIdx] = (canSduPtr ->SduDataPtr)[nmIdx];    
            }
        }
    }
    else
    {

    }

    NmCanInterruptRestore();
}
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmSetAlarm
*
* Description:   Set the Counts into defaut values.
*             
* Inputs:        nmAlarm
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmSetAlarm
(
    const nmIndexType nmCurChannel,
    const nmAlarmType nmAlarm
)
{
    switch(nmAlarm)
    {
        case NM_TIMER_TYP:
        nmTimerCntTtyp = NM_COUNT_TYP;
        break;

        case NM_TIMER_MAX:
        nmTimerCntTmax = NM_COUNT_MAX;
        break;

        case NM_TIMER_ERR:
        nmTimerCntTerr = NM_COUNT_ERR;
        break;

        case NM_TIMER_WBS:
		      nmTimerCntTwbs = NM_COUNT_WBS;
        break;

        case NM_TIMER_TX:
        nmTimerCntTtx = NM_COUNT_TX;
        break;
        
        /*OsekNm_006*/
        case NM_FAST_TIMER_TBUSOFF:   
        nmTimerCntTbusoff = NM_COUNT_FAST_TBUSOFF;
        break;
        
        /*OsekNm_006*/
        case NM_SLOW_TIMER_TBUSOFF:   
        nmTimerCntTbusoff = NM_COUNT_SLOW_TBUSOFF;
        break;
        
        #if defined (NM_FOR_FAW)                
        case NM_TIMER_ACTIVEMINI:          
        nmTimerCntTactivemini = NM_COUNT_TACTIVEMINI;
        break;
        
        case NM_TIMER_LIMPHOME:       
        nmTimerCntTlimphome = NM_COUNT_TLIMPHOME;
        break;
        #endif
        
        default :
            /* do nothing */
        break;

    }
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmCancelAlarm
*
* Description:   Cancel the Counts into defaut values.
*             
* Inputs:        nmAlarm
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmCancelAlarm
(
    const nmIndexType nmCurChannel,
    const nmAlarmType nmAlarm
)
{
    switch(nmAlarm)
    {
        case NM_TIMER_TYP:
        nmTimerCntTtyp = NM_BIT_CLEAR;
        break;

        case NM_TIMER_MAX:
        nmTimerCntTmax = NM_BIT_CLEAR;
        break;

        case NM_TIMER_ERR:
        nmTimerCntTerr = NM_BIT_CLEAR;
        break;

        case NM_TIMER_WBS:
        nmTimerCntTwbs = NM_BIT_CLEAR;
        break;

        case NM_TIMER_TX:
        nmTimerCntTtx = NM_BIT_CLEAR;
        break;
        
        /*OsekNm_006*/
        case NM_FAST_TIMER_TBUSOFF:    
        nmTimerCntTbusoff = NM_BIT_CLEAR;
        break;
        
        /*OsekNm_006*/
        case NM_SLOW_TIMER_TBUSOFF:   
        nmTimerCntTbusoff = NM_BIT_CLEAR;
        break;
        
        /*OsekNm_013*/    
#if defined (NM_FOR_FAW)                    
        case NM_TIMER_ACTIVEMINI:      
        nmTimerCntTactivemini = NM_BIT_CLEAR;
        break;
    
        case NM_TIMER_LIMPHOME:       
        nmTimerCntTlimphome = NM_BIT_CLEAR;
        break;
#endif
        case NM_TIMER_All:
        nmTimerCntTtyp = NM_BIT_CLEAR;
        nmTimerCntTmax = NM_BIT_CLEAR;
        nmTimerCntTerr = NM_BIT_CLEAR;
        nmTimerCntTwbs = NM_BIT_CLEAR;
        nmTimerCntTtx = NM_BIT_CLEAR;
        nmTimerCntTbusoff = NM_BIT_CLEAR;    
#if defined (NM_FOR_FAW)                    
        nmTimerCntTactivemini = NM_BIT_CLEAR;
        nmTimerCntTlimphome = NM_BIT_CLEAR;
#endif
        break;    
        default :
            /* do nothing */
        break;

    }
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmGetCurState
*
* Description:   Get the current state from parameter nmStatus of pointed node 
*             
* Inputs:        None
* 
* Outputs:       curState -- Current state of the pointed node
* 
* Limitations:   Value -- Output must in the set [0x00;0x01;0x02;0x03;0x04;
*                0x05;0x08;0x10]
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(nmStateType, OSEKNM_PRIVATE_CODE) CanNm_GetCurState
(
    const nmIndexType nmCurChannel
)
{
    nmStateType tempstate;
    tempstate = (nmStateType)(nmRealTimeStateCode);
    return tempstate;
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNm_SetNxtState
*
* Description:   Set the next state of pointed node
*                
* Inputs:        nxtState -- Next state of the pointed node
* 
* Outputs:       None
* 
* Limitations:   Value -- Input must in the set [0x00;0x01;0x02;0x03;0x04;
*                0x05;0x08;0x10]
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) CanNm_SetNxtState
(
    const nmIndexType nmCurChannel,
    const nmStateType nxtState
)
{
    uint32_t primask;
    /* Disable Interrupt */
    NmCanInterruptDisable();   
    nmRealTimeStateCode = nxtState;
    /* Restore Interrupt */
    NmCanInterruptRestore();
}

#define OSEKNM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNmTalk
*
* Description:   Set the node status to Active
*             
* Inputs:        nmChannelHandle
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Talk
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    /* Disable Interrupt */
    NmCanInterruptDisable();    
    if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(STD_ON == NM_USED_SWITCH))
    {
        nmStatusNMActive = NM_BIT_SET;
    }
    else
    {

    }
    /* Restore Interrupt */
    NmCanInterruptRestore();  
}                              

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNmSilent
*
* Description:   Set the node status to Passive
*                
* Inputs:        nmChannelHandle
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Silent
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    /* Disable Interrupt */
    NmCanInterruptDisable(); 
    if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(STD_ON == NM_USED_SWITCH))
    {
        nmStatusNMActive = NM_BIT_CLEAR;
    }
    else
    {

    }
    /* Restore Interrupt */
    NmCanInterruptRestore();    
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNmStart
*
* Description:   Application Layer call it to start the NM service
*             
* Inputs:        nmChannelHandle
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Start
(
    const nmIndexType nmCurChannel
)
{
    /*Step 1: Get the current State*/
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par; 
    uint32_t primask;

    NmCanInterruptDisable();

    if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(STD_ON == NM_USED_SWITCH))
    {
        
        nmCurState_Par = CanNm_GetCurState(nmCurChannel);
        /* State is NMOff */
        if(nmCurState_Par == NM_STATE_CODE_OFF)
        {
            /* sleep.ind:=0 and sleep.ack:=0 */
            /*move to NmInitReset*/
            /*nmTxDataFldOpCodeSleepInd = NM_BIT_CLEAR;*/
            /*nmTxDataFldOpCodeSleepAck = NM_BIT_CLEAR;*/
            /* initialise the hardware D_init(...;BusInit) */
            nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_BUSINIT);
        }
        /* NMState is not NMOff */
        else
        {
            /* nmNxtState_Par = CanNm_GetCurState(nmCurChannel);*/
            nmNxtState_Par = nmCurState_Par;
        }
        /*Step 3: Set the next State*/
        
        CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);
        
        if(1 == nmStatusTransmitFlag)
        {
            nmStatusTransmitFlag = NM_BIT_CLEAR;
            (void)NMCanTransmit();
        }
        else
        {
            /* do nothing */
        }

    }
    else 
    {

    }

    NmCanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNmStop
*
* Description:   Application Layer call it to stop the NM service
*             
* Inputs:        nmChannelHandle
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_Stop
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    NmCanInterruptDisable();
    if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(STD_ON == NM_USED_SWITCH))
    {
        CanNm_SetNxtState(nmCurChannel,NM_STATE_CODE_SHUTDOWN);
        nmStatusTransmitFlag = NM_BIT_CLEAR;
        /* NMInitReset:NMrxcount:=0 and NMtxcount:=0 */
        nmRxCount = DATA_INIT;
        nmTxCount = DATA_INIT;

        /*replace by  NmCancelAlarm(nmCurChannel,NM_TIMER_All);*/
        /*
        NmCancelAlarm(nmCurChannel,NM_TIMER_TYP);
        NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
        NmCancelAlarm(nmCurChannel,NM_TIMER_ERR);
        NmCancelAlarm(nmCurChannel,NM_TIMER_WBS);
        NmCancelAlarm(nmCurChannel,NM_TIMER_TX);
        NmCancelAlarm(nmCurChannel,NM_FAST_TIMER_TBUSOFF);

        #if defined (NM_FOR_FAW)                
        NmCancelAlarm(nmCurChannel,NM_TIMER_ACTIVEMINI);        
        NmCancelAlarm(nmCurChannel,NM_TIMER_LIMPHOME);        
        #endif
        */
        NmCancelAlarm(nmCurChannel,NM_TIMER_All);
    #if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)

        /*nm config reset*/
        NmResetConfig(nmCurChannel);
       
    #endif
        /*nmCurChannel =  nmOldChannelHandle;*/
        if(1 == NM_EXTENDED_CALLBACK_ENABLE)
        {
            ApplNmShutDown(nmCurChannel);
        }
        else
        {

        }
        CanNm_SetNxtState(nmCurChannel,NM_STATE_CODE_OFF);
    }
    else
    {

    }
    NmCanInterruptRestore();

}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNmGotoMode
*
* Description:   Application Layer call it to stop the NM service
*             
* Inputs:        nmChannelHandle
*                modeSleepAwake
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_GotoMode
(
    const nmIndexType nmCurChannel,
    const nmSleepAwakeModeType modeSleepAwake
)
{
    if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(STD_ON == NM_USED_SWITCH))
    {
        /**/
        if((GOTO_AWAKE == modeSleepAwake)&&(1 == nmStatusLocalSleepInd))
        {
            nmStatusNMModeInd = GOTO_AWAKE;
        }
        else
        {

        }
        if((GOTO_BUSSLEEP == modeSleepAwake)&&(0 == nmStatusLocalSleepInd))
        {
            nmStatusNMModeInd = GOTO_BUSSLEEP;
        }
        else
        {

        }
        /**/
    }
    else
    {

    }
    /*nmCurChannel =  nmOldChannelHandle;*/
}
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmModeChekFun
*
* Description:   Application Layer call it to stop the NM service
*             
* Inputs:        nmChannelHandle
*                modeSleepAwake
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmModeCheckFun
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    NmCanInterruptDisable();
    if(nmStatusNMModeInd == GOTO_AWAKE)
    {	
        nmStatusNMModeInd = 0;
        nmStatusLocalSleepInd = NM_BIT_CLEAR;
        NmGotoModeAwake(nmCurChannel);
    }
    else if(nmStatusNMModeInd == GOTO_BUSSLEEP)
    {
        nmStatusNMModeInd = 0;
        nmStatusLocalSleepInd = NM_BIT_SET;
        NmGotoModeSleep(nmCurChannel);
    }
    else
    {

    }
    NmCanInterruptRestore();
}
#define OSEKNM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

/*******************************************************************************
* Definations of FSM event functions -- Part1: Upper service Events Happened
*******************************************************************************/

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmGotoModeAwake
*
* Description:   Carry out this event Function when 
*                GotoMode(Awake) is called by Application layer
*
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmGotoModeAwake
(
    const nmIndexType nmCurChannel
)

{
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par;
    uint32_t primask;
    NmCanInterruptDisable();
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    switch(nmCurState_Par)
    {
        case NM_STATE_CODE_NORMALPREPSLEEP:
        NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
        NmSetAlarm(nmCurChannel,NM_TIMER_MAX);
        nmNxtState_Par = NM_STATE_CODE_NORMAL; 
        break;

        case NM_STATE_CODE_TWBSNORMAL:
        (void)NmCanTxOnline(nmCurChannel);
        NmCancelAlarm(nmCurChannel,NM_TIMER_WBS);
        /*NMInitReset: goto SDL Enter6*/
        nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_NORMAL);
        ApplNmWaitBusSleepCancel(nmCurChannel);
        break;

        case NM_STATE_CODE_LIMPHOMEPREPSLEEP:
        NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
        NmSetAlarm(nmCurChannel,NM_TIMER_ERR);
        #if defined (NM_FOR_FAW)                
        NmSetAlarm(nmCurChannel,NM_TIMER_LIMPHOME);            
        nmStatusLHTimeReach =  NM_BIT_CLEAR;    
        #endif
        nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
        break;

        case NM_STATE_CODE_TWBSLIMPHOME:
        /*OsekNm_005*/
        (void)NmCanTxOnline(nmCurChannel);
        NmCancelAlarm(nmCurChannel,NM_TIMER_WBS);
        NmSetAlarm(nmCurChannel,NM_TIMER_ERR);
        #if defined (NM_FOR_FAW)                
        NmSetAlarm(nmCurChannel,NM_TIMER_LIMPHOME);            
        nmStatusLHTimeReach =  NM_BIT_CLEAR;    
        nmStatusLHtoBusSleep = NM_BIT_CLEAR;
        #endif
        nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
        ApplNmWaitBusSleepCancel(nmCurChannel);
        break;

        case NM_STATE_CODE_BUSSLEEP:
        /*Enter3: Leave NMBusSleep*/
        nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_BUSAWAKE);
        break;

        default :
        nmNxtState_Par = CanNm_GetCurState(nmCurChannel);
        break;

    }
    /*Step 3: Set the next State*/

    /*NmCanInterruptDisable();*/

    CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);

    if(1 == nmStatusTransmitFlag)
    {
        nmStatusTransmitFlag = NM_BIT_CLEAR;
        (void)NMCanTransmit();
    }
    else
    {
        /* do nothing */
    }
    NmCanInterruptRestore();

}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmGotoModeSleep
*
* Description:   Carry out this event Function when 
*                GotoMode(Awake) is called by Application layer
*
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmGotoModeSleep
(
    const nmIndexType nmCurChannel
)
{
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par;
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    /* NMState is Normal */
    if(nmCurState_Par == NM_STATE_CODE_NORMAL)
    {
    #if defined (NM_FOR_FAW)                
        if((1 == nmStatusNMActive)||(1 !=nmStatusActTimeReach)) 
    #else
        if(1 == nmStatusNMActive) 
    #endif  
        {
            nmNxtState_Par = NM_STATE_CODE_NORMAL;
        }
        else
        {
            nmNxtState_Par = NM_STATE_CODE_NORMALPREPSLEEP;
        }
    }
    /* NMState is not Normal */
    else
    {
        nmNxtState_Par = CanNm_GetCurState(nmCurChannel);
    }
    /*Step 3: Set the next State*/
    CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);

}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTtypTimeOutProc
*
* Description:   Carry out this event Function when Time-out Ttyp
*                
* Inputs:        None
* 
* Outputs:       None
*
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTtypTimeOutProc
(
    const nmIndexType nmCurChannel
)
{
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par;
    uint32_t primask;
    NmCanInterruptDisable();
    
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    /* NMState is Normal */
    if(nmCurState_Par == NM_STATE_CODE_NORMAL)
    {
        NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
        NmSetAlarm(nmCurChannel,NM_TIMER_MAX);
        if(1 == nmStatusNMActive)
        {
            nmStatusTransmitFlag = NM_BIT_SET;
            nmTxDataFldOpCodeMsgType = RING;
            #if defined (NM_FOR_FAW)                
            if((1 == nmStatusLocalSleepInd) && (1 == nmStatusActTimeReach))  
            #else
            /*OsekNm_012*/ 
            if(1 == nmStatusLocalSleepInd)     
            #endif 
            {
                /* send ring message with sleep.ind bit: request bus sleep */                
                nmTxDataFldOpCodeSleepInd = NM_BIT_SET;            
            }
            else
            {        
                nmTxDataFldOpCodeSleepInd = NM_BIT_CLEAR;
            }
            nmTxDataFldOpCodeSleepAck = NM_BIT_CLEAR;
            nmTxDataFldByteDestID = nmLogicalSuccessor;
            NmTxPDUCreat(nmCurChannel);               
        
            nmTxCount++;
        }
        else
        {
            /* do nothing: if (nmStatusNMActive != 1) */
        }

        /*Rx and Tx of Counter limitUP*/
        /*nmTxCount > TX_LIMIT:enter limphome;nmRxCount>RX_LIMIT:enter reset*/
        if(nmTxCount > TX_LIMIT)    
        {
            /* Enter state NMLimpHome */
            NmSetAlarm(nmCurChannel,NM_TIMER_ERR);
            #if defined (NM_FOR_FAW)               
            NmSetAlarm(nmCurChannel,NM_TIMER_LIMPHOME);               
            nmStatusLHTimeReach =  NM_BIT_CLEAR;       
            #endif
            nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
            /*OsekNm_007*/
            ApplNmCanLimpHomeStart(nmCurChannel);
        }
        else
        {
            if(1 == nmStatusMerkerStable)
            {
                nmStatusNMCfgStable = NM_BIT_SET;
            }
            else
            {
                nmStatusMerkerStable = NM_BIT_SET;
            }
            
            nmNxtState_Par = NM_STATE_CODE_NORMAL;
        }
    }
    /* NMState is NormalPrepSleep */
    else if(nmCurState_Par == NM_STATE_CODE_NORMALPREPSLEEP)
    {
        /*OsekNm_009*/
        NmSetAlarm(nmCurChannel,NM_TIMER_MAX);       
        if(1 == nmStatusNMActive)
        {
            /*nmTxDataFldByteUser5 = 8;*/
            /* send ring message with sleep.ack bit */
            nmStatusTransmitFlag = NM_BIT_SET;
            nmTxDataFldOpCodeMsgType = RING;
            nmTxDataFldOpCodeSleepInd = NM_BIT_SET;
            nmTxDataFldOpCodeSleepAck = NM_BIT_SET;
            nmTxDataFldByteDestID = nmLogicalSuccessor;
            NmTxPDUCreat(nmCurChannel);              
        }
        else
        {
            /* do nothing */
        }
        
        nmNxtState_Par = NM_STATE_CODE_NORMALPREPSLEEP;
    }
    /* NMState is not Normal or NormalPrepSleep */
    else
    {
        nmNxtState_Par = CanNm_GetCurState(nmCurChannel);
    }
    /*Step 3: Set the next State*/
    /*NmCanInterruptDisable();*/
    CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);
  
    if(1 == nmStatusTransmitFlag)
    {
        nmStatusTransmitFlag = NM_BIT_CLEAR;
        (void)NMCanTransmit();
    }
    else
    {
        /* do nothing */
    }
    NmCanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTmaxTimeOutProc
*
* Description:   Carry out this event Function when Time-out Tmax
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTmaxTimeOutProc
(
    const nmIndexType nmCurChannel
)
{
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par;
    uint32_t primask;
    NmCanInterruptDisable();
    
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    /* NMState is Normal */
    if((nmCurState_Par == NM_STATE_CODE_NORMAL) || 
        (nmCurState_Par == NM_STATE_CODE_NORMALPREPSLEEP))
    {
        /*Enter6: Leave NMNormal*/
        nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_NORMAL);
    }
    /* NMState is NormalPrepSleep */
    else if(nmCurState_Par == NM_STATE_CODE_LIMPHOMEPREPSLEEP)
    {
        /* SDL Enter7: NMInitBusSleep to NMTwbsLimpHome */
        /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);ggg*/
        nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);
    }
    /* NMState is not Normal or NormalPrepSleep */
    else
    {
        nmNxtState_Par = nmCurState_Par;
    }
    /*Step 3: Set the next State*/

    /*NmCanInterruptDisable();*/

    CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);

    if(1 == nmStatusTransmitFlag)
    {
        nmStatusTransmitFlag = NM_BIT_CLEAR;
        (void)NMCanTransmit();
    }
    else
    {
        /* do nothing */
    }
    NmCanInterruptRestore();

}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTerrTimeOutProc
*
* Description:   Carry out this event Function when Time-out Terr
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTerrTimeOutProc    
(
    const nmIndexType nmCurChannel
)
{
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par;
    uint32_t primask;
    NmCanInterruptDisable();
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    /* NMState is LimpHome */
    if(nmCurState_Par == NM_STATE_CODE_LIMPHOME)
    {
    #if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)
        
        /*nm config reset*/
        /*NmResetConfig(nmCurChannel); */
        
    #endif
        /*OsekNm_007*/
        ApplNmLimpHomeState(nmCurChannel);
        /*OsekNm_004 end*/
        if(1 == nmStatusNMBusError )
        {
            nmStatusNMBusError = 0;
            /*clear CAN driver buffer*/
            NmCanClearBuffer(nmCurChannel);
            /*OsekNm_008 start*/
            if(1 == NM_EXTENDED_CALLBACK_ENABLE)
            {
                NMBusErrorCanInit(nmCurChannel);
                NMBusErrorCanStart(nmCurChannel);
                ApplNmBusOffEnd(nmCurChannel);
            }
            else
            {
                /* do nothing */
            }
            /* enable application communication (D_online) */
            (void)NmCanTxOnline(nmCurChannel);
            /*OsekNm_008 end*/
        }
        else
        {
            /*nothing to do*/
        }
        #if defined (NM_FOR_FAW)                   
        if((1 == nmStatusLocalSleepInd) && (1 == nmStatusLHTimeReach))
        #else
        if(1 == nmStatusLocalSleepInd) 
        #endif
        {
            NmSetAlarm(nmCurChannel,NM_TIMER_MAX);
            
            if(1 == nmStatusNMActive)
            {
                /* send limphome message with set bit sleep.ind */
                nmStatusTransmitFlag = NM_BIT_SET;
                nmTxDataFldOpCodeMsgType = LIMPHOME;
                nmTxDataFldOpCodeSleepInd = NM_BIT_SET;
                nmTxDataFldOpCodeSleepAck = NM_BIT_CLEAR;
                nmTxDataFldByteDestID = nmLogicalSuccessor;
                NmTxPDUCreat(nmCurChannel);                      
            }
            else
            {
                /* do nothing */
            }
            #if defined (NM_FOR_FAW)               
            nmStatusLHTimeReach = NM_BIT_CLEAR;    
            #endif        
            nmNxtState_Par = NM_STATE_CODE_LIMPHOMEPREPSLEEP;

			nmStatusUseRingData = NM_BIT_CLEAR;
        }
        /* (nmTxDataFldOpCodeSleepInd == 1) is false */
        else
        {
            NmSetAlarm(nmCurChannel,NM_TIMER_ERR);

            if(1 == nmStatusNMActive)
            {
                /* send limphome message */
				nmTxDataFldOpCodeSleepInd = NM_BIT_CLEAR;
				if(nmStatusUseRingData != NM_BIT_CLEAR)
				{
					nmTxDataFldOpCodeSleepInd = NM_BIT_SET;
					nmStatusUseRingData = NM_BIT_CLEAR;
				}
				else
				{

				}
                nmStatusTransmitFlag = NM_BIT_SET;
                nmTxDataFldOpCodeMsgType = LIMPHOME;
                nmTxDataFldOpCodeSleepAck = NM_BIT_CLEAR;
                nmTxDataFldByteDestID = nmLogicalSuccessor;
                NmTxPDUCreat(nmCurChannel);              
            }
            else
            {
                nmStatusUseRingData = NM_BIT_CLEAR;/* do nothing */
            }
            nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
        }
    }
    /* NMState is not LimpHome */
    else
    {
        nmNxtState_Par = CanNm_GetCurState(nmCurChannel);
    }
    /*Step 3: Set the next State*/
    /*NmCanInterruptDisable();*/
    CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);

    if(1 == nmStatusTransmitFlag)
    {
        nmStatusTransmitFlag = NM_BIT_CLEAR;
        (void)NMCanTransmit();
    }
    else
    {
        /* do nothing */
    }
    NmCanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTwbsTimeOutProc
*
* Description:   Carry out this event Function when Time-out Twbs
*                
* Inputs:        None
* 
* Outputs:       None
*
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/

STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTwbsTimeOutProc    
(
    const nmIndexType nmCurChannel
)
{    
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par;
    uint32_t primask;
    NmCanInterruptDisable();
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    /* NMState is Normal or LimpHome */
    if((nmCurState_Par == NM_STATE_CODE_TWBSNORMAL) ||
        (nmCurState_Par == NM_STATE_CODE_TWBSLIMPHOME))
    {
        nmNxtState_Par = NM_STATE_CODE_BUSSLEEP;
        CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);
        #if defined (NM_FOR_FAW)               
        /* Clear T[Active_mini]andT[LimpHome]correlative Flag bit*/
        nmStatusAliveSended = NM_BIT_CLEAR;
        nmStatusActTimeReach = NM_BIT_CLEAR;
        nmStatusLHTimeReach = NM_BIT_CLEAR;
        nmStatusLHtoBusSleep = NM_BIT_CLEAR;
        #endif
        /* initialize the sleep mode of the bus hardware */
        ApplNmCanSleep(nmCurChannel);/*liubo*/
    }
    /* NMState is not Normal or LimpHome */
    else
    {
        nmNxtState_Par = CanNm_GetCurState(nmCurChannel);
        CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);
    }
    /*Step 3: Set the next State*/

    NmCanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTtxTimeOutProc
*
* Description:   Carry out this event Function when Time-out Ttx
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTtxTimeOutProc
(
    const nmIndexType nmCurChannel
)
{
    nmStateType nmCurState_Par;
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    if((nmCurState_Par == NM_STATE_CODE_NORMAL) ||
       (nmCurState_Par == NM_STATE_CODE_NORMALPREPSLEEP))
    {
        /* Send actual NM message (Ring or Alive) */
        NmTxPDUCreat(nmCurChannel);       
        (void)NMCanTransmit();
    }
    else
    {
        /*nothing to do*/    
    }
}

#if defined (NM_FOR_FAW)               
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTactiveminiTimeOutProc
*
* Description:   Carry out this event Function when Time-out T[active_mini]
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTactiveminiTimeOutProc
(
    const nmIndexType nmCurChannel
)       
{
/*******************************************************************************
  When T[Active_Mini] time-out,setup the time-out Flag bit nmStatusActTimeReach; 
*******************************************************************************/
    nmStatusActTimeReach = NM_BIT_SET;
    /*End of T[active_mini] Time-out Proc function.*/
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTlimphomeTimeOutProc
*
* Description:   Carry out this event Function when Time-out T[LimpHome]
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTlimphomeTimeOutProc
(
    const nmIndexType nmCurChannel
)       
{
    /*Step 1: Get the current State*/
    nmStateType nmCurState_Par;

    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    /* NMState is LimpHome */
    if(nmCurState_Par == NM_STATE_CODE_LIMPHOME)
    {
        nmStatusLHTimeReach =  NM_BIT_SET;
    }
}
#endif
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTbusOffRecoveryTimeOutProc
*
* Description:   BusOff recovery process
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
/*OsekNm_006*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTBusOffRecoveryTimeOutProc
(
    const nmIndexType nmCurChannel
)
{
    NmTerrTimeOutProc(nmCurChannel);
}

/*******************************************************************************
 * Definations of FSM event functions -- Part3: Bottom flag Events Happened
*******************************************************************************/
 
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTxConfirmation
*
* Description:   Carry out this event Function when Transmitted any NM message
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTxConfirmation
(
    const nmIndexType nmCurChannel
)
{
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par;
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);
    /*Step 2: Carry Out the ralated Action*/
    /* NMState is Normal */
    if(nmCurState_Par == NM_STATE_CODE_NORMAL)
    {
        nmTxCount = NM_BIT_CLEAR;
        if(nmTxDataFldOpCodeMsgType == RING)
        {
            NmCancelAlarm(nmCurChannel,NM_TIMER_TYP);
            NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
            NmSetAlarm(nmCurChannel,NM_TIMER_MAX);

            if((1 == nmTxDataFldOpCodeSleepInd) &&
                (1 == nmStatusLocalSleepInd))
            {
                /*nmTxDataFldOpCodeSleepAck = NM_BIT_SET;*/
                /* change Normal to  NormalPrepSleep */
                nmNxtState_Par = NM_STATE_CODE_NORMALPREPSLEEP;   
            }
            else
            {    /* change NormalPrepSleep to Normal */
                nmNxtState_Par = NM_STATE_CODE_NORMAL;
            }
        }
        else
        {
            nmNxtState_Par = NM_STATE_CODE_NORMAL;
        }
    }
    /* NMState is NormalPrepSleep */
    else if(nmCurState_Par == NM_STATE_CODE_NORMALPREPSLEEP)
    {
        /* send Messge with ring and sleep ack */
        /*if (nmTxDataFldOpCodeMsgType == RING)*/
        if((nmTxDataFldOpCodeMsgType == RING) && 
            (1 == nmTxDataFldOpCodeSleepAck))
        {
            /* SDL Enter2: NMInitBusSleep to NMTwbsNormal */ 
            /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_NORMAL_TO_BUSSLEEP);ggg*/
            nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_NORMAL_TO_BUSSLEEP);
        }
        else
        {
            nmNxtState_Par = NM_STATE_CODE_NORMALPREPSLEEP;
        }
    }
    /* NMState is LimpHome */
    else if(nmCurState_Par == NM_STATE_CODE_LIMPHOME)
    {
        if(nmTxDataFldOpCodeMsgType == LIMPHOME)
        {
            nmStatusMerkerLimpHome = NM_BIT_SET;
        }
        else
        {
            /* do nothing */
        }
        nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
    }
    /* NMState is LIMPHOMEPREPSLEEP */
    else if(nmCurState_Par == NM_STATE_CODE_LIMPHOMEPREPSLEEP)
    {
        /*Send Limphome Message ?*/
        if(nmTxDataFldOpCodeMsgType == LIMPHOME)
        {
            /*set MerkerLimpHome*/
            nmStatusMerkerLimpHome = NM_BIT_SET;
        }
        else
        {
            /* do nothing */
        }
        nmNxtState_Par = NM_STATE_CODE_LIMPHOMEPREPSLEEP;
    }
    /* NMState is not Normal, NormalPrepSleep and LimpHome */
    else
    {
        nmNxtState_Par = CanNm_GetCurState(nmCurChannel);
    }

    /*Step 3: Set the next State*/
    CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmRxIndication
*
* Description:   Carry out this event Function when Received any NM message
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmRxIndication
(
    const nmIndexType nmCurChannel
)
{
    nmStateType nmCurState_Par;
    nmStateType nmNxtState_Par;
    NmReturnType Ret;
    uint32_t primask;
    NmCanInterruptDisable();
    
    /*Step 1: Get the current State*/
    nmCurState_Par = CanNm_GetCurState(nmCurChannel);

    /*Step 2: Carry Out the ralated Action*/
    switch( nmCurState_Par )
    {
        /****************************************************************
        The case is from NmRemoteWakeupSignal ;
        replace function :NmRemoteWakeupSignal and CanNm_WakeupSignal
        ****************************************************************/
        case NM_STATE_CODE_BUSSLEEP:
        {
            /* handle the remote sakeup signal */
            if(1 == NM_REMOTE_WAKEUP_ENABLE )
            {
                ApplNmCanWakeUp(nmCurChannel);
                nmNxtState_Par = nmCurState_Par; 
            }
            else
            {
                nmNxtState_Par = NM_STATE_CODE_BUSSLEEP;
            }
            
        }
        break;

        case NM_STATE_CODE_NORMAL:
        {
            /* do the procedure of NormalStandardNM and  NORMALPREPSLEEP*/
            Ret = NmStandardNormal(nmCurChannel);
            if(NM_E_OK == Ret)
            {
                if(1 == nmRxDataFldOpCodeSleepAck)    
                {
                    if(1 == nmStatusLocalSleepInd)
                    {
                        /* SDL Enter2: NMInitBusSleep to NMTwbsNormal */
                        /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_NORMAL_TO_BUSSLEEP);ggg*/
                        nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_NORMAL_TO_BUSSLEEP);
                    }
                    else
                    {
                        /*nmCurState_Par = NM_STATE_CODE_NORMAL;*/
                        nmNxtState_Par = NM_STATE_CODE_NORMAL;
                    }
                }
                else
                {
                    /*nmCurState_Par = NM_STATE_CODE_NORMAL;*/
                    nmNxtState_Par = NM_STATE_CODE_NORMAL;
                }
            }
            else
            {
                nmNxtState_Par = NM_STATE_CODE_NORMAL;
            }
        }
        break;
        
        case NM_STATE_CODE_NORMALPREPSLEEP:
        {
                
            Ret = NmStandardNormal(nmCurChannel); 
            if(NM_E_OK == Ret)
            {
                if((1 == nmRxDataFldOpCodeSleepAck) &&
                    (1 == nmStatusLocalSleepInd))
                {
                    /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_NORMAL_TO_BUSSLEEP);ggg*/
                    nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_NORMAL_TO_BUSSLEEP);
                }
                else if(1 == nmRxDataFldOpCodeSleepInd)
                {
                    nmNxtState_Par = NM_STATE_CODE_NORMALPREPSLEEP;
                }
                else
                {
                    nmNxtState_Par = NM_STATE_CODE_NORMAL;
                }
            }
            else
            {
                nmNxtState_Par = NM_STATE_CODE_NORMALPREPSLEEP;
            }
        }
        break;   
        
        case NM_STATE_CODE_TWBSNORMAL:
        {
            #if defined (NM_FOR_FAW)  
            if((nmRxDataFldOpCodeSleepInd == 0) ||
                (nmRxDataFldOpCodeSleepAck == 0))        
            #else
            if(nmRxDataFldOpCodeSleepInd == 0)
            #endif
            {
                #if defined (NM_FOR_FAW)                
                nmStatusLocalSleepInd = NM_BIT_CLEAR;
                #endif
                NmCancelAlarm(nmCurChannel,NM_TIMER_WBS);
                if(1 == NM_EXTENDED_CALLBACK_ENABLE )
                {
                    ApplNmWaitBusSleepCancel(nmCurChannel);
                }
                else
                {
                    /* do nothing */
                }
                /*SDL Enter6: leave NMNormal*/
                nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_NORMAL);
            }
            else
            {
                nmNxtState_Par = NM_STATE_CODE_TWBSNORMAL;
            }
        }
        break;
        
        case NM_STATE_CODE_TWBSLIMPHOME:
        {
            #if defined (NM_FOR_FAW)                       
            if((nmRxDataFldOpCodeSleepInd == 0) ||
                (nmRxDataFldOpCodeSleepAck == 0))
            #else
            if(nmRxDataFldOpCodeSleepInd == 0)
            #endif
            {
                NmCancelAlarm(nmCurChannel,NM_TIMER_WBS);
                if(1 == NM_EXTENDED_CALLBACK_ENABLE )
                {
                    ApplNmWaitBusSleepCancel(nmCurChannel);
                }
                else
                {
                    /* do nothing */
                }
                /* CAN Communication on*/
                (void)NmCanTxOnline(nmCurChannel);
                #if defined (NM_FOR_FAW)              
                if(1 == nmStatusLHtoBusSleep)
                {
                    nmStatusLHtoBusSleep = NM_BIT_CLEAR;
                    NmSetAlarm(nmCurChannel,NM_TIMER_ERR);
                    NmSetAlarm(nmCurChannel,NM_TIMER_LIMPHOME);      
                    nmStatusLHTimeReach =  NM_BIT_CLEAR; 
                    nmNxtState_Par = NM_STATE_CODE_LIMPHOME;    
                    
                }
                else
                {
                    NmSetAlarm(nmCurChannel,NM_TIMER_MAX);
                    nmNxtState_Par = NM_STATE_CODE_LIMPHOMEPREPSLEEP;        
                }
                #else
				nmStatusUseRingData = NM_BIT_SET;
                NmSetAlarm(nmCurChannel,NM_TIMER_ERR);
                nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
                #endif
            }
            else
            {
                nmNxtState_Par = NM_STATE_CODE_TWBSLIMPHOME;
            }
        }
        break;
        
        case NM_STATE_CODE_LIMPHOMEPREPSLEEP:
        {        
            /*****************************************************************/
            /* The case is according to OSEKNM Page 45 figure*/
            if(nmStatusNMActive == 1)
            {
                if((nmRxDataFldOpCodeSleepAck == 1)&&(nmStatusLocalSleepInd == 1))
                {
                    /*Cancel Tmax in NmGoToSleepFun(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);*/
                    /*NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);*/
                    /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);ggg*/
                    nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);
                }
                else if(nmRxDataFldOpCodeSleepInd == 0)
                {
                    NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
                    NmSetAlarm(nmCurChannel,NM_TIMER_ERR);
                #if defined (NM_FOR_FAW)            
                    NmSetAlarm(nmCurChannel,NM_TIMER_LIMPHOME);          
                    nmStatusLHTimeReach =  NM_BIT_CLEAR;
                #endif
                    nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
                }
                else if(nmRxDataFldOpCodeSleepInd == 1)
                {
                    nmNxtState_Par = NM_STATE_CODE_LIMPHOMEPREPSLEEP;
                }				
                else if(( nmStatusMerkerLimpHome == 1)&&
                    (((nmStatusLocalSleepInd == 0)&&(1))||
                    ((nmStatusLocalSleepInd == 1)&&
                    (nmRxDataFldOpCodeSleepAck == 0))))
                /*else if ((1)&&(((nmStatusLocalSleepInd == 0)&&(1))||((nmStatusLocalSleepInd == 1u)&&(nmRxDataFldOpCodeSleepAck == 0))))*/
                {
                    /*NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);*/
                    nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME);
                }

                else
                {/*nmStatusMerkerLimpHome == 0,nmRxDataFldOpCodeSleepInd ==1*/
                    nmNxtState_Par = NM_STATE_CODE_LIMPHOMEPREPSLEEP;
                }

            }
            else
            {
                if((nmRxDataFldOpCodeSleepAck == 1)&&(nmStatusLocalSleepInd == 1))
                {
                    /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);ggg*/
                    nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);
                }

                
                else if(nmRxDataFldOpCodeSleepInd == 0)
                {
                    NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
                    NmSetAlarm(nmCurChannel,NM_TIMER_ERR);
                #if defined (NM_FOR_FAW)            
                    NmSetAlarm(nmCurChannel,NM_TIMER_LIMPHOME);        
                    nmStatusLHTimeReach =  NM_BIT_CLEAR;
                #endif
                    nmNxtState_Par = NM_STATE_CODE_LIMPHOME;
                }
                else if(nmRxDataFldOpCodeSleepInd == 1)
                {
                    nmNxtState_Par = NM_STATE_CODE_LIMPHOMEPREPSLEEP;
                }
                else if(((nmStatusLocalSleepInd == 0)&&(1))||
                    ((nmStatusLocalSleepInd == 1)&&
                    (nmRxDataFldOpCodeSleepAck == 0)))
                {
                    nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME);
                }
                				
                else
                {
                    nmNxtState_Par = NM_STATE_CODE_LIMPHOMEPREPSLEEP;
                }
            }
            /*****************************************************************/
        }
        break;
        
        case NM_STATE_CODE_LIMPHOME:
        {
            if(1 == nmStatusNMActive)
            {
                if(1== nmStatusMerkerLimpHome)
                {
                    if(1 == nmStatusLocalSleepInd)
                    {
                        if(1 == nmRxDataFldOpCodeSleepAck)
                        {
                            /* SDL Enter7: NMInitBusSleep to NMTwbsLimpHome */
                            #if defined (NM_FOR_FAW)   
                            nmStatusLHtoBusSleep = NM_BIT_SET;
                            #endif
                            /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);ggg*/
                            nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);
                        }
                        else
                        {
                            /* SDL Enter5: leave NMLimpHome */
                            nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME);
                        }
                    }
                    else
                    {
                        /* SDL Enter5: leave NMLimpHome */
                        nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME);
                    }
                }
                else
                {
                    if(1 == nmRxDataFldOpCodeSleepAck)
                    {
                        /* SDL Enter7: NMInitBusSleep to NMTwbsLimpHome */
                        #if defined (NM_FOR_FAW)           
                        nmStatusLHtoBusSleep = NM_BIT_SET;
                        #endif
                        /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);ggg*/
                        nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);
                    }
                    else
                    {
                        /* SDL Enter5: leave NMLimpHome */
                        nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME);
                    }
                }
            }
            else
            {
                if(1 == nmStatusLocalSleepInd)
                {
                    if(1 == nmRxDataFldOpCodeSleepAck)
                    {
                        /* SDL Enter7: NMInitBusSleep to NMTwbsLimpHome */
                        #if defined (NM_FOR_FAW)     
                        nmStatusLHtoBusSleep = NM_BIT_SET;
                        #endif
                        /*nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);ggg*/
                        nmNxtState_Par = NmReadyToBusSleep(nmCurChannel,NM_D_LIMPHOME_TO_BUSSLEEP);
                    }
                    else
                    {
                        /* SDL Enter5: leave NMLimpHome */
                        nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME);
                    }
                }
                else
                {
                    /* SDL Enter5: leave NMLimpHome */
                    nmNxtState_Par = NmInitReset(nmCurChannel,NM_D_LIMPHOME);
                }
            }
            #if defined (NM_FOR_FAW)      
            NmCancelAlarm(nmCurChannel,NM_TIMER_LIMPHOME);   
            #endif
        }
        break;
        
        default :      
        nmNxtState_Par = CanNm_GetCurState(nmCurChannel);
        break;
    
    }

    /*Step 3: Set the next State*/
    /*NmCanInterruptDisable();*/
    CanNm_SetNxtState(nmCurChannel,nmNxtState_Par);
    if(1 == nmStatusTransmitFlag)
    {
        nmStatusTransmitFlag = NM_BIT_CLEAR;
        (void)NMCanTransmit();
    }
    else
    {
        /* do nothing */
    }  
    NmCanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmStandardNormal
*
* Description:   Procedure of Normal Standard NM
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(NmReturnType, OSEKNM_PRIVATE_CODE) NmStandardNormal
(
    const nmIndexType nmCurChannel
)
{
    NmReturnType Ret = NM_E_OK;

    /* Clear NMrxcount */
    nmRxCount = NM_BIT_CLEAR;
  
    if(nmRxDataFldOpCodeMsgType == LIMPHOME)
    {
    #if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)
        /* add sender to config.limphome */
        NmSetConfig(nmCurChannel,nmRxMsgSourceId_S,CfgLimpHome);
    #endif
        #if defined (NM_ENABLE_LIMPHOME_RX_IND)
        ApplNmLimpHIndication(nmCurChannel,nmRxMsgSourceId_S);
        #endif
    }
    else if(nmRxDataFldOpCodeMsgType == RING)
    {
        /*********************start*********************/
        /* add sender to config.present */
    #if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)
        NmSetConfig(nmCurChannel,nmRxMsgSourceId_S,CfgNormal);
    #endif
        #if defined (NM_ENABLE_RING_RX_IND)
        ApplNmRingIndication(nmCurChannel,nmRxMsgSourceId_S);
        #endif
    
        /* determine logical successor */
        NmUpdateLogSuc(nmCurChannel);
        /**********************end********************/
        
        NmCancelAlarm(nmCurChannel,NM_TIMER_TYP);
        NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
        /*successor == Local ID*/ /*CR1-6*/
        if((nmRxDataFldByteDestID==NM_OWN_STATION)|| /*D == R*/
            (nmRxDataFldByteDestID==nmRxMsgSourceId_S)) /*D == S*/  
        {
            NmSetAlarm(nmCurChannel,NM_TIMER_TYP);
        }
        else
        {
            NmSetAlarm(nmCurChannel,NM_TIMER_MAX);

            /* determine if Skipped in the logical ring */
            NmJudgeSkipped(nmCurChannel);
    
            if(1 == nmStatusImSkipped)
            {
                nmStatusImSkipped = NM_BIT_CLEAR;
                if(1 == nmStatusNMActive)
                {
                    #if defined (NM_FOR_FAW)  
                    if((1 == nmStatusLocalSleepInd) &&
                        (1 == nmStatusActTimeReach))
                    #else
                    if(1 == nmStatusLocalSleepInd)
                    #endif
                    {
                        /* send an alive message with set bit sleep.ind */
                        nmTxDataFldOpCodeSleepInd = NM_BIT_SET;                  
                    }
                    else
                    {
                        /* send an alive message */                
                        nmTxDataFldOpCodeSleepInd = NM_BIT_CLEAR;
                    }
                    nmStatusTransmitFlag = NM_BIT_SET;
                    nmTxDataFldOpCodeMsgType = ALIVE;
                    nmTxDataFldOpCodeSleepAck = NM_BIT_CLEAR;
                    nmTxDataFldByteDestID = nmLogicalSuccessor;
                    NmTxPDUCreat(nmCurChannel);           
                    #if defined (NM_FOR_FAW)                                          
                    /*if alive message has already send*/
                    if(nmStatusAliveSended == 0)                 
                    {
                    /*if not send,set T[avtive_mini] Alarm*/
                        nmStatusAliveSended = NM_BIT_SET;
                        NmSetAlarm(nmCurChannel,NM_TIMER_ACTIVEMINI);
                     } 
                     else 
                     {
                        /*nothing to do*/
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
    else if(nmRxDataFldOpCodeMsgType == ALIVE)
    {
        /*********************start*********************/
    #if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)
        /* add sender to config.present */
        NmSetConfig(nmCurChannel,nmRxMsgSourceId_S,CfgNormal);
    #endif
        /* determine logical successor */
        NmUpdateLogSuc(nmCurChannel);
        /*********************end*********************/

        /* NMMerker.stable:=0 and networkstatus.configurationstable:=0 */
        nmStatusMerkerStable = NM_BIT_CLEAR;
        nmStatusNMCfgStable = NM_BIT_CLEAR;
    }
    else
    {
        /*nothing to do*/
        Ret = NM_E_NOT_OK;
    }

    return Ret;
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmUpdateLogSuc
*
* Description:   Update the Logical Successor.
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmUpdateLogSuc
(
    const nmIndexType nmCurChannel
)
{
    /* R: Receiver is the id of receive node -- nmReceiverId_R */
    /* L: Log.successor of the receive node -- nmLogSucId_L */
    /* S: Source id of the RxMsg -- nmRxMsgSourceId_S */
    if(nmLogSucId_L == nmReceiverId_R)
    {
        /* update logical successor: L=S */
        nmLogSucId_L = nmRxMsgSourceId_S;
    }
    else
    {
        if(nmLogSucId_L < nmReceiverId_R)
        {
            if(nmRxMsgSourceId_S < nmLogSucId_L)
            {
                /* update logical successor: L=S */
                nmLogSucId_L = nmRxMsgSourceId_S;
            }
            else
            {
                if(nmRxMsgSourceId_S < nmReceiverId_R)
                {
                  /* do nothing */
                }
                else
                {
                    /* update logical successor: L=S */
                    nmLogSucId_L = nmRxMsgSourceId_S;
                }
            }
        }
        else
        {
            if(nmRxMsgSourceId_S < nmLogSucId_L)
            {
                if(nmRxMsgSourceId_S < nmReceiverId_R)
                {
                    /* do nothing */
                }
                else
                {
                    /* update logical successor: L=S */
                    nmLogSucId_L = nmRxMsgSourceId_S;
                }
            }
            else
            {
                /* do nothing */
            }
        }
    }
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmJudgeSkipped
*
* Description:   Determine if the receive node is Skipped.
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmJudgeSkipped
(
    const nmIndexType nmCurChannel
)
{
    /* S: Source id of the RxMsg -- nmRxMsgSourceId_S */
    /* D: Destination id of the RxMsg -- nmRxMsgDestId_D */
    /* R: Receiver is the id of receive node -- nmReceiverId_R */
    if(nmRxMsgDestId_D < nmReceiverId_R)
    {
        if(nmRxMsgSourceId_S < nmRxMsgDestId_D)
        {
            /* not skipped: do nothing */
        }
        else
        {
            if(nmRxMsgSourceId_S < nmReceiverId_R)
            {
                /* skipped: Set nmStatus.ImSkipped:=1 */
                nmStatusImSkipped = 1;
            }
            else
            {
                /* not skipped: do nothing */
            }
        }
    }
    else
    {
        if(nmRxMsgSourceId_S < nmRxMsgDestId_D)
        {
            if(nmRxMsgSourceId_S < nmReceiverId_R)
            {
                /* skipped: Set nmStatus.ImSkipped:=1 */
                nmStatusImSkipped = 1;
            }
            else
            {
                /* not skipped: do nothing */
            }
        }
        else
        {
            /* skipped: Set nmStatus.ImSkipped:=1 */
            nmStatusImSkipped = 1;
        }
    }
}

#define OSEKNM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNm_MainFunction
*
* Description:   cycle function
*                
* Inputs:        nmChannelHandle
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_MainFunction(void)
{
    /*nmCurChannel = nmChannelHandle;*/
    nmIndexType nmCurChannel;
    
    for(nmCurChannel = 0;nmCurChannel < NM_NUMBER_OF_CHANNELS;nmCurChannel++)    
    {    /*if inited,(1 == nmStatusChnInit) is syc with (STD_ON== NM_ACTIVE_SWITCH) */
        if((1 == nmStatusChnInit)&&(STD_ON == NM_USED_SWITCH))
        {
            NmModeCheckFun(nmCurChannel);
            #if defined (NM_FOR_FAW)                
            /*used by OsekNm when Time-out Tmini */
            NmTimerTactiveminiCntFun(nmCurChannel);                
            /*used by OsekNm when Time-out TlimpHome */
            NmTimerTlimphomeCntFun(nmCurChannel);                  
            #endif
            /*used by OsekNM when Can Bus Off*/
            NmCanBusOffCheckFun(nmCurChannel);
            /* used by OsekNM when Time-out Ttyp */
            NmTimerTTypCntFun(nmCurChannel);
            /* used by OsekNM when Time-out Tmax */
            NmTimerTMaxCntFun(nmCurChannel);
            /* used by OsekNM when Time-out Terr */
            NmTimerTErrCntFun(nmCurChannel);
            /* used by OsekNM when Time-out Twbs */
            NmTimerTWBSCntFun(nmCurChannel);
            /* used by OsekNM when Reption    Time-out Ttx */
            NmTimerTTxCntFun(nmCurChannel);
            /*used by OsekNm when Bus Off recovery*/
            NmTimerTBusOffCntFun(nmCurChannel);

        }
        else
        {

        }
        
    }
}

#define OSEKNM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmCanBusOffCheckFun
*
* Description:   bus-off
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmCanBusOffCheckFun
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    if(1 == nmStatusNMBusOffFlag)
    {
        NmCanInterruptDisable();
        
        (void)NmCanTxOffline(nmCurChannel);
        /*Rx counter clear*/
        nmRxCount = DATA_INIT;
        /*Tx counter clear*/
        nmTxCount = DATA_INIT;  
        /*Tx counter ++*/
        nmTxCount++; 
           
        nmStatusNMBusOffFlag = NM_BIT_CLEAR;
        nmStatusNMBusError = NM_BIT_SET;
        /*******************************************************
               nmCanBusOffBeing = 1; replace by 
              nmStatusErrRestoring and move to NmBusErrorRecover
              *******************************************************/
        /*******************************************************
        move to NmBusOffProcessFun and replace by 
        NmCancelAlarm(nmCurChannel,NM_TIMER_All);

        NmCancelAlarm(nmCurChannel,NM_TIMER_TYP);
        NmCancelAlarm(nmCurChannel,NM_TIMER_MAX);
        NmCancelAlarm(nmCurChannel,NM_TIMER_ERR);
        NmCancelAlarm(nmCurChannel,NM_TIMER_WBS);
        NmCancelAlarm(nmCurChannel,NM_TIMER_TX);
             *******************************************************/        
        NmCanClearBuffer(nmCurChannel);
        /* 因为复旦微芯片 没有退出发送功能，在这里初始化了就会 马上总线恢复，所以要移到 总线恢复定时器那里重新初始化*/
        //NMBusErrorCanInit(nmCurChannel);
        //NMBusErrorCanStart(nmCurChannel);
        /*Bus off recovery*/
        /*(void)NmInitReset(nmCurChannel,NM_D_BUSOFF); */
        NmBusErrorRecover(nmCurChannel);

        NmCanInterruptRestore();
    }
    else
    {
        /*do nothing*/
    }
}
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTimerTTypCntFun
*
* Description:   Ttyp timer function
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTTypCntFun
(
    const nmIndexType nmCurChannel
)
{
  uint32_t primask;
    /*const nmIndexType nmCurChannel = nmChannelHandle;*/
    NmCanInterruptDisable();
    
    if(nmTimerCntTtyp != 0)
    {
        nmTimerCntTtyp--;
        
        if(0 == nmTimerCntTtyp)
        {
            NmTtypTimeOutProc(nmCurChannel);
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
    NmCanInterruptRestore();
}
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTimerTMaxCntFun
*
* Description:   Tmax timer function
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTMaxCntFun
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    /*const nmIndexType nmCurChannel = nmChannelHandle;*/
    NmCanInterruptDisable();

    if(nmTimerCntTmax != 0)
    {
        nmTimerCntTmax--;
        
        if(0 == nmTimerCntTmax)
        {
            NmTmaxTimeOutProc(nmCurChannel);
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
    NmCanInterruptRestore();
}
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTimerTErrCntFun
*
* Description:   Terr timer function
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTErrCntFun
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    NmCanInterruptDisable();

    if(nmTimerCntTerr != 0)
    {
        nmTimerCntTerr--;
        
        if( 0== nmTimerCntTerr)
        {
            NmTerrTimeOutProc(nmCurChannel);
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
    NmCanInterruptRestore();
}
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTimerTTxCntFun
*
* Description:   TTx timer function 
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTTxCntFun
(
    const nmIndexType nmCurChannel
)
{
    if(nmTimerCntTtx != 0)
    {
        nmTimerCntTtx--;
        
        if(0 == nmTimerCntTtx)
        {
            NmTtxTimeOutProc(nmCurChannel);
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
* Function Name: NmTimerTWaitBusSleepCntFun
*
* Description:   TWaitBusSleep timer function
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTWBSCntFun
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    NmCanInterruptDisable();

    if(nmTimerCntTwbs != 0)
    {
        nmTimerCntTwbs--;
        
        if(0 == nmTimerCntTwbs)
        {
            NmTwbsTimeOutProc(nmCurChannel);
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
    NmCanInterruptRestore();
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTimerTTxCntFun
*
* Description:   bus-off timer function
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
/*OsekNm_006*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTBusOffCntFun
(
    const nmIndexType nmCurChannel
)
{
    uint32_t primask;
    NmCanInterruptDisable();

    if(nmTimerCntTbusoff != 0)
    {
        nmTimerCntTbusoff--;
        
        if(0 == nmTimerCntTbusoff)
        {
            NmTBusOffRecoveryTimeOutProc(nmCurChannel);
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
    NmCanInterruptRestore();
}

#if defined (NM_FOR_FAW)                
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTimerTactiveminiCntFun
*
* Description:   Tactivemini timer function
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTactiveminiCntFun
(
    const nmIndexType nmCurChannel
)     
{
    if(nmTimerCntTactivemini != 0)
    {
        nmTimerCntTactivemini--;
        
        if(0 == nmTimerCntTactivemini)
        {
            NmTactiveminiTimeOutProc(nmCurChannel);
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
* Function Name: NmTimerTlimphomeCntFun
*
* Description:   Tlimphome timer function
*                
* Inputs:        None
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTimerTlimphomeCntFun
(
    const nmIndexType nmCurChannel
)  
{
    if(nmTimerCntTlimphome != 1UL)
    {
        nmTimerCntTlimphome--;
        
        if(1UL == nmTimerCntTlimphome)
        {
            NmTlimphomeTimeOutProc(nmCurChannel);
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
#endif

#define OSEKNM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: nmCanBusOff
*
* Description:   CanNm bus-off 
*                
* Inputs:        nmChannelHandle
*                 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_BusOff
(
    const nmIndexType CanControllerId
)
{
    /*if(CanToNmChnAry[nmChannelHandle] != NM_INVALID_CHANNEL)*/
    nmIndexType nmCurChannel;
    nmStateType nmCurState_Par;
    /*Logic CAN channel is valid ?*/
    if(CanControllerId < NM_CANIF_USED_CONTROLLER_NUM)
    {
        /*Get the current logic NM channel*/
        nmCurChannel = LogCanToNmChn(CanControllerId);
        
        /*Logic NM channel is valid ?*/
        /*if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(nmCurChannel!= NM_INVALID_CHANNEL))*/
        if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(STD_ON == NM_USED_SWITCH))
        {
            /*Get the current state of the current logic NM channel*/
            nmCurState_Par = CanNm_GetCurState(nmCurChannel);
            /*The current NM channel is OK ?(State and init)*/
            if((1 == nmStatusChnInit)&&(nmCurState_Par != NM_STATE_CODE_OFF)&&
               (nmCurState_Par != NM_STATE_CODE_SHUTDOWN))
            {
                /*ApplNmCanWakeUp(nmCurChannel); */
                nmStatusNMBusOffFlag = NM_BIT_SET;
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
}
#define OSEKNM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */


#if defined ( NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmSetConfig
*
* Description:   Network Configuration
*                
* Inputs:        nmSourceID,nmMsgType
* 
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmSetConfig
(
    const nmIndexType nmCurChannel,
    const nmNetIdType   nmSourceID ,
    const ConfigKindName   CfgKind
)
{
    uint8 nmByteOffset;
    uint8 nmBitOffSet;
    uint16 tempSid;
    
    if(nmSourceID < NM_NUMBER_OF_CFG_NODE) 
    {
        tempSid = nmSourceID;

        /* calculate the index for the config table */
        nmByteOffset = (uint8)(tempSid / BYTE_LENGTH);
        nmBitOffSet = (uint8)(tempSid % BYTE_LENGTH);

        #if defined (NM_ENABLE_CONFIG_NORMAL)
        if(CfgKind == CfgNormal)
        {
            if(STD_ON == NM_CHN_CONFIG_NORMAL)
            {              
                NM_NORMAL_CFG_RECORD(nmByteOffset) |= (uint8)(1 << nmBitOffSet);
            }
                
            #if defined (NM_ENABLE_CONFIG_LIMPHOME)
            if(STD_ON == NM_CHN_CONFIG_LIMPHOME)
            {
                NM_LIMPHOME_CFG_RECORD(nmByteOffset) ^= (uint8)(1 << nmBitOffSet);
            }
            #endif
        }
        else
        {

        }
        #endif
        #if defined (NM_ENABLE_CONFIG_LIMPHOME)
        
        if(CfgKind == CfgLimpHome)
        {
            if(STD_ON == NM_CHN_CONFIG_LIMPHOME)
            {
                NM_LIMPHOME_CFG_RECORD(nmByteOffset)  |= (uint8)(1 << (nmBitOffSet));
            }
            #if defined (NM_ENABLE_CONFIG_NORMAL)
            if(STD_ON == NM_CHN_CONFIG_NORMAL)
            {
                NM_NORMAL_CFG_RECORD(nmByteOffset) ^= (uint8)(1 << nmBitOffSet);
            }
            #endif
        }
        else 
        {

        }
        #endif
    } 
    else
    {
    
    }

}

#endif

#define OSEKNM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

#if defined (NM_ENABLE_CONFIG_LIMPHOME)||defined(NM_ENABLE_CONFIG_NORMAL)
#if 0
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmCmpConfig
*                
* Description:   compare configuration
*                
* Inputs:        idx
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_CmpConfig
(
    const nmIndexType nmCurChannel,
    const uint8 idx,
    ConfigKindName    CfgKind
)
{   
    uint8 tgtValue;
    uint8 mask;
    /*uint8 returnValue = 1;*/
    NmReturnType Ret = NM_E_NOT_OK;
    uint8 nmByteOffset;
    uint8 nmBitSet;
#if 0    
    /*NetworkHandleType nmOldChannelHandle;*/
    /* calculate the index for the config table */
    nmByteOffset = idx / BYTE_LENGTH;
    nmBitSet = idx % BYTE_LENGTH;
    NmCanInterruptDisable();

    if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(STD_ON == NM_USED_SWITCH))
    {
        tgtValue = TargetConfigTableAry[nmCurChannel][nmByteOffset] & 
            ((uint8)(1 << nmBitSet));
        mask = ConfigMaskTableAry[nmCurChannel][nmByteOffset] &
            ((uint8)(1 << nmBitSet));
                 
        /* compare data */
        if( ((tgtValue ^ (nmConfigNormal(nmByteOffset)&((uint8)( 1 << nmBitSet)))) & mask) != 0)
        {
            Ret = NM_E_OK;
        }
        else
        {
            /*nothing to do*/
        }

    }
    else
    {

    }
    /*nmCurChannel =  nmOldChannelHandle;*/
    NmCanInterruptRestore();
#endif
    return(Ret);
}
#endif

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmGetConfig
*                
* Description:   Get configuration
*                
* Inputs:        *dataPtr,nmNodeState
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_GetConfig
(
    const nmIndexType nmCurChannel,
    const ConfigKindName   CfgKind,
    CONSTP2VAR(uint8, AUTOMATIC, OSEKNM_APPL_DATA) dataPtr
)
{
    uint8 ByteIdx;
    uint8 configByte; 
    NmReturnType ret = NM_E_OK;
    uint32_t primask;
    /*NetworkHandleType nmOldChannelHandle;*/
    if(dataPtr != NULL_PTR)
    {
        NmCanInterruptDisable();
        /*nmOldChannelHandle = nmCurChannel; */
        /*nmCurChannel =  nmChannelHandle;*/

        configByte = (uint8)(((NM_NUMBER_OF_CFG_NODE) / (uint8)BYTE_LENGTH) + 1) ;

#if defined (NM_ENABLE_CONFIG_NORMAL)
        if((CfgKind == CfgNormal)&&(NM_CHN_CONFIG_NORMAL))
        { 
            for(ByteIdx = 0;ByteIdx < configByte;ByteIdx++)
            {
                /* copy data */
                *(dataPtr + ByteIdx) = NM_NORMAL_CFG_RECORD(ByteIdx);
            }
        }
#endif
#if defined (NM_ENABLE_CONFIG_NORMAL) && !defined( NM_ENABLE_CONFIG_LIMPHOME)
        else
        {
            ret = NM_E_NOT_OK;
        }
#endif

#if defined (NM_ENABLE_CONFIG_LIMPHOME)&& defined (NM_ENABLE_CONFIG_NORMAL)
        else 
#endif

#if defined (NM_ENABLE_CONFIG_LIMPHOME)
        if((CfgKind == CfgLimpHome)&&(NM_CHN_CONFIG_LIMPHOME))
        { 
            for(ByteIdx = 0;ByteIdx < configByte;ByteIdx++)
            {
                /* copy data */
                *(dataPtr + ByteIdx) = NM_LIMPHOME_CFG_RECORD(ByteIdx);
            }
        }
        else
        {
            ret = NM_E_NOT_OK;
        }
#endif
        /*nmCurChannel =  nmOldChannelHandle;*/
        NmCanInterruptRestore();
    }
    else
    {
        ret = NM_E_NOT_OK;
    }
    return ret;
}
#endif

#define OSEKNM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmResetConfig
*                
* Description:   reset configuration
*                
* Inputs:        None
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
#if defined (NM_ENABLE_CONFIG_LIMPHOME)|| defined(NM_ENABLE_CONFIG_NORMAL)
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmResetConfig
(
    const nmIndexType nmCurChannel
)
{
    uint8 ByteIdx;
    uint8 configByte;
    configByte = (uint8)(((NM_NUMBER_OF_CFG_NODE) / (uint8)BYTE_LENGTH) + 1) ;
    
    for(ByteIdx = 0; ByteIdx < configByte; ByteIdx++)
    {
        #if defined ( NM_ENABLE_CONFIG_LIMPHOME)
        if(STD_ON == NM_CHN_CONFIG_LIMPHOME)
        {
            NM_LIMPHOME_CFG_RECORD(ByteIdx) = NODE_OFFLINE;
        }
        #endif
        #if defined ( NM_ENABLE_CONFIG_NORMAL)
        if(STD_ON == NM_CHN_CONFIG_NORMAL)
        {
            NM_NORMAL_CFG_RECORD(ByteIdx) = NODE_OFFLINE;
        }
        #endif
    }
  
}
#endif

#define OSEKNM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

#if defined NM_ENABLE_DATA_ACCESS

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNm_GetUserData
*                
* Description:   
*                
* Inputs:        *nmRingData
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_GetUserData
(
    const nmIndexType nmCurChannel,
    CONSTP2VAR(uint8, AUTOMATIC, OSEKNM_APPL_DATA) nmUserData
)
{
    uint8 loop;
    uint32_t primask;
    NmReturnType Ret = NM_E_NOT_OK; 

    if((nmUserData != NULL_PTR)&&(nmCurChannel < NM_NUMBER_OF_CHANNELS)&&\
        (STD_ON == NM_USED_SWITCH))
    {
        
        if(0 == nmStatusNMCfgStable)
        {
            /*Ret = NM_E_NOT_OK;*/
        }
        else
        {
            NmCanInterruptDisable();
            for(loop = 0; loop < (NM_PDU_DLC - NM_PDU_CONTROL_DLC); loop++)
            {
                *(nmUserData + loop) = nmRxDataFld.R[loop + NM_PDU_CONTROL_DLC];
            }
            NmCanInterruptRestore();
        
            Ret = NM_E_OK;
        }

    }
    else
    {
        /*Ret = NM_E_NOT_OK;*/
    }

    /*nmCurChannel =  nmOldChannelHandle;*/
    return Ret;
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNm_ReadNmData
*                
* Description:   
*                
* Inputs:        *nmRingData
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_ReadNmData
(
    const nmIndexType nmCurChannel,
    CONSTP2VAR(uint8, AUTOMATIC, OSEKNM_APPL_DATA) nmData
)
{
    uint8 loop;
    uint32_t primask;
    NmReturnType Ret = NM_E_NOT_OK; 

    if((nmData != NULL_PTR)&&(nmCurChannel < NM_NUMBER_OF_CHANNELS)&&\
        (STD_ON == NM_USED_SWITCH))
    {
        
        if(0 == nmStatusNMCfgStable)
        {
            /*Ret = NM_E_NOT_OK;*/
        }
        else
        {
            NmCanInterruptDisable();
            for(loop = 0; loop < (NM_PDU_DLC - NM_PDU_CONTROL_DLC); loop++)
            {
                *(nmData + loop) = nmRxDataFld.R[loop];
            }
            NmCanInterruptRestore();
        
            Ret = NM_E_OK;
        }

    }
    else
    {
        /*Ret = NM_E_NOT_OK;*/
    }

    /*nmCurChannel =  nmOldChannelHandle;*/
    return Ret;
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTransmitRingData
*                
* Description:   
*                
* Inputs:        *nmRingData
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(NmReturnType, OSEKNM_PUBLIC_CODE) CanNm_SetUserData
(
    const nmIndexType nmCurChannel,
    CONSTP2CONST(uint8, AUTOMATIC, OSEKNM_PUBLIC_CODE) nmUserData
)
{
    uint8 loop;
    uint32_t primask;
    NmReturnType Ret = NM_E_NOT_OK;
    
    NmCanInterruptDisable();
    if((nmUserData != NULL_PTR)&&(nmCurChannel < NM_NUMBER_OF_CHANNELS)&&\
        (STD_ON == NM_USED_SWITCH))
    {
        for(loop = 0; loop < (NM_PDU_DLC - NM_PDU_CONTROL_DLC); loop++)
        {
            nmTxDataFld.R[loop + NM_PDU_CONTROL_DLC] = *(nmUserData + loop);
        }
        Ret = NM_E_OK;
    }
    else
    {

    }
    NmCanInterruptRestore();

    return Ret;
}
#endif
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: Can_ApplRxIndication
*
* Description:   This function is implemented by user ,processing the received
*                CAN message.The received CAN message is stored in 
*                CanRxMsgBuffer[][].
*                
* Inputs:        Hth
*                   - bit0-5: MB Number
*                   - bit6-7: FlexCAN Controller Number,
*                             FlexCAN ControllerA:0;B:1;C:2
*                   
* Outputs:       None
* 
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/

FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_RxIndication
(
    const PduIdType canNmPduId,  
    const Can_IdType canNmRxId,
    CONSTP2CONST(PduInfoType,AUTOMATIC,OSEKNM_APPL_DATA) pCanNmRxPduPtr
)
{
    uint32 auhNmOwnId;
    uint8 auhNmSourceId;  
    nmStateType nmCurState_Par;
    /*nmStateType nmNxtState_Par;*/
    const nmIndexType nmCurChannel = canNmPduId;

    if((1 == nmStatusChnInit)&&(nmCurChannel < NM_NUMBER_OF_CHANNELS))
    {
        /*nmCurChannel=CanNm_GetNmChn(canNmPduId);*/

        /*const NetworkHandleType nmOldChannelHandle = nmCurChannel; */
        /*const nmIndexType nmCurChannel = nmChannelHandle;*/
        /*NetworkHandleType*/
        /*nmCurChannel =  nmChannelHandle; */    
        nmCurState_Par = CanNm_GetCurState(nmCurChannel);

        if((nmCurState_Par != NM_STATE_CODE_OFF)&&(nmCurState_Par != NM_STATE_CODE_SHUTDOWN))
        {    
           
            if(1 == NM_CAN_ID_STANDARD_ENABLE)
            {
                auhNmOwnId = canNmRxId;
            }
            else
            {
                auhNmOwnId = NmExtendIdToStand(canNmRxId);                                   
            }
            
        
            if(auhNmOwnId == NM_CAN_TXID)
            {
                /* do nothing */
            }
            else if(GetNmRxMsgBasic(auhNmOwnId) != NM_PDU_BASICID)
            {

            }
            else
            {
                auhNmSourceId = GetNmRxMsgSrcID(auhNmOwnId);
        
                if(0 == nmStatusNMBusError)
                {
                    nmRxSourceAddr = auhNmSourceId;
                    /* Copy the Can Buffer array Data to NM Data Feild */
                    NmRxPDUCopy(nmCurChannel,pCanNmRxPduPtr);
                #if defined (NM_FOR_FAW)               
                    (void) NmRxMsgDecode(nmCurChannel);        
                #endif
                    /*the fun of CanNm_WakeupSignal is moved to NmRxIndication*/
                    /* Carry out any message Received E3 */
                    NmRxIndication(nmCurChannel);
                }
                else
                {
                /*nothing to do*/
                }
            } 
        
            /*nmCurChannel =  nmOldChannelHandle;*/
        
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
* Function Name: CanNm_TxConfirmation
*
* Description:   processing the case of transmiting message successfully.
*                
* Inputs:        canNmTxPduId
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_TxConfirmation
(
    const PduIdType canNmTxPduId
)
{
    /*const NetworkHandleType nmOldChannelHandle = nmCurChannel;*/ 
    const nmIndexType nmCurChannel = canNmTxPduId;
    nmStateType nmCurState_Par;

    if((1 == nmStatusChnInit)&&(nmCurChannel < (NM_NUMBER_OF_CHANNELS)))
    {
        nmCurState_Par = CanNm_GetCurState(nmCurChannel);


        
        if(1 == nmStatusErrRestoring)
        {
            /*OsekNm_008*/
            nubNmBusOffCnt = 0; 
            nmStatusSlowRecovery = NM_BIT_CLEAR;
            nmStatusErrRestoring = NM_BIT_CLEAR;
            if(1 == NM_EXTENDED_CALLBACK_ENABLE)
            {
                ApplNmBusOffRecoverySuccess(nmCurChannel);
            }
        }
        /* Carry out any message Transmitted E3 */
        if((nmCurState_Par != NM_STATE_CODE_OFF)&&
           (nmCurState_Par != NM_STATE_CODE_SHUTDOWN))
        { 
            NmTxConfirmation(nmCurChannel);   
        }
        else
        {

        }

    }
    else
    {
        /*do nothing*/
    }
    /*nmCurChannel =  nmOldChannelHandle;*/
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: CanNm_WakeUp
*
* Description:   This function is called by can driver when bus wakeup 
*                
* Inputs:        nmChannelHandle
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
FUNC(void, OSEKNM_PUBLIC_CODE) CanNm_WakeUp
(
    const nmIndexType CanControllerId
)
{
    nmIndexType nmCurChannel;
    nmStateType nmCurState_Par;
    /*Logic CAN channel is valid ?*/
    if(CanControllerId < NM_CANIF_USED_CONTROLLER_NUM)
    {
        /*Get the current logic NM channel*/
        nmCurChannel = LogCanToNmChn(CanControllerId);
        
        /*Logic NM channel is valid ?*/
        
        /*if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(nmCurChannel!= NM_INVALID_CHANNEL))*/
        if((nmCurChannel < NM_NUMBER_OF_CHANNELS)&&(STD_ON == NM_USED_SWITCH))
        {
            /*Get the current state of the current logic NM channel*/
            nmCurState_Par = CanNm_GetCurState(nmCurChannel);
            /*The current NM channel is OK ?(State and init)*/
            if((1 == nmStatusChnInit)&&(nmCurState_Par != NM_STATE_CODE_OFF)&&
                (nmCurState_Par != NM_STATE_CODE_SHUTDOWN)&&
                (1 == NM_REMOTE_WAKEUP_ENABLE ))
            {
                
                ApplNmCanWakeUp(nmCurChannel); 
                
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
}

#define OSEKNM_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"/* PRQA S 5087 */

#define OSEKNM_START_SEC_PRIVATE_CODE
#include "MemMap.h"/* PRQA S 5087 */

#if defined (NM_FOR_FAW)               
/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmRxMsgDecode
*
* Description:   This function is used for decode NMPDU OpCode field.
*                
* Inputs:        none
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmRxMsgDecode
(
    const nmIndexType nmCurChannel
)                
{
    switch (nmRxDataFldByteOpCode)
    {
        case MSG_ALIVE_IND:        
            nmRxDataFldOpCodeMsgType=ALIVE;
            nmRxDataFldOpCodeSleepInd=TRUE;
            nmRxDataFldOpCodeSleepAck=FALSE;
        break;
        case MSG_ALIVE:               
            nmRxDataFldOpCodeMsgType=ALIVE;
            nmRxDataFldOpCodeSleepInd=FALSE;
            nmRxDataFldOpCodeSleepAck=FALSE;
        break;
        case MSG_RING_IND_ACK:    
            nmRxDataFldOpCodeMsgType=RING;
            nmRxDataFldOpCodeSleepInd=TRUE;
            nmRxDataFldOpCodeSleepAck=TRUE;
        break;
        case MSG_RING_IND_ACK1:    
            nmRxDataFldOpCodeMsgType=RING;
            nmRxDataFldOpCodeSleepInd=TRUE;
            nmRxDataFldOpCodeSleepAck=TRUE;
        break;
        case MSG_RING_IND:         
            nmRxDataFldOpCodeMsgType=RING;
            nmRxDataFldOpCodeSleepInd=TRUE;
            nmRxDataFldOpCodeSleepAck=FALSE;
        break;
        case MSG_RING:                
            nmRxDataFldOpCodeMsgType=RING;
            nmRxDataFldOpCodeSleepInd=FALSE;
            nmRxDataFldOpCodeSleepAck=FALSE;
        break;
        case MSG_LIMPHOME_IND:  
            nmRxDataFldOpCodeMsgType=LIMPHOME;
            nmRxDataFldOpCodeSleepInd=TRUE;
            nmRxDataFldOpCodeSleepAck=FALSE;
        break;
        case MSG_LIMPHOME:   
            nmRxDataFldOpCodeMsgType=LIMPHOME;
            nmRxDataFldOpCodeSleepInd=FALSE;
            nmRxDataFldOpCodeSleepAck=FALSE;
        break;
        default:
        break;  
    }
}

/* BEGIN_FUNCTION_HDR
******************************************************************************** 
* Function Name: NmTxMsgCoding
*
* Description:   This function is used for coding NMPDU OpCode field.
*                
* Inputs:        none
*                   
* Outputs:       None
* 
* Limitations:   None
******************************************************************************** 
END_FUNCTION_HDR*/
STATIC FUNC(void, OSEKNM_PRIVATE_CODE) NmTxMsgCoding
(
    const nmIndexType nmCurChannel
)                
{
    if(ALIVE == nmTxDataFldOpCodeMsgType)
    {
        if(TRUE == nmTxDataFldOpCodeSleepInd)
            nmTxDataFldByteOpCode= MSG_ALIVE_IND;
        else
            nmTxDataFldByteOpCode= MSG_ALIVE;
    } 
    else if(RING == nmTxDataFldOpCodeMsgType)
    {
        if((FALSE == nmTxDataFldOpCodeSleepInd) && 
            (FALSE == nmTxDataFldOpCodeSleepAck))
            nmTxDataFldByteOpCode = MSG_RING;
        else if((TRUE == nmTxDataFldOpCodeSleepInd) && 
            (FALSE == nmTxDataFldOpCodeSleepAck))
            nmTxDataFldByteOpCode = MSG_RING_IND;
        else
            nmTxDataFldByteOpCode= MSG_RING_IND_ACK1;
    } 
    else if(LIMPHOME == nmTxDataFldOpCodeMsgType)
    {
        if(TRUE == nmTxDataFldOpCodeSleepInd)
            nmTxDataFldByteOpCode= MSG_LIMPHOME_IND;
        else
            nmTxDataFldByteOpCode= MSG_LIMPHOME;
    }
}
#endif

#define OSEKNM_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

