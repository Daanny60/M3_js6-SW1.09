#ifndef __TIM_H
#define __TIM_H

#include "mf_config.h"
#include "types.h"



typedef uint16_t SystemTimeType;



void BSTIM16_Init(void);
SystemTimeType Get_SystemTime(void);


#endif
