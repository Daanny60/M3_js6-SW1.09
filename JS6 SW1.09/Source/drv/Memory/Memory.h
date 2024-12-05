/**
 * @copyright: KUWE technologies Co.,Ltd. 2013-2016.all rights reserved.
 * @file: memory.h
 * @brief: 
 * @author: xucong
 * @version: V1.0.0
 * @date: 2014-5-8
 * @history: 
 *     1.
 *	author:   
 *	version: 
 *	date: 
 *	modification:  
 */
#ifndef __MEMORY_STRING_H__
#define __MEMORY_STRING_H__

 
#include "types.h"
#include "mf_config.h"
#include "fm33lg0xx_fl.h"


/**
@ static void data_cpys(uint8_t* dst,uint8_t* src,uint8_t length)
@ Parameter:   
@ Decription:
@ Return:      
@ Creator:
*/
void memory_copys(uint8_t* dst,const uint8_t* src,uint16_t length);

/**
@ static void bit_inversion(uint8_t *pdata)
@ Parameter:   
@ Decription:
@ Return:      
@ Creator:
*/
void memory_clear(uint8_t *dst , uint16_t length);



/**
@ static str_length(const uint8_t *chr)
@ Parameter:   
@ Decription:

@ Return:      
@ Creator:
*/
uint8_t str_length(const uint8_t *chr);

/**
 * @brief      	比较字符串的內容是否相同；
 * @param  			
 * @return  	相同返回1；不同返回0 
 */ 
/*lint -efunc(818,string_compare) */ /*（建議）16.7 函數的指針參數 如果不是用來修改的話，建議聲明指向 const 類型*/
bool string_compare(const uint8_t *ps1, const uint8_t *ps2);


#endif
