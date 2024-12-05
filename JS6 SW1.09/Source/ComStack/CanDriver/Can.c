/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Can.c
********************************************************************************
*   Project/Product : AUTOSAR 4.0
*   Title           : CAN Driver module Source File
*   Author          : HeYang
********************************************************************************
*   Description     : Implementation of CAN Driver                        
*
********************************************************************************
*   Limitations     : only used for MSCAN
*
********************************************************************************
*
********************************************************************************
*   Revision History:
* 
*   Version     Date          Initials      CR#          Descriptions
*   ---------   ----------    ------------  ----------   ---------------
*   01.00.00    07/03/2010    HeYang        N/A          Original
*   01.01.00    09/06/2010    HeYang        N/A          support multi channel
*   01.02.00    17/07/2010    HeYang        N/A          modify GlobalInterupts
*   02.00.00    29/07/2013    NingChen      N/A          DPN_MSCAN_130729_1
*   02.00.01    15/08/2013    NingChen      N/A          D10_MSCAN_130815_1   
*   02.00.02    23/09/2013    NingChen      N/A          D10_MSCAN_130923_1  
*   02.00.03    23/01/2014    SuyunYing     N/A          D10_MSCAN_140123_1                                                  
********************************************************************************
* END_FILE_HDR*/

/*******************************************************************************
*   Includes 
*******************************************************************************/ 
#include "Can.h"
#include "CanIf_Cbk.h"
#include "mf_config.h"
#include "fm33lg0xx_fl.h"
#include "Event.h"
#include "LowPowerManage.h"
#include "App_Can.h"
/*CAN390: The Can module shall include the header file EcuM_Cbk.h, in which the
callback functions called by the Can module at the Ecu State Manager module are
declared.*/

/*CAN397: The Can module shall include the header file Os.h file. By this 
inclusion,the API to read a free running timer value (GetCounterValue) provided 
by the system service shall be included.*/

#if (STD_ON==CAN_DEV_ERROR_DETECT)
    #include "Det.h"
#endif


/*******************************************************************************
*   Macro 
*******************************************************************************/
#define REQUEST_FOR_IDENTICAL_ID_ABORT  (0x02u)
#if (1 == CAN_USED_CONTROLLER_NUM)
    #define CAN_CONTROLLER_ID           (0u)
#else
    #define CAN_CONTROLLER_ID           (Controller)
#endif

/*******************************************************************************
*   Local Data Declaration 
*******************************************************************************/
#define CAN_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"
/* For CAN_DEV_ERROR_DETECT */
#if (CAN_DEV_ERROR_DETECT == STD_ON )
STATIC VAR(Can_StateType, CAN_PRIVATE_DATA)  Can_State[CAN_USED_CONTROLLER_NUM];
STATIC VAR(Can_InitType, CAN_PRIVATE_DATA) Can_DriverInitStatus = 
                                                              CAN_UNINITIALIZED;
#endif

/*CAN418: Constants, global data types and functions that are only used by the 
Can module internally, are declared in Can.c */
STATIC volatile VAR(uint8, CAN_PRIVATE_DATA) CCRCopyValue ;
///STATIC VAR(uint8, CAN_PRIVATE_DATA) CanGlobalInterruptCount;
STATIC VAR(uint8, CAN_PRIVATE_DATA)       
                           CanControllerInterruptCount[CAN_USED_CONTROLLER_NUM];
STATIC VAR(Can_OldIERType, CAN_PRIVATE_DATA)   
                           CanControllerOldInterrupReg[CAN_USED_CONTROLLER_NUM];
STATIC VAR(uint8, CAN_PRIVATE_DATA) 
               EmptyFlagForHth[CAN_USED_CONTROLLER_NUM][HTH_NUM_PER_CONTROLLER];
STATIC VAR(PduIdType, CAN_PRIVATE_DATA)  
                   PduIdForHth[CAN_USED_CONTROLLER_NUM][HTH_NUM_PER_CONTROLLER];
STATIC VAR(Can_StateTransitionType, CAN_PRIVATE_DATA)   
                                       Can_ModeMonitor[CAN_USED_CONTROLLER_NUM];
#if (STD_ON == CAN_HW_TRANSMIT_CANCELLATION)
STATIC VAR(PduIdType, CAN_PRIVATE_DATA) 
                   CanIDForHth[CAN_USED_CONTROLLER_NUM][HTH_NUM_PER_CONTROLLER];    
#endif
/* CAN Controller Base Address */
STATIC VAR(uint32, CAN_PRIVATE_DATA) CanBasisAddress[TOTAL_CAN_MODULE_NUM] =
{
    0x40019400
};
#define CAN_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/*******************************************************************************
*   Local Functions declare
*******************************************************************************/
#define CAN_START_SEC_PRIVATE_CODE
#include "MemMap.h"

STATIC FUNC(void, CAN_PRIVATE_CODE) Can_MemSet
(
    P2VAR(void, AUTOMATIC, CAN_NOINIT_DATA) pDest,
    uint8 data, 
    Can_LenTyp length
);
STATIC FUNC(void, CAN_PRIVATE_CODE) Can_MemCpy
(
    P2VAR(void, AUTOMATIC, CAN_NOINIT_DATA) dest,
    P2CONST(void, AUTOMATIC, CAN_CONST) source, 
    Can_LenTyp count
); 
STATIC FUNC(void, CAN_PRIVATE_CODE) CAN_Controller_Tx_handler
(
    uint8 Controller
);
STATIC FUNC(void, CAN_PRIVATE_CODE) CAN_Controller_Rx_handler
(
    uint8 Controller
);
STATIC FUNC(void, CAN_PRIVATE_CODE) CAN_Controller_Busoff_handler
(
    uint8 Controller
);
STATIC FUNC(void, CAN_PRIVATE_CODE) CAN_Controller_Wakeup_handler
(
    uint8 Controller
);
STATIC FUNC(void, CAN_PRIVATE_CODE) Can_BusoffRecovery
(
    uint8 phyController
);
STATIC FUNC(void, CAN_PRIVATE_CODE) Can_Controller_Transmit_Abort
(
    uint8 phyController
);
STATIC FUNC(void, CAN_PRIVATE_CODE) Can_Transmit_Abort
(
    uint8 phyController,
    Can_HwHandleType phyHth
);

#define CAN_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"    

/*******************************************************************************
*   Local Functions define
*******************************************************************************/
#define CAN_START_SEC_PRIVATE_CODE
#include "MemMap.h"  

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_MemSet
*                
* Description:   used for setting data value.
*                             
* Inputs:        pDest,data,length
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE) Can_MemSet
(
    P2VAR(void, AUTOMATIC, CAN_PRIVATE_DATA) pDest,
    uint8 data, 
    Can_LenTyp length
)
{
    while(length > (uint16)0x0000)
    {  
        length--;
        ((uint8*)pDest)[length] = data;
    } 
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_MemCpy
*                
* Description:   used for copy data value. 
*                             
* Inputs:        pDest,source,count
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE) Can_MemCpy
(
    P2VAR(void, AUTOMATIC, CAN_PRIVATE_DATA) dest,
    P2CONST(void, AUTOMATIC, AUTOMATIC) source, 
    Can_LenTyp count
)
{ 
    P2VAR(uint8, AUTOMATIC,AUTOMATIC)  sd = (uint8 *)dest;
    P2CONST(uint8, AUTOMATIC, CAN_CONST) ss = source;
    uint16 i = 0;

    while(count > 0)
    {
        sd[i] = ss[i];
        i++;
        count--;
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_BusoffRecovery
*                
* Description:   This function is used for bus off recovery 
*                             
* Inputs:        phyController
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE)  Can_BusoffRecovery
(
    uint8 phyController
)
{
  (void)phyController;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CAN_Controller_Tx_handler
*                
* Description:   This function is used for handle the success TX confirmation 
*                             
* Inputs:        Controller
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE) CAN_Controller_Tx_handler
(
    uint8 Controller
)
{  
    uint8 phyhth = 0;
    uint8 phyController = CanControllerIDtoPhys[CAN_CONTROLLER_ID];
 //   uint8 tempSdu[8];
 //   Can_PduType  tempCanAbortPdu;
    
    for(phyhth = 0; phyhth < HTH_NUM_PER_CONTROLLER; phyhth++)
    {
        if(TRUE != EmptyFlagForHth[CAN_CONTROLLER_ID][phyhth]) 
        {
            if(0 != (CAN_ISR(phyController) & CAN_TXMASK(phyhth)))
            {
            /*use (CanRegInit[CAN_CONTROLLER_ID].CanInitTIER) to judge whether 
            the TX interrupt is enabled if enabled ,disable the relative 
            TX interrupt */
                CAN_IER(phyController) &=(((CanRegInit[CAN_CONTROLLER_ID].CanInitIER)) & (~CAN_IER_TXOKIE_Msk));
        
                #if (STD_ON ==CAN_HW_TRANSMIT_CANCELLATION)
                /*the message was aborted */
                if(0 != (CAN_TAAK(phyController) & CAN_TXMASK(phyhth)))
                {
                    Can_MemCpy(tempSdu,
                                 (const uint8*)CAN_TxBuf(phyController).DataSeg, 8);
                
                    tempCanAbortPdu.length = 
                                     (CAN_TxBuf(phyController).Dlc) & ((uint8)0x0f);
                    tempCanAbortPdu.sdu = tempSdu;
                    tempCanAbortPdu.swPduHandle = 
                                         PduIdForHth[CAN_CONTROLLER_ID][phyhth];
                    tempCanAbortPdu.id = CanIDForHth[CAN_CONTROLLER_ID][phyhth];
                    #if (STD_ON == CAN_IDENTICAL_ID_CANCELLATION)   
                    if(REQUEST_FOR_IDENTICAL_ID_ABORT != 
                                     EmptyFlagForHth[CAN_CONTROLLER_ID][phyhth])
                    {
                        CanIf_CancelTxConfirmation(&tempCanAbortPdu);
                    }
                    #else
                    CanIf_CancelTxConfirmation(&tempCanAbortPdu);
                    #endif

                    EmptyFlagForHth[CAN_CONTROLLER_ID][phyhth] = TRUE;
                }
            
                else
                #endif
                {
                    EmptyFlagForHth[CAN_CONTROLLER_ID][phyhth] = TRUE;
                    /*the message was sented */
                  
                    CanIf_TxConfirmation(PduIdForHth[CAN_CONTROLLER_ID][phyhth]);
                }
 
            }
        }
        else
        {
        /*use (CanRegInit[CAN_CONTROLLER_ID].CanInitTIER) to judge whether 
        the TX interrupt is enabled if enabled ,disable the relative 
        TX interrupt */
            CAN_IER(phyController) &= (((CanRegInit[CAN_CONTROLLER_ID].CanInitIER)) & (~CAN_IER_TXOKIE_Msk));
        
        }
  
    }
} 

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CAN_Controller_Rx_handler
*                
* Description:   This function is used for handle the success RX indication 
*                             
* Inputs:        Controller
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE) CAN_Controller_Rx_handler
(
    uint8 Controller
)
{
 
    Can_IdType canID = 0;
    uint32  dlc = 0;
    uint32_t data1 = 0;
    uint32_t data2 = 0;
    uint8 temprxbuffer[8];
    Can_HwHandleType hrh = CanControllerIDtoHRH[CAN_CONTROLLER_ID];
    uint8 phyController = CanControllerIDtoPhys[CAN_CONTROLLER_ID];
    
    if(CAN_CONTROLLER_NOT_HOH != hrh)
    /* if there is no hrh is related to this controller, do nothing*/
    {
        while(0!= ((CAN_ISR(phyController)) & CAN_IER_RXNEMPIE_Msk))
        {
            canID = CAN_RXF_IDR(phyController);
            dlc = CAN_RXF_DLCR(phyController);
            data1 = CAN_RXF_DW1R(phyController);
            data2 = CAN_RXF_DW2R(phyController);

            if(0 == GET_IDE(canID))
            /*standard ID*/
            {
                canID = GET_STDID(canID);
            }
            else
            /*externed ID*/
            {        
                canID = GET_EXTID(canID);
            }
            
            dlc = GET_DLC(dlc);
            temprxbuffer[0] = (uint8_t)data1 & 0xff;
            temprxbuffer[1] = (uint8_t)(data1 >> 8) & 0xff;
            temprxbuffer[2] = (uint8_t)(data1 >> 16) & 0xff;
            temprxbuffer[3] = (uint8_t)(data1 >> 24) & 0xff;
            temprxbuffer[4] = (uint8_t)data2 & 0xff;
            temprxbuffer[5] = (uint8_t)(data2 >> 8) & 0xff;
            temprxbuffer[6] = (uint8_t)(data2 >> 16) & 0xff;
            temprxbuffer[7] = (uint8_t)(data2 >> 24) & 0xff;
            
			#if 0
            /*clear RX interrupt flag*/
            CAN_RFLG(phyController) = CAN_RXF;
			
			{asm cli;}  /* ??? */
			
			CanIf_RxIndication(hrh, canID, dlc,(const uint8*)(temprxbuffer));  /* ??? */
			#else

            CanIf_RxIndication(hrh, (Can_IdType)(canID), (uint8)(dlc),(const uint8*)(temprxbuffer));

            /*clear RX interrupt flag*/
            CAN_ICR(phyController) = CAN_ICR_CRXNEMP_Msk;
			
			#endif
        }
        
    } 

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CAN_Controller_Wakeup_handler
*                
* Description:   This function is used to handle the success wakeup confirmation
*                             
* Inputs:        Controller
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE) CAN_Controller_Wakeup_handler
(
    uint8 Controller
)
{
#if 0
    uint8 phyController = CanControllerIDtoPhys[CAN_CONTROLLER_ID];
   
    if(0 != ((CAN_RFLG(phyController)) & CAN_WUPIF))
    {
        CAN_RFLG(phyController) = CAN_WUPIF;
        (void)Can_SetControllerMode(CAN_CONTROLLER_ID, CAN_T_WAKEUP);
        CanIf_ControllerWakeUp(CAN_CONTROLLER_ID) ;
       
    }
#endif
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CAN_Controller_Busoff_handler
*                
* Description:   This function is used for handle the bus off event 
*                             
* Inputs:        Controller
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE) CAN_Controller_Busoff_handler
(
    uint8 Controller
)

{
#if 0
    uint8 phyController = CanControllerIDtoPhys[CAN_CONTROLLER_ID];
    
    if(((((CAN_RFLG(phyController)) & CAN_BOFFTIF) == CAN_BOFFTIF )
        ||(((CAN_RFLG(phyController)) & CAN_BOFFRIF) == CAN_BOFFRIF ))
      &&(((CAN_RFLG(phyController)) & CAN_CSCIF) == CAN_CSCIF))
    /*this controller has entered the bus off state*/
    {
        (void)Can_SetControllerMode(CAN_CONTROLLER_ID, CAN_T_STOP);                  
        CanIf_ControllerBusOff(CAN_CONTROLLER_ID);
 
    }
    else if(((((CAN_RFLG(phyController)) & CAN_BOFFTIF) == CAN_TSTAT1 )
        ||(((CAN_RFLG(phyController)) & CAN_BOFFRIF) == CAN_RSTAT1 ))
      &&(((CAN_RFLG(phyController)) & CAN_CSCIF) == CAN_CSCIF))
    /*this controller has entered the error passive state*/
    {                
        CanNm_ControllerPassive(CAN_CONTROLLER_ID);
 
    }
    else if(((CAN_RFLG(phyController)) & CAN_CSCIF) == CAN_CSCIF)
    /*this controller has entered the error active state*/
    {
        CanNm_ControllerActive(CAN_CONTROLLER_ID); 
    }
    else
    {}
    /*because CSCIF and OVRIF are both in the same register-CANRFLG,
    so if OVRIF is set ,we also need to clear it*/
    if(0 != ((CAN_RFLG(phyController)) & CAN_CSCIF))
    {
        CAN_RFLG(phyController) = CAN_CSCIF ;
    }
    if(0 != ((CAN_RFLG(phyController)) & CAN_OVRIF))
    {
        CAN_RFLG(phyController) = CAN_OVRIF ;
    }
#endif
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_Controller_Transmit_Abort
*                
* Description:   This function is used for abort all the message in the 
*                transmission buffer which is belong to one controller
*                             
* Inputs:        Controller
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE) Can_Controller_Transmit_Abort
(
    uint8 phyController
) 
{
  
    (void)phyController;
   
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_Transmit_Abort
*                
* Description:   This function is used for abort 1 message in the 
                 transmission buffer which is belong to one hth
*                             
* Inputs:        phyController,phyHth
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
STATIC FUNC(void, CAN_PRIVATE_CODE) Can_Transmit_Abort
(
    uint8 phyController,
    Can_HwHandleType phyHth
)
{
  (void)phyController;
  (void)phyHth;
}

#define CAN_STOP_SEC_PRIVATE_CODE
#include "MemMap.h" 

/*******************************************************************************
*  PUBLIC Functions define 
*******************************************************************************/
#define CAN_START_SEC_PUBLIC_CODE
#include "MemMap.h" 

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_GetVersionInfo
*                
* Description:   This function returns the version information of this module. 
*                             
* Inputs:        versioninfo:Pointer to where to store the version information 
*                            of this module
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if (STD_ON == CAN_VERSION_INFO_API)
FUNC(void, CAN_PUBLIC_CODE) Can_GetVersionInfo
( 
    P2VAR(Std_VersionInfoType, AUTOMATIC, CAN_APPL_DATA) versioninfo
)
{

    #if (STD_ON == CAN_DEV_ERROR_DETECT)
    if(NULL_PTR == versioninfo)
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_GETVERSIONINFO_APIID,
                        CAN_E_PARAM_POINTER);                          
    }
    else
    #endif
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = CAN_VENDOR_ID_VALUE;
        /* Copy the module Id */
        versioninfo->moduleID = CAN_MODULE_ID_VALUE;
         /* Copy the Instance Id */
        versioninfo->instanceID = CAN_INSTANCE_ID_VALUE_0;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = CAN_SW_MAJOR_VERSION_VALUE;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = CAN_SW_MINOR_VERSION_VALUE;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = CAN_SW_PATCH_VERSION_VALUE;
    }

}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_Init
*                
* Description:   This function initializes the module. 
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/

FUNC(void, CAN_PUBLIC_CODE) Can_Init(void)
{
  
    uint8 ControllerIndex = 0;
    uint8 phyController = 0;

    #if (STD_ON == CAN_DEV_ERROR_DETECT)
    if(CAN_UNINITIALIZED != Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_INIT_APIID,
                        CAN_E_TRANSITION);                
    }
    else     
    #endif  
    {

        Can_MemSet(EmptyFlagForHth, TRUE, 
                            (CAN_USED_CONTROLLER_NUM * HTH_NUM_PER_CONTROLLER));
        Can_MemSet(PduIdForHth, 0xff, 
        (CAN_USED_CONTROLLER_NUM * HTH_NUM_PER_CONTROLLER * sizeof(PduIdType)));  
        Can_MemSet(CanControllerInterruptCount, 0x00, CAN_USED_CONTROLLER_NUM);
        Can_MemSet(CanControllerOldInterrupReg, 0x00, 
                              CAN_USED_CONTROLLER_NUM * sizeof(Can_OldIERType));  

        #if (STD_ON == CAN_HW_TRANSMIT_CANCELLATION)
        Can_MemSet(CanIDForHth, 0x00, 
         CAN_USED_CONTROLLER_NUM * HTH_NUM_PER_CONTROLLER * sizeof(Can_IdType));    
        #endif
        /*时钟总线配置*/
        FL_CMU_EnableGroup3BusClock(FL_CMU_GROUP3_BUSCLK_CAN);
        FL_CMU_EnableGroup3OperationClock(FL_CMU_GROUP3_OPCLK_CAN);
        /*CAN时钟源选择*/
        FL_CMU_SetCANClockSource(FL_CMU_CAN_CLK_SOURCE_XTHF);
        for(ControllerIndex = 0; ControllerIndex < CAN_USED_CONTROLLER_NUM; 
                                                              ControllerIndex++)
        { 
            Can_ModeMonitor[ControllerIndex] = CAN_T_NONE;
            phyController = CanControllerIDtoPhys[ControllerIndex];
            /*request enter into initialization mode*/

            CAN_CR(phyController) = 1ul;
            CAN_LOOP_BEGIN(INIT_ENTER_TIMEOUT);
            while(( 0 == (CAN_SR(phyController) & CAN_INITAK))
                 && (CAN_OK != CAN_LOOP_TIMEOUT(INIT_ENTER_TIMEOUT)) )
            {
            ; /* wait while not in initialization mode */
            }
            
            CAN_LOOP_END(INIT_ENTER_TIMEOUT);
        
            if( 0 != (CAN_SR(phyController) & CAN_INITAK))
            /*already in initialization mode*/
            {
                /*The following registers enter their hard reset state and 
                restore their default values: CANCTL0 (Not including WUPE, 
                INITRQ, and SLPRQ.), CANRFLG,CANRIER, CANTFLG, CANTIER, CANTARQ,
                CANTAAK, and CANTBSEL.*/
                /*The registers CANCTL1, CANBTR0, CANBTR1, CANIDAC, CANIDAR0-7, 
                and CANIDMR0-7 can only be written by the CPU when the MSCAN is
                in initialization mode (INITRQ = 1 and INITAK = 1). The values 
                of the error counters are not affected by initialization mode.*/
                
                /*enable can module*/

                /*set Baudrate*/
                CAN_BRPR(phyController) = CanRegInit[ControllerIndex].CanInitBRPR;
                CAN_BTR(phyController) = CanRegInit[ControllerIndex].CanInitBTR;
                
                /* set filter mode */
                CAN_AFR(phyController) = CanRegInit[ControllerIndex].CanInitAFR;
                
                /* set acceptance filter registers */
                CAN_AFIR1(phyController) = CanRegInit[ControllerIndex].CanInitAFIR1;
                CAN_AFIR2(phyController) = CanRegInit[ControllerIndex].CanInitAFIR2;
                CAN_AFIR3(phyController) = CanRegInit[ControllerIndex].CanInitAFIR3;
                CAN_AFIR4(phyController) = CanRegInit[ControllerIndex].CanInitAFIR4;
                
                CAN_AFMR1(phyController) = CanRegInit[ControllerIndex].CanInitAFMR1;
                CAN_AFMR2(phyController) = CanRegInit[ControllerIndex].CanInitAFMR2;
                CAN_AFMR3(phyController) = CanRegInit[ControllerIndex].CanInitAFMR3;
                CAN_AFMR4(phyController) = CanRegInit[ControllerIndex].CanInitAFMR4;
                

                 
            }
            else 
            /*fail to enter into initialization mode*/
            {
            
            }
            #if (CAN_DEV_ERROR_DETECT == STD_ON )
            Can_State[ControllerIndex] = CAN_STOPPED;   
            #endif        
        }
        #if (CAN_DEV_ERROR_DETECT == STD_ON ) 
           Can_DriverInitStatus = CAN_INITIALIZED;  
        #endif
    }

} /* END OF CanInit */
  
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_InitController
*                
* Description:   This function initializes the bit timing related settings 
*                of a CAN controller. 
*                             
* Inputs:        Controller:  CAN controller to be initialized 
*                config: paramter config pointer
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/ 
FUNC(void, CAN_PUBLIC_CODE)Can_InitController
( 
    uint8 Controller, 
    P2CONST(Can_ControllerBaudrateConfigType, AUTOMATIC, CAN_CONFIG_CONST) 
                                                                          Config
)
{  
    uint8 phyController = CanControllerIDtoPhys[CAN_CONTROLLER_ID];

    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    /* For some reson , the current version SW can not detect the following 2 
    DET error,dont remove the code to easy the further update */
    /*
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_INITCONTROLLER_APIID,
                        CAN_E_UNINIT);
    }
    if(Config == NULL_PTR) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_INITCONTROLLER_APIID,
                        CAN_E_PARAM_CONTROLLER);
    }*/
    if(Controller >= CAN_USED_CONTROLLER_NUM) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_INITCONTROLLER_APIID,
                        CAN_E_PARAM_CONTROLLER);
    }
    else
    #endif
    {

        (void)Config;
        Can_BusoffRecovery(phyController);   
        /*request enter into initialization mode*/
        CAN_CR(phyController) = 0ul;
        
        CAN_LOOP_BEGIN(INIT_ENTER_TIMEOUT);
        while(( 0 == (CAN_SR(phyController) & CAN_INITAK))
                            && (CAN_OK != CAN_LOOP_TIMEOUT(INIT_ENTER_TIMEOUT)))
        {
            /*wait while not in initialization mode */
            CAN_CR(phyController) = 0ul;
        }
        
        CAN_LOOP_END(INIT_ENTER_TIMEOUT);
        
        if(0 != (CAN_SR(phyController) & CAN_INITAK))
        /*already in initialization mode*/
        {
            /*The following registers enter their hard reset state and restore  
            default values: CANCTL0 (Not including WUPE, INITRQ, and SLPRQ.), 
            CANRFLG,CANRIER, CANTFLG, CANTIER, CANTARQ, CANTAAK, and CANTBSEL.*/
            /*The registers CANCTL1, CANBTR0, CANBTR1, CANIDAC, CANIDAR0-7, and 
            CANIDMR0-7 can only be written by the CPU when the MSCAN is in 
            initialization mode (INITRQ = 1 and INITAK = 1). The values of the
            error counters are not affected by initialization mode.*/
            
            /*enable can module*/
            
            /*set Baudrate*/
            CAN_BRPR(phyController) = CanRegInit[CAN_CONTROLLER_ID].CanInitBRPR;
            CAN_BTR(phyController) = CanRegInit[CAN_CONTROLLER_ID].CanInitBTR;
        }
        #if (CAN_DEV_ERROR_DETECT == STD_ON )
        else 
        /*fail to enter into freeze mode*/
        {   
            Det_ReportError(CAN_MODULE_ID_VALUE,
                            CAN_INSTANCE_ID_VALUE_0,
                            CAN_INITCONTROLLER_APIID,
                            CAN_PL_INIT_ERROR);
        }
        #endif
    
    }
    
    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    Can_State[CAN_CONTROLLER_ID] = CAN_STOPPED;   
    #endif 
       
} 

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_SetControllerMode
*                
* Description:   This function performs software triggered state 
*                transitions of the CAN controller State machine.  
*                             
* Inputs:        Controller:CAN controller for which the status 
*                           shall be changed 
*                Transition:Transition value to request new 
*                           CAN controller state
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(Can_ReturnType, CAN_PUBLIC_CODE) Can_SetControllerMode
(
    uint8 Controller,
    Can_StateTransitionType Transition
)

{
    Can_ReturnType ret = CAN_OK;
    uint8 phyController = CanControllerIDtoPhys[CAN_CONTROLLER_ID];

    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_SETCONTROLLERMODE_APIID,
                        CAN_E_UNINIT);
        ret = CAN_NOT_OK;
    }
    if(Controller >= CAN_USED_CONTROLLER_NUM) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_SETCONTROLLERMODE_APIID,
                        CAN_E_PARAM_CONTROLLER);
        ret = CAN_NOT_OK;
    }
    if(CAN_OK == ret)
    #endif

    {
        switch(Transition)  
        {
            case CAN_T_START:
            #if (CAN_DEV_ERROR_DETECT == STD_ON )
            if(Can_State[CAN_CONTROLLER_ID] != CAN_STOPPED)
            {
                ret = CAN_NOT_OK;
                Det_ReportError(CAN_MODULE_ID_VALUE,
                            CAN_INSTANCE_ID_VALUE_0,
                            CAN_SETCONTROLLERMODE_APIID,
                            CAN_E_TRANSITION);
            }
            else
            #endif
            {

                Can_MemSet(EmptyFlagForHth[CAN_CONTROLLER_ID], TRUE, 
                                                        HTH_NUM_PER_CONTROLLER);
                Can_MemSet(PduIdForHth[CAN_CONTROLLER_ID], 0xff, 
                                  HTH_NUM_PER_CONTROLLER * (sizeof(PduIdType)));
                /*Select Normal Mode*/
                CAN_MSR(phyController) = 0ul;
                /*leave initialize*/
                CAN_CR(phyController) = 0x02ul;

                CAN_LOOP_BEGIN(INIT_LEAVE_TIMEOUT);
              
                while(( 0 == ((CAN_SR(phyController)) & CAN_NORMALAK))
                     &&(CAN_OK != CAN_LOOP_TIMEOUT(INIT_LEAVE_TIMEOUT))) 
                {
                    ; /* wait while  in initialization mode */
                }
                
                CAN_LOOP_END(INIT_LEAVE_TIMEOUT);
               
                if( 0 != ((CAN_SR(phyController)) & CAN_NORMALAK))
                {
                    /*Writing to bits in CANCTL0(except for INITRQ bit), 
                    CANRFLG, CANRIER, CANTFLG, or CANTIER must be done only 
                    after initialization mode is exited, which is INITRQ = 0 
                    and INITAK = 0.*/
             
                    /* set interrupt mode */
                    CAN_IER(phyController) =
                                      CanRegInit[CAN_CONTROLLER_ID].CanInitIER;
 
                    CanIf_ControllerModeIndication(CAN_CONTROLLER_ID, 
                                                              CANIF_CS_STARTED); 
                    #if (CAN_DEV_ERROR_DETECT == STD_ON )
                    Can_State[CAN_CONTROLLER_ID] = CAN_STARTED;   
                    #endif    
                                 
                }
                else
                /*fail to leave initialization mode*/
                { 
                    Can_ModeMonitor[CAN_CONTROLLER_ID] = CAN_T_START;
                }
        
            }
            break;
        
            case CAN_T_STOP:
            { 
                /*cancel pending messages*/ 
                Can_Controller_Transmit_Abort(phyController) ;
                /*The user is responsible for ensuring that the MSCAN is not 
                active when initialization mode is entered. Otherwise, the abort
                of an on-going message can cause an error condition and can 
                impact other CAN bus devices.*/
                
                /*request enter into initialization mode*/
                CAN_CR(phyController) = 0ul;
                 
                CAN_LOOP_BEGIN(INIT_ENTER_TIMEOUT);
                while(( 0 == (CAN_SR(phyController) & CAN_INITAK))
                     &&(CAN_OK != CAN_LOOP_TIMEOUT(INIT_ENTER_TIMEOUT)) )
                {
                    ; /* wait while not in initialization mode */
                }
                
                CAN_LOOP_END(INIT_ENTER_TIMEOUT);
                 
                if( 0 != (CAN_SR(phyController) & CAN_INITAK))
                {
                    CanIf_ControllerModeIndication(CAN_CONTROLLER_ID, 
                                                              CANIF_CS_STOPPED); 
                    #if (CAN_DEV_ERROR_DETECT == STD_ON )
                    Can_State[CAN_CONTROLLER_ID] = CAN_STOPPED;   
                    #endif  
                }
                else
                /*fail to enter into initialization(stop) mode*/
                { 
                    Can_ModeMonitor[CAN_CONTROLLER_ID] = CAN_T_STOP;
                }
      
            }   
            break;
        
            case CAN_T_SLEEP:    
#if 0
            #if (CAN_DEV_ERROR_DETECT == STD_ON )
            if(Can_State[CAN_CONTROLLER_ID] == CAN_STARTED) 
            {
                Det_ReportError(CAN_MODULE_ID_VALUE,
                                CAN_INSTANCE_ID_VALUE_0,
                                CAN_SETCONTROLLERMODE_APIID,
                                CAN_E_TRANSITION);
                ret = CAN_NOT_OK;             
            }
            else
            #endif 
            {
                /* First set hw start,then sleep ,otherwise set sleep is wrong*/
                CAN_CTL0(phyController) &= (~CAN_INITRQ);
                CAN_LOOP_BEGIN(INIT_LEAVE_TIMEOUT);
              
                while(( 0 != ((CAN_CTL1(phyController)) & CAN_INITAK))
                     &&(CAN_OK != CAN_LOOP_TIMEOUT(INIT_LEAVE_TIMEOUT))) 
                {
                    ; /* wait while  in initialization mode */
                }
                
                CAN_LOOP_END(INIT_LEAVE_TIMEOUT);
                
                if( 0 == ((CAN_CTL1(phyController)) & CAN_INITAK))
                {
                    /*Writing to bits in CANCTL0(except for INITRQ bit), 
                    CANRFLG, CANRIER, CANTFLG, or CANTIER must be done only 
                    after initialization mode is exited, which is INITRQ = 0 
                    and INITAK = 0.*/
             
                    /* set interrupt mode */
                    CAN_RIER(phyController) = 
                                      CanRegInit[CAN_CONTROLLER_ID].CanInitRIER;
                }
                
                Can_Controller_Transmit_Abort(phyController) ;
                
                /*request enter into sleep mode and  allows the MSCAN to 
                restart from sleep mode when traffic on CAN is detected*/
                CAN_CTL0(phyController) |= (CAN_SLPRQ | CAN_WUPE);

                CAN_LOOP_BEGIN(SLEEP_ENTER_TIMEOUT);

                while(( 0 == (CAN_CTL1(phyController) & CAN_SLPAK)) 
                     && (CAN_OK != CAN_LOOP_TIMEOUT(SLEEP_ENTER_TIMEOUT)) )
                {
                    if(0 != (CAN_RFLG(phyController) & (CAN_RXF | CAN_WUPIF)))
                    {
                        break;
                    } 
                    else
                    {

                    }
                }
                
                CAN_LOOP_END(INIT_ENTER_TIMEOUT);
                
                if( 0 != (CAN_CTL1(phyController) & CAN_SLPAK))
                {
                    CanIf_ControllerModeIndication(CAN_CONTROLLER_ID, 
                                                                CANIF_CS_SLEEP); 
                    #if (CAN_DEV_ERROR_DETECT == STD_ON )
                    Can_State[CAN_CONTROLLER_ID] = CAN_SLEEP;   
                    #endif  
                }
                else
                /*fail to enter into disable can module*/
                { 
                    Can_ModeMonitor[CAN_CONTROLLER_ID] = CAN_T_SLEEP;
                }
            } 
#endif
            break;
        
            case CAN_T_WAKEUP:
#if 0
            #if (CAN_DEV_ERROR_DETECT == STD_ON )
            if(Can_State[CAN_CONTROLLER_ID] != CAN_SLEEP) 
            {
                 Det_ReportError(CAN_MODULE_ID_VALUE,
                                 CAN_INSTANCE_ID_VALUE_0,
                                 CAN_SETCONTROLLERMODE_APIID,
                                 CAN_E_TRANSITION);
                 ret = CAN_NOT_OK;             
            }
            else
            #endif  
            {
                /*request leave sleep mode*/

                
                CAN_CR(phyController) = 0ul;
                CAN_LOOP_BEGIN(INIT_ENTER_TIMEOUT);
                while(( 0 == (CAN_SR(phyController) & CAN_INITAK))
                     &&(CAN_OK != CAN_LOOP_TIMEOUT(INIT_ENTER_TIMEOUT)) )
                {
                    ; /* wait while not in initialization mode */
                }
                
                CAN_LOOP_END(INIT_ENTER_TIMEOUT);
                
    
                if((( 0 != (CAN_SR(phyController) & CAN_INITAK))))
                {
                    CanIf_ControllerModeIndication(CAN_CONTROLLER_ID, 
                                                              CANIF_CS_STOPPED); 
                    #if (CAN_DEV_ERROR_DETECT == STD_ON )
                    Can_State[CAN_CONTROLLER_ID] = CAN_STOPPED;   
                    #endif 
                }
                else
                /*fail to enter into stop can module*/
                { 
                    Can_ModeMonitor[CAN_CONTROLLER_ID] = CAN_T_WAKEUP;
                }
            }
                 
#endif
            break;
        
            default:
                ret = CAN_NOT_OK;
            break;
        }
    }
    return ret;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_DisableControllerInterrupts
*                
* Description:   This function used to disable all can controller interrupts 
*                             
* Inputs:        Controller
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE) Can_DisableControllerInterrupts
( 
    uint8 Controller 
) 
{
    uint8  phyController = CanControllerIDtoPhys[CAN_CONTROLLER_ID];

    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_DISABLECONTROLLERINTERRUPTS_APIID,
                        CAN_E_UNINIT);
    }
    else if(Controller >= CAN_USED_CONTROLLER_NUM) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_DISABLECONTROLLERINTERRUPTS_APIID,
                        CAN_E_PARAM_CONTROLLER);
        
    }
    else
    #endif
    {
        if(0 == (CanControllerInterruptCount[CAN_CONTROLLER_ID]))
        {
            CanControllerOldInterrupReg[CAN_CONTROLLER_ID].oldCanIER =
                                                           CAN_IER(phyController);

            
            CAN_IER(phyController) &= ~(uint32)((uint32)((CAN_ICR_CRXNEMP_Msk) | (uint32)(CAN_IER_TXOKIE_Msk)));

        }
        CanControllerInterruptCount[CAN_CONTROLLER_ID]++;
        if(CanControllerInterruptCount[CAN_CONTROLLER_ID] > 250)
        {
            CanControllerInterruptCount[CAN_CONTROLLER_ID] = 250;
        }
    
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_EnableControllerInterrupts
*                
* Description:   This function enables all allowed interrupts. 
*                             
* Inputs:        Controller
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE)Can_EnableControllerInterrupts
( 
    uint8 Controller 
) 
{
    uint8  phyController = CanControllerIDtoPhys[CAN_CONTROLLER_ID];

    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_ENABLECONTROLLERINTERRUPTS_APIID,
                        CAN_E_UNINIT);
    }
    else if(Controller >= CAN_USED_CONTROLLER_NUM) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_ENABLECONTROLLERINTERRUPTS_APIID,
                        CAN_E_PARAM_CONTROLLER);
    }
    else
    #endif
    {
        if(CanControllerInterruptCount[CAN_CONTROLLER_ID] > 0)
        {
            (CanControllerInterruptCount[CAN_CONTROLLER_ID])--;
        }
        if(0 == (CanControllerInterruptCount[CAN_CONTROLLER_ID]))
        {
            CAN_IER(phyController) =
                      CanControllerOldInterrupReg[CAN_CONTROLLER_ID].oldCanIER;
        }
      
    }

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_DisableGlobalInterrupt
*                
* Description:   This function disables all maskable interrupts 
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE)Can_DisableGlobalInterrupt(void) 
{
#if 0
    if(0 == CanGlobalInterruptCount)
    {
        asm
        {
            psha
            tpa    ; CCR into A
            sei    ; set I flag
            staa CCRCopyValue ; save old interrup state
            pula
        }
    } 
    
    CanGlobalInterruptCount++;
    if(CanGlobalInterruptCount > 250)
    {
        CanGlobalInterruptCount = 250;
    }
#endif
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_EnableGlobalInterrupt
*                
* Description:   This function enables all maskable interrupts 
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE) Can_EnableGlobalInterrupt(void) 
{
#if 0
    if(CanGlobalInterruptCount>0)
    {
        CanGlobalInterruptCount--;
    }
    
    if(0 == CanGlobalInterruptCount)
    {
        asm
        {
            ldaa CCRCopyValue;
            tap  ; restore the previously saved CCR
        }
    }
#endif
}

/*******************************************************************************
*  Services affecting a Hardware Handle 
*******************************************************************************/

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_Write
*                
* Description:   This function used to send 1 can frame
*                             
* Inputs:        PduInfo:  Pointer to SDU user memory, DLC and Identifier.
*                Hth:  information which HW-transmit handle shall be used 
*                      for transmit. Implicitly this is also the information 
*                      about the controller to use because the Hth numbers 
*                      are unique inside one hardware unit.
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(Can_ReturnType, CAN_PUBLIC_CODE) Can_Write
(
    Can_HwHandleType Hth,
    P2CONST(Can_PduType, AUTOMATIC, CAN_APPL_DATA) PduInfo
)
{
    Can_ReturnType ret = CAN_OK;
    uint8 hthmatchflag = CAN_NOT_MATCH;
   // uint8 Dlccount = 0;
    uint8 phyhth = 0;       
    uint8 phyhthTXEMask = 0;
    uint8 Controller = CanHardwareObjectConfig[Hth].controllerRef;  
    uint8 phyController = CanControllerIDtoPhys[Controller];
  //  uint8 dlc;
   //uint32 id;
    uint32 data1 = 0;
    uint32 data2 = 0;
    /*the canID which is requested to transmit*/  
    Can_IdType canID = 0;
    uint32_t primask;
    #if (STD_ON ==CAN_HW_TRANSMIT_CANCELLATION)
        #if(STD_ON == CAN_MULTIPLEXED_TRANSMISSION)
        Can_IdType canIDBiggest=CanIDForHth[Controller][0];
        Can_HwHandleType  canphyhthmatch=0;
        #endif
    #endif
      
    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_WRITE_APIID,
                        CAN_E_UNINIT);
        ret =  CAN_NOT_OK ;
    }
    if(Hth >= CAN_USED_HOH_NUM) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_WRITE_APIID,
                        CAN_E_PARAM_HANDLE);
        ret =  CAN_NOT_OK;
    }
    if(NULL_PTR == PduInfo)
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_WRITE_APIID,
                        CAN_E_PARAM_POINTER);
        ret =  CAN_NOT_OK ;
    
    }
    if((PduInfo->length) > 8)
    {
    
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_WRITE_APIID,
                        CAN_E_PARAM_DLC);
        ret =  CAN_NOT_OK ;
    
    }
    if(CAN_OK==ret)
    #endif
    {     

        CAN_ENTER_CRITICAL_SECTION();

        #if (STD_ON == CAN_MULTIPLEXED_TRANSMISSION)
        /*look for the empty phyth to transmit*/
        for(phyhth=0; phyhth<CAN_MULTIPLEXED_HTH_MBNUM; phyhth++)
        {
            phyhthTXEMask = CAN_TXMASK(phyhth);
            if((0 != (CAN_TFLG(phyController) & phyhthTXEMask)) 
              &&(TRUE == EmptyFlagForHth[Controller][phyhth]))
            {
                hthmatchflag = CAN_MATCH;
                break;
            }   
        }
        #else 
        phyhth = CanHardwareObjectConfig[Hth].phyHohRef;
        phyhthTXEMask = CAN_TXMASK(phyhth);

        if(( 0 == (CAN_SR(phyController) & CAN_SR_TXFLL_Msk) )
          &&(TRUE == EmptyFlagForHth[Controller][phyhth]))
        {
            hthmatchflag = CAN_MATCH;
        }

        #endif
        if(CAN_MATCH == hthmatchflag)
        /*Hth is empty*/
        {
            EmptyFlagForHth[Controller][phyhth] = FALSE;

            phyhthTXEMask = CAN_TXMASK(phyhth);
            
            PduIdForHth[Controller][phyhth] = PduInfo->swPduHandle ;
            #if (STD_ON ==CAN_HW_TRANSMIT_CANCELLATION)
            CanIDForHth[Controller][phyhth] = PduInfo->id;
            #endif
            
            /*select the valid phyhth*/

            /*stores the identifier, the control bits, and the data content 
            into one of the transmit buffers*/
            /*Identifier Registers*/

            if(0 == ((PduInfo->id) & MSK_EXT_ID_MSB))
            /*standard ID*/
            { 
                CAN_TXF_IDR(phyController) = GET_REG_STDID(PduInfo->id);
            }
            else
            /*externed ID*/
            {
                CAN_TXF_IDR(phyController) = GET_REG_EXTID(PduInfo->id);
            }
     
            /*Data Length Register*/
            CAN_TXF_DLCR(phyController) = PduInfo->length;
            /*Data Segment Registers*/
            data1 = (((uint32_t)PduInfo->sdu[3] << 24) |
                     ((uint32_t)PduInfo->sdu[2] << 16) |
                     ((uint32_t)PduInfo->sdu[1] << 8) |
                     ((uint32_t)PduInfo->sdu[0]));
            data2 = (((uint32_t)PduInfo->sdu[7] << 24) |
                     ((uint32_t)PduInfo->sdu[6] << 16) |
                     ((uint32_t)PduInfo->sdu[5] << 8) |
                     ((uint32_t)PduInfo->sdu[4]));
            CAN_TXF_DW1R(phyController) = data1;
            CAN_TXF_DW2R(phyController) = data2;


            /*the buffer is flagged as ready for transmission 
            by clearing the associated TXE flag*/
            CAN_ICR(phyController) = phyhthTXEMask ;
        
            /*use (CanRegInit[Controller].CanInitTIER) to judge whether the TX 
            interrupt is enabled if enabled ,enable the relative TX interrupt
            */     
            CAN_IER(phyController) |= ((CanRegInit[Controller].CanInitIER) & phyhthTXEMask);
               
        }
        else
        /*Hth is full*/
        {
            ret = CAN_BUSY;
        
            /*the canID which is requested to transmit*/  
            canID = PduInfo->id;
            
            #if (STD_ON == CAN_HW_TRANSMIT_CANCELLATION)
                #if(STD_ON == CAN_MULTIPLEXED_TRANSMISSION)
    
                for(phyhth = 0; phyhth < CAN_MULTIPLEXED_HTH_MBNUM; phyhth++)
                {
                    if(CanIDForHth[Controller][phyhth] > canIDBiggest)
                    {
                        canIDBiggest = CanIDForHth[Controller][phyhth];
                        canphyhthmatch = phyhth;
                    }
                    #if (STD_ON == CAN_IDENTICAL_ID_CANCELLATION)
                    if(canID == CanIDForHth[Controller][phyhth])
                    {
                        canphyhthmatch = phyhth;
                        EmptyFlagForHth[Controller][phyhth] = 
                                                 REQUEST_FOR_IDENTICAL_ID_ABORT;
                        break;
                    }
                    #endif
                }
                phyhth = canphyhthmatch;
                #else
            
                #endif
                /*CAN286: The Can module shall initiate a cancellation, when the
                hardware transmit object assigned by a HTH is busy and an L-PDU 
                with higher priority is requested to be transmitted*/
                #if (STD_ON == CAN_IDENTICAL_ID_CANCELLATION)         
                if(CanIDForHth[Controller][phyhth] >= canID)
                #else
                if(CanIDForHth[Controller][phyhth] > canID)
                #endif
                {
                    Can_Transmit_Abort(phyController, phyhth);
                }
            #endif
        }
        CAN_LEAVE_CRITICAL_SECTION();
    }
    return ret;    
}

/*******************************************************************************
*  Scheduled functions
*******************************************************************************/

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_MainFunction_Write
*                
* Description:   This function performs the polling of TX confirmation 
*                when CAN_TX_PROCESSING is set to CAN_POLLING. 
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_Write(void)
{
    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_MAINFUNCTION_WRITE_APIID,
                        CAN_E_UNINIT);
    }
    else
    #endif
    {
        #if (CAN_USED_CONTROLLER_NUM >= 1)
            #if (CAN0_TX_PROCESSING == CAN_POLLING)
                CAN_Controller_Tx_handler(CAN_DRIVER_CONTROLLER_ID_0);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 2)
            #if (CAN1_TX_PROCESSING == CAN_POLLING)
                CAN_Controller_Tx_handler(CAN_DRIVER_CONTROLLER_ID_1);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 3)
            #if (CAN2_TX_PROCESSING == CAN_POLLING)
                CAN_Controller_Tx_handler(CAN_DRIVER_CONTROLLER_ID_2);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 4)
            #if (CAN3_TX_PROCESSING == CAN_POLLING)
                CAN_Controller_Tx_handler(CAN_DRIVER_CONTROLLER_ID_3);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 5)
            #if (CAN4_TX_PROCESSING == CAN_POLLING)
                CAN_Controller_Tx_handler(CAN_DRIVER_CONTROLLER_ID_4);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM == 6)
            #if (CAN5_TX_PROCESSING == CAN_POLLING)
                CAN_Controller_Tx_handler(CAN_DRIVER_CONTROLLER_ID_5);
            #endif
        #endif
    }
} 

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_MainFunction_Read
*                
* Description:   This function performs the polling of RX indications 
*                when CAN_RX_PROCESSING is set to CAN_POLLING. 
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_Read(void)
{
    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_MAINFUNCTION_READ_APIID,
                        CAN_E_UNINIT);
    }
    else
    #endif
    {
        #if (CAN_USED_CONTROLLER_NUM >= 1)
            #if (CAN0_RX_PROCESSING == CAN_POLLING)
                CAN_Controller_Rx_handler(CAN_DRIVER_CONTROLLER_ID_0);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 2)
            #if (CAN1_RX_PROCESSING == CAN_POLLING)
                CAN_Controller_Rx_handler(CAN_DRIVER_CONTROLLER_ID_1);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 3)
            #if (CAN2_RX_PROCESSING == CAN_POLLING)
                CAN_Controller_Rx_handler(CAN_DRIVER_CONTROLLER_ID_2);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 4)
            #if (CAN3_RX_PROCESSING == CAN_POLLING)
                CAN_Controller_Rx_handler(CAN_DRIVER_CONTROLLER_ID_3);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM == 5)
            #if (CAN4_RX_PROCESSING == CAN_POLLING)
                CAN_Controller_Rx_handler(CAN_DRIVER_CONTROLLER_ID_4);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM == 6)
            #if (CAN5_RX_PROCESSING == CAN_POLLING)
                CAN_Controller_Rx_handler(CAN_DRIVER_CONTROLLER_ID_5);
            #endif
        #endif
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_MainFunction_BusOff
*                
* Description:   This function performs the polling of bus-off events that 
*                are configured statically as 'to be polled'.
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_BusOff(void)   
{
    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_MAINFUNCTION_BUSOFF_APIID,
                        CAN_E_UNINIT);
    }
    else
    #endif
    {
        #if (CAN_USED_CONTROLLER_NUM >= 1)
            #if (CAN0_BUSOFF_PROCESSING == CAN_POLLING)
                CAN_Controller_Busoff_handler(CAN_DRIVER_CONTROLLER_ID_0);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 2)
            #if (CAN1_BUSOFF_PROCESSING == CAN_POLLING)
                CAN_Controller_Busoff_handler(CAN_DRIVER_CONTROLLER_ID_1);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 3)
            #if (CAN2_BUSOFF_PROCESSING == CAN_POLLING)
                CAN_Controller_Busoff_handler(CAN_DRIVER_CONTROLLER_ID_2);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 4)
            #if (CAN3_BUSOFF_PROCESSING == CAN_POLLING)
                CAN_Controller_Busoff_handler(CAN_DRIVER_CONTROLLER_ID_3);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 5)
            #if (CAN4_BUSOFF_PROCESSING == CAN_POLLING)
                CAN_Controller_Busoff_handler(CAN_DRIVER_CONTROLLER_ID_4);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM == 6)
            #if (CAN5_BUSOFF_PROCESSING == CAN_POLLING)
                CAN_Controller_Busoff_handler(CAN_DRIVER_CONTROLLER_ID_5);
            #endif
        #endif
    }
} 

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_MainFunction_Wakeup
*                
* Description:   This function performs the polling of wake-up events that 
*                are configured statically as 'to be polled'.
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_Wakeup(void)
{
    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_MAINFUNCTION_WAKEUP_APIID,
                        CAN_E_UNINIT);
    }
    else
    #endif
    
    {
        #if (CAN_USED_CONTROLLER_NUM >= 1)
            #if (CAN0_WAKEUP_PROCESSING == CAN_POLLING)
                CAN_Controller_Wakeup_handler(CAN_DRIVER_CONTROLLER_ID_0);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 2)
            #if (CAN1_WAKEUP_PROCESSING == CAN_POLLING)
                CAN_Controller_Wakeup_handler(CAN_DRIVER_CONTROLLER_ID_1);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 3)
            #if (CAN2_WAKEUP_PROCESSING == CAN_POLLING)
                CAN_Controller_Wakeup_handler(CAN_DRIVER_CONTROLLER_ID_2);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM >= 4)
            #if (CAN3_WAKEUP_PROCESSING == CAN_POLLING)
                CAN_Controller_Wakeup_handler(CAN_DRIVER_CONTROLLER_ID_3);
            #endif
        #endif
        #if (CAN_USED_CONTROLLER_NUM == 5)
            #if (CAN4_WAKEUP_PROCESSING == CAN_POLLING)
                CAN_Controller_Wakeup_handler(CAN_DRIVER_CONTROLLER_ID_4);
            #endif
        #endif 
    }
} 

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: Can_MainFunction_Mode
*                
* Description:   This function performs the polling of CAN controller 
*                mode transitions. 
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_Mode(void)
{
    uint8 ControllerIndex = 0;
    uint8 phyController = 0;
    #if (CAN_DEV_ERROR_DETECT == STD_ON )
    if(CAN_UNINITIALIZED == Can_DriverInitStatus) 
    {
        Det_ReportError(CAN_MODULE_ID_VALUE,
                        CAN_INSTANCE_ID_VALUE_0,
                        CAN_MAINFUNCTION_MODE_APIID,
                        CAN_E_UNINIT);
    }
    else
    #endif
    {
  
        for(ControllerIndex = 0; ControllerIndex < CAN_USED_CONTROLLER_NUM; 
                                                              ControllerIndex++)
        {
            phyController = CanControllerIDtoPhys[ControllerIndex];
            switch(Can_ModeMonitor[ControllerIndex])
            {
                
                case CAN_T_START:
                {
                    if(0 != ((CAN_SR(phyController)) & CAN_NORMALAK))
                    {
                        CanIf_ControllerModeIndication(ControllerIndex, 
                                                              CANIF_CS_STARTED); 
                        #if (CAN_DEV_ERROR_DETECT == STD_ON )
                        Can_State[ControllerIndex] = CAN_STARTED;   
                        #endif 
                        Can_ModeMonitor[ControllerIndex] = CAN_T_NONE; 
                    }
                }
                break;

                case CAN_T_STOP:
                {
                    if( 0 != (CAN_SR(phyController)) & CAN_INITAK)
                    {
                        CanIf_ControllerModeIndication(ControllerIndex, 
                                                              CANIF_CS_STOPPED); 
                        #if (CAN_DEV_ERROR_DETECT == STD_ON )
                        Can_State[ControllerIndex] = CAN_STOPPED;   
                        #endif 
                        Can_ModeMonitor[ControllerIndex] = CAN_T_NONE; 
                    }
                }
                break;

                case CAN_T_SLEEP:
                {
#if 0
                    if(0 != (CAN_CTL1(phyController) & CAN_SLPAK))
                    {
                        CanIf_ControllerModeIndication(ControllerIndex, 
                                                                CANIF_CS_SLEEP); 
                        #if (CAN_DEV_ERROR_DETECT == STD_ON )
                        Can_State[ControllerIndex] = CAN_SLEEP;   
                        #endif 
                        Can_ModeMonitor[ControllerIndex] = CAN_T_NONE; 
                    }
#endif
                }
                break;

                case CAN_T_WAKEUP:
                {
#if 0
                    if(0 == (CAN_CTL1(phyController) & CAN_SLPAK))
                    {
                        CanIf_ControllerModeIndication(ControllerIndex, 
                                                              CANIF_CS_STOPPED); 

                        #if (CAN_DEV_ERROR_DETECT == STD_ON )
                        Can_State[ControllerIndex] = CAN_STOPPED ;   
                        #endif 
                        Can_ModeMonitor[ControllerIndex] = CAN_T_NONE; 
                    }
#endif
                }
                break;
                default:
                break;
                
            }
        }
    }
} 

#define CAN_STOP_SEC_PUBLIC_CODE

/*******************************************************************************
*  ISR
*******************************************************************************/                                
#define CAN_START_SEC_ISR_CODE


/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name: CAN_PHYx_Tx_ISR \ CAN_PHYx_Rx_ISR
*                \CAN_PHYx_Busoff_ISR\CAN_PHYx_Wakeup_ISR
*                
* Description:   These functions are ISR for can event 
*                             
* Inputs:        None
*             
* Outputs:       None
* 
* Limitations:   None
********************************************************************************
END_FUNCTION_HDR*/
#if(CAN_AUTOENROLL_ISR == STD_ON)
#pragma CODE_SEG  NON_BANKED
#endif


//Main Function
/**
  * @brief  CAN中断函数
  * @param  void
  * @retval void
  */


void CAN_IRQHandler(void)        
{
/* rx ok*/
  if((FL_ENABLE == FL_CAN_IsEnabledIT_RXNotEmpty(CAN))
          && (FL_SET == FL_CAN_IsActiveFlag_RXNotEmpty(CAN)))
  {		
    //vAppCanRxTxIndication();
    vSetBusOk();
    CAN_Controller_Rx_handler(CAN_PHY0_TO_LOGIC);
    FL_CAN_ClearFlag_RXNotEmpty(CAN);
  }
  /* tx ok*/
  if((FL_ENABLE == FL_CAN_IsEnabledIT_TXOK(CAN))
          && (FL_SET == FL_CAN_IsActiveFlag_TXOK(CAN)))
  {
    //vAppCanRxTxIndication();
    vSetBusOk();
    CAN_Controller_Tx_handler(CAN_PHY0_TO_LOGIC);
    FL_CAN_ClearFlag_TXOK(CAN);

  }
/*busoff int*/
  if((FL_ENABLE == FL_CAN_IsEnabledIT_BusOff(CAN))
          && (FL_SET == FL_CAN_IsActiveFlag_BusOff(CAN)))
  {
   
    //vAppCanBusOffIndication();
    vSetbusoffFlag();
    (void)Can_SetControllerMode(CAN_CONTROLLER_ID, CAN_T_STOP);                  
    (void)CanIf_ControllerBusOff(CAN_CONTROLLER_ID);
    FL_CAN_ClearFlag_BusOff(CAN);
  }
}



#if 0
#if (STD_ON == CAN_PHY0_ACTIVATION)
    #if(CAN_INTERRUPT == CAN_PHY0_TX_PROCESSING)
        CAN_INTERRUPT_ISR CAN0TX_VECTOR_NUMBER void CAN_PHY0_Tx_ISR(void)
        {
            CAN_Controller_Tx_handler(CAN_PHY0_TO_LOGIC);
        }
    #endif
    #if(CAN_INTERRUPT == CAN_PHY0_RX_PROCESSING)
        CAN_INTERRUPT_ISR CAN0RX_VECTOR_NUMBER void CAN_PHY0_Rx_ISR(void)
        {
            CAN_Controller_Rx_handler(CAN_PHY0_TO_LOGIC);
        }
    #endif
    #if(CAN_INTERRUPT == CAN_PHY0_BUSOFF_PROCESSING)
        CAN_INTERRUPT_ISR CAN0ERR_VECTOR_NUMBER void CAN_PHY0_Busoff_ISR(void)
        {
            CAN_Controller_Busoff_handler(CAN_PHY0_TO_LOGIC);
        }
    #endif
    #if(CAN_INTERRUPT == CAN_PHY0_WAKEUP_PROCESSING)
        CAN_INTERRUPT_ISR CAN0WAKEUP_VECTOR_NUMBER void CAN_PHY0_Wakeup_ISR(void)
        {
            CAN_Controller_Wakeup_handler(CAN_PHY0_TO_LOGIC);
        }
    #endif
#endif
#endif
#if(CAN_AUTOENROLL_ISR == STD_ON)
#pragma CODE_SEG DEFAULT
#endif

    
                
