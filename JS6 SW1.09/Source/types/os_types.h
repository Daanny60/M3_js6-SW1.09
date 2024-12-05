/*********************************************************************************************************
** 文件名称:      	 	type.h
** 文件功能:        	类型定义			
*********************************************************************************************************/ 

#ifndef __OS_TYPES_H__
#define __OS_TYPES_H__

#include "types.h"

#ifndef TRUE
	#define TRUE		1
#endif                                                                  /*  TRUE                        */

#ifndef FALSE
	#define FALSE		0
#endif                                                                  /*  FALSE                       */

#ifndef NULL
	#define NULL		0
#endif                                                                  /*  NULL                        */
typedef const unsigned char	C8;
typedef const unsigned int	C16;
typedef unsigned char  		BOOL;                     		          	/*  布尔变量					*/
typedef unsigned char  		U8;                                     	/*  无符号8位整型变量			*/
typedef signed   char  		S8;                             	      	/*  有符号8位整型变量			*/
typedef unsigned int   		U16;                                      	/*  无符号16位整型变量			*/
typedef signed   int   		S16;                                      	/*  有符号16位整型变量			*/
typedef unsigned long  		U32;                                   		/*  无符号32位整型变量			*/
typedef signed   long  		S32;                                    	/*  有符号32位整型变量			*/
typedef float          		F32;                                     	/*  单精度浮点数（32位长度）	*/
typedef double         		F64;                                   		/*  双精度浮点数（64位长度）	*/

typedef            void        	OS_VOID;
typedef            char        	OS_CHAR;                        /*  8-bit character                                     */
typedef  unsigned  char			OS_BOOL;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char			OS_U8;                     	/*  8-bit unsigned integer                              */
typedef    signed  char 		OS_S8;                      /*  8-bit   signed integer                              */
typedef  unsigned  short		OS_U16;                      /* 16-bit unsigned integer                              */
typedef    signed  short		OS_S16;                      /* 16-bit   signed integer                              */
typedef  unsigned  int			OS_U32;                      /* 32-bit unsigned integer                              */
typedef    signed  int			OS_S32;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long	OS_U64;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long	OS_S64;                      /* 64-bit   signed integer                              */
typedef            float		OS_F32;                        /* 32-bit floating point                                */
typedef            double		OS_F64;                        /* 64-bit floating point                                */
typedef  volatile  OS_U8		OS_V08;                       /*  8-bit register                                      */
typedef  volatile  OS_U16		OS_V16;                       /* 16-bit register                                      */
typedef  volatile  OS_U32		OS_V32;                       /* 32-bit register                                      */
typedef  volatile  OS_U64		OS_V64;                       /* 64-bit register                                      */
typedef            void     	 (*CPU_FNCT_VOID)(void);            /* See Note #2a.                                        */
typedef            void     	 (*CPU_FNCT_PTR )(void *p_obj);     /* See Note #2b.                                        */

typedef   OS_U16 		OS_CPU_USAGE;                /* CPU Usage 0..10000                                  <16>/32 */
typedef   OS_U32 		OS_CTR;                      /* Counter,                                                 32 */
typedef   OS_U32 		OS_CTX_SW_CTR;               /* Counter of context switches,                             32 */
typedef   OS_U32 		OS_CYCLES;                   /* CPU clock cycles,                                   <32>/64 */
typedef   OS_U32 		OS_FLAGS;                    /* Event flags,                                      8/16/<32> */
typedef   OS_U32 		OS_IDLE_CTR;                 /* Holds the number of times the idle task runs,       <32>/64 */
typedef   OS_U16 		OS_MEM_QTY;                  /* Number of memory blocks,                            <16>/32 */
typedef   OS_U16 		OS_MEM_SIZE;                 /* Size in bytes of a memory block,                    <16>/32 */
typedef   OS_U16 		OS_MSG_QTY;                  /* Number of OS_MSGs in the msg pool,                  <16>/32 */
typedef   OS_U16 		OS_MSG_SIZE;                 /* Size of messages in number of bytes,                <16>/32 */
typedef   OS_U8   		OS_NESTING_CTR;              /* Interrupt and scheduler nesting,                  <8>/16/32 */
typedef   OS_U16 		OS_OBJ_QTY;                  /* Number of kernel objects counter,                   <16>/32 */
typedef   OS_U32 		OS_OBJ_TYPE;                 /* Special flag to determine object type,                   32 */
typedef   OS_U16 		OS_OPT;                      /* Holds function options                              <16>/32 */
typedef   OS_U8			OS_PRIO;                     /* Priority of a task,                               <8>/16/32 */
typedef   OS_U16		OS_QTY;                      /* Quantity                                            <16>/32 */
typedef   OS_U32		OS_RATE_HZ;                  /* Rate in Hertz                                            32 */
typedef   OS_U32		OS_REG;                      /* Task register                                     8/16/<32> */
typedef   OS_U8			OS_REG_ID;                   /* Index to task register                            <8>/16/32 */
typedef   OS_U32		OS_SEM_CTR;                  /* Semaphore value                                     16/<32> */
typedef   OS_U8			OS_STATE;                    /* State variable                                    <8>/16/32 */
typedef   OS_U8			OS_STATUS;                   /* Status                                            <8>/16/32 */
typedef   OS_U16		OS_TICK;                     /* Clock tick counter                                  <32>/64 */
typedef   OS_U16		OS_TICK_SPOKE_IX;            /* Tick wheel spoke position                         8/<16>/32 */
typedef   OS_U16		OS_TMR_SPOKE_IX;             /* Timer wheel spoke position                        8/<16>/32 */
typedef   OS_U32		OS_TS32;
typedef   OS_U64		OS_TS64;
typedef   OS_TS32		OS_TS;                                    /* Req'd for backwards-compatibility.         */

typedef   OS_U8			OS_TIME_ID;
#define 	OS_TRUE		1u
#define 	OS_FALSE	0u
#define 	OS_NULL		0u

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


#define TEST_WHILE_ENABLE		//测试的 while(1);开启


#endif  /* __TYPE_H__ */
