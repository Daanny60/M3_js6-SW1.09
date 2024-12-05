/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Can_Cfg.h
*
********************************************************************************
*   Project/Product : AUTOSAR 4.0
*   Title           : CAN Driver(MSCAN) module configuration File
*   Author          : Hirain
********************************************************************************
*   Description     : Contain the pre-compile-time configuration parameters.
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
*   02.00.01    15/08/2013    NingChen      N/A          D10_MSCAN_130815_1
*
********************************************************************************
* END_FILE_HDR*/



#ifndef _CAN_CFG_H_
#define _CAN_CFG_H_

#include "fm33lg0xx_fl.h"
#include "mf_config.h"
/*******************************************************************************
*   Common Macro 
*******************************************************************************/
/* event process mode */
#define CAN_INTERRUPT               (1u)
#define CAN_POLLING                 (0u)
#define CAN_STANDARD_ID_ONLY        (1u)
#define CAN_EXTERNED_ID_ONLY        (2u)
#define CAN_MIXED_ID                (3u)

#define CAN_HOH_TX_MASK             ((uint8)0x01)
#define CAN_HOH_RX_MASK             ((uint8)0x00)
#define CAN_HOH_FULL_MASK           ((uint8)0x02)
#define CAN_HOH_BASIC_MASK          ((uint8)0x00)

/*******************************************************************************
*   CanGeneral
*******************************************************************************/
#define CAN_DEV_ERROR_DETECT                 STD_OFF
#define CAN_HW_TRANSMIT_CANCELLATION         STD_OFF
#define CAN_IDENTICAL_ID_CANCELLATION        STD_OFF
#define CAN_MULTIPLEXED_TRANSMISSION         STD_OFF
#define CAN_TIMEOUT_DURATION                 (100u)
#define CAN_VERSION_INFO_API                 STD_OFF
#define CAN_MAIN_FUNCTION_BUS_OFF_PERIOD     (1u)
#define CAN_MAIN_FUNCTION_MODE_PERIOD        (1u)
#define CAN_MAIN_FUNCTION_READ_PERIOD        (1u)
#define CAN_MAIN_FUNCTION_WAKEUP_PERIOD      (1u)
#define CAN_MAIN_FUNCTION_WRITE_PERIOD       (1u)
#define USE_CANIF                            STD_ON
#define CAN_AUTOSET_INTERRUPT_PRIORITY       STD_OFF


#define CAN_ID_TYPE_SUPPORT                  CAN_STANDARD_ID_ONLY

#if (CAN_ID_TYPE_SUPPORT ==CAN_STANDARD_ID_ONLY)

   #define CAN_ENABLE_EXTERNED_ID  STD_OFF

#else

   #define CAN_ENABLE_EXTERNED_ID  STD_ON
#endif


#define CAN_USED_HOH_NUM                     (4u)

#define CAN_VENDOR_ID_VALUE                  ((uint16)26)
#define CAN_MODULE_ID_VALUE                  ((uint16)80)
#define CAN_INSTANCE_ID_VALUE_0              ((uint8)0) 

/*******************************************************************************
*    CanController
*******************************************************************************/
#define CAN_USED_CONTROLLER_NUM           (1u)
#define CAN0_CONTROLLER_ACTIVATION        STD_ON
#define CAN0_CONTROLLER_BAUD_RATE         (500u)
#define CAN0_CLOCK_REFERENCE              (8000u)
#define CAN0_CONTROLLER_BASE_ADDRESS      (0x140u)
#define CAN0_CLKSRC_BUSCLK                STD_OFF
#define CAN0_RX_PROCESSING                CAN_INTERRUPT
#define CAN0_TX_PROCESSING                CAN_INTERRUPT
#define CAN0_WAKEUP_PROCESSING            CAN_INTERRUPT
#define CAN0_WAKEUP_SUPPORT               STD_ON
#define CAN0_BUSOFF_PROCESSING            CAN_INTERRUPT

#define CAN_PHY0_ACTIVATION               CAN0_CONTROLLER_ACTIVATION        
#define CAN_PHY0_RX_PROCESSING            CAN0_RX_PROCESSING                
#define CAN_PHY0_TX_PROCESSING            CAN0_TX_PROCESSING                
#define CAN_PHY0_WAKEUP_PROCESSING        CAN0_WAKEUP_PROCESSING            
#define CAN_PHY0_WAKEUP_SUPPORT           CAN0_WAKEUP_SUPPORT               
#define CAN_PHY0_BUSOFF_PROCESSING        CAN0_BUSOFF_PROCESSING            
#define CAN_PHY0_TO_LOGIC                 (0u)

#define Can_DisableGlobalInterrupts()        {primask = __get_PRIMASK();__disable_irq();}
#define Can_EnableGlobalInterrupts()         __set_PRIMASK(primask)

#endif  /* _CAN_CFG_H_ */

