/**************************************************************************//**
 *
 *  \copyright  This software is the property of TAIKING Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              TAIKING Technologies.
 *
 *  \brief      This is the source file of the keyboard config.
 *
 *  \file       keyboard config.h
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

#ifndef __KEYBOARD_CONFIG_H
#define __KEYBOARD_CONFIG_H

/*****************************************************************************
 *  Files Include
 *****************************************************************************/
#include "fm33lg0xx_fl_conf.h"
#include "types.h"



typedef unsigned short int KeyboardContinueTimeType;
typedef unsigned short int ADC_KeyboardChannelStateType;
/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/

#define IO_KEYBOARD_FUN_ENABLE             
#define ADC_KEYBOARD_FUN_ENABLE          
  


#define KEYBOARD_TASK_PERIOD_TIME 2        //////按键的调用周�?
#define KEYBOARD_DEBOUNCE_TIME      30 / KEYBOARD_TASK_PERIOD_TIME          /*30MS  Debounce Time*/

#define ADCFILTERMAX             10            ///////adc 计数的最大次�?
#define GPIOFILTERMAX            10           /////////io 计数的最大次�?
#define U32MAX                   0xffffffff   ////////4字节的最大数�?

#define  KEYTXBUFNUM            7
/*key driver  used */
struct _tag_key_filter
{
    uint32_t  u32keyContinueTime;  //////continue time
    uint8_t   u8KeyFilterCnt;      //////filter cnt
    uint8_t   u8keyFilterFlag;     /////filer result
    uint8_t   u8KeyStatus;         //////key status;
};
/* key app used */
struct _tagAppKeyTx
{
    uint8_t u8TimerCnt;       /////timer  cnt   
    uint8_t u8Value;          /////tx data value
    uint8_t u8TxId;           //// txid
    uint8_t u8Wptr;          /////write point
    uint8_t u8rptr;          /////read point
    uint8_t u8buf[KEYTXBUFNUM];       //////buf
};
////////////////////////////////////////

/* 按键配置
以下三个宏定义，三选一
*/
#define    SWSCONFIG_HIGH      //ZF01SFH01
//#define    SWSCONFIG_MID         //ZF01SFH03
//#define    SWSCONFIG_LOW       //ZF01SFH02

#ifdef SWSCONFIG_HIGH
#define APP_KEYFUN_NUM                 17    /////key num 10+7=17
#else
#define APP_KEYFUN_NUM                 15    /////key num 10+7=17
#endif

#define APP_KEYENCODERFUN_NUM          6     /////encoder 3*2=6

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
#define APP_KEYBOARD_PERIOD_TIME  2       ///////task time 

#define TXCYCLE20MS              (20/APP_KEYBOARD_PERIOD_TIME) 
#define TXCYCLE100MS             (100/APP_KEYBOARD_PERIOD_TIME)              ///////delay 100ms
#define SILENCE_TIME_300MS       (300/APP_KEYBOARD_PERIOD_TIME)
#define LONG_PRESSED_TIME_400MS  (400/APP_KEYBOARD_PERIOD_TIME)
#define LONG_PRESSED_TIME_600MS  (600/APP_KEYBOARD_PERIOD_TIME)
#define LONG_PRESSED_TIME_1S     (1000/APP_KEYBOARD_PERIOD_TIME)
#define MULTI_KEY_TIME_10S       (10*1000/APP_KEYBOARD_PERIOD_TIME)
#define STUCK_TIME_300S          (300*1000/APP_KEYBOARD_PERIOD_TIME)    //////for test 100s,->300
#define STUCK_TIME_20S           (20*1000/APP_KEYBOARD_PERIOD_TIME)

#define     KEY_NORMALPRESS_TX_NUM   3        /////      normal key tx num 
#define     KEY_SHORTLONG_TX_NUM     1        //////short_long  key tx num 

#define     KEY_RELEASE_TX_NUM       3U         ////key released    tx num 
#define     KEY_RELEASE_TX_VALUE     0U         ////key released    tx value

#define      APP_KEY_MODE_IN_NUM     3
#define      APP_KEY_MODE_IN_VALUE   1

#define      APP_KEY_MODE_EXIT_NUM     3
#define      APP_KEY_MODE_EXIT_VALUE   1


/* 车展信号的对应ID*/
#define    APP_KEY_CHEZHAN_IN_ID         6
#define    APP_KEY_CHEZHAN_OUT_ID        7
/* 长按功能使能开关*/
#define    APP_KEY_LONGPRESSED_ENABLE   1U
#define    APP_KEY_LONGPRESSED_DISABLE  0U
/* 按下及弹起对应的数值*/
#define  APP_KEY_VALUE_RELEASE            0
#define  APP_KEY_VALUE_PRESSED            1
/*****************************************************************************
 *  ADC Keyboard Min Value Macro
 *****************************************************************************/
/* 所有參數都已轉換成電壓形式 誤差�?.25V */
#if defined (ADC_KEYBOARD_FUN_ENABLE)
#define NUM_OF_ADC_KEYBOARD_CHANNEL 6
#define NUM_OF_ADC_KEYBOARD_CH1_KEY 2
#define NUM_OF_ADC_KEYBOARD         (NUM_OF_ADC_KEYBOARD_CHANNEL*NUM_OF_ADC_KEYBOARD_CH1_KEY)
#define ADC_KEYBOARD_ERROR_VALUE    200u


#define ADC_SW_CHANNEL_0_RELEASE_LO         (4095u - 2*ADC_KEYBOARD_ERROR_VALUE)
#define ADC_SW_CHANNEL_0_SW1_PRESSED_LO     (2234u - ADC_KEYBOARD_ERROR_VALUE)
#define ADC_SW_CHANNEL_0_SW2_PRESSED_LO     (2730u - ADC_KEYBOARD_ERROR_VALUE)
#define ADC_SW_CHANNEL_0_SW12_PRESSED_LO    (1755u - ADC_KEYBOARD_ERROR_VALUE)
#define ADC_SW_CHANNEL_0_SHORT_TO_GND_LO    (0U)
#define ADC_SW_CHANNEL_0_RELEASE_UP         (4095u)
#define ADC_SW_CHANNEL_0_SW1_PRESSED_UP     (2234u + ADC_KEYBOARD_ERROR_VALUE)
#define ADC_SW_CHANNEL_0_SW2_PRESSED_UP     (2730u + ADC_KEYBOARD_ERROR_VALUE)
#define ADC_SW_CHANNEL_0_SW12_PRESSED_UP    (1755u + ADC_KEYBOARD_ERROR_VALUE)
#define ADC_SW_CHANNEL_0_SHORT_TO_GND_UP    (0U    + 2*ADC_KEYBOARD_ERROR_VALUE)


#endif
////////////////////////////////////////////////////////////////////////////////////////////
////////key  sequence  ADC

#define   SWS_KEY_RIGHT_VOICE_INDEX           0                ////RIGHT_S1:  RIGHT VOICE
#define   SWS_KEY_RIGHT_BACK_INDEX            1             /////// RIGHT S5： <
#define   SWS_KEY_RIGHT_SILENCE_INDEX         2             /////   RIGHT S6:  SILICE
#define   SWS_KEY_RIGHT_STAR_INDEX            3             ////    RIGHT S7:  STAR
#define   SWS_KEY_LEFT_CROSSCOUNTRY_INDEX     4               /////LEFT_S1:    YUE YE

#define   SWS_KEY_LEFT_MEDIA_INDEX            5               //// LEFT_S2:    360 MEDIA
#define   SWS_KEY_LEFT_SPD_ADD_INDEX          6                ////LEFT_S3:   LEFT SPD+
#define   SWS_KEY_LEFT_DIS_ADD_INDEX          7                ////LEFT_S4:   LEFT DIS+
#define   SWS_KEY_LEFT_DIS_DEC_INDEX          8                ////LEFT_S5:   LEFT DIS-
#define   SWS_KEY_LEFT_SPD_DEC_INDEX          9                ////LEFT_S6:   LEFT SPD-

#define   SWS_KEY_LEFT_INFORMATION_INDEX      10               ////LEFT_S7:   LEFT MODE
#define   SWS_KEY_LEFT_ACC_INDEX              11               /////LEFT_S8:   LEFT *
///////KEY SEQUENCE  GPIO
#define   SWS_KEY_RIGHT_PHONE_INDEX           12             /////// RIGHT_S4:  RIGHT PHONE
#define   SWS_KEY_RIHT_NEXT_INDEX             13                ////RIGHT_S2:  RIGHT NEXT
#define   SWS_KEY_RIGHT_MENU_INDEX            14                ////RIGHT_S3:  RIGHT MENU

#define   SWS_KEY_LEFT_PICK_INDEX             15                ////LEFT PICK
#define   SWS_KEY_RIGHT_PICK_INDEX            16                /////RIGHT PICK


#define   SWS_KEY_UNUSED              255
/*****************************************************************************
 *  Global Structure Definitions
 *****************************************************************************/

#define NUMBER_OF_IO_KEYBOARD 5


GPIO_Type* io_keyboard_gpio_get(uint8_t keyboard_id);
uint32_t io_keyboard_gpio_pin_get(uint8_t keyboard_id);


#endif

