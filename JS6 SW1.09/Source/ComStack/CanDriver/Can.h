/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Can.h
********************************************************************************
*   Project/Product : AUTOSAR 4.0
*   Title           : CAN Driver module head File
*   Author          : HeYang
********************************************************************************
*   Description     : The header file Can.h contains the declaration of the Can 
*                     module API;contains 'extern' declarations of constants,
*                     global data,type definitions and services that are 
*                     specified in the Can module SWS.
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
#ifndef _CAN_H_
#define _CAN_H_

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "ComStack_Types.h"
#include "Can_Cfg.h"
#include "Can_Type.h"
#include "Can_Pl.h"

/*******************************************************************************
*   Macro 
*******************************************************************************/
#define CAN_DATA_DLC               ((uint8)0x08U)
/*CAN104: The Can module shall be able to detect the following errors and 
exceptions depending on its configuration (development/production)*/
#if (CAN_DEV_ERROR_DETECT == STD_ON )
    /*API Service called with wrong parameter;Development*/
    #define  CAN_E_PARAM_POINTER      (0x01u) 
    /*API Service called with wrong parameter;Development*/  
    #define  CAN_E_PARAM_HANDLE       (0x02u)
    /*API Service called with wrong parameter;Development*/
    #define  CAN_E_PARAM_DLC          (0x03u)
    /*API Service called with wrong parameter;Development*/
    #define  CAN_E_PARAM_CONTROLLER   (0x04u)
    /*API Service used without initialization;Development*/
    #define  CAN_E_UNINIT             (0x05u)
    /*Invalid transition for the current mode;Development*/
    #define  CAN_E_TRANSITION         (0x06u)
    /*Received CAN message is lost;Development*/
    #define  CAN_E_DATALOST           (0x07u)
    /*CAN can't enter into initialize mode or leave initialize mode*/
    #define  CAN_PL_INIT_ERROR        (0x08u)  
    
    #define  CAN_INIT_APIID                                 (0x00u)
    #define  CAN_MAINFUNCTION_WRITE_APIID                   (0x01u)
    #define  CAN_INITCONTROLLER_APIID                       (0x02u)
    #define  CAN_SETCONTROLLERMODE_APIID                    (0x03u)
    #define  CAN_DISABLECONTROLLERINTERRUPTS_APIID          (0x04u)
    #define  CAN_ENABLECONTROLLERINTERRUPTS_APIID           (0x05u)
    #define  CAN_WRITE_APIID                                (0x06u)
    #define  CAN_GETVERSIONINFO_APIID                       (0x07u)
    #define  CAN_MAINFUNCTION_READ_APIID                    (0x08u)
    #define  CAN_MAINFUNCTION_BUSOFF_APIID                  (0x09u) 
    #define  CAN_MAINFUNCTION_WAKEUP_APIID                  (0x0au)
    #define  CAN_MAINFUNCTION_MODE_APIID                    (0x0cu) 
#endif

/*******************************************************************************
*   CanController 
*******************************************************************************/
/*CAN316_Conf    :This parameter rovides the controller ID which is unique in a 
                  given CAN Driver. The value for this parameter starts with 0 
                  and continue without any gaps*/
#if (CAN_USED_CONTROLLER_NUM >= 1)
    #define CAN_DRIVER_CONTROLLER_ID_0  (0u)
#endif
#if (CAN_USED_CONTROLLER_NUM >= 2)
    #define CAN_DRIVER_CONTROLLER_ID_1  (1u)
#endif
#if (CAN_USED_CONTROLLER_NUM >= 3)
    #define CAN_DRIVER_CONTROLLER_ID_2  (2u)
#endif
#if (CAN_USED_CONTROLLER_NUM >= 4)
    #define CAN_DRIVER_CONTROLLER_ID_3  (3u)
#endif
#if (CAN_USED_CONTROLLER_NUM >= 5)
    #define CAN_DRIVER_CONTROLLER_ID_4  (4u) 
#endif
#if (CAN_USED_CONTROLLER_NUM == 6)
    #define CAN_DRIVER_CONTROLLER_ID_5  (5u) 
#endif
#if (CAN_USED_CONTROLLER_NUM > 6)
    #error "!!!ERROR FOR CAN_USED_CONTROLLER_NUM!!!"
#endif

/*******************************************************************************
*   Global Data Declaration 
*******************************************************************************/
#define CAN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"

extern CONST(uint8, CAN_CONFIG_CONST) CanControllerIDtoPhys[];
extern CONST(Can_RegInitType, CAN_CONFIG_CONST) CanRegInit[];
extern CONST(Can_HardwareObjectConfigType, CAN_CONFIG_CONST)    
                                                      CanHardwareObjectConfig[];
extern CONST(Can_HwHandleType, CAN_CONFIG_CONST)  CanControllerIDtoHRH[];

#define CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "MemMap.h"

/*******************************************************************************
*   Global Function Declaration 
*******************************************************************************/
#define CAN_START_SEC_PUBLIC_CODE
#include "MemMap.h"

#if (STD_ON == CAN_VERSION_INFO_API)
extern FUNC(void, CAN_PUBLIC_CODE) Can_GetVersionInfo
( 
    P2VAR(Std_VersionInfoType, AUTOMATIC, CAN_APPL_DATA) versioninfo
);
#endif

extern FUNC(void, CAN_PUBLIC_CODE) Can_Init(void);

extern FUNC(void, CAN_PUBLIC_CODE)Can_InitController
( 
    uint8 Controller, 
    P2CONST(Can_ControllerBaudrateConfigType, AUTOMATIC, CAN_CONFIG_CONST) 
                                                                          Config
);
                       
extern FUNC(Can_ReturnType, CAN_PUBLIC_CODE) Can_SetControllerMode
(
    uint8 Controller,
    Can_StateTransitionType Transition
);
                                    
extern FUNC(void, CAN_PUBLIC_CODE) Can_DisableControllerInterrupts
( 
    uint8 Controller 
); 
                                    
extern FUNC(void, CAN_PUBLIC_CODE)Can_EnableControllerInterrupts
( 
    uint8 Controller 
);
                                    
extern FUNC(void, CAN_PUBLIC_CODE)Can_DisableGlobalInterrupt(void); 

extern FUNC(void, CAN_PUBLIC_CODE) Can_EnableGlobalInterrupt(void);
                            
extern FUNC(Can_ReturnType, CAN_PUBLIC_CODE) Can_Write
(
    Can_HwHandleType Hth,
    P2CONST(Can_PduType, AUTOMATIC, CAN_APPL_DATA) PduInfo
);

/* Scheduled functions */                        
extern FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_Write(void);
                                                     
extern FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_Read(void);

extern FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_BusOff(void);   

extern FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_Wakeup(void);

extern FUNC(void, CAN_PUBLIC_CODE) Can_MainFunction_Mode(void);

#define CAN_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

/* Application Call back function */
#define CAN_START_SEC_APPL_CODE
#include "MemMap.h"

#if (STD_ON == CAN_HARDWARE_TIMEOUT_CHECK) 
extern FUNC(void, CAN_APPL_CODE) ApplTimerBegin(uint8 LoopReason);
extern FUNC(Can_ReturnType, CAN_APPL_CODE) ApplTimerOutCheck(uint8 LoopReason);
extern FUNC(void, CAN_APPL_CODE) ApplTimerEnd(uint8 LoopReason);
#endif/* #if (STD_ON == CAN_HARDWARE_TIMEOUT_CHECK)  */ 

/*Nm callback function*/
extern void CanNm_ControllerActive(uint8 Controller);
extern void CanNm_ControllerPassive(uint8 Controller);
 
#define CAN_STOP_SEC_APPL_CODE
#include "MemMap.h"


#endif /* _CAN_H_ */

