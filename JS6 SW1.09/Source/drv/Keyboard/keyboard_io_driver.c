/**************************************************************************//**
 *
 *  \copyright  This software is the property of TAIKING Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              TAIKING Technologies.
 *
 *  \brief      This is the source file of the keyboard io driver.
 *
 *  \file       keyboard_io_driver.c
 *  \ingroup    driver
 *  \author     by.lin
 *
 *  \version    1.0.0
 *  \date       09/04/2021
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      01.00.00 | 09/04/2021 | by.lin           | N/A |
 *
******************************************************************************/



/*****************************************************************************
 *  Files Include
 *****************************************************************************/
#include "fm33lg0xx_fl_conf.h"
#include "keyboard_io_driver.h"
#include "keyboard_config.h"
#include "gpio.h"


/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/


/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/

extern  struct _tag_key_filter  stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+NUMBER_OF_IO_KEYBOARD];

/*
读取按键及并进行滤波
*/
void vGpioKeyboardFilter(void)
{
    uint8_t u8channel=0;
    GPIO_Type* gpio;
    uint32_t gpio_pin;
    for(u8channel=0; u8channel<NUMBER_OF_IO_KEYBOARD; u8channel++)
    {
        gpio          = io_keyboard_gpio_get(u8channel);
        gpio_pin      = io_keyboard_gpio_pin_get(u8channel);
        if(0==FL_GPIO_GetInputPin(gpio,gpio_pin))
        {
            if(stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u8KeyFilterCnt < GPIOFILTERMAX )
            {
                stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u8KeyFilterCnt++;
            }
        }
        else
        {
            if(stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u8KeyFilterCnt > 0 )
            {
                stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u8KeyFilterCnt--;
            }
        }
    }
}
/*
根据滤波计数器判断按键状�?*/
void  vGpioKeyCheck(void)
{
    uint8_t u8channel=0;
    vGpioKeyboardFilter();
    for(u8channel=0; u8channel<NUMBER_OF_IO_KEYBOARD; u8channel++)
    {
        if(GPIOFILTERMAX==stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u8KeyFilterCnt)
        {
            stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u8keyFilterFlag=APP_KEY_VALUE_PRESSED;
            if(stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u32keyContinueTime<U32MAX)
            {
                stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u32keyContinueTime++;
            }
        }
        else if(0==stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u8KeyFilterCnt)
        {
            stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u8keyFilterFlag=APP_KEY_VALUE_RELEASE;
            stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+u8channel].u32keyContinueTime=0;
        }
        else
        {
        }
    }
}






