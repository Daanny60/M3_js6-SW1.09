/**
  ******************************************************************************
  * @attention
  * Copyright (c) 2022, SHANGHAI FUDAN MICROELECTRONICS GROUP CO., LTD.(FUDAN MICROELECTRONICS./ FUDAN MICRO.)
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
  * THIS SOFTWARE IS PROVIDED BY FUDAN MICRO "AS IS" AND ANY EXPRESSED
    ORIMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.IN NO EVENT SHALL FUDAN MICRO OR ITS CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISINGIN ANY WAY OUT OF THE
    USE OF THIS SOFTWARE, EVEN IF ADVISED OFTHE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "svd_interrupt.h"


/* SVD监测结果 */
static volatile SVD_RESULT eSVDResult = SVD_HIGHER_THRESHOLD; 


/**
  * @brief  SVD 中断服务函数
  * @param  void
  * @retval None
  */
void SVD_IRQHandler(void)
{
    if (0X01 == FL_SVD_IsActiveFlag_PowerFall(SVD)) 
    {
        /* 欠压中断 */
        FL_SVD_ClearFlag_PowerFall(SVD);
        eSVDResult = SVD_BELOW_THRESHOLD;
        NVIC_SystemReset();
    }
    else if (0X01 == FL_SVD_IsActiveFlag_PowerRise(SVD))
    {    
        /* 过压中断 */
        FL_SVD_ClearFlag_PowerRise(SVD);
        eSVDResult = SVD_HIGHER_THRESHOLD;
         NVIC_SystemReset();
    }
}


/**
  * @brief  NVIC Initialization function
  * @param  void
  * @retval None
  */
void SVD_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 2;
    FL_NVIC_Init(&InterruptConfigStruct, SVD_IRQn);    
}


/**
  * @brief  SVD Initialization function
  * @param  void
  * @retval None
  */
void SVD_Interrupt_Init(FL_FunState DigitalFilter)
{
    /* 使能SVD欠压和过压中断 */
    FL_SVD_ClearFlag_PowerRise(SVD);
    FL_SVD_ClearFlag_PowerFall(SVD);
    FL_SVD_EnableIT_PowerRise(SVD );
    FL_SVD_EnableIT_PowerFall(SVD );
    /* 使能NVIC中断 */
    SVD_NVIC_Init();
    /* 基于SVD监测结果初始赋值 */
    /* 注意：如果有使能数字滤波，应使用SVDR查询； 如未使能数字滤波，应使用SVDO查询 */
    if (FL_ENABLE == DigitalFilter)
    {
        eSVDResult = SVD_SVDR_POLL();
    }
    else
    {
        eSVDResult = SVD_SVDO_POLL();
    }
}


/**
  * @brief  SVD DeInitialization function
  * @param  void
  * @retval None
  */
void SVD_Interrupt_DeInit(void)
{
    /* 使能SVD欠压和过压中断 */
    FL_SVD_ClearFlag_PowerRise(SVD);
    FL_SVD_ClearFlag_PowerFall(SVD);
    FL_SVD_DisableIT_PowerRise(SVD );
    FL_SVD_DisableIT_PowerFall(SVD );
    /* 禁止NVIC中断 */
    NVIC_ClearPendingIRQ(SVD_IRQn);
    NVIC_DisableIRQ(SVD_IRQn);
}


/**
  * @brief  SVD监测结果中断输出
  * @param  void
  * @retval None
  */
SVD_RESULT SVD_IT(void)
{
    /* 错误冗余，变量异常按掉电复位处理 */
    if (eSVDResult > SVD_HIGHER_THRESHOLD)
    {
        eSVDResult = SVD_BELOW_THRESHOLD; 
    }
    /* 应用到产品中时，需注意防抖处理 */
    return eSVDResult;
}



