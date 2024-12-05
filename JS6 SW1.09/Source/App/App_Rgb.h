#ifndef __APP_RGB_H
#define __APP_RGB_H

#include <stdint.h>


#define   APP_RGB_PERIOD     10
#define   APP_RGB_200MS     (200/APP_RGB_PERIOD)
#define   APP_RGB_400MS     (400/APP_RGB_PERIOD)
#define   APP_RGB_500MS     (500/APP_RGB_PERIOD)
#define   APP_RGB_1000MS    (1000/APP_RGB_PERIOD)

/////////////////////////////////////////////
#define APP_MODE_NUM          0x05
//////////////////////////////////////////////DAY NIGHT
#define   APP_MODE_DAY        0X02
#define   APP_MODE_NIGHT      0X01
///////////////////////////////////////////////MODE
#define  APP_MODE_NOACTVATIONG       0x00
#define  APP_MODE_ACTVATION          0x01
#define  APP_MODE_ALARM_LEVEL2       0x02
#define  APP_MODE_ALARM_LEVEL3       0x03
#define  APP_MODE_TIMEOUT            0x04
/////////////////////////////////////////////ACCMODE
#define  ACCMODE_OFF                 0X00
#define  ACCMODE_PASSIVE             0X01
#define  ACCMODE_STANDEBY            0X02
#define  ACCMODE_ACTIVECONTROL       0X03
#define  ACCMODE_BRAKEONLY           0X04
#define  ACCMODE_OVERFIDE            0X05
#define  ACCMODE_STANDSTILLACTIVE    0X06
#define  ACCMODE_STANDSTILLWAIT      0X07
#define  ACCMODE_TMPFAILURE          0X08
#define  ACCMODE_PERMANENTFAIL       0X09
#define  ACCMODE_NOUSUED             0XA
//////////////////////////////////////////////DNPSTATUS
#define  DNPSTATUS_OFF               0X00
#define  DNPSTATUS_PASSIVE           0X01
#define  DNPSTATUS_LCCREADY          0X02
#define  DNPSTATUS_DNPREADY          0X03
#define  DNPSTATUS_LCCRACTIVE2       0X04
#define  DNPSTATUS_ILCAACTIVE        0X05
#define  DNPSTATUS_DNPACTVIE         0X06
#define  DNPSTATUS_FAULT             0X07
#define  DNPSTATUS_MRC               0X08
#define  DNPSTATUS_FORBIDDEN         0X09
#define  DNPSTATUS_LCCACTIVE1        0X0A
#define  DNPSTATUS_NOUSED1           0X0B
#define  DNPSTATUS_NOUSED2           0X0C
#define  DNPSTATUS_NOUSED3           0X0D
#define  DNPSTATUS_NOUSED4           0X0E
#define  DNPSTATUS_NOUSED5           0X0F
/////////////////////////////////////////////DNPTAKEOVER
#define DNPTAKEOVERREQ_INACTIVE        0X00
#define DNPTAKEOVERREQ_WARNLEVEL1      0X01
#define DNPTAKEOVERREQ_WARNLEVEL2      0X02
#define DNPTAKEOVERREQ_WARNLEVEL3      0X03
#define DNPTAKEOVERREQ_SAFESTOP        0X04
#define DNPTAKEOVERREQ_EXIT            0X05
#define DNPTAKEOVERREQ_NOUSED1         0X06
#define DNPTAKEOVERREQ_NOUSED2         0X07
////////////////////////////////////////////////


#define   RETURN_OK              1
#define   RETURN_NOTOK           0

typedef struct  
       {
        uint8_t   u8DayNight;  /////白天或夜晚
        uint8_t   u8Mode;      ////未激活，激活，二级报警，三级报警 ，超时
        uint16_t  u16TimeCnt;  ////时间计数器 
        } strRgb;

void AppRgbInit(void);
void AppRgbTask(void);
uint8_t u8ReadRGBMode(void);
#endif
