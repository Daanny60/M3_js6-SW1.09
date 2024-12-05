/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Ccp.h
********************************************************************************
*   Project/Product : CCP module
*   Title           : Ccp.h
*   Author          : Peng.Liu
********************************************************************************
*   Description     : This file includes macro definition and function 
*                     declaration of CCP module.                    
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
*   01.00.00    26/08/11      Peng.Liu      N/A          HrAsrCcp110826-01
*   01.01.00    30/08/11      Peng.Liu      N/A          HrAsrCcp110830-01
*   01.02.00    12/04/12      Peng.Liu      N/A          HrAsrCcp120412-01
*   01.02.01    12/04/12      Peng.Liu      N/A          HrAsrCcp120412-02
*   01.03.00    23/05/12      Peng.Liu      N/A          HrAsrCcp120523-01
*   01.03.01    23/05/12      Peng.Liu      N/A          HrAsrCcp120523-02
*   01.04.00    18/07/12      Peng.Liu      N/A          HrAsrCcp120718-01
*   01.04.01    18/07/12      Peng.Liu      N/A          HrAsrCcp120718-02
*   01.04.02    13/08/12      Peng.Liu      N/A          HrAsrCcp120813-01
*   01.04.03    13/08/12      Peng.Liu      N/A          HrAsrCcp120813-02
*   01.05.00    13/08/12      Peng.Liu      N/A          HrAsrCcp120813-03
*   01.06.00    14/08/12      Peng.Liu      N/A          HrAsrCcp120814-01
*   01.07.00    13/10/12      Peng.Liu      N/A          HrAsrCcp121013-01
*   01.08.00    13/10/12      Peng.Liu      N/A          HrAsrCcp121013-02
*   01.09.00    13/10/12      Peng.Liu      N/A          HrAsrCcp121013-02
*   02.00.00    24/12/12      Peng.liu      N/A          HrAsrCcp121224-01
*   02.10.00    08/05/13      Wenrui.Li     N/A          HrAsrCcp130508-01
*   02.11.00    27/05/13      Wenrui.Li     N/A          HrAsrCcp130527-01 
*   02.12.00    31/07/13      Wenrui.Li     N/A          HrAsrCcp130731-01 
*   02.13.00    06/08/13      Wenrui.Li     N/A          HrAsrCcp130806-01 
*   02.14.00    08/08/13      Wenrui.Li     N/A          HrAsrCcp130808-01 
*   02.15.00    19/08/13      Wenrui.Li     N/A          HrAsrCcp130819-01 
*   02.16.00    27/08/13      Wenrui.Li     N/A          HrAsrCcp130827-01 
*   02.17.00    22/10/13      Wenrui.Li     N/A          HrAsrCcp131022-01 
*   02.18.00    22/11/13      Wenrui.Li     N/A          HrAsrCcp131122-01 
*   02.19.00    28/11/13      Wenrui.Li     N/A          HrAsrCcp131128-01 
*   02.20.00    17/02/14      Wenrui.Li     N/A          HrAsrCcp140217-01 
********************************************************************************
* END_FILE_HDR*/

#ifndef _CCP_H_
#define _CCP_H_

/*******************************************************************************
*   Includes 
*******************************************************************************/
#include "Ccp_Cfg.h"
#include "CanIf.h"

/*******************************************************************************
*   Macro definition
*******************************************************************************/
/* Declare CCP-protocol version */
#define CCP_VERSION_MAJOR       ((uint8)(0x02u))
#define CCP_VERSION_MINOR       ((uint8)(0x01u))

/* Resource Mask */
#define CCP_RM_ALL_LOCKED       ((uint8)(0x00u))
#define CCP_RM_CAL              ((uint8)(0x01u))
#define CCP_RM_DAQ              ((uint8)(0x02u))
#define CCP_RM_PGM              ((uint8)(0x40u))

/* CCP Status */
#define CCP_DISCONNECTED        ((uint8)(0x00u))
#define CCP_CONNECTED           ((uint8)(0x20u))
#define CCP_TMP_DISCONNECTED    ((uint8)(0x10u))

#define CCP_RESUME              ((uint8)(0x04u))
#define CCP_STORE               ((uint8)(0x40u))
#define CCP_RUN                 ((uint8)(0x80u))

/* Basic Commands */
#define CCP_CONNECT             ((uint8)(0x01u))
#define CCP_SET_MTA             ((uint8)(0x02u))
#define CCP_DNLOAD              ((uint8)(0x03u))
#define CCP_UPLOAD              ((uint8)(0x04u))
#define CCP_TEST                ((uint8)(0x05u))
#define CCP_START_STOP          ((uint8)(0x06u))
#define CCP_DISCONNECT          ((uint8)(0x07u))
#define CCP_START_STOP_ALL      ((uint8)(0x08u))
#define CCP_SHORT_UPLOAD        ((uint8)(0x0Fu))
#define CCP_GET_DAQ_SIZE        ((uint8)(0x14u))
#define CCP_SET_DAQ_PTR         ((uint8)(0x15u))
#define CCP_WRITE_DAQ           ((uint8)(0x16u))
#define CCP_EXCHANGE_ID         ((uint8)(0x17u))
#define CCP_GET_CCP_VERSION     ((uint8)(0x1Bu))
#define CCP_DNLOAD6             ((uint8)(0x23u))

/* Optional Commands */
#define CCP_GET_CAL_PAGE        ((uint8)(0x09u))
#define CCP_SET_S_STATUS        ((uint8)(0x0Cu))
#define CCP_GET_S_STATUS        ((uint8)(0x0Du))
#define CCP_BUILD_CHKSUM        ((uint8)(0x0Eu))
#define CCP_CLEAR_MEMORY        ((uint8)(0x10u))
#define CCP_SET_CAL_PAGE        ((uint8)(0x11u))
#define CCP_GET_SEED            ((uint8)(0x12u))
#define CCP_UNLOCK              ((uint8)(0x13u))
#define CCP_PROGRAM             ((uint8)(0x18u))
#define CCP_MOVE                ((uint8)(0x19u))
#define CCP_DIAG_SERVICE        ((uint8)(0x20u))
#define CCP_ACTION_SERVICE      ((uint8)(0x21u))
#define CCP_PROGRAM6            ((uint8)(0x22u))

/* Response ID */
#define CCP_RESPONSE_CODE       ((uint8)(0xFFu))

/* Error codes */
#define CCP_NO_ERROR            ((uint8)(0x00u))
#define CCP_DAQ_OVERLOAD        ((uint8)(0x01u))
#define CCP_COMMAND_BUSY        ((uint8)(0x10u))
#define CCP_DAQ_BUSY            ((uint8)(0x11u))
#define CCP_INTERNAL_TIMEOUT    ((uint8)(0x12u))
#define CCP_KEY_REQUEST         ((uint8)(0x18u))
#define CCP_SESSION_REQUEST     ((uint8)(0x19u))
#define CCP_COLD_START_REQ      ((uint8)(0x20u))
#define CCP_CAL_INIT_REQ        ((uint8)(0x21u))
#define CCP_DAQ_INIT_REQ        ((uint8)(0x22u))
#define CCP_CODE_UPDATE_REQ     ((uint8)(0x23u))
#define CCP_UNKNOWN_COMMAND     ((uint8)(0x30u))
#define CCP_COM_SYNTAX_ERROR    ((uint8)(0x31u))
#define CCP_OUT_OF_RANGE        ((uint8)(0x32u))
#define CCP_ACCESS_DENIED       ((uint8)(0x33u))
#define CCP_OVERLOAD            ((uint8)(0x34u))
#define CCP_ACCESS_LOCKED       ((uint8)(0x35u))
#define CCP_RESOURCE_USELESS    ((uint8)(0x36u))

/* Session Status */
#define CCP_DEFAULT_SESSION     ((uint8)(0x00u))
#define CCP_SESSION_CAL         ((uint8)(0x01u))
#define CCP_SESSION_DAQ         ((uint8)(0x02u))
#define CCP_SESSION_RESUME      ((uint8)(0x04u))
#define CCP_SESSION_STORE       ((uint8)(0x40u))
#define CCP_SESSION_RUN         ((uint8)(0x80u))

/* For Disconnect */
#define CCP_END_SESSION         ((uint8)(0x01u))
#define CCP_TEM_END_SESSION     ((uint8)(0x00u))


/* For Exchange ID */
#define CCP_ASCII_TYPE          ((uint8)(0x00u))

/* For Upload */
#define CCP_UPLOAD_MAX          ((uint8)(0x05u))

/* For Get DAQ Size */
#define CCP_UNDEFINED_DAQ       ((uint8)(0x00u))
#define CCP_NO_FIRST_PID        ((uint8)(0x00u))

/* For Get_S_Status */
#define CCP_NO_ADDITIONAL_INFO  ((uint8)(0x00u))

/* For Start_Stop */
#define CCP_STOP_DAQ            ((uint8)(0x00u))
#define CCP_START_DAQ           ((uint8)(0x01u))
#define CCP_PREPARE_DAQ         ((uint8)(0x02u))

/* For Start_Stop_All */
#define CCP_STOP_DAQ_ALL        ((uint8)(0x00u))
#define CCP_START_DAQ_ALL       ((uint8)(0x01u))

/* For Program */
#define CCP_PROGRAM_MAX_LENGTH  ((uint8)(0x05u))
#define CCP_PROGRAM6_LENGTH     ((uint8)(0x06u))

/* For Seed&Key */
#define CCP_NEED_UNLOCK         ((uint8)(0x01u))
#define CCP_NO_UNLOCK           ((uint8)(0x00u))
#define CCP_RIGHT_KEY           ((uint8)(0x00u))
#define CCP_WRONG_KEY           ((uint8)(0x01u))

/* For Checksum */
#define CCP_CHECKSUM_RESULT_BYTES   ((uint8)(0x02u))
#define CCP_CRC_TABLE_LENGTH        (256u)

/* Other Definition */
#define CCP_COMMAND_CODE        (CCP_RX_Data[CCP_Index_0])
#define CCP_CTR                 (CCP_RX_Data[CCP_Index_1])

#define CCP_CAN_LENGTH          ((uint8)(8u))
#define CCP_MAX_ELEMENT         ((uint8)(7u))
#define CCP_READ_BUF_LENGTH     ((uint8)(5u))

#define CCP_DEFAULT_ZERO        ((uint8)(0x00u))
#define CCP_DEFAULT_ZERO_16     ((uint16)(0x00u))
#define CCP_DEFAULT_ZERO_32     (0uL) 
#define CCP_ALL_BITS_SET_8      ((uint8)0xFFu)
#define CCP_ALL_BITS_SET_16     ((uint16)0xFFFFu)

#define CCP_NO_PRESCALER        ((uint16)(1u))
#define CCP_DAQ_CHANNEL_DEFAULT ((uint8)(0xFFu))

/* Index Order Definition */
#define CCP_Index_0             (0u)    
#define CCP_Index_1             (1u)
#define CCP_Index_2             (2u)
#define CCP_Index_3             (3u)
#define CCP_Index_4             (4u)
#define CCP_Index_5             (5u)
#define CCP_Index_6             (6u)
#define CCP_Index_7             (7u)

/* Offset Definition */
#define CCP_OffSet_0            ((uint8)(0u))
#define CCP_OffSet_1            ((uint8)(1u))
#define CCP_OffSet_2            ((uint8)(2u))
#define CCP_OffSet_3            ((uint8)(3u))
#define CCP_OffSet_4            ((uint8)(4u))
#define CCP_OffSet_5            ((uint8)(5u))
#define CCP_OffSet_6            ((uint8)(6u))
#define CCP_OffSet_7            ((uint8)(7u))

/* Normal Value Definition */
#define CCP_UINT8_0             ((uint8)(0u))
#define CCP_UINT8_1             ((uint8)(1u))
#define CCP_UINT8_2             ((uint8)(2u))
#define CCP_UINT8_3             ((uint8)(3u))
#define CCP_UINT8_4             ((uint8)(4u))
#define CCP_UINT8_5             ((uint8)(5u))
#define CCP_UINT8_6             ((uint8)(6u))
#define CCP_UINT8_7             ((uint8)(7u))

/* Definition for bit moving */
#define CCP_MOVE_BITS_1         (1u)
#define CCP_MOVE_BITS_8         (8u)
#define CCP_MOVE_BITS_16        (16u)
#define CCP_MOVE_BITS_24        (24u)

/* Pointer to Null Definitions */
#define CCP_NULL                (0u)


/*******************************************************************************
*   Macro Function
*******************************************************************************/
#ifdef CCP_MOTOROLA
#define CCP_Get_First_Byte(x)           ((uint8)((x)>> 8u))
#define CCP_Get_Second_Byte(x)          ((uint8)((x)&((uint16)0x00FFu)))
#define CCP_Make_16Bits(x,y)            ((uint16)((((uint16)(x))<<((uint16)8u))|((uint16)(y))))
#define CCP_Get_1Byte(x)                ((uint8)(((x)>>24u) & ((0x00FFuL))))
#define CCP_Get_2Byte(x)                ((uint8)(((x)>>16u) & (0x00FFuL)))
#define CCP_Get_3Byte(x)                ((uint8)(((x)>> 8u) & (0x00FFuL)))
#define CCP_Get_4Byte(x)                ((uint8)((x) & (0x00FFuL)))
#else
#define CCP_Get_First_Byte(x)           ((uint8)((x)&(0x00FFu)))
#define CCP_Get_Second_Byte(x)          ((uint8)((x)>>((uint16)8u)))
#define CCP_Make_16Bits(x,y)            ((uint16)((((uint16)(y))<<((uint16)8u))|((uint16)(x))))
#define CCP_Get_1Byte(x)                ((uint8)((x)& (0x00FFu)))
#define CCP_Get_2Byte(x)                ((uint8)(((x)>>8u)& (0x00FFu)))
#define CCP_Get_3Byte(x)                ((uint8)(((x)>>16u)& (0x00FFu)))
#define CCP_Get_4Byte(x)                ((uint8)((x)>>24u))
#endif 

#ifdef CCP_DAQ_REQUEST
/*******************************************************************************
*   Typedef 
*******************************************************************************/ 
typedef struct 
{
  uint8 flag;                      /* DAQ Request flag */
  uint8 channel;                   /* event channel of DAQ */
  uint8 prepare;                   /* Prepare Flag */ 
}CCP_DAQ_strcut;
#endif

/*******************************************************************************
*   Global Variable Declaration 
*******************************************************************************/
#define CCP_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

#ifdef CCP_16BITS_ADDRESS_MODE
extern VAR(uint8,CCP_PUBLIC_CODE) CCP_PGM5_Flag;
#endif 

#define CCP_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/*******************************************************************************
*   Global Function Declaration 
*******************************************************************************/
#define CCP_START_SEC_PUBLIC_CODE
#include "MemMap.h"

extern FUNC(void, CCP_PUBLIC_CODE) Ccp_RxIndication
(
    PduIdType RxPudId,
    P2CONST(PduInfoType,AUTOMATIC,CAN_APPL_DATA) PduInfoPtr
);

extern FUNC(void, CCP_PUBLIC_CODE) Ccp_Init
(
    void
);

extern FUNC(void, CCP_PUBLIC_CODE) Ccp_Event_DAQ_Transmit
(
    const uint8 event_channel
);

extern FUNC(void, CCP_PUBLIC_CODE) Ccp_TxConfirmation
(
    PduIdType PduId
);

extern FUNC(void, CCP_PUBLIC_CODE) Ccp_Checksum_Background
( 
    void 
);

extern Std_ReturnType App_Ccp_FlashErase
(
    uint32 Flash_Erase_Ptr,
    uint32 size
);

extern Std_ReturnType App_Ccp_FlashWrite
(
    uint8* data, 
    uint32 Flash_Write_Ptr,
    uint32 size
);

extern Std_ReturnType App_Ccp_FlashRead
(
    uint8* data, 
    uint32 Flash_Read_Ptr, 
    uint32 size
);

#ifdef CCP_CAL_PAGE 
extern Std_ReturnType App_Ccp_Select_Page_Indication
(
    uint8 page_index
);
#endif

#ifdef CCP_STORE_CAL_DATA
extern void App_Ccp_Store_Cal_Data
(
    void
);
#endif

extern uint32 App_Ccp_GetSeed
(
    uint8 Resource_Mask
);

extern uint8 App_Ccp_Unlock    
(
    uint32 key
);

#define CCP_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

#endif


