/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Ccp.c
********************************************************************************
*   Project/Product : CCP module
*   Title           : Ccp.c
*   Author          : Peng.Liu
********************************************************************************
*   Description     : This file includes the core codes of CCP module.                    
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
*   02.00.00    24/12/12      Peng.Liu      N/A          HrAsrCcp121224-01
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

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "fm33lg0xx_fl.h"
#include "Ccp.h"

/*******************************************************************************
*   Static Function Declaration 
*******************************************************************************/
#define CCP_START_SEC_PRIVATE_CODE
#include "MemMap.h"

STATIC FUNC(uint32, CCP_PRIVATE_CODE) Ccp_Make_32Bits
(
    const uint8 data0,
    const uint8 data1,
    const uint8 data2,
    const uint8 data3
);

STATIC FUNC(Std_ReturnType,CCP_PRIVATE_CODE) Ccp_CommFunction
(
    void
);

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_DTO_Transmit
(
    void
);

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Test_Connect
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_DisConnect
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Exchange_ID
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Set_MTA
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Upload
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_Version
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Download
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Move
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_DAQ_Size
(
    void
); 

#ifdef CCP_DAQ_REQUEST 
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Set_DAQ_Ptr
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Write_DAQ
(
    void
);
#endif

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Set_S_Status
(
    void
);

#ifdef CCP_DAQ_REQUEST 
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_S_Status
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Start_Stop
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Start_Stop_All
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Move_Buffer_Ptr
(
    CONSTP2VAR(uint8,AUTOMATIC,CCP_PRIVATE_DATA) Ptr
);

STATIC FUNC(Std_ReturnType, CCP_PRIVATE_CODE) Ccp_Check_FIFO
(
    void
); 

STATIC FUNC(void, CCP_PRIVATE_CODE) Ccp_DAQ_Copy_Data
(
    const uint8 DAQ_Count,
    const uint8 ODT_Count
);

#endif

#ifdef CCP_FLASH
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Clear_Memory
(
    void
); 
    
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Program
(
    void
); 
#endif

#ifdef CCP_CHECKSUM
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Build_Checksum
(
    void
);
#endif

#ifdef CCP_SEED_KEY
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_Seed
(
    void
); 

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Unlock
(
    void
); 
#endif

#ifdef CCP_CAL_PAGE
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Set_Cal_Page
(
    void
);


STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_Cal_Page
(
    void
); 
#endif

#ifdef CCP_DAQ_REQUEST 
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Transmit_DAQ
(
    const uint8 event_channel
); 
#endif


#define CCP_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

/*******************************************************************************
*   Local Data Define 
*******************************************************************************/
#define CCP_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_RX_Data[CCP_CAN_LENGTH];
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_TX_Data[CCP_CAN_LENGTH];

STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_Status;
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_SessionStatus;

STATIC VAR(uint32,CCP_PRIVATE_DATA) CCP_MTA[CCP_MTA_NUM];
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_Extension_Address[CCP_MTA_NUM];

#ifdef CCP_CAL_PAGE
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_Active_Page;
#endif 

#ifdef CCP_DAQ_REQUEST
STATIC VAR(CCP_DAQ_strcut,CCP_PRIVATE_DATA) CCP_DAQ_Flag[CCP_MAX_DAQ];
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_DAQ_ODT_Used[CCP_MAX_DAQ][CCP_MAX_ODT];
STATIC VAR(uint16,CCP_PRIVATE_DATA) CCP_DAQ_Prescaler[CCP_MAX_DAQ];
STATIC VAR(uint16,CCP_PRIVATE_DATA) CCP_DAQ_Prescaler_Count[CCP_MAX_DAQ];
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_DAQ_PDU_Data_Buffer[CCP_ODT_BUF_NUM][CCP_CAN_LENGTH];
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_DAQ_PDU_Data_Buffer_Empty;

STATIC VAR(uint8,CCP_PRIVATE_DATA)  Ccp_DAQ_Buffer_First_Ptr;
STATIC VAR(uint8,CCP_PRIVATE_DATA)  Ccp_DAQ_Buffer_Last_Ptr;


/* Declare CCP_ODT */
STATIC VAR(uint32,CCP_PRIVATE_DATA)  CCP_ODT[CCP_MAX_DAQ][CCP_MAX_ODT][CCP_MAX_ELEMENT];  

/* The number of CCP_ODT in the DAQ */
STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_DAQ_List_Ptr;
/* The element of CCP_ODT arrary */
STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_DAQ_ODT_Ptr;
/* Variale Index */
STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_DAQ_Element_Ptr;
#endif   

STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_Tx_Busy;
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_DTO_PDU_Data_Buffer[CCP_CAN_LENGTH];
STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_DTO_PDU_Data_Buffer_Empty;

#ifdef CCP_CHECKSUM
STATIC VAR(uint32,CCP_PRIVATE_DATA) CCP_CheckSumSize;
#endif         

/*Seed&Key Related*/
#ifdef CCP_SEED_KEY
/* Resource Protection Status */
STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_ProtectionStatus; 
STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_ResourceMask;
#endif

#ifdef CCP_16BITS_ADDRESS_MODE
STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_Odd_Even; 
#ifdef CCP_FLASH
STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_Flash_Read_Buffer[CCP_READ_BUF_LENGTH]; 
#endif
#endif 

#ifdef CCP_CHECKSUM
STATIC VAR(uint8,CCP_PRIVATE_DATA) CCP_Checksum_Flag;
#endif

/*******************************************************************************
*   Global Data Define 
*******************************************************************************/
#ifdef CCP_16BITS_ADDRESS_MODE
VAR(uint8,CCP_PRIVATE_DATA) CCP_PGM5_Flag;
#endif 

#define CCP_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/*******************************************************************************
*   Codes Start 
*******************************************************************************/

#define CCP_START_SEC_PUBLIC_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Init 
*                
* Description:      This functin initializes internal and external interfaces of 
*                   the CCP for the further processing. 
*                             
* Inputs:           None
*                   
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

FUNC(void, CCP_PUBLIC_CODE) Ccp_Init
(
    void
) 
{
    uint8 count;
    uint8 sub_count;
    
    #ifdef CCP_DAQ_REQUEST   
    uint8 DAQ_Element_Cnt;
    #endif

    /* Init DAQ related Data */
    #ifdef CCP_DAQ_REQUEST   
    for(count = CCP_DEFAULT_ZERO; count < CCP_MAX_DAQ; count++) 
    { 
        for(sub_count = CCP_DEFAULT_ZERO; sub_count < CCP_MAX_ODT; sub_count++)
        {
            for(DAQ_Element_Cnt = CCP_DEFAULT_ZERO; DAQ_Element_Cnt < CCP_MAX_ELEMENT; DAQ_Element_Cnt++)
            {
                CCP_ODT[count][sub_count][DAQ_Element_Cnt] = CCP_DEFAULT_ZERO_32;
            }
            CCP_DAQ_ODT_Used[count][sub_count] = ((uint8)STD_OFF);
        }
        CCP_DAQ_Prescaler[count] = CCP_NO_PRESCALER;
        CCP_DAQ_Prescaler_Count[count] = CCP_DEFAULT_ZERO_16;
        CCP_DAQ_Flag[count].flag = ((uint8)STD_OFF);      
        CCP_DAQ_Flag[count].channel = CCP_DAQ_CHANNEL_DEFAULT;
        CCP_DAQ_Flag[count].prepare = ((uint8)STD_OFF);
    }
    
    /*Init PDU Data Buffer of DAQ*/
    for(count = CCP_DEFAULT_ZERO; count < CCP_ODT_BUF_NUM; count++)
    {
        for(sub_count = CCP_DEFAULT_ZERO; sub_count < CCP_CAN_LENGTH; sub_count++)
        {
            CCP_DAQ_PDU_Data_Buffer[count][sub_count] = CCP_DEFAULT_ZERO;
        }
    }
    CCP_DAQ_PDU_Data_Buffer_Empty = ((uint8)STD_ON);
    Ccp_DAQ_Buffer_First_Ptr = CCP_DEFAULT_ZERO;
    Ccp_DAQ_Buffer_Last_Ptr = CCP_DEFAULT_ZERO;
    #endif /* End of ifdef CCP_DAQ_REQUEST */

    /* Init CCP_Status */
    CCP_Status = CCP_DISCONNECTED;
    CCP_SessionStatus = CCP_DEFAULT_SESSION;

    /* Init Ccp Tx status */
    CCP_Tx_Busy = ((uint8)STD_OFF);
    
    /* Init PDU data buffer of DTO */
    for(sub_count = CCP_DEFAULT_ZERO; sub_count < CCP_CAN_LENGTH; sub_count++)
    {
        CCP_DTO_PDU_Data_Buffer[sub_count] = CCP_DEFAULT_ZERO;
    }
    CCP_DTO_PDU_Data_Buffer_Empty = ((uint8)STD_ON);

    #ifdef CCP_SEED_KEY
    CCP_ProtectionStatus = CCP_RM_ALL_LOCKED; 
    CCP_ResourceMask = CCP_DEFAULT_ZERO;
    #endif


    /* Init CAL_PAGE related data */
    #ifdef CCP_CAL_PAGE
    CCP_Active_Page = CCP_INIT_PAGE_INDEX;
    #endif

    /* Init data for special chips */
    #ifdef CCP_16BITS_ADDRESS_MODE     
    CCP_Odd_Even = ((uint8)STD_OFF);
    CCP_PGM5_Flag = ((uint8)STD_OFF);
    #endif

    /* Init checksum related data */
    #ifdef CCP_CHECKSUM
    CCP_CheckSumSize = CCP_DEFAULT_ZERO_32;
    CCP_Checksum_Flag = ((uint8)STD_OFF);
    #endif 

    /* Init DAQ_PTR */
    #ifdef CCP_DAQ_REQUEST
    /*The number of CCP_ODT in the DAQ*/
    CCP_DAQ_List_Ptr = CCP_DEFAULT_ZERO;
    /*The element of CCP_ODT arrary*/
    CCP_DAQ_ODT_Ptr = CCP_DEFAULT_ZERO;
    /*Variale Index*/
    CCP_DAQ_Element_Ptr = CCP_DEFAULT_ZERO;
    #endif
    
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_RxIndication 
*                
* Description:      This function indicates a successful reception of a 
*                   received CANIf Rx L-PDU to the CCP after passing all 
*                   filters and validation checks. 
*                             
* Inputs:           PduIdType RxPudId: 
*                           Rx Handle of CanIf module
*                   const PduInfoType* PduInfoPtr:
*                           Pointer to received SDU
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

FUNC(void, CCP_PUBLIC_CODE) Ccp_RxIndication
(
    PduIdType RxPudId,
    P2CONST(PduInfoType,AUTOMATIC,CAN_APPL_DATA) PduInfoPtr
)
{
    uint8 count;
    
    /*judge CanIf channel*/    
    if((RxPudId == CCP_RX_UL_HANDLE) && (PduInfoPtr != CCP_NULL))
    {
        /*judge DLC, if not 8, do nothing*/
        if(PduInfoPtr->SduLength == ((uint16)CCP_CAN_LENGTH))
        {
            for (count = CCP_DEFAULT_ZERO; count < CCP_CAN_LENGTH; count++)
            {  
                CCP_RX_Data[count] = PduInfoPtr->SduDataPtr[count] ; 
            }   
            (void)Ccp_CommFunction();
        }
        else
        {
            /*do nothing*/
        }
    }
}

#define CCP_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"

#define CCP_START_SEC_PRIVATE_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Make_32Bits 
*                
* Description:      This function is used to make 4*8bits data to 32bits data.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(uint32, CCP_PRIVATE_CODE) Ccp_Make_32Bits
(
    const uint8 data0,
    const uint8 data1,
    const uint8 data2,
    const uint8 data3
) 
{
    #ifdef CCP_MOTOROLA
    /*Get CCP_MTA Address from Matser Device for MSB Format*/  
    const uint32 CCP_Result = ((((uint32)data0) << CCP_MOVE_BITS_24) | (((uint32)data1) << CCP_MOVE_BITS_16) | (((uint32)data2) << CCP_MOVE_BITS_8) | ((uint32)data3)); 
    #else
    /*Get CCP_MTA Address from Matser Device for LSB Format*/
    const uint32 CCP_Result = ((((uint32)data3) << CCP_MOVE_BITS_24) | (((uint32)data2) << CCP_MOVE_BITS_16) | (((uint32)data1) << CCP_MOVE_BITS_8) | ((uint32)data0)); 
    #endif  /*End of ifdef MSB*/

    return CCP_Result;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Test_Connect 
*                
* Description:      This function is used to process Test and Connect command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Test_Connect
(
    void
) 
{
    uint16 ADDR_16;
    
    /* Get Station Address */
    ADDR_16 = ((uint16)(CCP_RX_Data[CCP_Index_3]) << (uint16)CCP_MOVE_BITS_8);   
    ADDR_16 |= CCP_RX_Data[CCP_Index_2] ;
    
    /* Check Station Address */
    if ((ADDR_16 == CCP_NODE_ID) || (ADDR_16 == CCP_BROADCAST_ID)) 
    {  
        if (CCP_COMMAND_CODE == CCP_CONNECT)  
        {
            CCP_Status = CCP_CONNECTED;        
        }
        else
        {
            /* Do nothing */
        }
        Ccp_DTO_Transmit();          
    } 
    else
    {
        /* If connected, temporary disconnect */
        if (CCP_Status == CCP_CONNECTED) 
        {
            CCP_Status = CCP_TMP_DISCONNECTED;
        }
        else
        {
            /* Do nothing */
        }
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_DisConnect 
*                
* Description:      This function is used to process DisConnect command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_DisConnect
(
    void
) 
{
    uint16 ADDR_16;
    #ifdef CCP_DAQ_REQUEST
    uint8  count;
    #endif

    /* Get Station Address */
    ADDR_16 = (((uint16)CCP_RX_Data[CCP_Index_5]) << ((uint16)CCP_MOVE_BITS_8));   
    ADDR_16 |= CCP_RX_Data[CCP_Index_4] ;
    
    /* This station */
    if ((ADDR_16 == CCP_NODE_ID) || (ADDR_16 == CCP_BROADCAST_ID)) 
    { 
        if((CCP_RX_Data[CCP_Index_2] == CCP_TEM_END_SESSION)
            || (CCP_RX_Data[CCP_Index_2]== CCP_END_SESSION)) 
        {
            /* 0x00 Temporary */
            if (CCP_RX_Data[CCP_Index_2] == CCP_TEM_END_SESSION) 
            { 
                CCP_Status = CCP_TMP_DISCONNECTED;
            }
            /* 0x01 End of session */
            else 
            {                
                /*No resume session */
                if (((CCP_SessionStatus & CCP_SESSION_RESUME)) == CCP_SESSION_RESUME) 
                {
                    #ifdef CCP_SEED_KEY
                    /* Clear Protection Status */
                    CCP_ProtectionStatus = CCP_RM_ALL_LOCKED;
                    #endif
                    
                    #ifdef CCP_DAQ_REQUEST
                    /* Stop DAQ */
                    for (count = CCP_DEFAULT_ZERO; count < CCP_MAX_DAQ; count++) 
                    {
                         /*Stop all DAQ */
                        CCP_DAQ_Flag[count].flag = ((uint8)STD_OFF); 
                        CCP_DAQ_Flag[count].prepare = ((uint8)STD_OFF);
                    }
                    #endif                    
                }
                else
                {                    
                    #ifdef CCP_SEED_KEY
                    /*The protections status for DAQ remains*/    
                    CCP_ProtectionStatus = CCP_RM_DAQ; 
                    #endif
                }

                /* Store CAL DATA */
                #ifdef CCP_STORE_CAL_DATA
                if ((CCP_SessionStatus & CCP_SESSION_STORE) == CCP_SESSION_STORE) 
                {
                    /*API for storing the Cal data to Flash */
                    App_Ccp_Store_Cal_Data();  
                }
                else
                {
                    /* Do nothing */
                }
                #endif

                /* Change Status */
                CCP_Status = CCP_DISCONNECTED;    
                CCP_SessionStatus = CCP_DEFAULT_SESSION;     
            }
        }
        /*if CCP_RX_Data[2] does not equal to 0 or 1, error*/
        else
        {
            CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
        }
        /*Send DTO for disconnect command*/
        Ccp_DTO_Transmit();
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Exchange_ID 
*                
* Description:      This function is used to process Exchange_ID command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Exchange_ID
(
    void
) 
{
    /* Lenght of slave device identifier */
    CCP_TX_Data[CCP_Index_3] = CCP_ECU_NAME_LENGTH; 
    /*data type qualifier of slave device(ASCI)*/
    CCP_TX_Data[CCP_Index_4] = CCP_ASCII_TYPE;  
    /* Build the Resource Availability and Protection Mask */
    /* Default: Calibration available */
    CCP_TX_Data[CCP_Index_5] = CCP_RM_CAL;
    /* Default: No Protection */
    CCP_TX_Data[CCP_Index_6] = CCP_DEFAULT_ZERO;      
    
    #ifdef CCP_SEED_KEY
    /* Protected Calibration*/
    CCP_TX_Data[CCP_Index_6] |= CCP_RM_CAL;    
    #endif
    
    #ifdef CCP_DAQ_REQUEST
    /* Data Acquisition */
    CCP_TX_Data[CCP_Index_5] |= CCP_RM_DAQ;     
    #ifdef CCP_SEED_KEY
    /* Protected Data Acquisition */
    CCP_TX_Data[CCP_Index_6] |= CCP_RM_DAQ;   
    #endif
    #endif  /*End of ifdef CCP_DAQ_REQUEST*/
    
    
    #ifdef CCP_FLASH
    /* Flash Programming */
    CCP_TX_Data[CCP_Index_5] |= CCP_RM_PGM;     
    #ifdef CCP_SEED_KEY
    /* Protected Flash Programming*/
    CCP_TX_Data[CCP_Index_6] |= CCP_RM_PGM;    
    #endif
    #endif  /*End of ifdef CCP_FLASH*/
    
    #ifdef CCP_DAQ_REQUEST
    /* Data Acquisition */
    CCP_TX_Data[CCP_Index_5] |= CCP_RM_DAQ;     
    #ifdef CCP_SEED_KEY
    /* Protected Data Acquisition */
    CCP_TX_Data[CCP_Index_6] |= CCP_RM_DAQ;   
    #endif
    #endif  /*End of ifdef CCP_DAQ_REQUEST*/
    
    CCP_MTA[CCP_Index_0] = (uint32)Ccp_Station_ID;

}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Set_MTA
*                
* Description:      This function is used to process Set_MTA command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Set_MTA
(
    void
) 
{
    uint32      MTA_ptr;
    const uint8 MTA_Index = CCP_RX_Data[CCP_Index_2];

    MTA_ptr = Ccp_Make_32Bits(CCP_RX_Data[CCP_Index_4], CCP_RX_Data[CCP_Index_5], CCP_RX_Data[CCP_Index_6], CCP_RX_Data[CCP_Index_7]);
        
    /*Memory transfer address CCP_MTA number (0,1)*/
    if(MTA_Index < CCP_MTA_NUM)
    {
        CCP_MTA[MTA_Index] = MTA_ptr; 
        CCP_Extension_Address[MTA_Index] = CCP_RX_Data[CCP_Index_3];     
    }
    else
    {
        CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;    
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Upload
*                
* Description:      This function is used to process Upload & Shour_up command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Upload
(
    void
) 
{
    uint8  count; 
    uint32 MTA_Temp;
        
    #ifdef CCP_FLASH
    uint8  Return_Code;   
    #ifdef CCP_16BITS_ADDRESS_MODE
    #else 
    /*Upload Data from FLASH/EEPROM*/
    uint8  CCP_Flash_Read_Buffer[CCP_READ_BUF_LENGTH]; 
    #endif
    #endif

    #ifdef CCP_16BITS_ADDRESS_MODE
    uint8  Temp_Data_Count;
    #endif

    /* check the request length  */
    if((CCP_RX_Data[CCP_Index_2]) > CCP_UPLOAD_MAX)
    {
        CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
    }
    else
    {
        if (CCP_COMMAND_CODE == CCP_UPLOAD)
        {
            MTA_Temp = CCP_MTA[CCP_Index_0];
        }
        else
        {
            MTA_Temp = Ccp_Make_32Bits(CCP_RX_Data[CCP_Index_4], CCP_RX_Data[CCP_Index_5], CCP_RX_Data[CCP_Index_6], CCP_RX_Data[CCP_Index_7]);
        }
        
        #ifdef CCP_FLASH
        /*The range of variable Flash Adress*/
        if((MTA_Temp >= CCP_FLASH_MIN_ADDR) && (MTA_Temp < CCP_FLASH_MAX_ADDR)) 
        {          
            /*Call Read Flash function*/
            #ifdef CCP_16BITS_ADDRESS_MODE
            Return_Code = App_Ccp_FlashRead(CCP_Flash_Read_Buffer, CCP_Global_Address(MTA_Temp), (CCP_RX_Data[CCP_Index_2] >> CCP_MOVE_BITS_1));  
            #else
            Return_Code = App_Ccp_FlashRead(CCP_Flash_Read_Buffer, CCP_Global_Address(MTA_Temp), CCP_RX_Data[CCP_Index_2]);
            #endif
            
            if(Return_Code == ((uint8)E_NOT_OK)) 
            {
                CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
            }
            else
            {
                /*Data be copied  into the corresponding DTO data field. */
                #ifdef CCP_16BITS_ADDRESS_MODE
                if(CCP_COMMAND_CODE == CCP_UPLOAD)
                {
                    /* If Upload, needn't change the CCP_Odd_Even */
                    /* Auto change MTA */
                    CCP_MTA[CCP_Index_0] += (CCP_RX_Data[CCP_Index_2] >> CCP_MOVE_BITS_1);      
                }
                else
                {                
                    /* Short_Up can't change MTA value */
                    /* If short_up, reset CCP_Odd_Even */
                    CCP_Odd_Even = ((uint8)STD_OFF);
                }
                
                for(count = CCP_DEFAULT_ZERO; count < CCP_RX_Data[CCP_Index_2]; count++) 
                { 
                    if(CCP_Odd_Even == ((uint8)STD_OFF))
                    {
                        CCP_TX_Data[count + CCP_OffSet_3] = 
                            CCP_Get_First_Byte(CCP_Flash_Read_Buffer[Temp_Data_Count]);
                        CCP_Odd_Even = ((uint8)STD_ON);
                    }
                    else
                    {
                        CCP_TX_Data[count + CCP_OffSet_3] = 
                            CCP_Get_Second_Byte(CCP_Flash_Read_Buffer[Temp_Data_Count]);
                        CCP_Odd_Even = ((uint8)STD_OFF);
                        Temp_Data_Count++;
                    }    
                }
                #else
                for(count = CCP_DEFAULT_ZERO; count < CCP_RX_Data[CCP_Index_2]; count++) 
                { 
                    CCP_TX_Data[count + CCP_OffSet_3] = CCP_Flash_Read_Buffer[count];
                }
                /* Auto change MTA */
                CCP_MTA[CCP_Index_0] += CCP_RX_Data[CCP_Index_2];
                #endif  /*End of ifdef CCP_16BITS_ADDRESS_MODE*/
            }
        } 
        else
        #endif  /*End of ifdef CCP_FLASH*/
        
        /*Upload Data from RAM*/
        { 
            #ifdef CCP_16BITS_ADDRESS_MODE      
            for(count = CCP_DEFAULT_ZERO; count < CCP_RX_Data[CCP_Index_2]; count++) 
            { 
                if(MTA_Temp != CCP_DEFAULT_ZERO_32)
                {
                    if(CCP_Odd_Even == ((uint8)STD_OFF))
                    {
                        CCP_TX_Data[count + CCP_OffSet_3] = CCP_Get_First_Byte(*((uint8 CCP_FAR)CCP_Global_Address(MTA_Temp)));
                        CCP_Odd_Even = ((uint8)STD_ON);
                    }
                    else
                    {
                        CCP_TX_Data[count + CCP_OffSet_3] =  CCP_Get_Second_Byte(*((uint8 CCP_FAR)CCP_Global_Address(MTA_Temp)));
                        CCP_Odd_Even = ((uint8)STD_OFF);
                        MTA_Temp++;
                        if(CCP_COMMAND_CODE == CCP_UPLOAD)
                        {
                            /* Auto change MTA */
                            CCP_MTA[CCP_Index_0]++;
                        }
                        else
                        {
                            /* Short_Up can't change MTA value, so do nothing */
                        }
                    }                        
                }
                else
                {
                    /* MTA Point to NULL, so do nothing */
                }
            }
            #else
            for(count = CCP_DEFAULT_ZERO; count < CCP_RX_Data[CCP_Index_2]; count++) 
            { 
                if(MTA_Temp != CCP_DEFAULT_ZERO_32)
                {
                    CCP_TX_Data[count + CCP_OffSet_3] = *((uint8 CCP_FAR)CCP_Global_Address(MTA_Temp));
                    MTA_Temp++;
                    if(CCP_COMMAND_CODE == CCP_UPLOAD)
                    {
                        /* Auto change MTA */
                        CCP_MTA[CCP_Index_0]++;
                    }
                    else
                    {
                        /* Short_Up can't change MTA value, so do nothing */
                    }
                }
                else
                {
                    /* MTA Point to NULL, so do nothing */
                }
            }
            #endif  /*End of ifdef CCP_16BITS_ADDRESS_MODE*/
        }
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Get_Version
*                
* Description:      This function is used to process Get CCP Version command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_Version
(
    void
) 
{
    CCP_TX_Data[CCP_Index_3] = CCP_VERSION_MAJOR;
    CCP_TX_Data[CCP_Index_4] = CCP_VERSION_MINOR;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Download
*                
* Description:      This function is used to process Download & Download_6 
*                   command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Download
(
    void
) 
{
    uint8 count;
    uint8 Bytes_Number;
    uint8 Error = (uint8)STD_OFF;
    
    #ifdef CCP_SEED_KEY
    if (((uint8)(CCP_ProtectionStatus & CCP_RM_CAL)) == CCP_DEFAULT_ZERO) 
    {
        CCP_TX_Data[CCP_Index_1] = CCP_ACCESS_DENIED;
    }
    else
    #endif 
    { 
        /* Get Download Bytes */
        if(CCP_COMMAND_CODE == CCP_DNLOAD6)
        {   
            Bytes_Number = CCP_UINT8_6;
        }   
        else
        {
            Bytes_Number = CCP_RX_Data[CCP_Index_2];
            if(Bytes_Number > CCP_UINT8_5)
            {
                Error = (uint8)STD_ON;
            }
            else
            {
                /* do nothing */
            }
        }

        if(Error == (uint8)STD_ON)
        {
            CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
        }
        else
        {
            /*The download specified length data */
            #ifdef CCP_16BITS_ADDRESS_MODE
            for(count = CCP_DEFAULT_ZERO; count < Bytes_Number; (count += CCP_UINT8_2))     
            {
                if(CCP_COMMAND_CODE == CCP_DNLOAD6)
                {   
                    *(uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_0]) = 
                        CCP_Make_16Bits(CCP_RX_Data[count + CCP_OffSet_2], CCP_RX_Data[count + CCP_OffSet_3]);
                }   
                else
                {
                    *(uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_0]) = 
                        CCP_Make_16Bits(CCP_RX_Data[count + CCP_OffSet_3], CCP_RX_Data[count + CCP_OffSet_4]);
                }            
                CCP_MTA[CCP_Index_0]++;
            } 
            #else                    
            for(count = CCP_DEFAULT_ZERO; count < Bytes_Number; count++) 
            {
                if(CCP_COMMAND_CODE == CCP_DNLOAD6)
                {   
                    *(uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_0]) = CCP_RX_Data[count + CCP_OffSet_2] ;
                }   
                else
                {
                    *(uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_0]) = CCP_RX_Data[count + CCP_OffSet_3] ;
                }            
                CCP_MTA[CCP_Index_0]++;
            }
            #endif  /*End of ifdef CCP_16BITS_ADDRESS_MODE*/                    
            
            /* Set Extension Address */
            CCP_TX_Data[CCP_Index_3] = CCP_Extension_Address[CCP_Index_0];
            
            /* Get MTA Value */
            CCP_TX_Data[CCP_Index_4] = CCP_Get_1Byte(CCP_MTA[CCP_Index_0]);
            CCP_TX_Data[CCP_Index_5] = CCP_Get_2Byte(CCP_MTA[CCP_Index_0]);
            CCP_TX_Data[CCP_Index_6] = CCP_Get_3Byte(CCP_MTA[CCP_Index_0]);
            CCP_TX_Data[CCP_Index_7] = CCP_Get_4Byte(CCP_MTA[CCP_Index_0]);
        }        
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Move
*                
* Description:      This function is used to process Move command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Move
(
    void
) 
{
    uint8  count;
    uint32 move_length;
   
    move_length = Ccp_Make_32Bits(CCP_RX_Data[CCP_Index_2], CCP_RX_Data[CCP_Index_3], CCP_RX_Data[CCP_Index_4], CCP_RX_Data[CCP_Index_5]);
    
    for(count = CCP_DEFAULT_ZERO; count < move_length; count++) 
    {
        *(uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_1]) = (*(uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_0]));
        CCP_MTA[CCP_Index_1]++;
        CCP_MTA[CCP_Index_0]++;
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Get_DAQ_Size
*                
* Description:      This function is used to process Get_DAQ_Size command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_DAQ_Size        
(
    void
) 
{
    #ifdef CCP_DAQ_REQUEST
    uint8 count;
    uint8 sub_count;

    if (CCP_RX_Data[CCP_Index_2] < CCP_MAX_DAQ) 
    {
        CCP_SessionStatus &= ((CCP_SESSION_RUN) ^ CCP_ALL_BITS_SET_8);      
        CCP_TX_Data[CCP_Index_3] = CCP_MAX_ODT ;
        CCP_TX_Data[CCP_Index_4] = (uint8)(CCP_RX_Data[CCP_Index_2] * CCP_MAX_ODT);
        
        /*Reset specified DAQ list*/
        for (count = CCP_DEFAULT_ZERO; count < CCP_MAX_ODT; count++)  
        {
            for(sub_count = CCP_DEFAULT_ZERO; sub_count < CCP_MAX_ELEMENT; sub_count++) 
            {
                CCP_ODT[CCP_RX_Data[CCP_Index_2]][count][sub_count] = CCP_DEFAULT_ZERO_32;
            }
        }   
    } 
    else 
    {
        CCP_TX_Data[CCP_Index_3] = CCP_DEFAULT_ZERO;     
        CCP_TX_Data[CCP_Index_4] = CCP_NO_FIRST_PID;
    }    
    #else
    CCP_TX_Data[CCP_Index_3] = CCP_DEFAULT_ZERO;     
    CCP_TX_Data[CCP_Index_4] = CCP_NO_FIRST_PID;
    #endif  /*End of CCP_DAQ_REQUEST*/
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Set_DAQ_Ptr
*                
* Description:      This function is used to process Set_DAQ_Ptr command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

#ifdef CCP_DAQ_REQUEST 
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Set_DAQ_Ptr
(
    void
) 
{
    /*Error Codes*/
    if((CCP_RX_Data[CCP_Index_2] >= CCP_MAX_DAQ) 
        || (CCP_RX_Data[CCP_Index_3] >= CCP_MAX_ODT) 
        || (CCP_RX_Data[CCP_Index_4] >= CCP_MAX_ELEMENT)) 
    {
        CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
    } 
    else
    {   
        CCP_DAQ_List_Ptr = CCP_RX_Data[CCP_Index_2];
        CCP_DAQ_ODT_Ptr = CCP_RX_Data[CCP_Index_3];
        CCP_DAQ_Element_Ptr = CCP_RX_Data[CCP_Index_4];
    }
}
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Write_DAQ
*                
* Description:      This function is used to process Write_DAQ command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

#ifdef CCP_DAQ_REQUEST
STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Write_DAQ
(
    void
) 
{
    uint8        count;
    const uint8  size = CCP_RX_Data[CCP_Index_2];
    uint32       write_daq_ptr;
        
    if((size != CCP_UINT8_1) && (size != CCP_UINT8_2) && (size != CCP_UINT8_4)) 
    {
        CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
    }    
    #ifdef CCP_16BITS_ADDRESS_MODE
    else if((CCP_DAQ_Element_Ptr + (size >> CCP_UINT8_1)) > CCP_UINT8_3)
    #else
    else if((CCP_DAQ_Element_Ptr + size) > CCP_UINT8_7)
    #endif
    {
        CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
    }        
    else
    {
        /* Wirte Address of DAQ element */
        write_daq_ptr = Ccp_Make_32Bits(CCP_RX_Data[CCP_Index_4], CCP_RX_Data[CCP_Index_5], CCP_RX_Data[CCP_Index_6], CCP_RX_Data[CCP_Index_7]);
              
        #ifdef CCP_16BITS_ADDRESS_MODE
        for(count = CCP_DEFAULT_ZERO; count < (size >> CCP_UINT8_1); count++) 
        #else
        for(count = CCP_DEFAULT_ZERO; count < size; count++) 
        #endif                    
        { 
            CCP_ODT[CCP_DAQ_List_Ptr][CCP_DAQ_ODT_Ptr][CCP_DAQ_Element_Ptr] = write_daq_ptr;
            CCP_DAQ_Element_Ptr++;
            write_daq_ptr++;
        }
    }
}
#endif  /*End of ifdef CCP_DAQ_REQUEST*/

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Set_S_Status
*                
* Description:      This function is used to process Set_S_Status command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Set_S_Status
(
    void
) 
{
    /* Set SessionStatus */
    CCP_SessionStatus = CCP_RX_Data[CCP_Index_2];    
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Get_S_Status
*                
* Description:      This function is used to process Get_S_Status command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_S_Status
(
    void
) 
{
    CCP_TX_Data[CCP_Index_3] = CCP_SessionStatus;
    /* No additional status */
    CCP_TX_Data[CCP_Index_4] = CCP_NO_ADDITIONAL_INFO; 
}

#ifdef CCP_DAQ_REQUEST
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Start_Stop
*                
* Description:      This function is used to process Start_Stop command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Start_Stop
(
    void
) 
{
    uint8       count;
    uint8       CCP_Run_Flag = CCP_DEFAULT_ZERO;
    uint16      temp;
    const uint8 DAQ_Index = CCP_RX_Data[CCP_Index_3];
    const uint8 ODT_Index = CCP_RX_Data[CCP_Index_4];
    
    #ifdef CCP_SEED_KEY
    if (((uint8)(CCP_ProtectionStatus & CCP_RM_DAQ)) == CCP_UINT8_0) 
    {
        /*error code*/
        CCP_TX_Data[CCP_Index_1] = CCP_ACCESS_DENIED;
    }
    else
    #endif 
    {
        if((DAQ_Index < CCP_MAX_DAQ) && (ODT_Index < CCP_MAX_ODT))
        {
            /*0x00 stops specified DAQ list*/
            if(CCP_RX_Data[CCP_Index_2] == CCP_STOP_DAQ) 
            {                     
                CCP_DAQ_Flag[DAQ_Index].flag = ((uint8)STD_OFF);
                CCP_DAQ_Flag[DAQ_Index].prepare = ((uint8)STD_OFF);

                /*Reset the CCP_ODT used flag*/
                for(count = CCP_DEFAULT_ZERO; count <= ODT_Index; count++)
                {
                    CCP_DAQ_ODT_Used[DAQ_Index][count] = ((uint8)STD_OFF);
                }
                
                /* Check if all the DAQ Lists are Stoped */
                for(count = CCP_DEFAULT_ZERO; count < CCP_MAX_DAQ; count++)
                {
                    CCP_Run_Flag += CCP_DAQ_Flag[count].flag;
                }
                /*Set CCP Status in NOT Run*/
                if(CCP_Run_Flag == CCP_DEFAULT_ZERO)
                {
                    CCP_SessionStatus &= ((CCP_RUN) ^ CCP_ALL_BITS_SET_8);
                }
                else
                {
                    /* Don't change status */
                }
            } 
            /* 0x01 starts specified DAQ list */
            /*0x02 prepares DAQ list for synchronised start*/
            else if((CCP_RX_Data[CCP_Index_2] == CCP_START_DAQ) 
                    || (CCP_RX_Data[CCP_Index_2] == CCP_PREPARE_DAQ))
            {                 
                /* Get DAQ prescaler value */
                temp = CCP_Make_16Bits(CCP_RX_Data[CCP_Index_6], CCP_RX_Data[CCP_Index_7]);
                
                if (temp == CCP_DEFAULT_ZERO_16)     
                {
                    /* Error Code */
                    CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
                } 
                else
                {
                    CCP_DAQ_Prescaler[DAQ_Index] = temp;

                    /* Set the event channel of this DAQ */
                    CCP_DAQ_Flag[DAQ_Index].channel = CCP_RX_Data[CCP_Index_5];
                    /* Set  CCP_ODT used flags */
                    for(count = CCP_DEFAULT_ZERO; count <= ODT_Index; count++)
                    {
                        CCP_DAQ_ODT_Used[DAQ_Index][count] = ((uint8)STD_ON);  
                    }
                    
                    if(CCP_RX_Data[CCP_Index_2] == CCP_START_DAQ)
                    {
                        /* Start specifiek DAQ */
                        CCP_DAQ_Flag[DAQ_Index].flag = ((uint8)STD_ON); 
                        /* Set SessionStatus */
                        CCP_SessionStatus |= (CCP_RUN);
                    }
                    else
                    {
                        /* Can't start DAQ, just prepare */
                        CCP_DAQ_Flag[DAQ_Index].prepare = ((uint8)STD_ON); 
                    }
                }
            } 
            else
            {
                /*Error Code*/
                CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
            }
        }
        else
        {
            /*Error Code*/
            CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
        }
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Start_Stop_All
*                
* Description:      This function is used to process Start_Stop_All command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Start_Stop_All
(
    void
) 
{
    uint8 count;
    uint8 sub_count;
    
    #ifdef CCP_SEED_KEY
    if (((uint8)(CCP_ProtectionStatus & CCP_RM_DAQ)) == CCP_UINT8_0) 
    {
        CCP_TX_Data[CCP_Index_1] = CCP_ACCESS_DENIED;
    }
    else
    #endif 
    { 
        /*0x00 stops transmission*/
        if(CCP_RX_Data[CCP_Index_2] == CCP_STOP_DAQ_ALL) 
        {
            for (count = CCP_DEFAULT_ZERO; count < CCP_MAX_DAQ; count++) 
            {
                /*Stop all DAQ */
                CCP_DAQ_Flag[count].flag = ((uint8)STD_OFF);     
            }
            /*Reset the CCP_ODT used flag*/
            for(count = CCP_DEFAULT_ZERO; count < CCP_MAX_DAQ; count++) 
            { 
                for(sub_count = CCP_DEFAULT_ZERO; sub_count < CCP_MAX_ODT; sub_count++)
                {
                    CCP_DAQ_ODT_Used[count][sub_count] = ((uint8)STD_OFF);
                }            
            }
            CCP_DAQ_PDU_Data_Buffer_Empty = ((uint8)STD_ON);  
            CCP_SessionStatus &= ((CCP_RUN) ^ CCP_ALL_BITS_SET_8);
        }
        /*0x01 starts data transmission*/
        else if(CCP_RX_Data[CCP_Index_2] == CCP_START_DAQ_ALL) 
        {
            for (count = CCP_DEFAULT_ZERO;count < CCP_MAX_DAQ; count++) 
            {
                if(CCP_DAQ_Flag[count].prepare == ((uint8)STD_ON))  
                {
                    /*Start DAQ*/ 
                    CCP_DAQ_Flag[count].flag = ((uint8)STD_ON);      
                }
            }
            CCP_SessionStatus |= (CCP_RUN);
        } 
        else
        {
            /*Error Code*/
            CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;
        }

    }
}
#endif/*End of ifdef CCP_DAQ_REQUEST*/

#ifdef CCP_FLASH
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Clear_Memory
*                
* Description:      This function is used to process Clear Memory command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Clear_Memory
(
    void
) 
{
    uint32 size;
    uint8  Return_Code;
    
    #ifdef CCP_SEED_KEY
    if (((uint8)(CCP_ProtectionStatus & CCP_RM_PGM)) == ((uint8)STD_OFF))
    { 
        /*Error code*/
        CCP_TX_Data[CCP_Index_1] = CCP_ACCESS_DENIED;
    }
    else
    #endif 
    {          
        size = Ccp_Make_32Bits(CCP_RX_Data[CCP_Index_2], CCP_RX_Data[CCP_Index_3], CCP_RX_Data[CCP_Index_4], CCP_RX_Data[CCP_Index_5]);
        
        /*Call Flash Erase function*/
        Return_Code = App_Ccp_FlashErase(CCP_Global_Address(CCP_MTA[CCP_Index_0]),size);
        if(Return_Code == ((uint8)E_NOT_OK)) 
        {
            CCP_TX_Data[CCP_Index_1] = CCP_RESOURCE_USELESS;     
        } 
        else
        {
            /*Do nothing*/
        }
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Program
*                
* Description:      This function is used to process Program & Program6 command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Program
(
    void
) 
{
    uint8  size;
    uint8  Return_Code;  
    uint8  Length_Available = ((uint8)STD_OFF);
    uint8* Data_Start = NULL_PTR;
    
    #ifdef CCP_SEED_KEY
    if (((uint8)(CCP_ProtectionStatus & CCP_RM_PGM)) == ((uint8)STD_OFF))
    {
        /*Error Code*/
        CCP_TX_Data[CCP_Index_1] = CCP_ACCESS_DENIED;   
    }
    else
    #endif 
    {
        if(CCP_COMMAND_CODE == CCP_PROGRAM)
        {
            size = CCP_RX_Data[CCP_Index_2];
            if(size <= CCP_PROGRAM_MAX_LENGTH)
            {
                Length_Available = ((uint8)STD_ON);    
                Data_Start = (&CCP_RX_Data[CCP_Index_3]);
            }
            else
            {
                /*Error Code*/
                CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;   
            }
        }
        else
        {
            size = CCP_PROGRAM6_LENGTH;
            Length_Available = ((uint8)STD_ON); 
            Data_Start = (&CCP_RX_Data[CCP_Index_2]);
        }

        /* If length is available, write data and move MTA */
        if(Length_Available == ((uint8)STD_ON))
        {
            /*Call Flash write function*/
            Return_Code = App_Ccp_FlashWrite(Data_Start, CCP_Global_Address(CCP_MTA[CCP_Index_0]), size);
            if(Return_Code == ((uint8)E_NOT_OK)) 
            {
                /* Error Code */
                CCP_TX_Data[CCP_Index_1] = CCP_RESOURCE_USELESS;
            }
            else
            {
                /* Move MTA */
                #ifdef CCP_16BITS_ADDRESS_MODE
                CCP_MTA[CCP_Index_0] += (size >> CCP_MOVE_BITS_1); 
                #else
                CCP_MTA[CCP_Index_0] += size;
                #endif             
            }
        }
        else
        {
            /* Do nothing */
        }
    }

    /* Set Extension Address */
    CCP_TX_Data[CCP_Index_3] = CCP_Extension_Address[CCP_Index_0];

    /* Get MTA Value */
    CCP_TX_Data[CCP_Index_4] = CCP_Get_1Byte(CCP_MTA[CCP_Index_0]);
    CCP_TX_Data[CCP_Index_5] = CCP_Get_2Byte(CCP_MTA[CCP_Index_0]);
    CCP_TX_Data[CCP_Index_6] = CCP_Get_3Byte(CCP_MTA[CCP_Index_0]);
    CCP_TX_Data[CCP_Index_7] = CCP_Get_4Byte(CCP_MTA[CCP_Index_0]);
}
#endif /*End of ifdef CCP_FLASH*/


#ifdef CCP_CHECKSUM
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Build_Checksum
*                
* Description:      This function is used to process Build Checksum command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Build_Checksum
(
    void
) 
{
    /* Initialize Responce */    
    #ifdef CCP_CHECKSUM_CRC2 
     /* Checksum */
    CCP_TX_Data[CCP_Index_4] = CCP_ALL_BITS_SET_8;
    CCP_TX_Data[CCP_Index_5] = CCP_ALL_BITS_SET_8;
    CCP_TX_Data[CCP_Index_6] = CCP_ALL_BITS_SET_8;
    CCP_TX_Data[CCP_Index_7] = CCP_ALL_BITS_SET_8;
    #else
    CCP_TX_Data[CCP_Index_4] = CCP_DEFAULT_ZERO;
    CCP_TX_Data[CCP_Index_5] = CCP_DEFAULT_ZERO;
    CCP_TX_Data[CCP_Index_6] = CCP_DEFAULT_ZERO;
    CCP_TX_Data[CCP_Index_7] = CCP_DEFAULT_ZERO;
    #endif  /*End of ifdef CCP_CHECKSUM_CRC2*/
    
    /* CCP_MTA[0] is not affected */
    CCP_MTA[CCP_Index_1] = CCP_MTA[CCP_Index_0];     

    CCP_CheckSumSize = Ccp_Make_32Bits(CCP_RX_Data[CCP_Index_2], CCP_RX_Data[CCP_Index_3], CCP_RX_Data[CCP_Index_4], CCP_RX_Data[CCP_Index_5]);
    
    /* Checksum calculation will be performed by Ccp_Checksum_Background() */
    CCP_Checksum_Flag = ((uint8)STD_ON); 
}
#endif /*End of ifdef CCP_CHECKSUM*/

#ifdef CCP_SEED_KEY
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Get_Seed
*                
* Description:      This function is used to process Get Seed command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_Seed
(
    void
) 
{
    uint32      Getseed;
    const uint8 Req_Resource = CCP_RX_Data[CCP_Index_2];

    /* Check Req_Resource */
    if((Req_Resource == CCP_RM_CAL)
        || (Req_Resource == CCP_RM_PGM)
        || (Req_Resource == CCP_RM_DAQ))
    {
        if(((uint8)(CCP_ProtectionStatus & Req_Resource)) == ((uint8)STD_OFF))
        {
            /* Need Unlock */
            CCP_TX_Data[CCP_Index_3] = CCP_NEED_UNLOCK;
            CCP_ResourceMask = Req_Resource;
        }
        else
        {
            /* Don't Need Unlock */
            CCP_TX_Data[CCP_Index_3] = CCP_NO_UNLOCK;
        }
        
        /* Get Seed */
        Getseed = App_Ccp_GetSeed(Req_Resource);
        CCP_TX_Data[CCP_Index_4] = CCP_Get_1Byte(Getseed);
        CCP_TX_Data[CCP_Index_5] = CCP_Get_2Byte(Getseed);
        CCP_TX_Data[CCP_Index_6] = CCP_Get_3Byte(Getseed);
        CCP_TX_Data[CCP_Index_7] = CCP_Get_4Byte(Getseed);
    }
    else
    {
        CCP_TX_Data[1] = CCP_OUT_OF_RANGE;
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Unlock
*                
* Description:      This function is used to process Unlock command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Unlock
(
    void
) 
{
    /* Check key */
    uint8  Result;
    uint32 Key;
    
    /*Always LSB when get KEY*/
    Key = (((uint32)CCP_RX_Data[CCP_Index_5] << CCP_MOVE_BITS_24) 
            | ((uint32)CCP_RX_Data[CCP_Index_4] << CCP_MOVE_BITS_16) 
            | ((uint32)CCP_RX_Data[CCP_Index_3] << CCP_MOVE_BITS_8) 
            | ((uint32)CCP_RX_Data[CCP_Index_2])); 
    
    Result = App_Ccp_Unlock(Key);
    if(Result == CCP_WRONG_KEY) 
    {
        /*Error Code*/
        CCP_TX_Data[CCP_Index_1] = CCP_ACCESS_LOCKED;
    } 
    else
    {
        /* Reset the appropriate resource protection mask bit */
        CCP_ProtectionStatus |= CCP_ResourceMask;
        /* Current Protection Status */
        CCP_TX_Data[CCP_Index_3] = CCP_ProtectionStatus; 
    }
}
#endif  /*End of ifdef CCP_SEED_KEY*/

#ifdef CCP_CAL_PAGE
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Set_Cal_Page
*                
* Description:      This function is used to process Set_Cal_Page command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Set_Cal_Page
(
    void
) 
{
    uint8 Return_Code;     
    if ((CCP_MTA[CCP_Index_0] == CCP_REF_PAGE_ADDR) || (CCP_MTA[CCP_Index_0] == CCP_WORK_PAGE_ADDR) )
    {
        if(CCP_MTA[CCP_Index_0] == CCP_REF_PAGE_ADDR )
        {
            CCP_Active_Page = CCP_REF_PAGE_INDEX;
        }
        else
        {
            CCP_Active_Page = CCP_WORK_PAGE_INDEX;
        }
        
        Return_Code = App_Ccp_Select_Page_Indication(CCP_Active_Page);
        if (Return_Code != ((uint8)E_OK))
        {
            /*ACKNOWLEDGE 0x36*/
            CCP_TX_Data[CCP_Index_1] = CCP_RESOURCE_USELESS;    
        }
        else
        {
            /*Do nothing*/
        }    
    }
    else
    {
        /*return error code*/
        CCP_TX_Data[CCP_Index_1] = CCP_OUT_OF_RANGE;        
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Fun_Get_Cal_Page
*                
* Description:      This function is used to process Get_Cal_Page command.
*                             
* Inputs:           None
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Fun_Get_Cal_Page
(
    void
) 
{
    uint32 temp;  
    if(CCP_Active_Page == CCP_REF_PAGE_INDEX)
    {
        temp = CCP_REF_PAGE_ADDR;
    }
    else 
    {
        temp = CCP_WORK_PAGE_ADDR;
    }
    /* Set Extetion Address */
    CCP_TX_Data[CCP_Index_3] = CCP_Extension_Address[CCP_Index_0];        
    /* Set Address */
    CCP_TX_Data[CCP_Index_4] = CCP_Get_1Byte(temp);
    CCP_TX_Data[CCP_Index_5] = CCP_Get_2Byte(temp);
    CCP_TX_Data[CCP_Index_6] = CCP_Get_3Byte(temp);
    CCP_TX_Data[CCP_Index_7] = CCP_Get_4Byte(temp);
}
#endif  /*End of ifdef CCP_CAL_PAGE*/

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_CommFunction 
*                
* Description:      This function analyse the command of CCP Tool,and send the 
*                   resopse message back.
*                             
* Inputs:           None
*
* Outputs:          Std_ReturnType
*                   E_OK:       Controller mode request has been accepted. 
*                   E_NOT_OK:   Controller mode request has not been accepted.
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/
 
STATIC FUNC(Std_ReturnType, CCP_PRIVATE_CODE) Ccp_CommFunction
(
    void
) 
{ 
    /* prepare the responce */
    CCP_TX_Data[CCP_Index_0] = CCP_RESPONSE_CODE; 
    CCP_TX_Data[CCP_Index_1] = CCP_NO_ERROR;
    CCP_TX_Data[CCP_Index_2] = CCP_CTR;
    
    /*Test command is used to test if the slave with the specified station 
    address is available for CCP communication.*/
    /*Connect command establishes a continuous logical point-to-point connection 
    with the selected slave station for the master-slave command protocol.*/
    if((CCP_COMMAND_CODE == CCP_TEST)||(CCP_COMMAND_CODE == CCP_CONNECT))                           
    {
        Ccp_Fun_Test_Connect();
    }  
    /*Disconnects the slave device.*/
    else if(CCP_COMMAND_CODE == CCP_DISCONNECT) 
    {
        Ccp_Fun_DisConnect();
    }
    else if (((uint8)(CCP_Status & CCP_CONNECTED)) == CCP_CONNECTED) 
    {
        switch (CCP_COMMAND_CODE) 
        {
            /************CCP_EXCHANGE_ID************/
            /* Exchange Station Identifications
            The CCP master and slave stations exchange IDs for automatic session
            configuration.*/
        case CCP_EXCHANGE_ID: 
            {
                Ccp_Fun_Exchange_ID();
            }
            break;
            
            /***********Set Memory Transfer Address****************/
            /*This command will initialize a base pointer for all following 
            memory transfers.*/
        case CCP_SET_MTA:               
            {
                Ccp_Fun_Set_MTA();
            }
            break;
            
            /****************Data Upload*******************/
            /*A data block of the specified length (size), starting at current 
            MTA0, will be copied into the corresponding DTO data field.*/
        case CCP_UPLOAD:                  
            {
                Ccp_Fun_Upload();
            }
            break;
            
            /*****************Short Upload*****************/
            /*A data block of the specified length (size), starting at source 
            address will be copied into the corresponding DTO data field.*/
        case CCP_SHORT_UPLOAD:            
            {
                Ccp_Fun_Upload();
            }
            break;
            
            /****************Get the CCP Version****************/
            /*This command performs a mutual identification of the protocol 
            version used in the master and in the slave device to aggree on a 
            common protocol version. This command is expected to be executed 
            prior to the EXCHANGE_ID command.*/
        case CCP_GET_CCP_VERSION: 
            {
                Ccp_Fun_Get_Version();
            }
            break;
                        
            /*********Data Download 6 Bytes*******/
            /*The data block with the fixed length (size) of 6 bytes contained 
            in the CRO will be copied into memory, starting at the current 
            Memory Transfer Address 0 (MTA0).*/
        case CCP_DNLOAD6:
            {
                Ccp_Fun_Download();
            }
            break;
            
            /*Data Download(Max 5 Bytes)*/
            /*The data block of the specified length (size) contained in the 
            CRO will be copied into memory, starting at the current Memory 
            Transfer Address 0 (MTA0).*/
        case CCP_DNLOAD:
            {
                Ccp_Fun_Download();
            }
            break;
            
            /************** Move memory block ***************/
            /*A data block of the specified length (size) will be copied from 
            the address defined by CCP_MTA 0 (source pointer) to the address defined
            by CCP_MTA 1 (destination pointer).*/
        case CCP_MOVE:                        
            {
                Ccp_Fun_Move();
            }
            break;
            /********* Get Size of DAQ list **********/
            /* Returns the size of the specified DAQ list as the number of 
            available Object DescriptorTables (ODTs) and clears the current 
            list. If the specified list number is not available, size = 0 
            should be returned. The DAQ list is initialized and data acquisition
            by this list is stopped. */
        case CCP_GET_DAQ_SIZE:         
            {
                Ccp_Fun_Get_DAQ_Size();
            }
            break;

            #ifdef CCP_DAQ_REQUEST 
            /*********CCP_SET_DAQ_PTR***********/
            /*Initializes the DAQ list pointer for a subsequent write to a 
            DAQ list.*/
        case CCP_SET_DAQ_PTR:            
            {
                Ccp_Fun_Set_DAQ_Ptr();
            }
            break;
            #endif   /*End of CCP_DAQ_REQUEST*/

            #ifdef CCP_DAQ_REQUEST
            /***********CCP_WRITE_DAQ***********/
            /*Writes one entry (description of single DAQ element) to a DAQ 
            list defined by the DAQ list pointer (see SET_DAQ_PTR). */
        case CCP_WRITE_DAQ:               
            {
                Ccp_Fun_Write_DAQ();
            }
            break;
            #endif   /*End of CCP_DAQ_REQUEST*/
            
            /*********** Set Session Status*************/
            /*Keeps the slave node informed about the current state of the 
            calibration session*/
        case CCP_SET_S_STATUS:            
            {
                Ccp_Fun_Set_S_Status();
            }
            break;
            
            /* Get Session Status */
        case CCP_GET_S_STATUS:           
            {
                Ccp_Fun_Get_S_Status();
            }
            break;

            #ifdef CCP_DAQ_REQUEST
            /********Start/STOP Specifiek DAQ Data transmission*******/
            /*This command is used to start or to stop the data acquisition 
            or to prepare a synchronized start of the specified DAQ list.*/
        case CCP_START_STOP:               
            {
                Ccp_Fun_Start_Stop();
            }
            break;
            
            /**********Start/STOP all DAQ Data's transmission**********/
            /*This command is used to start the periodic transmission of all DAQ
            lists configured with the previously sent START_STOP command 
            (start/stop modus = 2) as prepared to start  in a synchronized 
            manner.*/
        case CCP_START_STOP_ALL:               
            {
                Ccp_Fun_Start_Stop_All();
            }
            break;
            #endif   /*End of CCP_DAQ_REQUEST*/

            #ifdef CCP_FLASH
            /********* Clear Memory **********/
            /*This command may be used to erase FLASH EPROM prior to 
            reprogramming. The MTA0 pointer points to the memory location to be 
            erased.*/
        case CCP_CLEAR_MEMORY:                  
            {
                Ccp_Fun_Clear_Memory();
            }
            break;

            /********** Write Flash ********/
            /*The data block of the specified length (size) contained in the CRO
            will be programmed into non-volatile memory (FLASH, EEPROM), 
            starting at current MTA0.*/
        case CCP_PROGRAM:                     
            {        
                Ccp_Fun_Program();
            }
            break;
            
            /******* Write Flash 6 Bytes data *********/
            /*The data block with the length (size) of 6 bytes contained in the 
            CRO will be programmed into non-volatile memory (FLASH, EEPROM), 
            starting at current MTA0. The MTA0 pointer will be post-incremented 
            by 6.*/
        case CCP_PROGRAM6:                     
            {
                Ccp_Fun_Program();
            }
            break;
            #endif  /*End of ifdef CCP_FLASH*/

            #ifdef CCP_CHECKSUM
             /********* Build Checksum ***********/
            /*Returns a checksum result of the memory block that is defined by 
            MTA0 (memory transfer area start address) and block size.*/
        case CCP_BUILD_CHKSUM:                 
            {
                Ccp_Fun_Build_Checksum();
            }
            break;
            #endif /* End of ifdef CCP_CHECKSUM */

            #ifdef CCP_SEED_KEY
            /******** Get Seed for Key **********/
            /*Returns SEED  data for a seed&key algorithm for computing the KEY 
            to unlock the requested function for authorized access*/
        case CCP_GET_SEED:                     
            {
                Ccp_Fun_Get_Seed();
            }
            break;
            /*********Unlock Protection *********/
            /*Unlocks the slave devices security protection (if applicable) 
            using a KEY computed from SEED.*/
        case CCP_UNLOCK:                        
            {
                Ccp_Fun_Unlock();
            }
            break;

            #endif /* End of ifdef CCP_SEED_KEY */

            #ifdef CCP_CAL_PAGE
            /*******Select Calibration Data Page********/
            /*This command's function depends on the ECU implementation. The 
            previously initialized MTA0 points to the start of the calibration 
            data page that is selected as the currently active page by this 
            command.*/
        case CCP_SET_CAL_PAGE:
            {
                Ccp_Fun_Set_Cal_Page();
            }
            break;
            
            /*******Get currently active Calibration Page*********/
            /*This command returns the start address of the calibration page 
            that is currently active in the slave device.*/
        case CCP_GET_CAL_PAGE:
            {
                Ccp_Fun_Get_Cal_Page();
            }
            break;
            #endif  /*End of ifdef CCP_CAL_PAGE*/
            
        default: 
            {
                CCP_TX_Data[CCP_Index_1] = CCP_UNKNOWN_COMMAND;
            }
            break;
        }
        #ifdef CCP_CHECKSUM
        if(CCP_Checksum_Flag == ((uint8)STD_ON))
        {
            CCP_Checksum_Flag = ((uint8)STD_OFF);
        }
        else
        #endif    
        {
            Ccp_DTO_Transmit();
        }
    } 
    else
    {
        /*do nothing*/
    }

    return ((uint8)E_OK);
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_DTO_Transmit 
*                
* Description:      This service transmit the DTO to CAN Bus. 
*                             
* Inputs:           None 
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void, CCP_PRIVATE_CODE) Ccp_DTO_Transmit
(
    void
) 
{  
    PduInfoType  Transmit_CCP_PDU;  
    uint8        count;
    
    Transmit_CCP_PDU.SduDataPtr = CCP_TX_Data;
    Transmit_CCP_PDU.SduLength  = (uint16)CCP_CAN_LENGTH; 

    if((CCP_DTO_PDU_Data_Buffer_Empty == ((uint8)STD_ON)) && (CCP_Tx_Busy == ((uint8)STD_OFF)))
    {
        CCP_Tx_Busy = ((uint8)STD_ON);
        (void)CanIf_Transmit(CCP_DTO_INDEX, &Transmit_CCP_PDU);
        
    }
    /* DTO Transmission Buffer is NOT Empty or CAN BUS is busy */
    else
    {
        for(count = CCP_DEFAULT_ZERO; count < CCP_CAN_LENGTH; count++)
        {
            CCP_DTO_PDU_Data_Buffer[count] = CCP_TX_Data[count];        
        }
        CCP_DTO_PDU_Data_Buffer_Empty = ((uint8)STD_OFF);
    }
}

#ifdef CCP_DAQ_REQUEST 
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name : Ccp_Move_Buffer_Ptr
*
* Description   : This function is used to move PTR in Ccp DAQ Transmit Buffer. 
*
* Inputs        : const uint8* Ptr

* Outputs       : void
*
* Limitations   : None
*
********************************************************************************
END_FUNCTION_HDR */
STATIC FUNC(void, CCP_PRIVATE_CODE) Ccp_Move_Buffer_Ptr
(
    CONSTP2VAR(uint8,AUTOMATIC,CCP_PRIVATE_DATA) Ptr
) 
{   
    if(Ptr != 0)
    {
        if((*Ptr) < (CCP_ODT_BUF_NUM - CCP_UINT8_1))
        {
            (*Ptr)++;
        }
        else
        {
            (*Ptr) = CCP_DEFAULT_ZERO;
        }
    }
    else
    {
        /* Do nothing */
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name : Ccp_Check_FIFO
*
* Description   : This function is used to check the if FIFO is available or all data has been sent. 
*
* Inputs        : void

* Outputs       : Std_ReturnType
*
* Limitations   : None
*
********************************************************************************
END_FUNCTION_HDR */

STATIC FUNC(Std_ReturnType, CCP_PRIVATE_CODE) Ccp_Check_FIFO
(
    void
) 
{
    uint8 Ret = ((uint8)E_OK);
    
    if(CCP_DAQ_PDU_Data_Buffer_Empty != ((uint8)STD_ON))
    {   
        /* FIFO has not been full or some data has not been sent*/
        if(Ccp_DAQ_Buffer_Last_Ptr != Ccp_DAQ_Buffer_First_Ptr)
        {
            /*do nothing*/
        }        
        else
        {            
            /* FIFO has been full or all data has been sent*/
            Ret = ((uint8)E_NOT_OK);
        }        

    }
    else
    {
        /* Buffer is empty, FIFO is available, do nothing */
    }
    return Ret;
}

/* BEGIN_FUNCTION_HDR
************************************************************************************************
* Function Name : Ccp_DAQ_Copy_Data
*
* Description   : This function is used to copy data to Ccp DAQ Transmit Buffer. 
*
* Inputs        : None

* Outputs       : None
*
* Limitations   : None
*
************************************************************************************************
END_FUNCTION_HDR */

STATIC FUNC(void, CCP_PRIVATE_CODE) Ccp_DAQ_Copy_Data
(
    const uint8 DAQ_Count,
    const uint8 ODT_Count
) 
{  
    uint8 Entry_Count;
    uint8 Check_Result;
    uint32_t primask;

    Check_Result = Ccp_Check_FIFO();
    
    #ifdef CCP_16BITS_ADDRESS_MODE
    uint8 Two_Bytes_Count = CCP_DEFAULT_ZERO;
    #endif

    if(Check_Result == ((uint8)E_OK))
    {
        Ccp_DisableInterrupt();
        
        /* Get PID */      
        CCP_DAQ_PDU_Data_Buffer[Ccp_DAQ_Buffer_Last_Ptr][CCP_Index_0] =
                                (uint8)((DAQ_Count * CCP_MAX_ODT) + (ODT_Count));     
            
        #ifdef CCP_16BITS_ADDRESS_MODE
        for(Entry_Count = CCP_DEFAULT_ZERO; Entry_Count < CCP_UINT8_3; Entry_Count++)
        {   
            /* Get Data */
            if(CCP_ODT[DAQ_Count][ODT_Count][Entry_Count] != ((uint32)CCP_DEFAULT_ZERO))
            {
                CCP_DAQ_PDU_Data_Buffer[Ccp_DAQ_Buffer_Last_Ptr][Two_Bytes_Count + CCP_UINT8_1] = 
                    CCP_Get_First_Byte(*(uint8 CCP_FAR)CCP_Global_Address(CCP_ODT[DAQ_Count][ODT_Count][Entry_Count]));
                CCP_DAQ_PDU_Data_Buffer[Ccp_DAQ_Buffer_Last_Ptr][Two_Bytes_Count + CCP_UINT8_2] = 
                    CCP_Get_Second_Byte(*(uint8 CCP_FAR)CCP_Global_Address(CCP_ODT[DAQ_Count][ODT_Count][Entry_Count]));
            }
            /* if ODT address is NULL, set values as zero */
            else
            {
                CCP_DAQ_PDU_Data_Buffer[Ccp_DAQ_Buffer_Last_Ptr][Two_Bytes_Count + CCP_UINT8_1] = CCP_DEFAULT_ZERO;
            }
            Two_Bytes_Count += CCP_UINT8_2;
        }
        #else
        for(Entry_Count = CCP_DEFAULT_ZERO; Entry_Count < CCP_MAX_ELEMENT; Entry_Count++) 
        {  
            /* Get Data */
            if (CCP_ODT[DAQ_Count][ODT_Count][Entry_Count] != ((uint32)CCP_DEFAULT_ZERO))
            {
                CCP_DAQ_PDU_Data_Buffer[Ccp_DAQ_Buffer_Last_Ptr][Entry_Count + CCP_UINT8_1] = 
                (*(uint8 CCP_FAR)CCP_Global_Address(CCP_ODT[DAQ_Count][ODT_Count][Entry_Count]));
            }        
            /* if ODT address is NULL, set values as zero */
            else
            {
                CCP_DAQ_PDU_Data_Buffer[Ccp_DAQ_Buffer_Last_Ptr][Entry_Count + CCP_UINT8_1] = CCP_DEFAULT_ZERO;
            }
        }
        #endif   /*End of ifdef CCP_16BITS_ADDRESS_MODE*/ 
        
        /* Move FIFO Last Ptr */
        Ccp_Move_Buffer_Ptr(&Ccp_DAQ_Buffer_Last_Ptr);
        /* Set Flag */
        CCP_DAQ_PDU_Data_Buffer_Empty = ((uint8)STD_OFF);
        
        Ccp_EnableInterrupt();
    }
    else
    {
        /* No availabe FIFO */
    }
    
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Transmit_DAQ 
*                
* Description:      This function is used to transmit DTO to CAN Bus. 
*                             
* Inputs:           None 
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

STATIC FUNC(void,CCP_PRIVATE_CODE) Ccp_Transmit_DAQ
(
    const uint8 event_channel
) 
{  
    PduInfoType Transmit_DAQ_PDU;

    uint8       DAQ_Count;
    uint8       ODT_Count;

    uint8       Check_Result;
    
    #ifdef CCP_16BITS_ADDRESS_MODE
    uint8       Two_Bytes_Count = CCP_DEFAULT_ZERO;
    #endif

    /* Copy all data to CCP_DAQ_PDU_Data_Buffer, but can not start transmit */
    for (DAQ_Count = CCP_DEFAULT_ZERO; DAQ_Count < CCP_MAX_DAQ; DAQ_Count++) 
    {
        /* select the active DAQ */
        if ((CCP_DAQ_Flag[DAQ_Count].flag == ((uint8)STD_ON)) 
            && (CCP_DAQ_Flag[DAQ_Count].channel == event_channel))
        {
            /* increase Prescaler_Count */
            if(CCP_DAQ_Prescaler_Count[DAQ_Count] < CCP_DAQ_Prescaler[DAQ_Count])
            {
                CCP_DAQ_Prescaler_Count[DAQ_Count]++;
            }
            else
            {
                /* do nothing */
            }
            /* check Prescaler_Count */
            if(CCP_DAQ_Prescaler_Count[DAQ_Count] >= CCP_DAQ_Prescaler[DAQ_Count])
            {
                CCP_DAQ_Prescaler_Count[DAQ_Count] = CCP_DEFAULT_ZERO_16;
                for (ODT_Count = CCP_DEFAULT_ZERO; ODT_Count < CCP_MAX_ODT; ODT_Count++) 
                {
                    /* Only used CCP_ODT wil be display*/
                    if(CCP_DAQ_ODT_Used[DAQ_Count][ODT_Count] == ((uint8)STD_ON))                       
                    {          
                        /* Copy data to CCP_DAQ_PDU_Data_Buffer */
                        Ccp_DAQ_Copy_Data(DAQ_Count, ODT_Count);   
                    } 
                }
            }
            else
            {
                /* do nothing */
            }
        }
    }

    /* Check transmission condition, if CCP is not busy */
    if((CCP_DAQ_PDU_Data_Buffer_Empty != ((uint8)STD_ON)) && (CCP_Tx_Busy == ((uint8)STD_OFF)))
    {
        Transmit_DAQ_PDU.SduLength  = (uint16)CCP_CAN_LENGTH; 
        Transmit_DAQ_PDU.SduDataPtr = CCP_DAQ_PDU_Data_Buffer[Ccp_DAQ_Buffer_First_Ptr];
        
        CCP_Tx_Busy = ((uint8)STD_ON);
        (void)CanIf_Transmit(CCP_DTO_INDEX, &Transmit_DAQ_PDU);  
        Ccp_Move_Buffer_Ptr(&Ccp_DAQ_Buffer_First_Ptr);

        /* Check FIFO */
        Check_Result = Ccp_Check_FIFO();
        if(Check_Result == ((uint8)E_NOT_OK))
        {
            /* all data has been sent, so set CCP_DAQ_PDU_Data_Buffer_Empty */
            CCP_DAQ_PDU_Data_Buffer_Empty = ((uint8)STD_ON);
        }
        else
        {
            /* still has data wait to be sent in CCP_DAQ_PDU_Data_Buffer */
        }
    }
    else
    {
        /* FIFO is empty or bus is busy, do nothing */
    }
}                        
#endif

#define CCP_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"

#define CCP_START_SEC_PUBLIC_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Event_DAQ_Transmit 
*                
* Description:      Trigger the event of specifiek channel.  
*                             
* Inputs:           uint8 event_channel: Event_Channel Index 
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

#ifdef CCP_DAQ_REQUEST  
FUNC(void, CCP_PUBLIC_CODE) Ccp_Event_DAQ_Transmit
(
    const uint8 event_channel
) 
{
    if (((uint8)(CCP_SessionStatus & CCP_RUN)) == ((uint8)STD_OFF)) 
    {
        /* Do nothing */  
    }
    else
    {
        Ccp_Transmit_DAQ(event_channel);        
    }    
}
#endif  /*End of ifdef CCP_DAQ_REQUEST*/ 

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_TxConfirmation 
*                
* Description:      This function confirms a previously successfully processed 
*                   transmission of a CCP Pdu 
*                             
* Inputs:           PduIdType PduId: 
*                       indication which Pdu has been secusseful sent.
*
* Outputs:          None
* 
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

FUNC(void, CCP_PUBLIC_CODE) Ccp_TxConfirmation
(
    PduIdType PduId
)
{
    PduInfoType Transmit_PDU; 
    uint8       Check_Result;
    
    if(PduId == CCP_TX_UL_HANDLE)
    {
        CCP_Tx_Busy = ((uint8)STD_OFF);

        Transmit_PDU.SduLength  = (uint16)CCP_CAN_LENGTH; 
        
        /* If DTO PDU Buffer is NOT Empty, Send message from buffer */
        if(CCP_DTO_PDU_Data_Buffer_Empty != ((uint8)STD_ON)) 
        {
            Transmit_PDU.SduDataPtr = CCP_DTO_PDU_Data_Buffer;
            CCP_Tx_Busy = ((uint8)STD_ON);
            (void) CanIf_Transmit(CCP_DTO_INDEX, &Transmit_PDU);

            CCP_DTO_PDU_Data_Buffer_Empty = ((uint8)STD_ON);    
        }
        #ifdef CCP_DAQ_REQUEST  
        /* If DAQ PDU Buffer is NOT Empty, Send message from buffer */
        else if(CCP_DAQ_PDU_Data_Buffer_Empty != ((uint8)STD_ON))
        {
            Transmit_PDU.SduDataPtr = CCP_DAQ_PDU_Data_Buffer[Ccp_DAQ_Buffer_First_Ptr];
            CCP_Tx_Busy = ((uint8)STD_ON);
            
            (void) CanIf_Transmit(CCP_DTO_INDEX, &Transmit_PDU);
            Ccp_Move_Buffer_Ptr(&Ccp_DAQ_Buffer_First_Ptr);

            /* check FIFO */
            Check_Result = Ccp_Check_FIFO();
            if(Check_Result == ((uint8)E_NOT_OK))
            {
                /* all data has been sent, so set CCP_DAQ_PDU_Data_Buffer_Empty */
                CCP_DAQ_PDU_Data_Buffer_Empty = ((uint8)STD_ON);
            }
            else
            {
                /* still has data wait to be sent in CCP_DAQ_PDU_Data_Buffer */
            }
        }
        #endif  /*End of ifdef CCP_DAQ_REQUEST*/ 
        else
        {
            /* do nothing */
        }
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    Ccp_Checksum_Background 
*                
* Description:      Background Processing  used for Checksum Calculation. 
*                             
* Inputs:           None
*
* Outputs:          None
*
* Limitations:      Background Loop
********************************************************************************
END_FUNCTION_HDR*/

FUNC(void, CCP_PUBLIC_CODE) Ccp_Checksum_Background
(
    void
) 
{   
    #ifdef CCP_CHECKSUM
    #ifdef CCP_CHECKSUM_CRC2
    /* for CRC2, init value is 0xFFFF */
    STATIC uint16                           Tx_Temp = CCP_ALL_BITS_SET_16;
    #else
    /* for SUM, init value is zero */
    STATIC uint16                           Tx_Temp = CCP_DEFAULT_ZERO_16;
    #endif
    uint32                                  Check_Count;
    #ifdef CCP_16BITS_ADDRESS_MODE
    uint8                                   Tx_Temp_First = CCP_DEFAULT_ZERO;
    uint8                                   Tx_Temp_Second = CCP_DEFAULT_ZERO;
    #endif  

    #ifdef CCP_CHECKSUM_CRC2 
    STATIC CONST (uint16,CCP_PRIVATE_DATA)  CCP_CRC16CCITTtab[CCP_CRC_TABLE_LENGTH] = 
    {
        (uint16)0x0000u,(uint16)0x1189u,(uint16)0x2312u,(uint16)0x329Bu,(uint16)0x4624u,(uint16)0x57ADu,(uint16)0x6536u,(uint16)0x74BFu,
        (uint16)0x8C48u,(uint16)0x9DC1u,(uint16)0xAF5Au,(uint16)0xBED3u,(uint16)0xCA6Cu,(uint16)0xDBE5u,(uint16)0xE97Eu,(uint16)0xF8F7u,
        (uint16)0x1081u,(uint16)0x0108u,(uint16)0x3393u,(uint16)0x221Au,(uint16)0x56A5u,(uint16)0x472Cu,(uint16)0x75B7u,(uint16)0x643Eu,
        (uint16)0x9CC9u,(uint16)0x8D40u,(uint16)0xBFDBu,(uint16)0xAE52u,(uint16)0xDAEDu,(uint16)0xCB64u,(uint16)0xF9FFu,(uint16)0xE876u,
        (uint16)0x2102u,(uint16)0x308Bu,(uint16)0x0210u,(uint16)0x1399u,(uint16)0x6726u,(uint16)0x76AFu,(uint16)0x4434u,(uint16)0x55BDu,
        (uint16)0xAD4Au,(uint16)0xBCC3u,(uint16)0x8E58u,(uint16)0x9FD1u,(uint16)0xEB6Eu,(uint16)0xFAE7u,(uint16)0xC87Cu,(uint16)0xD9F5u,
        (uint16)0x3183u,(uint16)0x200Au,(uint16)0x1291u,(uint16)0x0318u,(uint16)0x77A7u,(uint16)0x662Eu,(uint16)0x54B5u,(uint16)0x453Cu,
        (uint16)0xBDCBu,(uint16)0xAC42u,(uint16)0x9ED9u,(uint16)0x8F50u,(uint16)0xFBEFu,(uint16)0xEA66u,(uint16)0xD8FDu,(uint16)0xC974u,
        (uint16)0x4204u,(uint16)0x538Du,(uint16)0x6116u,(uint16)0x709Fu,(uint16)0x0420u,(uint16)0x15A9u,(uint16)0x2732u,(uint16)0x36BBu,
        (uint16)0xCE4Cu,(uint16)0xDFC5u,(uint16)0xED5Eu,(uint16)0xFCD7u,(uint16)0x8868u,(uint16)0x99E1u,(uint16)0xAB7Au,(uint16)0xBAF3u,
        (uint16)0x5285u,(uint16)0x430Cu,(uint16)0x7197u,(uint16)0x601Eu,(uint16)0x14A1u,(uint16)0x0528u,(uint16)0x37B3u,(uint16)0x263Au,
        (uint16)0xDECDu,(uint16)0xCF44u,(uint16)0xFDDFu,(uint16)0xEC56u,(uint16)0x98E9u,(uint16)0x8960u,(uint16)0xBBFBu,(uint16)0xAA72u,
        (uint16)0x6306u,(uint16)0x728Fu,(uint16)0x4014u,(uint16)0x519Du,(uint16)0x2522u,(uint16)0x34ABu,(uint16)0x0630u,(uint16)0x17B9u,
        (uint16)0xEF4Eu,(uint16)0xFEC7u,(uint16)0xCC5Cu,(uint16)0xDDD5u,(uint16)0xA96Au,(uint16)0xB8E3u,(uint16)0x8A78u,(uint16)0x9BF1u,
        (uint16)0x7387u,(uint16)0x620Eu,(uint16)0x5095u,(uint16)0x411Cu,(uint16)0x35A3u,(uint16)0x242Au,(uint16)0x16B1u,(uint16)0x0738u,
        (uint16)0xFFCFu,(uint16)0xEE46u,(uint16)0xDCDDu,(uint16)0xCD54u,(uint16)0xB9EBu,(uint16)0xA862u,(uint16)0x9AF9u,(uint16)0x8B70u,
        (uint16)0x8408u,(uint16)0x9581u,(uint16)0xA71Au,(uint16)0xB693u,(uint16)0xC22Cu,(uint16)0xD3A5u,(uint16)0xE13Eu,(uint16)0xF0B7u,
        (uint16)0x0840u,(uint16)0x19C9u,(uint16)0x2B52u,(uint16)0x3ADBu,(uint16)0x4E64u,(uint16)0x5FEDu,(uint16)0x6D76u,(uint16)0x7CFFu,
        (uint16)0x9489u,(uint16)0x8500u,(uint16)0xB79Bu,(uint16)0xA612u,(uint16)0xD2ADu,(uint16)0xC324u,(uint16)0xF1BFu,(uint16)0xE036u,
        (uint16)0x18C1u,(uint16)0x0948u,(uint16)0x3BD3u,(uint16)0x2A5Au,(uint16)0x5EE5u,(uint16)0x4F6Cu,(uint16)0x7DF7u,(uint16)0x6C7Eu,
        (uint16)0xA50Au,(uint16)0xB483u,(uint16)0x8618u,(uint16)0x9791u,(uint16)0xE32Eu,(uint16)0xF2A7u,(uint16)0xC03Cu,(uint16)0xD1B5u,
        (uint16)0x2942u,(uint16)0x38CBu,(uint16)0x0A50u,(uint16)0x1BD9u,(uint16)0x6F66u,(uint16)0x7EEFu,(uint16)0x4C74u,(uint16)0x5DFDu,
        (uint16)0xB58Bu,(uint16)0xA402u,(uint16)0x9699u,(uint16)0x8710u,(uint16)0xF3AFu,(uint16)0xE226u,(uint16)0xD0BDu,(uint16)0xC134u,
        (uint16)0x39C3u,(uint16)0x284Au,(uint16)0x1AD1u,(uint16)0x0B58u,(uint16)0x7FE7u,(uint16)0x6E6Eu,(uint16)0x5CF5u,(uint16)0x4D7Cu,
        (uint16)0xC60Cu,(uint16)0xD785u,(uint16)0xE51Eu,(uint16)0xF497u,(uint16)0x8028u,(uint16)0x91A1u,(uint16)0xA33Au,(uint16)0xB2B3u,
        (uint16)0x4A44u,(uint16)0x5BCDu,(uint16)0x6956u,(uint16)0x78DFu,(uint16)0x0C60u,(uint16)0x1DE9u,(uint16)0x2F72u,(uint16)0x3EFBu,
        (uint16)0xD68Du,(uint16)0xC704u,(uint16)0xF59Fu,(uint16)0xE416u,(uint16)0x90A9u,(uint16)0x8120u,(uint16)0xB3BBu,(uint16)0xA232u,
        (uint16)0x5AC5u,(uint16)0x4B4Cu,(uint16)0x79D7u,(uint16)0x685Eu,(uint16)0x1CE1u,(uint16)0x0D68u,(uint16)0x3FF3u,(uint16)0x2E7Au,
        (uint16)0xE70Eu,(uint16)0xF687u,(uint16)0xC41Cu,(uint16)0xD595u,(uint16)0xA12Au,(uint16)0xB0A3u,(uint16)0x8238u,(uint16)0x93B1u,
        (uint16)0x6B46u,(uint16)0x7ACFu,(uint16)0x4854u,(uint16)0x59DDu,(uint16)0x2D62u,(uint16)0x3CEBu,(uint16)0x0E70u,(uint16)0x1FF9u,
        (uint16)0xF78Fu,(uint16)0xE606u,(uint16)0xD49Du,(uint16)0xC514u,(uint16)0xB1ABu,(uint16)0xA022u,(uint16)0x92B9u,(uint16)0x8330u,
        (uint16)0x7BC7u,(uint16)0x6A4Eu,(uint16)0x58D5u,(uint16)0x495Cu,(uint16)0x3DE3u,(uint16)0x2C6Au,(uint16)0x1EF1u,(uint16)0x0F78u
    };
    #endif  /*End of ifdef CCP_CHECKSUM_CRC2*/ 


    /* Checksum calculation in progress */
    if (CCP_CheckSumSize != ((uint32)CCP_DEFAULT_ZERO)) 
    {
        if (CCP_CheckSumSize < CCP_CHECKSUM_BLOCKSIZE) 
        {
            Check_Count = CCP_CheckSumSize;
            CCP_CheckSumSize = (uint32)CCP_DEFAULT_ZERO;
        } 
        else 
        {
            Check_Count = CCP_CHECKSUM_BLOCKSIZE;
            CCP_CheckSumSize -= CCP_CHECKSUM_BLOCKSIZE;
        }

        do 
        {
            /* CRC */
            #ifdef CCP_CHECKSUM_CRC2
                
            #ifdef CCP_16BITS_ADDRESS_MODE
            Tx_Temp_First = CCP_Get_First_Byte(*((uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_1])));
            Tx_Temp_Second = CCP_Get_Second_Byte(*((uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_1])));
            Tx_Temp = (Tx_Temp >> CCP_MOVE_BITS_8) ^ CCP_CRC16CCITTtab[(Tx_Temp ^ Tx_Temp_First)& CCP_ALL_BITS_SET_8];
            Tx_Temp = (Tx_Temp >> CCP_MOVE_BITS_8) ^ CCP_CRC16CCITTtab[(Tx_Temp ^ Tx_Temp_Second)& CCP_ALL_BITS_SET_8];
            #else
            Tx_Temp = (Tx_Temp >> CCP_MOVE_BITS_8) ^ CCP_CRC16CCITTtab[(Tx_Temp ^ (*(uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_1]))) & ((uint16)CCP_ALL_BITS_SET_8)];
            #endif    /*End of ifdef CCP_16BITS_ADDRESS_MODE*/
            
            /* Sum */    
            #else
            
            #ifdef CCP_16BITS_ADDRESS_MODE
            Tx_Temp_First = CCP_Get_First_Byte(*((uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_1])));
            Tx_Temp_Second = CCP_Get_Second_Byte(*((uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_1])));
            /* Sum */
            Tx_Temp += (Tx_Temp_First + Tx_Temp_Second);
            #else
            Tx_Temp += (*((uint8 CCP_FAR)CCP_Global_Address(CCP_MTA[CCP_Index_1])));
            #endif    /*End of ifdef CCP_16BITS_ADDRESS_MODE*/
                  
            #endif    /*End of ifdef CCP_CHECKSUM_CRC2*/
            
            CCP_MTA[CCP_Index_1]++; 
            
            #ifdef CCP_16BITS_ADDRESS_MODE
            Check_Count = Check_Count - CCP_UINT8_2;
            #else
            Check_Count--;
            #endif   
        } while (Check_Count != CCP_DEFAULT_ZERO_32);
        
        /* Checksum calculation finished ? */
        if (CCP_CheckSumSize != CCP_DEFAULT_ZERO_32) 
        {
            /* Do nothing */
        }
        else
        {
            CCP_TX_Data[CCP_Index_3] = CCP_CHECKSUM_RESULT_BYTES;  
            CCP_TX_Data[CCP_Index_4] = CCP_Get_First_Byte(Tx_Temp);
            CCP_TX_Data[CCP_Index_5] = CCP_Get_Second_Byte(Tx_Temp);
            #ifdef CCP_CHECKSUM_CRC2   
            /* for CRC2, init value is 0xFFFF */
            Tx_Temp = CCP_ALL_BITS_SET_16;
            /* for SUM, init value is zero */
            #else
            Tx_Temp = CCP_DEFAULT_ZERO_16;
            #endif
            Ccp_DTO_Transmit();    
        }
    } 
#endif  /*End of ifdef CCP_CHECKSUM*/
}

#define CCP_STOP_SEC_PRIVATE_CODE
#include "MemMap.h"


