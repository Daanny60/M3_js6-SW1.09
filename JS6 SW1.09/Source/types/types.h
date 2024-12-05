  /**
 * @file:     types.h
 * @brief:  contains definitions of basic types,
					register access macros and hardware specific macros
					which can be used in user application.		
 * @author:   卢明阳		         
 * @version: V1.1.0
 * @date:      2013-10-18	                                         
 * @history: 
 *      1.
 *	author:卢明阳		 
 *	version:V1.0.0
 *	date:2013-04-21	         
 *      2.
 *	author:林严海	 
 *	version:V1.0.1
 *	date:2014-01-24	                 
 *      3.增加lock,unlock,open,close定义
 *	author:林严海	 
 *	version:V1.0.2
 *	date:2014-03-12	                 
 */

#ifndef __TYPES_H__
#define __TYPES_H__


#include <stdint.h>


#ifndef FALSE 
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif


typedef uint8_t  u8 ;       /* USARTdiv 的小數部分  */
typedef uint16_t u16 ;      /* ENABLE or DISABLE  接收緩沖區DMA使能 */  
typedef uint32_t  u32;


/* Whole peripheral register access macros */
#define set_reg8(RegName, val)    (RegName = (u8)(val))
#define get_reg8(RegName)         ((u8)(RegName))


#define clr_set_reg8bits(RegName, ClrMask, SetMask) (RegName = (RegName & ((u8)(~(u8)(ClrMask)))) | (u8)(SetMask))
#define clr_set_reg16bits(RegName, ClrMask, SetMask) (RegName = (RegName & ((u16)(~(u16)(ClrMask)))) | (u16)(SetMask))
#define clr_set_reg32bits(RegName, ClrMask, SetMask) (RegName = (RegName & ((u32)(~(u32)(ClrMask)))) | (u32)(SetMask))


#define clear_reg(reg)        ((reg) = (0x0))     /* 清零 */
#define write_reg(reg, val)   ((reg) = (val))     /* 写入 */
#define read_reg(reg)         ((reg))               /* 读取 */

#define set_8bits_mask(reg, mask)     ((reg) |= (u8)(mask))    /* 将mask中的值为1的那几位置1 */
#define clear_8bits_mask(reg, mask)   ((reg) &= ((u8)~(mask)))  /* 将mask中的值为0的那几位清0 */
#define get_8bits_mask(reg, mask)    ((reg) & (u8)(mask))     /* 读取mask中的值为1的那几位的情况 */
#define change_8Bits_mask(reg, mask)   (reg ^= (u8)(mask))     /* 将mask中值为1的那几位最反 */
#define modify_8bits_reg(reg, clearmask, setmask)  write_reg((reg), (((read_reg(reg)) & (~(u8)(clearmask))) | (u8)(setmask)))
#define set_8bits_mask_val(reg, mask, val)   ((val) == 0 ? clear_8bits_mask(reg, mask) : (set_8bits_mask(reg, mask)))
#define set_8bits_bit(reg, bit)     ((reg) |= (u8)((u8)0x01)<<bit)    /*reg 第bit位置一*/
#define clear_8bits_bit(reg, bit)     ((reg) &= ~(u8)((u8)0x01)<<bit)    /*reg 第bit位清0*/

#define set_16bits_mask(reg, mask)     ((reg) |= (u16)(mask))    /* 将mask中的值为1的那几位置1 */
#define clear_16bits_mask(reg, mask)   ((reg) &= (u16)~(mask))   /* 将mask中的值为0的那几位清0 */
#define get_16bits_mask(reg, mask)    ((reg) & (u16)(mask))     /* 读取mask中的值为1的那几位的情况 */
#define change_16Bits_mask(reg, mask)   (reg ^= (u16)(mask))      /* 将mask中值为1的那几位最反 */
#define modify_16bits_reg(reg, clearmask, setmask)  write_reg((reg), (((read_reg(reg)) & (~(u16)(clearmask))) | (u16)(setmask)))
#define set_16bits_mask_val(reg, mask, val)   ((val) == 0 ? clear_16bits_mask(reg, mask) : (set_16bits_mask(reg, mask)))
#define set_16bits_bit(reg, bit)     ((reg) |= (u16)((u16)0x01)<<bit)    /*reg 第bit位置一*/
#define clear_16bits_bit(reg, bit)     ((reg) &= ~(u16)((u16)0x01)<<bit)    /*reg 第bit位清0*/

#define set_32bits_mask(reg, mask)     ((reg) |= (u32)(mask))    /* 将mask中的值为1的那几位置1 */
#define clear_32bits_mask(reg, mask)   ((reg) &= (u32)~(mask))   /* 将mask中的值为0的那几位清0 */
#define get_32bits_mask(reg, mask)    ((reg) & (u32)(mask))     /* 读取mask中的值为1的那几位的情况 */
#define change_32Bits_mask(reg, mask)   (reg ^= (u32)(mask))    /* 将mask中值为1的那几位最反 */
#define modify_32bits_reg(reg, clearmask, setmask)  write_reg((reg), (((read_reg(reg)) & (~(u32)(clearmask))) | (u32)(setmask)))
#define set_32bits_mask_val(reg, mask, val)   ((val) == 0 ? clear_32bits_mask(reg, mask) : (set_32bits_mask(reg, mask)))
#define set_32bits_bit(reg, bit)     ((reg) |= (u32)((u32)0x01)<<bit)    /*reg 第bit位置一*/
#define clear_32bits_bit(reg, bit)     ((reg) &= ~(u32)((u32)0x01)<<bit)    /*reg 第bit位清0*/


#define bits8_to_bits16(var,l,h)  (var = ((u16)(l)) | (((u16)(h)) << 8))   /*两个8位转成16位*/
#define bits16_to_bits8(l,h,val) (l = (u8)val, h = (u8)(val >> 8))  /*16位转成两个高低8位;h为高 l为低*/
#define bits16_to_bits32(var,l,h)  (var = ((u32)(l)) | (((u32)(h)) << 16))   /*两个16位转成32位*/
#define bits32_to_bits16(l,h,val) (l = (u16)val, h = (u16)(val >> 8))  /*16位转成两个高低8位;h为高 l为低*/

#if  !defined (_STDINT_H) && !defined(_STDINT)






#define     __VC    volatile const         /*!< defines 'read only' permissions     */
#define     __I     volatile const         /*!< defines 'read only' permissions     */
#define     __O     volatile               /*!< defines 'write only' permissions     */
#define     __IO    volatile               /*!< defines 'read / write' permissions   */

#define     _VOLATILE    volatile                 /*定义volatile*/

#define _OS_EVENT_

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;


/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short int   uint16_t;
typedef unsigned long int      uint32_t;
//typedef unsigned long long  uint64_t;
typedef double uint64_t; /* for CW, 选择编译选项：IEE64BIT DOUBLE, 否则只有32BIT */

#ifndef uint8
#define uint8 uint8_t     
#endif

#ifndef uint16
#define uint16 uint16_t     
#endif

#ifndef uint32
#define uint32 uint32_t     
#endif

#ifndef int8
#define int8 int8_t     
#endif

#ifndef int16
#define int16 int16_t     
#endif

#ifndef int32
#define int32 int32_t     
#endif

typedef union
{
	uint16_t w;
	uint8_t  a[2];
	struct{
	uint8_t b0; /*0 for CW 为HIGH_BYTE, FOR PICC 为LOW_BYTE */
	uint8_t b1; /* 1 for CW 为Low_BYTE, FOR PICC 为HIGH_BYTE */
	}b;
}u16_swap_t;





typedef union
{
	uint32_t dw;
	struct{
	uint8_t b0;   /*0  for CW 为HIGH_BYTE, FOR PICC 为LOW_BYTE */
	uint8_t b1;
	uint8_t b2;
	uint8_t b3;
	}b;
}u32_swap_t;

typedef union
{
	uint32_t dw;
	uint16_t a[2];/* 0 for CW 为HIGH_WORD, FOR PICC 为LOW_WORD */
	uint8_t  b[4];/* 0 for CW 为HIGH_BYTE, FOR PICC 为LOW_BYTE */
}u32_swap1_t;


typedef union
{
	uint32_t dw[2];
	struct{
	u32_swap_t dw_l;
	u32_swap_t dw_h;
	}lb;
}u64_swap_t;



typedef unsigned char     bool_t;


typedef float tpe_float;

typedef const char c8;
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t  s32;

typedef const int8_t sc8;    /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int32_t sc32;  /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;    /*!< Read Only */



typedef const uint8_t uc8;       /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint32_t uc32;     /*!< Read Only */

typedef __IO uint8_t  vu8;
typedef __IO uint16_t vu16;
typedef __IO uint32_t  vu32;

typedef __I u32 vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */


/*Types definition*/
typedef unsigned char ubool;
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef unsigned long dlong[2];


#ifndef NULL
	/*#define NULL 		(void*)0 */
#endif
/*  符号类型的值范围  */
#define U8_MAX     (255)
#define S8_MAX     (127)
#define S8_MIN     (-128)
#define U16_MAX    (65535u)
#define S16_MAX    (32767)
#define S16_MIN    (-32768)
#define U32_MAX    (4294967295uL)
#define S32_MAX    (2147483647)
#define S32_MIN    (-2147483648uL)

#define STD_On       (1)                                       
#define STD_Off      (0)


#if 0

typedef enum
{
	FALSE = 0,      /* 鍋� */
	TRUE  = 1   /* 鐪� */
}bool;  /* 甯冨皵杩愮畻 */

#else

typedef u8 bool;

#ifndef FALSE 
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif

#endif



typedef enum 
{
	RESET = 0,    /* 復位 */
	SET = !RESET    /* 設置 */
}set_status_e;


typedef enum 
{
	FAILED = 0,      /* 失敗 */
	SUCCESS = !FAILED,  /* 成功 */
	ERROR = 0,       /* 錯誤 */
	RIGHT = !ERROR     /* 正確 */
} result_status_e;


typedef enum 
{
	DISABLE = 0,    /* 禁能 */
	ENABLE = !DISABLE  /* 使能 */
}functional_state_e;

typedef enum 
{
	UNBUSY = 0,    /* 不在忙 */
	BUSY = !UNBUSY  /* 忙 */
} busy_status_e;



typedef enum 
{
	UNDONE = 0,     /* 未完成 */
	DONE = !UNDONE    /* 完成 */
} done_status_e; 


/*電平值 */
typedef enum
{ 
	GPIOP_LOW = 0,   /* 低电平 */
	GPIOP_HIGH = !GPIOP_LOW   /* 高电平 */
}gpio_pin_value_e;

/*
typedef enum
{
	LOCK = 0U,
	UNLOCK = !LOCK,
	LOCK_UNKNOW,
}lock_state_e;
*/

typedef enum
{
	LOCK = 0U,
	UNLOCK,
	LOCK_UNKNOW,
}lock_state_e;

typedef enum
{
	STATE_UNKNOW = 0,
	OPEN,
	CLOSE,
}operation_state_e;

typedef enum
{
	STATE_UNKNOWN		=0,
	STATE_SUCCESS,
	STATE_FAILED,
	STATE_STOP,
	STATE_ERROR,
	STATE_RETRYING,
}StateType;






#define output(P, V) (P = (V))  /*v鐨勫€艰祴缁檖*/
#define input(P) (P)

#define LOW_BYTE(Uintd) ((u8)((Uintd) & 0xffu))
#define HIGH_BYTE(Uintd) ((u8)(((Uintd) >> 8) & 0xffu))
#if 1
//#define Conn(x,y) x##y
//#define ToChar(x) #@x
//#define ToString(x) #x
#else
#define Conn(x,y) ((x)##(y))
#define ToChar(x) (#@(x))
#define ToString(x) (#(x))
#endif

/*Author: GodbachDate: Oct 23, 2008*/


#define STRUCT_OFFSET(stru_name, element) (unsigned int)&((stru_name*)0)->element 

#endif   /* __TYPES_H__ */


#endif

