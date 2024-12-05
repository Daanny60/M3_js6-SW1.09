/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : App_Ccp.c
********************************************************************************
*   Project/Product : CCP module
*   Title           : App_Ccp.c
*   Author          : Peng.Liu
********************************************************************************
*   Description     : This file includes some callback functions which are used
*                     to achieve application related function, such as the 
*                     Seed&Key and Program.
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
*   02.20.00    17/02/14      Wenrui.Li     N/A          HrAsrCcp140217-01 
********************************************************************************
* END_FILE_HDR*/

/*******************************************************************************
*   Includes 
*******************************************************************************/
#include "Ccp.h"

/*******************************************************************************
*   Macro definition 
*******************************************************************************/
/* change the macro according to the Min write byte of Flash Driver */
#define CCP_FLASH_WRITE_BYTES   128u

/*******************************************************************************
*   Variable definition 
*******************************************************************************/
#define CCP_START_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/*Program Related*/
#ifdef CCP_FLASH
///STATIC VAR(uint32,CCP_PRIVATE_DATA) CCP_Flash_Address_Temp = 0u;
///STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_Flash_Buffer_Temp[CCP_FLASH_WRITE_BYTES];
///STATIC VAR(uint8,CCP_PRIVATE_DATA)  CCP_Write_counter_Temp = 0u;

#define CCP_STOP_SEC_VAR_UNSPECIFIED
#include "MemMap.h"

/*******************************************************************************
*   Function definition 
*******************************************************************************/
#define CCP_START_SEC_PUBLIC_CODE
#include "MemMap.h"

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:     App_Ccp_FlashErase 
*                
* Description:       This functin is used to erase non-volatile memory. 
*                             
* Inputs:            uint32 Flash_Erase_Ptr: Address to be erased.
*                    uint32 size           : Size to be erased.
*                   
* Outputs:           Std_ReturnType
*                    E_OK:                   Erasing completed. 
*                    E_NOT_OK:               Erasing failure.
*
* Limitations:       None
********************************************************************************
END_FUNCTION_HDR*/

Std_ReturnType App_Ccp_FlashErase(uint32 Flash_Erase_Ptr, uint32 size) 
{
    /*demo code*/
    /*
    Ccp_DisableInterrupt();


    FlsIf_Erase(Flash_Erase_Ptr, size);

    Ccp_EnableInterrupt();
    */

    return E_OK;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    App_Ccp_FlashWrite 
*                
* Description:      This functin is used to write non-volatile memory. 
*                             
* Inputs:           uint8* data:            Source Address to get data.
*                   uint32 Flash_Erase_Ptr: Target Address to be wrote.
*                   uint32 size:            Size to be wrote.
*                   
* Outputs:          Std_ReturnType
*                   E_OK:                   Writing completed. 
*                   E_NOT_OK:               Writing failure.
*
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

Std_ReturnType App_Ccp_FlashWrite(uint8* data, uint32 Flash_Write_Ptr, uint32 size) 
{
    uint8 Ret = E_OK;
    /* demo code, can only write CCP_FLASH_WRITE_BYTES bytes one time */
    #if 0
    uint8 count = 0u;

    Ccp_DisableInterrupt();

    if(size==0u)
    {
        Ret = FlsIf_Write(CCP_Flash_Address_Temp, CCP_Write_counter_Temp, CCP_Flash_Buffer_Temp);    
        CCP_Write_counter_Temp = 0u;
    }
    else 
    {
        for(count = 0u; count < size; count++)
        {
            /* write CCP_FLASH_WRITE_BYTES bytes one time */
            if(CCP_Write_counter_Temp < CCP_FLASH_WRITE_BYTES)
            {
                if(CCP_Write_counter_Temp == 0u)
                {
                    CCP_Flash_Address_Temp = Flash_Write_Ptr;
                }
                else
                {
                    /*do nothing*/
                }
                CCP_Flash_Buffer_Temp[CCP_Write_counter_Temp] = data[count];
                CCP_Write_counter_Temp++;
            }
            else
            {
                Ret = FlsIf_Write(CCP_Flash_Address_Temp, CCP_Write_counter_Temp, CCP_Flash_Buffer_Temp);
                CCP_Write_counter_Temp = 0u;
                CCP_Flash_Address_Temp += CCP_FLASH_WRITE_BYTES;
                CCP_Flash_Buffer_Temp[CCP_Write_counter_Temp]= data[count];
                CCP_Write_counter_Temp++;
            }
        }
    }
    
    Ccp_EnableInterrupt();
    
    #endif 
    
    return Ret;      
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    App_Ccp_FlashRead 
*                
* Description:      This functin is used to read data from non-volatile memory. 
*                             
* Inputs:           uint8* data:            Address to store read data.
*                   uint32 Flash_Erase_Ptr: Source Address to get date.
*                   uint32 size:            Size to be read.
*                   
* Outputs:          Std_ReturnType
*                   E_OK:                   Reading completed. 
*                   E_NOT_OK:               Reading failure.
*
* Limitations:      None
********************************************************************************
END_FUNCTION_HDR*/

Std_ReturnType App_Ccp_FlashRead(uint8* data, uint32 Flash_Read_Ptr, uint32 size) 
{
    uint8 count;
    for(count = 0u; count < size; count++)
    {
        data[count] = *(((uint8 CCP_FAR)Flash_Read_Ptr) + count);
    }
    return E_OK;
}
#endif

#ifdef CCP_SEED_KEY
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    App_Ccp_GetSeed 
*                
* Description:      This functin is used to get seed to unlock resource. 
*                             
* Inputs:           uint8 Resource_Mask:    Resource type.
*                   
* Outputs:          uint32:                 Seed value
*
* Limitations:      Only used when Seed&Key enable.
********************************************************************************
END_FUNCTION_HDR*/

uint32 App_Ccp_GetSeed(uint8 Resource_Mask) 
{
    return 888;
}

/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    App_Ccp_Unlock 
*                
* Description:      This functin is used unlock resource. 
*                             
* Inputs:           uint32 key: Calculated key value.
*                   
* Outputs:          uint8:      If key is right, return CCP_RIGHT_KEY. 
*                               Otherwise, return CCP_WRONG_KEY.
*
* Limitations:      Only used when Seed&Key enable.
********************************************************************************
END_FUNCTION_HDR*/

uint8 App_Ccp_Unlock(uint32 key)
{
    #if 0 
    if (key != 888)
    {
        return CCP_WRONG_KEY;
    }
    else
    {
        return CCP_RIGHT_KEY;
    }
    #endif
    
    return CCP_RIGHT_KEY;
}
#endif

#ifdef  CCP_CAL_PAGE
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    App_Ccp_Select_Page_Indication 
*                
* Description:      This functin is used to indicate selecting page. 
*                             
* Inputs:           None page_index:       Selected page index.
*                   
* Outputs:          Std_ReturnType
*                   E_OK:                   Selecting completed. 
*                   E_NOT_OK:               Selecting failure.
*
* Limitations:      Only used when Page enable.
********************************************************************************
END_FUNCTION_HDR*/

Std_ReturnType App_Ccp_Select_Page_Indication(uint8 page_index)
{
    return E_OK;
}
#endif

#ifdef CCP_STORE_CAL_DATA
/* BEGIN_FUNCTION_HDR
********************************************************************************
* Function Name:    App_Ccp_Store_Cal_Data 
*                
* Description:      This functin is used to store calbration data to 
*                   non-volatile memory when disconnect. 
*                             
* Inputs:           None
*                   
* Outputs:          None
*
* Limitations:      Only used when Page enable.
********************************************************************************
END_FUNCTION_HDR*/

void App_Ccp_Store_Cal_Data(void)
{
    /*add your codes here*/
}
#endif

#define CCP_STOP_SEC_PUBLIC_CODE
#include "MemMap.h"


