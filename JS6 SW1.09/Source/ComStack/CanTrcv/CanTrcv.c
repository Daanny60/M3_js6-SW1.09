/*  BEGIN_FILE_HDR
************************************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information contained in this 
*   doc should not be reproduced, or used, or disclosed without the written authorization from 
*   HiRain Technologies.
************************************************************************************************
*   File Name       : CanTrcv.c
************************************************************************************************
*   Project/Product : All
*   Title           : CanTrcv module source File
*   Author          : sijia.hao
************************************************************************************************
*   Description     : 
*
************************************************************************************************
*   Limitations     : only used for MSCAN
*
************************************************************************************************
*
************************************************************************************************
*   Revision History:
* 
*   Version     Date          Initials      CR#          Descriptions
*   ---------   ----------    ------------  ----------   ---------------
*   1.0         2014/06/01    sijia.hao     N/A          Original
*
************************************************************************************************
* END_FILE_HDR*/
#include "CanTrcv.h"
#include "mf_config.h"
#include "fm33lg0xx_fl.h"
/* BEGIN_FUNCTION_HDR
************************************************************************************************
* Function Name : CanTrcv_Init
*
* Description   : Init CanTrcv Moudle 
*
* Inputs        : void 
*                 
*
* Outputs       : None
* 
* Limitations   : None
*                 
************************************************************************************************
END_FUNCTION_HDR */




void CanTrcv_Init(void)
{   
  FL_GPIO_InitTypeDef      GPIO_InitStruct = {0};

  /* STB引脚初始化 */
  GPIO_InitStruct.pin          = FL_GPIO_PIN_8;  
  GPIO_InitStruct.mode         = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.pull         = FL_DISABLE;
  GPIO_InitStruct.outputType   = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.remapPin     = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*CAN_STB拉低，使能CAN收发器,可根据实际项目进行调整*/    
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_8);   

}

void CanTrcv_Disable(void)
{   
  /*CAN_STB拉低，使能CAN收发器,可根据实际项目进行调整*/    
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_8);  
}

void CanTrcv_Enable(void)
{   
  /*CAN_STB拉低，使能CAN收发器,可根据实际项目进行调整*/    
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_8);  
}