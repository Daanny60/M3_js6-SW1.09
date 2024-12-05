#ifndef __ENCODER_DRV_H
#define __ENCODER_DRV_H

#include "fm33lg0xx_fl_conf.h"
#include "types.h"


#define ENCODER_EVENT_NULL       0U
#define ENCODER_EVENT_ROLL_UP    1U
#define ENCODER_EVENT_ROLL_DOWN  2U

void EncoderDrv_Init(void);
void EncoderDrv_Task(void);

uint8_t LeftEncoderDrvEventGet(void);
uint8_t RightEncoderDrvEventGet(void);
uint8_t MiddleEncoderDrvEventGet(void);
#endif