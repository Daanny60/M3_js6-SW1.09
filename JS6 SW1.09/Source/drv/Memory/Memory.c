/**
 * @copyright: KUWE technologies Co.,Ltd. 2013-2016.all rights reserved.
 * @file: memory.c
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
#include "memory.h"


/**
@ static void data_cpys(uint8_t* dst,uint8_t* src,uint8_t length)
@ Parameter:   
@ Decription:

@ Return:      
@ Creator:
*/
void memory_copys(uint8_t* dst,const uint8_t* src,uint16_t length)
{
    uint16_t i = 0;
	while(length--) 
	{
		  dst[i] = src[i];
		  i++;
	}
}


/**
@ static void bit_inversion(uint8_t *pdata)
@ Parameter:   
@ Decription:

@ Return:      
@ Creator:
*/
void memory_clear(uint8_t *dst , uint16_t length)
{
   uint16_t i = 0;
   while(length--)
   {
		dst[i] = 0u;
		i++;
   };
}



/**
@ static str_len(const uint8_t *chr)
@ Parameter:   
@ Decription:

@ Return:      
@ Creator:
*/
uint8_t str_length(const uint8_t *chr)
{
    uint8_t len = 0;
	/*求字符串s1的长度  */ 
	/*PCLINT_ERROS_960*/	/*lint --e(960)*/  /*過虙10.1 類型的強制隱制轉換 */
	for (len = 0u ; *(chr+len)!='\0' ; len++)   /*lint !e960 */ 
	{
		;  
	}
	return len;
}



/**
 * @brief      	比较字符串的內容是否相同；
 * @param  			
 * @return  	相同返回1；不同返回0 
 */ 
/*lint -efunc(818,string_compare) */ /*（建議）16.7 函數的指針參數 如果不是用來修改的話，建議聲明指向 const 類型*/
bool string_compare(const uint8_t *ps1, const uint8_t *ps2)
{
	uint8_t i;
	uint8_t n1 = str_length(ps1), n2 = str_length(ps2);		
	/*PCLINT_ERROS_960*/	/*lint --e(960)*/  /*過虙10.1 類型的強制隱制轉換 */
	bool l_result = TRUE ;
	
	/*字符长度不同，一定內容不同*/
	if(n1 != n2)
	{
		/*PCLINT_ERROS_960*/	/*lint --e(960)*/  /*過虙10.1 類型的強制隱制轉換 */
		l_result = FALSE;
		/* return FALSE; */
	}
	else
	{
		for(i=0u;i<n1;i++)
		{
			/*PCLINT_ERROS_931*/	/*lint --e(931) */ /* 	931: Both sides have side effects [MISRA 2004 Rule 1.2] 兩邊等式都有副作用 */
			if(*ps1++ != *ps2++)
			{
				/* return FALSE; */	
				/*PCLINT_ERROS_960*/	/*lint --e(960)*/  /*過虙10.1 類型的強制隱制轉換 */
				l_result = FALSE;
				break;
			}
		}
	}
	return l_result;	
}
