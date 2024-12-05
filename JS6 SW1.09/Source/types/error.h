/**
 * @file:    err.h
 * @brief:   错误机制的提示
 * @author:  Mingyea
 * @version: V1.0.0
 * @date:      2013-04-21	  
 * @history: 
 *      1.增加err_flag_e 负数定义宏
 *	author:linyanhai
 *	version:V1.0.1
 *	date:2014-03-13	  
 */
#ifndef __ERROR_H__
#define __ERROR_H__

#define ERROR_NEGATIVE

#ifdef ERROR_NEGATIVE
typedef signed short err_flag_e;

#define ERR_OK 0     /* OK */
#define 	ERR_SPEED (-1)  /* This device does not work in the active speed mode. */
#define 	ERR_RANGE  (-2) /* Parameter out of range. */
#define 	ERR_VALUE  (-3)   /* Parameter of incorrect value. */
#define 	ERR_OVERFLOW  (-4)     /* Timer overflow. */
#define 	ERR_MATH    (-5)      /* Overflow during evaluation. */
#define 	ERR_ENABLED  (-6)      /* Device is enabled. */
#define 	ERR_DISABLED  (-7)    /* Device is disabled. */
#define 	ERR_BUSY     (-8)     /* Device is busy. */
#define 	ERR_ID_NOEXIST  (-9)    /* ID not exist . */
#define 	ERR_ID_INVALID  (-10)    /* ID is invalid . */
#define 	ERR_NOTAVAIL (-11)       /* Requested value or method not available. */
#define 	ERR_RXEMPTY (-12)      /* No data in receiver. */
#define 	ERR_TXFULL  (-13)     /* Transmitter is full. */
#define 	ERR_BUSOFF  (-14)      /* Bus not available. */
#define 	ERR_OVERRUN   (-15)    /* Overrun error is detected. */
#define 	ERR_FRAMING (-16)     /* Framing error is detected. */
#define 	ERR_PARITY (-17)       /* Parity error is detected. */
#define 	ERR_NOISE  (-18)       /* Noise error is detected. */
#define 	ERR_IDLE (-19)       /* Idle error is detected. */
#define 	ERR_FAULT   (-20)      /* Fault error is detected. */
#define 	ERR_BREAK  (-21)       /* Break char is received during communication. */
#define 	ERR_CRC    (-22)     /* CRC error is detected. */
#define 	ERR_ARBITR   (-23)    /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
#define 	ERR_PROTECT (-24)     /* Protection error is detected. */
#define 	ERR_UNDERFLOW (-25)    /* Underflow error is detected. */
#define 	ERR_UNDERRUN  (-26)   /* Underrun error is detected. */
#define 	ERR_COMMON   (-27)    /* Common error of a device. */
#define 	ERR_LINSYNC    (-28)  /* LIN synchronization error is detected. */
#define 	ERR_FAILED    (-29)   /* Requested functionality or process failed. */

#else
/*总的错误提示 */
typedef enum
{ 
	ERR_OK= 0,      /* OK */
	ERR_SPEED,  /* This device does not work in the active speed mode. */
	ERR_RANGE,  /* Parameter out of range. */
	ERR_VALUE,    /* Parameter of incorrect value. */
	ERR_OVERFLOW ,     /* Timer overflow. */
	ERR_MATH    ,      /* Overflow during evaluation. */
	ERR_ENABLED ,      /* Device is enabled. */
	ERR_DISABLED ,     /* Device is disabled. */
	ERR_BUSY    ,      /* Device is busy. */
	ERR_ID_NOEXIST,    /* ID not exist . */
	ERR_ID_INVALID,    /* ID is invalid . */
	ERR_NOTAVAIL,      /* Requested value or method not available. */
	ERR_RXEMPTY ,     /* No data in receiver. */
	ERR_TXFULL  ,     /* Transmitter is full. */
	ERR_BUSOFF  ,     /* Bus not available. */
	ERR_OVERRUN   ,   /* Overrun error is detected. */
	ERR_FRAMING ,     /* Framing error is detected. */
	ERR_PARITY ,      /* Parity error is detected. */
	ERR_NOISE  ,      /* Noise error is detected. */
	ERR_IDLE   ,      /* Idle error is detected. */
	ERR_FAULT   ,     /* Fault error is detected. */
	ERR_BREAK  ,      /* Break char is received during communication. */
	ERR_CRC    ,      /* CRC error is detected. */
	ERR_ARBITR   ,    /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
	ERR_PROTECT,      /* Protection error is detected. */
	ERR_UNDERFLOW    ,/* Underflow error is detected. */
	ERR_UNDERRUN     ,/* Underrun error is detected. */
	ERR_COMMON       ,/* Common error of a device. */
	ERR_LINSYNC      ,/* LIN synchronization error is detected. */
	ERR_FAILED       ,/* Requested functionality or process failed. */
	ERR_QFULL        ,/* Queue is full. */
}err_flag_e;

#endif





#endif   /* __TYPES_H__ */




