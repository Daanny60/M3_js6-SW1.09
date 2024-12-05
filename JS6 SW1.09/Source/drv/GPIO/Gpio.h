#ifndef __GPIO_H
#define __GPIO_H


#include "fm33lg0xx_fl_conf.h"
#include "mf_config.h"


///////////////////////////////////////////////CAN
#define CAN_RXD_GPIO            GPIOA
#define CAN_RXD_PIN             FL_GPIO_PIN_6
#define CAN_TXD_GPIO            GPIOA
#define CAN_TXD_PIN             FL_GPIO_PIN_7
#define CAN_STB_GPIO            GPIOA
#define CAN_STB_PIN             FL_GPIO_PIN_8


////////////VBAT_CTRL
#define POWER_12V_ENABLE_GPIO   GPIOB
#define POWER_12V_ENABLE_PIN   FL_GPIO_PIN_12

////////////NTC VCC CTRL
#define HEATING_NTC_VCC_GPIO       GPIOB
#define HEATING_NTC_VCC_PIN        FL_GPIO_PIN_5


//////////HEAT_DEN
#define HEATING_DEN_GPIO       GPIOB
#define HEATING_DEN_PIN        FL_GPIO_PIN_10

//////////HEAT_IN
#define HEATING_CTRL_GPIO       GPIOC
#define HEATING_CTRL_PIN       FL_GPIO_PIN_0







void vLedSwitchOn(void);
void vLedSwitchOff(void);

void vHeatOutputOn(void);
void vHeatOutputOff(void);

uint32_t vHeatGetInputIO(void);

void  vGpioDeinit(void);
void GPIO_Init(void);
#endif