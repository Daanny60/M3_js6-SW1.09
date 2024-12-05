#include "Pll.h"
#include "fm33lg0xx_fl_conf.h"
#include "fm33lg0xx_fl.h"
/**
  * @brief    
  * @param    Source 锁相环输入参考时钟选择 
This parameter can be one of the following values:
  *           @arg @ref FL_CMU_PLL_CLK_SOURCE_RCHF
  *           @arg @ref FL_CMU_PLL_CLK_SOURCE_XTHF
  * @param    PLL_REFPSC 参考时钟分频, 目标分频值ֵ1M 
This parameter can be one of the following values:
  *           @arg @ref FL_CMU_PLL_PSC_DIV1
  *           @arg @ref FL_CMU_PLL_PSC_DIV2
  *           @arg @ref FL_CMU_PLL_PSC_DIV4
  *           @arg @ref FL_CMU_PLL_PSC_DIV8
  *           @arg @ref FL_CMU_PLL_PSC_DIV12
  *           @arg @ref FL_CMU_PLL_PSC_DIV16
  *           @arg @ref FL_CMU_PLL_PSC_DIV24
  *           @arg @ref FL_CMU_PLL_PSC_DIV32
  * @param    PLL_DB PLL倍频比, 输出时钟频率为1M*(clock+1)
This parameter can be Between parameters of the following values:
  *           @arg @ref

  * @param    PLL_OSEL PLL输出选择
This parameter can be one of the following values:
  *           @arg @ref FL_CMU_PLL_OUTPUT_X1
  *           @arg @ref FL_CMU_PLL_OUTPUT_X2
  * @retval   None
  */
void CMU_PLL_ConfigDomain_SYSTEM(uint32_t Source, uint32_t PLL_REFPSC, uint32_t PLL_DB, uint32_t PLL_OSEL)
{
    MODIFY_REG(CMU->PLLCR, CMU_PLLCR_DB_Msk | CMU_PLLCR_REFPRSC_Msk | CMU_PLLCR_OSEL_Msk | CMU_PLLCR_INSEL_Msk,
               (PLL_DB << CMU_PLLCR_DB_Pos) | PLL_REFPSC | PLL_OSEL | Source);
}

/**
  * @brief  RCHF初始化
  * @param  clock: RCHF频率选择
  * @retval void
  */
void RCHFInit(uint32_t clock)
{
    switch(clock)
    {
        case FL_CMU_RCHF_FREQUENCY_8MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF8M_TRIM);
            break;

        case FL_CMU_RCHF_FREQUENCY_16MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF16M_TRIM);
            break;

        case FL_CMU_RCHF_FREQUENCY_24MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF24M_TRIM);
            break;

        case FL_CMU_RCHF_FREQUENCY_32MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF32M_TRIM);
            break;

        default:
            FL_CMU_RCHF_WriteTrimValue(RCHF8M_TRIM);
            break;
    }

    FL_CMU_RCHF_SetFrequency(clock);
}

/**
  * @brief  选择内部RCHF作为锁相环的参考时钟
  *
  * @note   锁相环的参考时钟需要预分频至1M, 再进行PLL倍频
  * @param  rchf  RCHF参考时钟选择    This parameter can be one of the following values:    
  *           @arg @ref FL_CMU_RCHF_FREQUENCY_8MHZ
  *           @arg @ref FL_CMU_RCHF_FREQUENCY_16MHZ
  *           @arg @ref FL_CMU_RCHF_FREQUENCY_24MHZ
  *           @arg @ref FL_CMU_RCHF_FREQUENCY_32MHZ
  * @param  clock PLL输出频率
  * @note   输出时钟频率为1M*(clock+1), 最高输出频率可达64M
  * @retval none
  */
void SelRCHFToPLL(uint32_t rchf, uint32_t clock)
{
    uint32_t i=0;
    uint32_t div = FL_CMU_PLL_PSC_DIV8;

    if(clock > 63) { return; }

    RCHFInit(rchf);

    switch(rchf)
    {
        case FL_CMU_RCHF_FREQUENCY_16MHZ:
            div = FL_CMU_PLL_PSC_DIV16;
            break;

        case FL_CMU_RCHF_FREQUENCY_24MHZ:
            div = FL_CMU_PLL_PSC_DIV24;
            break;

        case FL_CMU_RCHF_FREQUENCY_32MHZ:
            div = FL_CMU_PLL_PSC_DIV32;
            break;

        default:
            break;
    }

    if(clock <= 23)
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
    }

    else
        if((clock > 23) && (clock <= 47))
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);
        }

        else
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE);
        }

    CMU_PLL_ConfigDomain_SYSTEM(FL_CMU_PLL_CLK_SOURCE_RCHF, div, clock, FL_CMU_PLL_OUTPUT_X1);

    FL_CMU_PLL_Enable();
    while (!FL_CMU_IsActiveFlag_PLLReady())                     /* 等待PLL锁定 */
    {
        if(i >4200)
         {
            break;
         }
        i++;
    }

    FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV8);               /* CDIF接口访问的最高频率建议不超过8M */

    FL_CMU_SetSystemClockSource(FL_CMU_SYSTEM_CLK_SOURCE_PLL);  /* 系统时钟源选择PLL */
    FL_CMU_SetAHBPrescaler(FL_CMU_AHBCLK_PSC_DIV1);
    FL_CMU_SetAPBPrescaler(FL_CMU_APBCLK_PSC_DIV1);
}

/**
  * @brief  XTHF初始化
  * @param  void
  * @retval void
  */
void XTHFInit(void)
{
    FL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* PC2.3配置成模拟功能, 外接XTHF */
    GPIO_InitStruct.pin = FL_GPIO_PIN_2 | FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* 使能XTHF */
    FL_CMU_XTHF_Enable();
    FL_CMU_XTHF_WriteDriverStrength(0x05);  /* 振荡强度 */
    FL_DelayMs(3);
}

/**
  * @brief  选择外部XTHF作为锁相环的参考时钟
  *
  * @note   锁相环的参考时钟需要预分频至1M, 再进行PLL倍频
  * @param  div  对XTHF参考时钟分频，目标分频到1M       
  *         
  * @param  clock PLL倍频输出频率
  * @note   输出时钟频率为1M*(clock+1), 最高输出频率可达64M
  * @retval none
  */
void SelXTHFToPLL(uint32_t div, uint32_t clock)
{
    uint32_t i= 0;
    if(clock > 63) { return; }

    XTHFInit();

    if(clock <= 23)
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
    }

    else
        if((clock > 23) && (clock <= 47))
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);
        }

        else
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE);
        }

    CMU_PLL_ConfigDomain_SYSTEM(FL_CMU_PLL_CLK_SOURCE_XTHF, div, clock, FL_CMU_PLL_OUTPUT_X1);

    FL_CMU_PLL_Enable();
    while (!FL_CMU_IsActiveFlag_PLLReady())         /* 等待PLL锁定 */
    {
        if(i >4200)
         {
            break;
         }
        i++;
    }
    FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV8);   /* CDIF接口访问的最高频率建议不超过8M */

    FL_CMU_SetSystemClockSource(FL_CMU_SYSTEM_CLK_SOURCE_PLL);
    FL_CMU_SetAHBPrescaler(FL_CMU_AHBCLK_PSC_DIV1);
    FL_CMU_SetAPBPrescaler(FL_CMU_APBCLK_PSC_DIV1);
}
