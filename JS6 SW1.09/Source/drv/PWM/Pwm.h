#ifndef __PWM_H
#define __PWM_H

#include "mf_config.h"
#include "stdint.h"

void PWM_Init(void);
void GPTime1Duty(uint8_t channel,uint16_t duty);
void GPTime0Duty(uint8_t channel,uint16_t duty);
void vRgbRight_RedDuty(uint16_t duty);
void vRgbRight_GreenDuty(uint16_t duty);
void vRgbRight_BlueDuty(uint16_t duty);
void vRgbLeft_RedDuty(uint16_t duty);
void vRgbLeft_GreenDuty(uint16_t duty);
void vRgbLeft_BlueDuty(uint16_t duty);
void vHeatPwmDuty(uint16_t u16duty);
void vBackLightPwmDuty(uint16_t u16duty);
#endif