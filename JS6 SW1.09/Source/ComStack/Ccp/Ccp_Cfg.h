/*  BEGIN_FILE_HDR
********************************************************************************
*  NOTICE
*  This software is the property of HiRain Technologies. Any information 
*  contained in this doc should not be reproduced, or used, or disclosed 
*  without the written authorization from HiRain Technologies.
********************************************************************************
*  File Name      : Ccp_Cfg.h
********************************************************************************
*  Project/Product: CCP programe
*  Title          : Ccp_Cfg.h
*  Author         : Peng.Liu
********************************************************************************
*  Description    : This file includes the macro configuration of CCP module.
*
********************************************************************************
*  Limitations    :None
* 
********************************************************************************
* 
********************************************************************************
*  Revision History:
*                                                                     
*   Version       Date         Initials      CR#          Descriptions
*  ---------   ----------    ------------  ----------   ---------------
*   02.20.00    17/02/14      Wenrui.Li      N/A        HrAsrCcp140217-01   
********************************************************************************
*  END_FILE_HDR*/
#ifndef _CCP_CFG_H_                              
#define _CCP_CFG_H_                                    
  
/******************************************************************************* 
*   Includes 
*******************************************************************************/ 
#include "CanIf.h"
                           
#include "ComStack_Types.h"                          
  
/******************************************************************************* 
*   Configuration Macro
*******************************************************************************/ 
 
/*******************************************************************************
*   Node ID Config
*******************************************************************************/
#define CCP_NODE_ID                     ((uint16)(0xa0a0u))
#define CCP_BROADCAST_ID                ((uint16)(0xffffu))
  
/*******************************************************************************
*   Seed&Key Config 
*******************************************************************************/
/* Seed&Key Enable */        	    
#define CCP_SEED_KEY                     
  
/******************************************************************************* 
*   DAQ Config 
*******************************************************************************/
/* DAQ Enable */                            
#define CCP_DAQ_REQUEST                          
  
/* Max Number of DAQ */
#define CCP_MAX_DAQ                     ((uint8)(1u))
/* Max Number of ODT */
#define CCP_MAX_ODT                     ((uint8)(3u))
/* Max Number of ODT buffer for DAQ function */
#define CCP_ODT_BUF_NUM                 ((uint8)(3u))
  
/******************************************************************************* 
*   Handle Config  
*******************************************************************************/
/* Handle Index between CanIf and CCP */          
#define CCP_DTO_INDEX                   ((uint16)(7u))
#define CCP_RX_UL_HANDLE                ((uint16)(0u))
#define CCP_TX_UL_HANDLE                ((uint16)(0u))
 
/******************************************************************************* 
*   Flash Config  
*******************************************************************************/
/* Flash Enable */ 	
#define CCP_FLASH                                             
 
/* Flash range for Upload & Short Upload Command */
#define CCP_FLASH_MIN_ADDR              (0x004000uL)
#define CCP_FLASH_MAX_ADDR              (0x004fffuL)
  
/******************************************************************************* 
*   Checksum Config 
*******************************************************************************/
/* Checksum Enable */  	                                   
#define CCP_CHECKSUM		                                      

/* Checksum Block */	                                   
#define CCP_CHECKSUM_BLOCKSIZE          (256uL)        
/* Checksum calculation algorithm */  

/*******************************************************************************
*   Byte Order Config  
*******************************************************************************/
#define CCP_MOTOROLA                             
 
/******************************************************************************* 
*   ECU Name Config  
*******************************************************************************/
#define CCP_ECU_NAME_LENGTH             (5u)  
#define CCP_ECU_NAME                    "PEPS"
 
/******************************************************************************* 
*   CCP_MTA Config  
*******************************************************************************/
/* Max number of CCP_MTA */ 
#define CCP_MTA_NUM                     ((uint8)(2u))
/******************************************************************************* 
*   Special Config  
*******************************************************************************/
/* Calculation Global_Address */
#define CCP_Global_Address(a)           (a)     
/* FAR Pointer */
#define CCP_FAR                         *           
/* Config for special chips which there are 16bits data in an address unit */
/* Such as some chips of TI */

/******************************************************************************* 
*   Interrupt  Config   
*******************************************************************************/
#define Ccp_DisableInterrupt()              CANIF_ENTER_CRITICAL_SECTION()
#define Ccp_EnableInterrupt()              CANIF_LEAVE_CRITICAL_SECTION()
  
/*******************************************************************************
*   Special Config for INCA 
*******************************************************************************/
/* Page Enable */ 
#define CCP_CAL_PAGE  
 
/* for CAL PAGE */
#define CCP_REF_PAGE_ADDR               (0x4000uL)
#define CCP_WORK_PAGE_ADDR              (0x40004000uL)
 
#define CCP_REF_PAGE_INDEX              ((uint8)0x01u)
#define CCP_WORK_PAGE_INDEX             ((uint8)0x02u)
#define CCP_INIT_PAGE_INDEX             CCP_WORK_PAGE_INDEX
/* Store CAL Enable */ 
#define CCP_STORE_CAL_DATA    

/******************************************************************************* 
*   Configuration Variable Declaration 
*******************************************************************************/
#define CCP_START_SEC_CONFIG_DATA_UNSPECIFIED  
#include "MemMap.h" 

extern CONST(uint8,CCP_CONFIG_CONST) Ccp_Station_ID[CCP_ECU_NAME_LENGTH];

#define CCP_STOP_SEC_CONFIG_DATA_UNSPECIFIED  
 #include "MemMap.h"  
 
 
#endif

