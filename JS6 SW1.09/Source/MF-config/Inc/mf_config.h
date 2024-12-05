/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : mf_config.h
  * @brief          : Header for mf_config.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) [2019] [Fudan Microelectronics]
  * THIS SOFTWARE is licensed under the Mulan PSL v1.
  * can use this software according to the terms and conditions of the Mulan PSL v1.
  * You may obtain a copy of Mulan PSL v1 at:
  * http://license.coscl.org.cn/MulanPSL
  * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
  * PURPOSE.
  * See the Mulan PSL v1 for more details.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MF_CONFIG_H
#define __MF_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions prototypes ---------------------------------------------*/
void MF_Clock_Init(void);
void MF_SystemClock_Config(void);
void MF_Config_Init(void);
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* __MF_CONFIG_H */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
