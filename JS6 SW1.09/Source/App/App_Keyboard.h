#ifndef __APP_KEYBOARD_H
#define __APP_KEYBOARD_H

#include <stdint.h>
#include "App_Keyboard.h"
#include "types.h"


#define APP_KEYBOARD_STATE_RELEASE        0
#define APP_KEYBOARD_STATE_PRESSED        1
#define APP_KEYBOARD_STATE_LONG_PRESSED   2
#define APP_KEYBOARD_STATE_STUCK          3
#define APP_KEYBOARD_STATE_MULTIKEY       4
#define DIA_KEYSTUCK 0

typedef   struct  keyconfig 
          { 
          uint32_t  u32KeyStruckTimeTable;  
          uint32_t  u32KeyLongTimeTable;
          uint8_t   u8KeyLongpressedfun;
          uint8_t   u8KeyTxidTable;
          uint8_t   u8KeyShortPressedTable;
          uint8_t   u8KeyLongtPressedTable;
          uint8_t   u8KeyTxNumTable;
          } strkeyconfig;



void vAppKeyBoardInit(void);
void AppKeyboard_Task(void);
uint16_t u16ReadSilence(void); 
void vSetEncoder(uint8_t txid,uint8_t value);
void vReadDidfour(uint8_t *hbyte,uint8_t *lbyte);
void vReadDidfive(uint8_t *hbyte,uint8_t *lbyte);
void   vCheckDidTask(void);
void  vModeCheZhanInCheck(void);
void  vModeCheZhanOutCheck(void);
uint8_t u8ReadCheZhanModeIn(void);
uint8_t u8ReadCheZhanModeOut(void);
#endif
