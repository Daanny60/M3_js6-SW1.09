 /**    
  *******************************************************************************************************    
  * @attention    
  * Copyright (c) 2023, SHANGHAI FUDAN MICROELECTRONICS GROUP CO., LTD.(FUDAN MICROELECTRONICS./ FUDAN MICRO.)    
  * All rights reserved.    
  *    
  * Processor:                   FM33FG0xxA    
  * http:                        http://www.fmdevelopers.com.cn/    
  *    
  * Redistribution and use in source and binary forms, with or without    
  * modification, are permitted provided that the following conditions are met    
  *    
  * 1. Redistributions of source code must retain the above copyright notice,    
  *    this list of conditions and the following disclaimer.    
  *    
  * 2. Redistributions in binary form must reproduce the above copyright notice,    
  *    this list of conditions and the following disclaimer in the documentation    
  *    and/or other materials provided with the distribution.    
  *    
  * 3. Neither the name of the copyright holder nor the names of its contributors    
  *    may be used to endorse or promote products derived from this software    
  *    without specific prior written permission.    
  *    
  * 4. To provide the most up-to-date information, the revision of our documents     
  *    on the World Wide Web will be the most Current. Your printed copy may be      
  *    an earlier revision. To verify you have the latest information avaliable,    
  *    refer to: http://www.fmdevelopers.com.cn/.    
  *    
  * THIS SOFTWARE IS PROVIDED BY FUDAN MICRO "AS IS" AND ANY EXPRESSED:     
  * ORIMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES     
  * OF MERCHANTABILITY NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE    
  * ARE DISCLAIMED.IN NO EVENT SHALL FUDAN MICRO OR ITS CONTRIBUTORS BE LIABLE     
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL     
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS     
  * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER    
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,     
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISINGIN ANY WAY OUT OF THE     
  * USE OF THIS SOFTWARE, EVEN IF ADVISED OFTHE POSSIBILITY OF SUCH DAMAGE.   
  *
  ******************************************************************************************************* 
  */    

#include "svd.h"


/**
  * @brief  将SVS引脚初始化为模拟端口
  * @param  void
  * @retval None
  */
//static void SVD_SVS_GPIO_Init(void)
//{
//  /*
//    FL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };    
//    GPIO_InitStruct.pin           = SVD_SVS_PIN;   
//    GPIO_InitStruct.mode          = FL_GPIO_MODE_ANALOG;
//    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
//    GPIO_InitStruct.pull          = FL_GPIO_BOTH_DISABLE;
//    GPIO_InitStruct.remapPin      = FL_GPIO_PINREMAP_FUNCTON0;
//    GPIO_InitStruct.driveStrength = FL_GPIO_DRIVESTRENGTH_X3;
//    (void)FL_GPIO_Init(SVD_SVS_GPIO, &GPIO_InitStruct);
//*/
//  
//}


/**
  * @brief  SVD Initialization function
  * @param  SVD_MONTIOR_POWER 选择SVD监控电源:
  *           @arg @ref SVD_MONTIOR_VDD 内部电源(VDD)
  *           @arg @ref SVD_MONTIOR_SVS 外部电源(SVS)
  *         u32WarningThreshold 报警阈值档位:
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP0
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP1
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP2
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP3
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP4
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP5
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP6
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP7
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP8
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP9
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP10
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP11
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP12
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP13
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP14
  *           @arg @ref FL_SVD_WARNING_THRESHOLD_GROUP15
  *         u32RevVoltage 参考基准源:
  *           @arg @ref FL_SVD_REFERENCE_1P0V
  *           @arg @ref FL_SVD_REFERENCE_0P95V
  *           @arg @ref FL_SVD_REFERENCE_0P9V
  * @retval FL_FAIL: 初始化失败
  *         FL_PASS: 初始化成功
  */
FL_ErrorStatus SVD_Init(SVD_MONTIOR_POWER eSVD_MonitroPower, uint32_t u32WarnThreshold, uint32_t u32RevVoltage)
{
    FL_ErrorStatus status;
    FL_SVD_InitTypeDef SVD_InitStruct;
    /*
    if (SVD_MONTIOR_SVS == eSVD_MonitroPower)
    {  
        SVD_SVS_GPIO_Init();
    }
    */
    /* 参考电压 */
    SVD_InitStruct.referenceVoltage = u32RevVoltage;
    /* 报警阈值 */
    SVD_InitStruct.warningThreshold = u32WarnThreshold;
    /* 数字滤波 */
    SVD_InitStruct.digitalFilter    = FL_ENABLE;
    /* 工作模式 */
    SVD_InitStruct.workMode         = FL_SVD_WORK_MODE_PERIODIC;
    /* 间歇使能间隔 */             
    SVD_InitStruct.enablePeriod     = FL_SVD_ENABLE_PERIOD_62P5MS;
    /* SVS通道选择 */
    SVD_InitStruct.SVSChannel       = (FL_FunState)(SVD_MONTIOR_SVS == eSVD_MonitroPower);
    /* 初始化寄存器 */
    status = FL_SVD_Init(SVD, &SVD_InitStruct);
    if (FL_PASS == status)
    {   /* 使能SVD */
        FL_SVD_Enable(SVD);
        /* SVD使能后到输出稳定建立至少需要500us时间 */
        FL_DelayUs(500);
    }
    return status;
}


/**
  * @brief  SVD监测结果（SVDO标志位）轮询输出
            SVD实时电压监测标志，无滤波
  * @param  None
  * @retval SVD_BELOW_THRESHOLD:  低于阈值
  *         SVD_HIGHER_THRESHOLD：高于阈值
  */
SVD_RESULT SVD_SVDO_POLL(void)
{
    /* SVD实时电压监测标志；软件避免写此寄存器 */
    return (SVD_RESULT)(SVD_ISR_SVDO_Msk == FL_SVD_GetCurrentPowerStatus(SVD));
}


/**
  * @brief  SVD监测结果（SVDR标志位）轮询输出
            仅在数字滤波使能时有意义，即滤波后的锁存信号 
  * @param  None
  * @retval SVD_BELOW_THRESHOLD:  低于阈值
  *         SVD_HIGHER_THRESHOLD：高于阈值
  */
SVD_RESULT SVD_SVDR_POLL(void)
{
    /* SVD内部滤波后的电压监测标志，仅在使能数字滤波时有意义；软件避免写此寄存器 */
    return (SVD_RESULT)(SVD_ISR_SVDR_Msk == FL_SVD_GetLatchedPowerStatus(SVD));
}


/**
  * @brief  确认SVD监测结果是否处于目标状态
  * @param  eSVDResultType 要确认的目状态:
  *           @arg @ref SVD_BELOW_THRESHOLD: 低于阈值
  *           @arg @ref SVD_BELOW_THRESHOLD: 高于阈值
  *         u32Delay_us 防抖时长，单位us
  * @retval bool   true： 当前SVD监测结果与目标状态一致
                   false：当前SVD监测结果与目标状态不一致
  */
bool SVD_Result_Confirmed(SVD_RESULT eSVDResultType, uint32_t u32Delay_us)
{
    uint32_t Residuals = u32Delay_us & ((1UL<<3) - 1); 
    
    u32Delay_us = (u32Delay_us >> 3);
    if ((0 == u32Delay_us) || Residuals)
    {
        u32Delay_us += 1;
    }
    /* 注意是否使能数字滤波，调用不同的结果轮询函数 */
    while ((SVD_SVDR_POLL() == eSVDResultType) && u32Delay_us)
    {
        if (--u32Delay_us)
        {
            FL_DelayUs(1UL<<3);
        }
    }
    
    return (bool)(0 == u32Delay_us);
}


