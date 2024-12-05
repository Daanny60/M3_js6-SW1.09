#include <string.h>
#include "mf_config.h"
#include "fm33lg0xx_fl.h"

#if defined(USE_FULL_ASSERT)
#include "fm33_assert.h"
#endif /* USE_FULL_ASSERT */
#include "App_Can.h"
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
#include "App_Rgb.h"
#include "App_Led.h"
#include "Event.h"
#include "App_WheelHeat.h"
#include "OsekNm.h"
#include "App_Demm.h"
//#include "CanNm.h"
//#include "CanNm_Lcfg.h"
#define NM_SLEEP_IND_TIME 4500 / 10

extern bool read_nm_can_sleep_flag(void);
extern bool clear_nm_can_sleep_flag(void);

typedef enum
{
     LOW_POWER_NORMAL = 0,
     LOW_POWER_PRE_SLEEP,
     LOW_POWER_SLEEP,
}LowPowerState_e;

typedef struct 
{
	LowPowerState_e State;
}LowPowerManageTypeDef;

LowPowerManageTypeDef LowPowerManage;

static void EXTI_interrupt_init(void);
static void LowPowerManageWakeUpProcess(void);
static void LowPowerManageSleepProcess(void);
const uint8_t u8UserData[6]={0x00,0x00,0x00,0x00,0x01,0xcc};




void vWdtInit(void)
    {
      FL_IWDT_InitTypeDef IWDT_InitStruct;
       IWDT_InitStruct.iwdtWindows    = 0;   
       IWDT_InitStruct.overflowPeriod = FL_IWDT_PERIOD_125MS; /*最长溢出时间*/
       FL_IWDT_Init(IWDT, &IWDT_InitStruct);
       FL_IWDT_EnableFreezeWhileSleep(IWDT);
    }
/*  init*/
void LowPowerManageInit(void)
{
    memset(&LowPowerManage, 0, sizeof(LowPowerManage));
    //memset(&spbusoff, 0, sizeof(spbusoff));
    //vSetTImeOutCycle();
}


void LowPowerManageSleepProcess(void)
{
    FL_BSTIM16_DeInit(BSTIM16);
    FL_ADC_DeInit(ADC);
    FL_DMA_DeInit(DMA);
    FL_GPTIM_DeInit(GPTIM1);
    FL_GPTIM_DeInit(GPTIM0);  
    FL_ATIM_DeInit(ATIM);
    FL_ATIM_DisableALLOutput(ATIM);
    vGpioDeinit();
    CanTrcv_Disable();
    EXTI_interrupt_init();
    FL_IWDT_ReloadCounter(IWDT);
    Sleep();
}


static void LowPowerManageWakeUpProcess(void)
{
    NVIC_DisableIRQ(GPIO_IRQn);
    FL_IWDT_ReloadCounter(IWDT);
    FL_RMU_BOR_SetThreshold(RMU, FL_RMU_BOR_THRESHOLD_2P40V);
    FL_RMU_BOR_Enable(RMU);
    FL_Init();
    /* XTHF 8M倍频至48M */
    SelXTHFToPLL(FL_CMU_PLL_PSC_DIV8, 48 - 1);
    SystemCoreClockUpdate();
  
    BSTIM16_Init();
    GPIO_Init();
    CanTrcv_Init();
    ADC_Init();
    DMA_Init();
    ADC_Start();
    LowPowerManageInit();
    EventInit();
    vAppCanInit();
    App_wheelHeatInit();
    FL_IWDT_ReloadCounter(IWDT);
    Can_Init();
    CanIf_Init();
    (void)CanIf_SetControllerMode(0,CANIF_CS_STARTED);
    Com_Init();    
    Com_RxStart();	
    CanNm_Init();
    CanTp_Init(((void *) 0));
    Dcm_Init(((void *) 0));
    App_Demm_Init();
    NVIC_DisableIRQ(CAN_IRQn);
    NVIC_SetPriority(CAN_IRQn, 0);/* 中断优先级配置 */
    NVIC_EnableIRQ(CAN_IRQn);
 

}

/**
  * @brief  引脚中断
  * @param  void
  * @retval void
  */
void GPIO_IRQHandler(void)
{
    if(FL_GPIO_IsActiveFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_1))
    {
        
        FL_GPIO_ClearFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_1);
        NVIC_ClearPendingIRQ(GPIO_IRQn);
    }
}

/**
  * @brief  外部引脚中断初始化
  * @param  void
  * @retval void
  */
static void EXTI_interrupt_init(void)
{
    FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    FL_EXTI_CommonInitTypeDef EXTI_CommonInitStruct = {0};
    FL_EXTI_InitTypeDef EXTI_InitStruct = {0};


    /* 引脚边沿触发选择 */
    FL_CMU_EnableEXTIOnSleep();                                 /* 休眠使能外部中断采样 */
    FL_CMU_EnableGroup1BusClock(FL_CMU_GROUP1_BUSCLK_PAD);      /* 使能时钟总线 */
    FL_CMU_EnableGroup3OperationClock(FL_CMU_GROUP3_OPCLK_EXTI);/* EXTI工作时钟使能 */

    /* 用到的GPIO引脚    设为输入 */
    GPIO_InitStruct.pin        = FL_GPIO_PIN_6;
    GPIO_InitStruct.mode       = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull       = FL_DISABLE;
    GPIO_InitStruct.remapPin   = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    EXTI_CommonInitStruct.clockSource = FL_CMU_EXTI_CLK_SOURCE_LSCLK;
    FL_EXTI_CommonInit(&EXTI_CommonInitStruct);

    EXTI_InitStruct.filter         = FL_DISABLE;
    EXTI_InitStruct.input          = FL_GPIO_EXTI_INPUT_GROUP2;
    EXTI_InitStruct.triggerEdge    = FL_GPIO_EXTI_TRIGGER_EDGE_BOTH;
    FL_EXTI_Init(FL_GPIO_EXTI_LINE_1, &EXTI_InitStruct);

    /* NVIC中断配置 */
    NVIC_DisableIRQ(GPIO_IRQn);
    NVIC_SetPriority(GPIO_IRQn, 2); /* 中断优先级配置 */
    NVIC_EnableIRQ(GPIO_IRQn);
}



void vPowerSleep(void)
{
      LowPowerManageSleepProcess();      /////sleep function
      LowPowerManageWakeUpProcess();     //////wakeup function
}


uint16_t timer = 0;
uint8_t LowPowerManageGotoSleepConditionIsTrue(void)
{
  
      uint8_t ret;
      uint8_t u8heatstatus;
      ret = 0;      
      u8heatstatus = u8HeatGetStatus();  
      if(0 == u8heatstatus)
      {
          ret = 1;
      }
      return (ret);
}

void LowPowerManageTask(void)
{
    uint8_t ret;
    switch (LowPowerManage.State)
    {
        case LOW_POWER_NORMAL:
            ret = LowPowerManageGotoSleepConditionIsTrue();

            if(ret)
            {
                CanNm_GotoMode(0, GOTO_BUSSLEEP);
                LowPowerManage.State = LOW_POWER_PRE_SLEEP;
            }
            break;
        case LOW_POWER_PRE_SLEEP:
            ret = read_nm_can_sleep_flag();
            clear_nm_can_sleep_flag();
            if(ret)
            {
                LowPowerManage.State = LOW_POWER_SLEEP;
            }
            
            ret = LowPowerManageGotoSleepConditionIsTrue();
            if(ret == 0)
            {
                CanNm_GotoMode(0, GOTO_AWAKE);
                LowPowerManage.State = LOW_POWER_NORMAL;
            }
            
            break;
        case LOW_POWER_SLEEP:
            LowPowerManageSleepProcess();
            LowPowerManageWakeUpProcess();
            LowPowerManage.State = LOW_POWER_NORMAL;
            break;
        default:
            LowPowerManage.State = LOW_POWER_NORMAL;
            break;
    }
}



