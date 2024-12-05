#include "Pwm.h"
#include "fm33lg0xx_fl_conf.h"
#include "gpio.h"
#include "App_Rgb.h"
#include "App_Led.h"

void ATIM_Init(void)
{
  FL_ATIM_InitTypeDef        InitStructer1;
  FL_ATIM_OC_InitTypeDef     InitStructer2;
  FL_ATIM_BDTR_InitTypeDef   InitStructer3;

  /*----------------------------------时基结构体初始化------------------------------------*/
  InitStructer1.clockSource           = FL_CMU_ATIM_CLK_SOURCE_APBCLK;  /* 时钟源选择APB2 */
  InitStructer1.prescaler             = 80-1;                         /* 分频系数8000 */
  InitStructer1.counterMode           = FL_ATIM_COUNTER_DIR_UP;         /* 向上计数 */
  InitStructer1.autoReload            = 3000;                         /* 自动重装载值5000 */
  InitStructer1.clockDivision         = FL_ATIM_CLK_DIVISION_DIV1;      /* 死区和滤波分频 */
  InitStructer1.repetitionCounter     = 0;                              /* 重复计数 */
  InitStructer1.autoReloadState       = FL_ENABLE;                      /* 预装载preload使能 */
  FL_ATIM_Init(ATIM, &InitStructer1);


  /*------------------------------输出比较结构体初始化------------------------------------*/
  InitStructer2.OCMode       = FL_ATIM_OC_MODE_PWM1;                    /* 输出比较模式PWM1 */
  InitStructer2.OCETRFStatus = FL_DISABLE;                              /* OC1REF不受ETR影响 */
  InitStructer2.OCFastMode   = FL_DISABLE;                              /* 关闭快速使能 */
  InitStructer2.compareValue = 0;                                       /* 比较值500 */
  InitStructer2.OCPolarity   = FL_ATIM_OC_POLARITY_NORMAL;              /* OC 高电平有效 */
  InitStructer2.OCPreload    = FL_DISABLE;                              /* OC preload 无效 */
  InitStructer2.OCIdleState  = FL_ATIM_OC_IDLE_STATE_LOW;              /* OC IDLE时为高电平 */
  InitStructer2.OCNIdleState = FL_ATIM_OCN_IDLE_STATE_HIGH;             /* OCN IDLE时为高电平 */
  InitStructer2.OCNPolarity  = FL_ATIM_OCN_POLARITY_NORMAL;             /* OCN 高电平有效 */
  InitStructer2.OCNState     = FL_DISABLE;                              /* 使能互补输出通道 */
  InitStructer2.OCState      = FL_ENABLE;                               /* 使能正向输出通道 */
  FL_ATIM_OC_Init(ATIM, FL_ATIM_CHANNEL_1, &InitStructer2);


  /*------------------------------刹车死区结构体初始化-----------------------------------*/
  InitStructer3.deadTime            = 100;                              /* 死区时间配置 */
  InitStructer3.lockLevel           = FL_ATIM_LOCK_LEVEL_OFF;           /* 寄存器写保护关闭 */
  InitStructer3.OSSRState           = FL_ATIM_OSSR_DISABLE;             /* OSSR=0 */
  InitStructer3.OSSIState           = FL_ATIM_OSSI_DISABLE;             /* OSSI=0 */
  InitStructer3.breakFilter         = FL_ATIM_BREAK_FILTER_DIV1;        /* 刹车信号滤波配置 */
  InitStructer3.breakPolarity       = FL_ATIM_BREAK_POLARITY_HIGH;      /* 刹车信号高电平有效 */
  InitStructer3.automaticOutput     = FL_DISABLE;                       /* MOE由软件置位 */
  InitStructer3.gatedBrakeSignal_1  = FL_ATIM_BREAK1_GATE_AUTO;         /* 刹车信号1不门控 */
  InitStructer3.gatedBrakeSignal_2  = FL_ATIM_BREAK2_GATE_AUTO;         /* 刹车信号2不门控 */
  InitStructer3.brakeSignalCombined = FL_ATIM_BREAK_COMBINATION_OR;     /* 刹车信号1和2或 */
  InitStructer3.breakState          = FL_DISABLE;                       /* 刹车信号禁止 */
  FL_ATIM_BDTR_Init(ATIM, &InitStructer3);

  FL_ATIM_Enable(ATIM);         /* 使能定时器 */

  FL_ATIM_EnableALLOutput(ATIM);/* 主控输出使能 */
  //////////////////////////////////////////  
}
void GPTIM1_Init(void)
{
    FL_GPTIM_InitTypeDef        timInit;
    FL_GPTIM_OC_InitTypeDef     timOCInit;   
    
    timInit.prescaler             = 80 - 1;                     /* 分频系数8000 */
    timInit.counterMode           = FL_GPTIM_COUNTER_DIR_UP;      /* 向上计数 */
    timInit.autoReload            = 3000;                     /* 自动重装载值1000 */
    timInit.clockDivision         = FL_GPTIM_CLK_DIVISION_DIV1;   /* 死区和滤波分频 */
    timInit.autoReloadState       = FL_ENABLE;                    /* 预装载preload使能 */
    FL_GPTIM_Init(GPTIM1, &timInit);  
    
    
    timOCInit.OCMode       = FL_GPTIM_OC_MODE_PWM1;               /* 输出比较模式PWM1 */
    timOCInit.OCETRFStatus = FL_DISABLE;                          /* OC1REF不受ETR影响 */
    timOCInit.OCFastMode   = FL_DISABLE;                          /* 关闭快速使能 */
    timOCInit.compareValue = 500;                                 /* 比较值500 */
    timOCInit.OCPolarity   = FL_GPTIM_OC_POLARITY_NORMAL;         /* OC正常输出 */
    timOCInit.OCPreload    = FL_DISABLE;                          /* OC preload 无效 */  
    FL_GPTIM_OC_Init(GPTIM1, FL_GPTIM_CHANNEL_1, &timOCInit);
    FL_GPTIM_OC_Init(GPTIM1, FL_GPTIM_CHANNEL_2, &timOCInit);
    FL_GPTIM_Enable(GPTIM1);    /* 使能定时器 */
}
void GPTIM0_Init(void)
{
    FL_GPTIM_InitTypeDef        timInit;
    FL_GPTIM_OC_InitTypeDef     timOCInit;   
  
   
    timInit.prescaler             = 80 - 1;                     /* 分频系数8000 */
    timInit.counterMode           = FL_GPTIM_COUNTER_DIR_UP;      /* 向上计数 */
    timInit.autoReload            = 3000;                     /* 自动重装载值1000 */
    timInit.clockDivision         = FL_GPTIM_CLK_DIVISION_DIV1;   /* 死区和滤波分频 */
    timInit.autoReloadState       = FL_ENABLE;                    /* 预装载preload使能 */
    FL_GPTIM_Init(GPTIM0, &timInit);

    timOCInit.OCMode       = FL_GPTIM_OC_MODE_PWM1;               /* 输出比较模式PWM1 */
    timOCInit.OCETRFStatus = FL_DISABLE;                          /* OC1REF不受ETR影响 */
    timOCInit.OCFastMode   = FL_DISABLE;                          /* 关闭快速使能 */
    timOCInit.compareValue = 300;                                 /* 比较值500 */
    timOCInit.OCPolarity   = FL_GPTIM_OC_POLARITY_NORMAL;         /* OC正常输出 */
    timOCInit.OCPreload    = FL_DISABLE;                          /* OC preload 无效 */   
    FL_GPTIM_OC_Init(GPTIM0, FL_GPTIM_CHANNEL_1, &timOCInit);   
    FL_GPTIM_Enable(GPTIM0);    /* 使能定时器 */
}
////////////////////////////////////////////////////////////T1CH4-RIGHT BLACKLIGHT
void vRgbRight_RedDuty(uint16_t duty)
     { 
         GPTIM1->CCR2=duty;
     }
///////////////////////////////////////////////////////////T0CH1-LEFT BLACKLIGHT

void vRgbLeft_RedDuty(uint16_t duty)
     { 
      GPTIM1->CCR1=duty;    
     }

void vHeatPwmDuty(uint16_t u16duty)
      {   
        if(u16duty)             FL_GPIO_SetOutputPin(HEATING_CTRL_GPIO,HEATING_CTRL_PIN);
        else                    FL_GPIO_ResetOutputPin(HEATING_CTRL_GPIO,HEATING_CTRL_PIN);
      }
void vBackLightPwmDuty(uint16_t u16duty)
      {       
       FL_ATIM_WriteCompareCH1(ATIM, u16duty);
          if((APP_MODE_NIGHT==u8ReadDayNight())&&(APP_MODE_NOACTVATIONG==u8ReadRGBMode()))  ////night && noactvation
          {
            vRgbLeft_RedDuty(u16duty);
            vRgbRight_RedDuty(u16duty);
          }
       else 
          {
            vRgbLeft_RedDuty(0);
            vRgbRight_RedDuty(0);
          }
      }
void PWM_Init(void)
      {
        ATIM_Init();
        GPTIM1_Init();
      //  GPTIM0_Init();      
        FL_ATIM_WriteCompareCH1(ATIM, 0);
       
        
      }