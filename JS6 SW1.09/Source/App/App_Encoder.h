#ifndef __APP_ENCODER_H
#define __APP_ENCODER_H

#include <stdint.h>

#define   APP_ENCODER_PERIOD    10
#define   APP_ENCODER_50MS     (100/APP_ENCODER_PERIOD)
#define   APP_ENCODER_100MS    (100/APP_ENCODER_PERIOD)
#define   APP_ENCODER_1000MS   (1000/APP_ENCODER_PERIOD)

#define   NUM_ENCODER_BUF      6






void App_EncoderInit(void);
void App_EncoderTask(void);
void App_EncoderRollClear(void);

struct _tagAppEncoderTx
{
    uint8_t u8Ecbuf[NUM_ENCODER_BUF];             //////buf
    uint8_t u8EcTxId[NUM_ENCODER_BUF];           //// txid
    uint8_t u8EcTimerCnt;       /////timer  cnt   
    uint8_t u8EcValue;          /////tx data value   
    uint8_t u8EcWptr;          /////write point
    uint8_t u8EcRptr;          /////read point   
};



#endif