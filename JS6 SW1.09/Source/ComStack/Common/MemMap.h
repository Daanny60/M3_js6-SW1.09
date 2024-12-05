/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Mem_Map.h
********************************************************************************
*   Project/Product : Common
*   Title           : Mem_Map.h 
*   Author          : ning.chen
********************************************************************************
*   Description     : AUTOSAR Memory map file
*                     This file will use #pragma key words a lot, so it may 
*                     be different in different compiler.                     
********************************************************************************
*   Limitations     : This file only used in Codewarrior for S12X
*
********************************************************************************
*
********************************************************************************
*   Revision History:
* 
*   Version     Date          Initials      CR#          Descriptions
*   ---------   ----------    ------------  ----------   ---------------
*   01.00.00    13/06/2013    ning.chen     N/A          Original
*   01.00.01    17/07/2013    ning.chen     N/A          DPN_COMMON_130717_01
*   01.00.02    14/08/2013    ning.chen     N/A          DPN_COMMON03_130814_01
*   01.00.03    13/09/2013    ning.chen     N/A          D10_COMMON03_130913_01
********************************************************************************
* END_FILE_HDR*/

#if defined (START_WITH_IF)
/*******************************************************************************
*   CAN Communication Stack START
*******************************************************************************/

/*******************************************************************************
*   CAN Driver START 
*******************************************************************************/
#elif defined (CAN_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CAN_START_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CAN_STOP_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CAN_START_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CAN_START_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CAN_STOP_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CAN_START_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CAN_STOP_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CAN_START_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CAN_STOP_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_START_SEC_ISR_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CAN_START_SEC_ISR_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_STOP_SEC_ISR_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CAN_STOP_SEC_ISR_CODE
    /* Add #pragma code here */
  #endif
#elif defined (CAN_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CAN_START_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CAN_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CAN_STOP_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
/*******************************************************************************
*   CAN Driver END 
*******************************************************************************/
 
/*******************************************************************************
*   CAN Interface START 
*******************************************************************************/
#elif defined (CANIF_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANIF_START_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANIF_STOP_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANIF_START_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANIF_STOP_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANIF_START_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANIF_STOP_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANIF_START_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANIF_STOP_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANIF_START_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANIF_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANIF_STOP_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
/*******************************************************************************
*   CAN Interface END 
*******************************************************************************/

/*******************************************************************************
*   DCM START 
*******************************************************************************/
#elif defined (DCM_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  DCM_START_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  DCM_STOP_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  DCM_START_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  DCM_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  DCM_START_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  DCM_STOP_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  DCM_START_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  DCM_STOP_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  DCM_START_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  DCM_STOP_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  DCM_START_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (DCM_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  DCM_STOP_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
/*******************************************************************************
*   DCM END 
*******************************************************************************/



/*******************************************************************************
*   CANTP START 
*******************************************************************************/
#elif defined (CANTP_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANTP_START_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANTP_STOP_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANTP_START_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANTP_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANTP_START_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANTP_STOP_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANTP_START_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANTP_STOP_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANTP_START_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANTP_STOP_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CANTP_START_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (CANTP_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CANTP_STOP_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
/*******************************************************************************
*   CANTP END 
*******************************************************************************/

/*******************************************************************************
*   CCP START 
*******************************************************************************/
#elif defined (CCP_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CCP_START_SEC_VAR_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CCP_STOP_SEC_VAR_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CCP_START_SEC_CONFIG_DATA_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CCP_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CCP_START_SEC_CONST_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CCP_STOP_SEC_CONST_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CCP_START_SEC_PUBLIC_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CCP_STOP_SEC_PUBLIC_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CCP_START_SEC_PRIVATE_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CCP_STOP_SEC_PRIVATE_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_START_SEC_ISR_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CCP_START_SEC_ISR_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_STOP_SEC_ISR_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CCP_STOP_SEC_ISR_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  CCP_START_SEC_APPL_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (CCP_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  CCP_STOP_SEC_APPL_CODE 
      /* Add #pragma code here */
  #endif
/*******************************************************************************
*   CCP END 
*******************************************************************************/


/*******************************************************************************
*   COM START 
*******************************************************************************/
#elif defined (COM_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  COM_START_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (COM_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  COM_STOP_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (COM_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  COM_START_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (COM_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  COM_START_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (COM_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  COM_STOP_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (COM_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  COM_START_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (COM_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  COM_STOP_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (COM_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  COM_START_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (COM_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  COM_STOP_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (COM_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  COM_START_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (COM_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  COM_STOP_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
/*******************************************************************************
*   COM END 
*******************************************************************************/

/*******************************************************************************
*   OSEKNM START 
*******************************************************************************/
#elif defined (OSEKNM_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  OSEKNM_START_SEC_VAR_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  OSEKNM_STOP_SEC_VAR_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  OSEKNM_START_SEC_CONFIG_DATA_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  OSEKNM_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  COSEKNM_START_SEC_CONST_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  COSEKNM_STOP_SEC_CONST_UNSPECIFIED 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  OSEKNM_START_SEC_PUBLIC_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  OSEKNM_STOP_SEC_PUBLIC_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  OSEKNM_START_SEC_PRIVATE_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  OSEKNM_STOP_SEC_PRIVATE_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_START_SEC_ISR_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  OSEKNM_START_SEC_ISR_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_STOP_SEC_ISR_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  OSEKNM_STOP_SEC_ISR_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  OSEKNM_START_SEC_APPL_CODE 
      /* Add #pragma code here */
  #endif
#elif defined (OSEKNM_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  OSEKNM_STOP_SEC_APPL_CODE 
      /* Add #pragma code here */
  #endif
/*******************************************************************************
*   OSEKNM END 
*******************************************************************************/

/*******************************************************************************
*   CAN Communication Stack END
*******************************************************************************/

/*******************************************************************************
*   LIN Communication Stack START
*******************************************************************************/

/*******************************************************************************
*   Lin Driver START 
*******************************************************************************/

#elif defined (LIN_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LIN_START_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LIN_STOP_SEC_VAR_UNSPECIFIED 
   /* Add #pragma code here */
  #endif
#elif defined (LIN_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LIN_START_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LIN_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LIN_START_SEC_CONST_UNSPECIFIED 
   /* Add #pragma code here */
  #endif
#elif defined (LIN_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LIN_STOP_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LIN_START_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LIN_STOP_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LIN_START_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LIN_STOP_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_START_SEC_ISR_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LIN_START_SEC_ISR_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_STOP_SEC_ISR_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LIN_STOP_SEC_ISR_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LIN_START_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LIN_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LIN_STOP_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
/*******************************************************************************
*   Lin Driver END 
*******************************************************************************/

/*******************************************************************************
*   Lin Interface START 
*******************************************************************************/

#elif defined (LINIF_START_SEC_VAR_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LINIF_START_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_STOP_SEC_VAR_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LINIF_STOP_SEC_VAR_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_START_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LINIF_START_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LINIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_START_SEC_CONST_UNSPECIFIED)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LINIF_START_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_STOP_SEC_CONST_UNSPECIFIED)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LINIF_STOP_SEC_CONST_UNSPECIFIED 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_START_SEC_PUBLIC_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LINIF_START_SEC_PUBLIC_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_STOP_SEC_PUBLIC_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LINIF_STOP_SEC_PUBLIC_CODE 
     /* Add #pragma code here */
  #endif
#elif defined (LINIF_START_SEC_PRIVATE_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LINIF_START_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_STOP_SEC_PRIVATE_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LINIF_STOP_SEC_PRIVATE_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_START_SEC_APPL_CODE)
  #ifdef SECTION_STARTED
    #error "Section is not stopped"
  #else
    #define SECTION_STARTED
    #undef  LINIF_START_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
#elif defined (LINIF_STOP_SEC_APPL_CODE)
  #ifndef SECTION_STARTED
    #error "Section is not started"
  #else
    #undef  SECTION_STARTED
    #undef  LINIF_STOP_SEC_APPL_CODE 
    /* Add #pragma code here */
  #endif
/*******************************************************************************
*   Lin Interface END 
*******************************************************************************/


/*******************************************************************************
*   LIN Communication Stack END
*******************************************************************************/



#endif/* START_WITH_IF */
