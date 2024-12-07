/****************************************************************************************
 *
 * File name: main.c
 * Project name:
 * Edition: v1.0
 * Data: 2022-04-19  15:22:14
 * Author: linbo
 * 
 ****************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mf_config.h"
#include "fm33lg0xx_fl.h"

#if defined(USE_FULL_ASSERT)
#include "fm33_assert.h"
#endif /* USE_FULL_ASSERT */

#include "Pll.h"
#include "Tim.h"
#include "CanTrcv.h"
#include "Can.h"
#include "CanIf.h"
#include "Com.h"
#include "Can_Pl.h"
#include "Gpio.h"
#include "Pwm.h"
#include "App_Led.h"
#include "Sleep.h"
#include "types.h"
#include "CanTp.h"
//#include "CanNm.h"
#include "OsekNm.h"
#include "Dcm.h"
#include "Adc_Drv.h"
#include "DMA.h"
#include "system_voltage_manage.h"
#include "keyboard_driver.h"
#include "App_Keyboard.h"
#include "App_WheelHeat.h"
#include "EncoderDrv.h"
#include "App_Encoder.h"
#include "LowPowerManage.h"
#include "Event.h"
#include "App_Rgb.h"
#include "eeprom.h"
#include "eeprom_cfg.h"
#include "Demm.h"
#include "App_Demm.h"
#include "App_Can.h"
#include "svd.h"
#include "svd_interrupt.h"
#include "App_Boot.h"
#if defined (__ICCARM__)
__root const uint32_t u32AppValidFlag __attribute__((section(".AppValidFlag"))) = 0xe504aaaaul;
__root const uint32_t u32AppValidCRC32 __attribute__((section(".AppValidCRC32"))) = 0x3D4206ED;//0xD192104A;//0xA1ABB5EF;// 0x0DEB3081 
__root uint32_t u32ReprogramFlag __attribute__((section(".ReprogramFlag")));
//#else

#endif //



void HardFault_Handler(void)
     {
       NVIC_SystemReset();
     }




int main (void)
{

  static SystemTimeType timer_1ms_a  = 0;
  static SystemTimeType timer_5ms_a  = 1;
  static SystemTimeType timer_10ms_b = 2; 
  static SystemTimeType timer_2ms_b = 1; 
  static SystemTimeType timer_2ms_a  = 0;
  static  SystemTimeType timer_100ms = 0;
  SystemTimeType now_time;
  
   vWdtInit(); 
  __disable_irq();

  /* 中断重映射 */
  SCB->VTOR = 0x00008100;
  FL_RMU_BOR_SetThreshold(RMU, FL_RMU_BOR_THRESHOLD_2P40V);
  FL_RMU_BOR_Enable(RMU);
  FL_Init();
  /* XTHF 8M倍频至48M */
  SelXTHFToPLL(FL_CMU_PLL_PSC_DIV8, 48 - 1);
  SystemCoreClockUpdate();
  FL_IWDT_ReloadCounter(IWDT);
  SVD_Init(SVD_MONTIOR_VDD,FL_SVD_WARNING_THRESHOLD_GROUP11,FL_SVD_REFERENCE_1P0V);
  while(false == SVD_Result_Confirmed(SVD_HIGHER_THRESHOLD, 2000U/*us*/));
  EE_Init();
  EE_Flash_Init();
  dem_task_init();
  BSTIM16_Init();
  GPIO_Init();
  ADC_Init();
  DMA_Init();
  ADC_Start();
  LowPowerManageInit();
  EventInit();
  vAppCanInit();
  App_Demm_Init();
  App_wheelHeatInit();
  FL_IWDT_ReloadCounter(IWDT);
  Can_Init();
  CanIf_Init();
  (void)CanIf_SetControllerMode(0,CANIF_CS_STARTED);
  Com_Init();  
  Com_TxStart();
  Com_RxStart();	
  CanNm_Init();
  CanTp_Init(((void *) 0));
  Dcm_Init(((void *) 0));
  CanTrcv_Init();
  NVIC_DisableIRQ(CAN_IRQn);
  NVIC_SetPriority(CAN_IRQn, 0);/* 中断优先级配置 */
  NVIC_EnableIRQ(CAN_IRQn); 
  CanNm_GotoMode(0, GOTO_AWAKE);
  FL_SVD_DisableDigitalFilter(SVD);
  SVD_Interrupt_Init(FL_DISABLE);
  __enable_irq();

  Boot_CheckBLUpgrade();
  for (;;)
  {

    now_time = Get_SystemTime();
    FL_IWDT_ReloadCounter(IWDT);
    if((SystemTimeType)(now_time - timer_1ms_a) >= 1)
    {
      //EncoderDrv_Task();
      timer_1ms_a = now_time;
    }

    if((SystemTimeType)(now_time - timer_5ms_a) >= 5)
    {
      /*user code*/
      (void)Com_MainFunctionRx();
      (void)Com_MainFunctionTx();
      //(void)CanNm_MainFunction();
      (void)Dcm_MainFunction();
      (void)CanTp_MainFunction();
       timer_5ms_a = now_time;
    }

    if((SystemTimeType)(now_time - timer_10ms_b) >= 10)
    { 
      (void)system_voltage_manage_task();
      (void)LowPowerManageTask();
      (void)vAppCanTask();
      (void)App_Demm_Task();
      (void)dem_task();
      (void)App_HeatSignal_Task();
      (void)App_WheelHeat_Task();      
      timer_10ms_b = now_time;
    }

    if((SystemTimeType)(now_time - timer_2ms_a) >= 2)
    {
      (void)ADC_Task();    
      timer_2ms_a = now_time;
    }


    if((SystemTimeType)(now_time - timer_2ms_b) >= 2)
    {
      (void)EE_Flash_Task();
      timer_2ms_b = now_time;
    }
    
     if((SystemTimeType)(now_time - timer_100ms) >= 30)
    {      
      timer_100ms = now_time;
      SystemVoltageDTCTask();
    }
    
 
    
  }

}

