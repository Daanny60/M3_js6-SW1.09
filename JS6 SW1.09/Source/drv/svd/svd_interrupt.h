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

#ifndef __SVD_INTERRUPT_H__
#define __SVD_INTERRUPT_H__
#include "fm33lg0xx_fl.h"
#include "svd.h"

extern void SVD_NVIC_Init(void);
extern void SVD_Interrupt_Init(FL_FunState DigitalFilter);
extern void SVD_Interrupt_DeInit(void);
extern SVD_RESULT SVD_IT(void);

   
#endif
