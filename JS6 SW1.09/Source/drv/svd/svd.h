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

#ifndef __SVD_H__
#define __SVD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "fm33lg0xx_fl.h"
#include "stdbool.h"

/* SVD监测电源 */
typedef enum
{
    SVD_MONTIOR_VDD = 0U,  /* 内部电源VDD */
    SVD_MONTIOR_SVS = 1U   /* 外部电源SVS */
}SVD_MONTIOR_POWER;

/* SVD监测结果 */
typedef enum
{
    SVD_BELOW_THRESHOLD  = 0U,                    /* 电源电压低于SVD当前阈值 */    
    SVD_HIGHER_THRESHOLD = !SVD_BELOW_THRESHOLD   /* 电源电压高于SVD当前阈值 */
}SVD_RESULT;


#define SVD_SVS_GPIO  (GPIOA)
#define SVD_SVS_PIN   (FL_GPIO_PIN_15)


extern FL_ErrorStatus SVD_Init(SVD_MONTIOR_POWER eSVD_MonitroPower, uint32_t u32WarnThreshold, uint32_t u32RevVoltage);
extern SVD_RESULT SVD_SVDO_POLL(void);
extern SVD_RESULT SVD_SVDR_POLL(void);
extern bool SVD_Result_Confirmed(SVD_RESULT eSVDResultType, uint32_t u32Delay_us);

#ifdef __cplusplus
}
#endif

#endif
