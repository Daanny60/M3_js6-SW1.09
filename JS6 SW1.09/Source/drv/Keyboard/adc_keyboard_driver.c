/**************************************************************************//**
 *
 *  \copyright  This software is the property of TAIKING Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              TAIKING Technologies.
 *
 *  \brief      This is the source file of the adc keyboard driver.
 *
 *  \file       adc_keyboard_driver.c
 *  \ingroup    driver
 *  \author     by.lin
 *
 *  \version    1.0.0
 *  \date       13/04/2021
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      01.00.00 | 13/04/2021 | by.lin           | N/A |
 *
******************************************************************************/



/*****************************************************************************
 *  Files Include
 *****************************************************************************/

#include "adc_keyboard_driver.h"
#include "keyboard_config.h"
#include "Adc_Drv.h"
#include "string.h"


//#if defined (ADC_KEYBOARD_FUN_ENABLE)
struct _tag_key_filter  stKeyAdcFilter[NUM_OF_ADC_KEYBOARD+NUMBER_OF_IO_KEYBOARD];
const uint8_t  U8TableAdcMap[NUM_OF_ADC_KEYBOARD_CHANNEL]= {ADC_KEY_RIGHT_AD1_INDEX,ADC_KEY_RIGHT_AD2_INDEX,ADC_KEY_LEDT_AD1_INDEX,ADC_KEY_LEDT_AD2_INDEX,ADC_KEY_LEDT_AD3_INDEX,ADC_KEY_LEDT_AD4_INDEX};  /////channel   map  adc
/**/
void vAdcKeyInit(void)
{
    memset(&stKeyAdcFilter,0,sizeof(struct _tag_key_filter)*NUM_OF_ADC_KEYBOARD_CHANNEL);
}
/*
compare voltage if in adc_lo  and adc_up,if ture  return 0,else 1;
*/
uint8_t u8AdcVoltCompare(uint16_t adc_result, uint16_t adc_lo, uint16_t adc_up)
{
    uint8_t sw_state;
    if((adc_result >= adc_lo) && (adc_result <= adc_up))
    {
        sw_state = 0x0;
    }
    else
    {
        sw_state = 0x01;
    }
    return (sw_state);
}
/*
filter if key  change in small  time
 */
void vAadcKeyboardFilter(void)
{
    uint8_t u8channel=0;
    adc_result_type adc_result;
    for(u8channel=0; u8channel<NUM_OF_ADC_KEYBOARD_CHANNEL; u8channel++)
    {
        adc_result = ADC_Result_Get(U8TableAdcMap[u8channel]);
        if(0==u8AdcVoltCompare(adc_result,ADC_SW_CHANNEL_0_RELEASE_LO,ADC_SW_CHANNEL_0_RELEASE_UP))               //////NUM  2n+0 ,2n+1  realeaed
        {
            if(stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt>0)
            {
                stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt--;
            }
            if(stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt>0)
            {
                stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt--;
            }
        }
        else if(0==u8AdcVoltCompare(adc_result,ADC_SW_CHANNEL_0_SW1_PRESSED_LO,ADC_SW_CHANNEL_0_SW1_PRESSED_UP))   //////NUM  2n+0   PRESSED
        {
            if(stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt < ADCFILTERMAX)
            {
                stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt++;
            }
            if(stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt>0)
            {
                stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt--;
            }
        }
        else if(0==u8AdcVoltCompare(adc_result,ADC_SW_CHANNEL_0_SW2_PRESSED_LO,ADC_SW_CHANNEL_0_SW2_PRESSED_UP))   //////NUM  2n+1 PRESSED
        {
            if(stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt>0)
            {
                stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt--;
            }
            if(stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt < ADCFILTERMAX)
            {
                stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt++;
            }
        }
        else if(0==u8AdcVoltCompare(adc_result,ADC_SW_CHANNEL_0_SW12_PRESSED_LO,ADC_SW_CHANNEL_0_SW12_PRESSED_UP))   //////NUM 2n and 2n+1 PRESSED
        {
            if(stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt< ADCFILTERMAX)
            {
                stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt++;
            }
            if(stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt< ADCFILTERMAX)
            {
                stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt++;
            }
        }
        else     //////////////////////////fun equal release
        {
             if(stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt>0)
            {
                stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt--;
            }
            if(stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt>0)
            {
                stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt--;
            }
        }
    }
}
/*
after filter,make sure key if pressed or not
*/
void  vAdcKeyCheck(void)
{
    ///////////////////////////adc filter
    uint8_t u8channel=0;
    vAadcKeyboardFilter();
    for(u8channel=0; u8channel<NUM_OF_ADC_KEYBOARD_CHANNEL; u8channel++)
    {
        if(ADCFILTERMAX == stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt)
        {
            stKeyAdcFilter[2*u8channel+0].u8keyFilterFlag   =  APP_KEY_VALUE_PRESSED;
            if(stKeyAdcFilter[2*u8channel+0].u32keyContinueTime<U32MAX)
            {
                stKeyAdcFilter[2*u8channel+0].u32keyContinueTime++;
            }
        }
        else    if(0==stKeyAdcFilter[2*u8channel+0].u8KeyFilterCnt)
        {
            stKeyAdcFilter[2*u8channel+0].u8keyFilterFlag    =APP_KEY_VALUE_RELEASE;
            stKeyAdcFilter[2*u8channel+0].u32keyContinueTime=0;
        }
        else   ////////key status  in change ,not ok
        {
        }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(ADCFILTERMAX == stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt)
        {
            stKeyAdcFilter[2*u8channel+1].u8keyFilterFlag   =  APP_KEY_VALUE_PRESSED;
            if(stKeyAdcFilter[2*u8channel+1].u32keyContinueTime<U32MAX)
            {
                stKeyAdcFilter[2*u8channel+1].u32keyContinueTime++;
            }
        }
        else    if(0 == stKeyAdcFilter[2*u8channel+1].u8KeyFilterCnt)
        {
            stKeyAdcFilter[2*u8channel+1].u8keyFilterFlag                   =APP_KEY_VALUE_RELEASE;
            stKeyAdcFilter[2*u8channel+1].u32keyContinueTime=0;
        }
        else   ////////key status  in change ,not ok
        {
        }
    }
    //////////////////////////io filter
}
/*
*/
uint8_t u8ReadAdcKeyFlag(uint8_t u8channel)
{
    uint8_t u8reslut=0;
    u8reslut=stKeyAdcFilter[u8channel].u8keyFilterFlag;
    return(u8reslut);
}
/*
*/
uint32_t u32ReadAdcKeyContinue(uint8_t u8channel)
{
    uint32_t u32result=0;
    u32result=stKeyAdcFilter[u8channel].u32keyContinueTime;
    return(u32result);
}
//#endif

