#include "App_WheelHeat.h"
#include "Gpio.h"
#include "com.h"
#include "Com_Cfg.h"
#include "Dcm_Cfg.h"
#include "system_voltage_manage.h"
#include "Adc_Drv.h"
#include "string.h"
#include "pwm.h"
#include "App_Can.h"
#include "keyboard_config.h"
#include "Event.h"
#include "App_PID.h"
#include "Event.h"
#include "App_Demm.h"
extern IpduSWS_36A_bufType TxIpduSWS_36A;

#define FLAG_CMD_HEAT 0X02    /// HEAT
#define FLAG_CMD_STOP 0X01    /// STOP
#define FLAG_CMD_INVALID 0X00 /// INVALID

#define UNPID
// 作为IS过流的时间条件，当此计数器到达一定值后，IS过流才正式成立，目的是预防偶发性的IS干扰，而导致的IS误触发过流。
uint16_t is_overcurrent_cnt;
uint16_t s16temp_test;  // danny
uint8_t u8HeatCmd_test; // danny
uint8_t u8HeatStatusSignal = 0;

uint8_t u8PidResult;

int16_t s16PidSetPoint = 630; ////571;//35.6875  //36<<4;//33℃
PIDController pid;

App_WheelHeatType App_stWheelHeat;

#define RIM_LOOKUP_SIZE (161U)
// #define RIM_LOOKUP_SIZE 161U

static const uint16_t RimTempLookUp[RIM_LOOKUP_SIZE] = {
    0xF41B,                                                                         /* -40 */
    0xF370, 0xF2BE, 0xF203, 0xF141, 0xF076, 0xEFA3, 0xEEC7, 0xEDE1, 0xECF3, 0xEBFB, /* -30 */
    0xEAFA, 0xE9EF, 0xE8DA, 0xE7BB, 0xE691, 0xE55D, 0xE41F, 0xE2D5, 0xE181, 0xE024, /* -20 */
    0xDEB8, 0xDD43, 0xDBC2, 0xDA37, 0xD89F, 0xD6FE, 0xD552, 0xD39A, 0xD1D7, 0xD009, /* -10 */
    0xCE32, 0xCC50, 0xCA63, 0xC86D, 0xC66D, 0xC463, 0xC250, 0xC034, 0xBE0F, 0xBBE5, /* 0 */
    0xB9AE, 0xB772, 0xB52E, 0xB2E4, 0xB096, 0xAE3F, 0xABE3, 0xA983, 0xA71E, 0xA4B7, /* 10 */
    0xA249, 0x9FDB, 0x9D69, 0x9AF6, 0x987F, 0x960A, 0x9393, 0x911D, 0x8EA6, 0x8C2F, /* 20 */
    0x89BB, 0x8748, 0x84D8, 0x826A, 0x8000, 0x7D99, 0x7B36, 0x78D7, 0x767D, 0x7429, /* 30 */
    0x71D7, 0x6F8E, 0x6D49, 0x6B0A, 0x68D7, 0x66A4, 0x6478, 0x6258, 0x603D, 0x5E2D, /* 40 */
    0x5C1E, 0x5A19, 0x581E, 0x562B, 0x5441, 0x525B, 0x5080, 0x4EAD, 0x4CE4, 0x4B21, /* 50 */
    0x496A, 0x47B8, 0x460F, 0x4470, 0x42D8, 0x4148, 0x3FC3, 0x3E44, 0x3CCD, 0x3B5D, /* 60 */
    0x39F7, 0x3897, 0x373F, 0x35EE, 0x34A9, 0x3365, 0x322D, 0x30FB, 0x2FCD, 0x2EAD, /* 70 */
    0x2D8F, 0x2C76, 0x2B64, 0x2A5D, 0x295C, 0x285D, 0x2766, 0x2671, 0x2588, 0x24AB, /* 80 */
    0x23BF, 0x22E4, 0x2211, 0x213C, 0x2070, 0x1FAC, 0x1EE7, 0x1E2B, 0x1D6E, 0x1CBA, /* 90 */
    0x1C0A, 0x1B5E, 0x1AB6, 0x1A12, 0x1973, 0x18D3, 0x183D, 0x17A7, 0x1715, 0x168D, /* 100 */
    0x15FF, 0x157B, 0x14F7, 0x1477, 0x1403, 0x1388, 0x1312, 0x12A2, 0x1231, 0x11C5, /* 110 */
    0x1159, 0x10F2, 0x108B, 0x102A, 0xFCE, 0xF6B, 0xF0F, 0xEB7, 0xE60, 0xE14,       /* 120 */
};

static int16_t convertRimTemp(uint16_t rawTemp)
{
#define NO_EXIT_VALUE 1000 /* a random value greater than the array size */
#define BASE_TEMP (-40)    /* index 0 equals -40 deg. C */

    uint16_t fpRawTemp = rawTemp << 4U; /* boost A2D value to fixed point */
    uint8_t stIndex = 0u;
    uint8_t endIndex = RIM_LOOKUP_SIZE - 1U;
    uint8_t midIndex;
    uint8_t parts = 0u;
    int16_t retVal = NO_EXIT_VALUE;

    /* The two indexes close in on the desired value. If they meet and no value has been
     *  selected, you are searching at the very beginning or very end */
    /* The process looks to find a segment btw two table values that contains the rawTemp
     *  value or a table value equal to the rawTemp value.  If the segment is found, the
     *  fractional part of the index/realTemp is found by interpolation   */

    while ((stIndex < (endIndex - 1U)) && (retVal == NO_EXIT_VALUE))
    {
        midIndex = (stIndex + endIndex) / 2U;    /* select the next test target */
        if (fpRawTemp > RimTempLookUp[midIndex]) /* looking too far into the table? */
        {
            /* BIGRAW.....s...........R.......m..............<-e.........SMALLRAW  */
            endIndex = midIndex; /* adjust the focus to earlier part of table */
        }
        else if (fpRawTemp < RimTempLookUp[midIndex]) /* far enough in? */
        {
            /* BIGRAW.....s...........m.......R................e.........SMALLRAW  */
            /* does raw value fall between */
            if (fpRawTemp < RimTempLookUp[midIndex + 1U]) /* next is still not far enough */
            {
                /* BIGRAW.....s->.........m....m1..R................e.........SMALLRAW  */
                stIndex = midIndex; /* adjust the focus to later part of table */
            }
            else
            {   /* we have found the segment for interpolation  */
                /* BIGRAW.....s...........m....R.m1................e.........SMALLRAW  */
                parts = (uint8_t)(((RimTempLookUp[midIndex] - fpRawTemp) << 4u) /
                                  (RimTempLookUp[midIndex] - RimTempLookUp[midIndex + 1u]));
                retVal = (int16_t)midIndex;
            }
        }
        else /* not too hot and not too cold.  Must be equal  */
        {
            /* BIGRAW.....s...........m=R................e.........SMALLRAW  */
            retVal = (int16_t)midIndex; /* perfect match */
        }
    }
    /* this part resolves selections that exceed the table boundaries */
    if (retVal == NO_EXIT_VALUE) /* raw value is not on the table */
    {
        if (endIndex <= 1U)
        {

            retVal = 0; /* just the first table value */
        }
        else if (stIndex >= (RIM_LOOKUP_SIZE - 3U))
        {
            retVal = (int16_t)RIM_LOOKUP_SIZE; /* just beyond the last table value */
        }
        else
        {
            /* Do Nothing */
        }
    }
    return (int16_t)(((retVal + BASE_TEMP) << 4u) + parts); /* PRQA S 1860, 4533  */
}

uint16_t u16readntcvalue(void)
{
    return App_stWheelHeat.u16NtcValue;
}

void vHeatControl(void)
{
    int16_t s16temp;
    App_stWheelHeat.u16pidcntr++;
    if (App_stWheelHeat.u16pidcntr >= WHEEL_HEAT_PID_TIMER3S)
    {
        App_stWheelHeat.u16pidcntr = 0;
        s16temp = convertRimTemp(u16readntcvalue());
        u8PidResult = (uint8_t)PID_Compute(&pid, (float)s16PidSetPoint, (float)s16temp);
    }

    if (App_stWheelHeat.u8heatcntr < u8PidResult)
    {
        vHeatOutputOn();
        App_stWheelHeat.u8OnFlag = 1;
    }
    else
    {
        vHeatOutputOff();
        App_stWheelHeat.u8OnFlag = 0;
    }

    App_stWheelHeat.u8heatcntr++;
    if (App_stWheelHeat.u8heatcntr >= 100)
    {
        App_stWheelHeat.u8heatcntr = 0;
    }
}

uint8_t u8HeatFeedBack = CANRXHEATCMD_INVALID;
uint8_t u8OldHeatCmd = 0;
uint8_t u8OldHeatVehlMode = 0;
uint8_t u8flagheat = FLAG_CMD_STOP;
uint16_t u16delat = 0;
suion_flag_type u8flag_heat;

void App_wheelHeatInit(void)
{
    uint8_t i;
    memset(&App_stWheelHeat, 0, sizeof(App_stWheelHeat));
    u8OldHeatCmd = CANRXHEATCMD_INVALID;
    u8OldHeatVehlMode = 0; ////INVALID
    u8flagheat = FLAG_CMD_STOP;
    vAppHeatInit();
    u16delat = 0;
    u8flag_heat.flag = 0;
    u8HeatStatusSignal = 0;
    for (i = 0; i < 16; i++)
    {
        App_stWheelHeat.u16OnBuf[i] = 4095; /////max value
    }
    // 初始化PID
    PID_Init(&pid, 1.0, 0.02f, 10, 90, 0, 600, -600, 96, -96);
}

void vResetHeatTime(void)
{
    App_stWheelHeat.u32HeatTimer = 0;
}

/*
1、BCM CMD
2�?G
3�?云平�?
4�?语音
5、NTC故障
6、电源电�?
7、加热时间控�?

检测以上的状态决定是否开启加热功�?
*/
uint8_t u8CheckCondition(void)
{
    uint32_t u32ntcsum = 0;
    uint32_t u32issum = 0;
    uint32_t u32Onsum = 0;
    uint8_t u8result = CMDHEAT_ENABLE; ////////默认是开
    uint8_t system_voltage_mod;
    uint8_t i;
    uint16_t u16IsVauleNow = 0;
    uint8_t u8OverloadDetected = 0; // danny
    system_voltage_mod = system_voltage_mode_get();

    App_stWheelHeat.u8Index++;
    App_stWheelHeat.u8Index %= 16;
#ifdef PID
    if (App_stWheelHeat.u8OnFlag == 0)
    {
        App_stWheelHeat.u8Ntcindex++;
        App_stWheelHeat.u8Ntcindex %= 16;
        App_stWheelHeat.u16NtcBuf[App_stWheelHeat.u8Ntcindex] = ADC_Result_Get(ADC_WHL_HEAT_NTC_INDEX);
    }

#endif

#ifdef UNPID
    App_stWheelHeat.u16NtcBuf[App_stWheelHeat.u8Index] = ADC_Result_Get(ADC_WHL_HEAT_NTC_INDEX);
#endif

    u16IsVauleNow = ADC_ResulFiltertMv_Get(ADC_WHL_HEAT_IS_INDEX);
    App_stWheelHeat.u16ISBuf[App_stWheelHeat.u8Index] = u16IsVauleNow;
    if (App_stWheelHeat.u8OnFlag != 0)
    {
        App_stWheelHeat.u8OnIndex++;
        App_stWheelHeat.u8OnIndex %= 16;
        App_stWheelHeat.u16OnBuf[App_stWheelHeat.u8OnIndex] = u16IsVauleNow;
    }

    App_stWheelHeat.u8OnFlag = 0;
    for (i = 0; i < 16; i++)
    {
        u32ntcsum += App_stWheelHeat.u16NtcBuf[i];
        u32issum += App_stWheelHeat.u16ISBuf[i];
        u32Onsum += App_stWheelHeat.u16OnBuf[i];
    }
    App_stWheelHeat.u16NtcValue = u32ntcsum >> 4;
    App_stWheelHeat.u16ISValue = u32issum >> 4;
    App_stWheelHeat.u16OnValue = u32Onsum >> 4;
    App_stWheelHeat.u16ShortValue = u16IsVauleNow;
    if (App_stWheelHeat.u16NtcValue > ADC_WHL_HEAT_NTC_OPEN_THRESD) ////NTC OPEN
    {
        u8result = CMDHEAT_DISABLE_NTC_OPEN;
    }
    else if (App_stWheelHeat.u16NtcValue < ADC_WHL_HEAT_NTC_SHORT_THRESD) ////NTC SHORT
    {
        u8result = CMDHEAT_DISABLE_NTC_SHORT;
    }
    else if (system_voltage_mod != SYSTEM_VOLTAGE_MODE_NORMAL) /////VOLT NOT OK
    {
        u8result = CMDHEAT_DISABLE_VOLT_MODE;
    }
    else if (0 == u8ReadHeatEnableCondition())
    {
        u8result = CMDHEAT_DISABLE_CONDITON;
    }
    else if (App_stWheelHeat.u16ISValue > CURRENT_13A_VALUE) ///////CURRENT OVERLOAD
    {
        if(u16IsVauleNow < ADC_WHL_HEAT_SHORT_THRESD)  
        {
            u8result = CMDHEAT_DISABLE_CURRENT;
            u8OverloadDetected = 1;                 // 标记过流
            App_stWheelHeat.u8HeatSwitchStauts = 0; // 立即关闭加热
        }
    }
    else
    {
        u8OverloadDetected = 0; // 清除过流标志
    }
    App_stWheelHeat.u8OverloadDetected = u8OverloadDetected;
    return u8result;
}

/*加热丝开路 短路诊断记录*/
void vHeatShortOpenDiag(void)
{
    /// uint8_t u8DtcCondition=0;
    /// u8DtcCondition = u8GetMonitorDtcCondition();
    if (App_stWheelHeat.u16ShortValue > ADC_WHL_HEAT_SHORT_THRESD) ////heat short
    {
        App_stWheelHeat.u16HeatOpenCnt = 0;
        App_stWheelHeat.u16HeatShortCnt++;
        if (App_stWheelHeat.u16HeatShortCnt > WHEEL_HEAT_TIMER_2S)
        {
            u8flag_heat.heatflag.flag_heat_short = 1;
            //  if(u8DtcCondition)
            {
                AppDemmHeatShortFailureEvent(DTC_TEST_EVENT_FAILED);
            }

            App_stWheelHeat.u16HeatShortCnt = WHEEL_HEAT_TIMER_2S;
        }
    }
    else if (App_stWheelHeat.u16OnValue < ADC_WHL_HEAT_OPEN_THRESD) ////heat open
    {
        App_stWheelHeat.u16HeatShortCnt = 0;
        App_stWheelHeat.u16HeatOpenCnt++;
        if (App_stWheelHeat.u16HeatOpenCnt > WHEEL_HEAT_TIMER_2S)
        {
            u8flag_heat.heatflag.flag_heat_open = 1;
            // if(u8DtcCondition)
            {
                AppDemmHeatOpenFailureEvent(DTC_TEST_EVENT_FAILED);
            }
            App_stWheelHeat.u16HeatOpenCnt = WHEEL_HEAT_TIMER_2S;
        }
    }
    else
    {
        App_stWheelHeat.u16HeatOpenCnt = 0;
        App_stWheelHeat.u16HeatShortCnt = 0;
        if (u8flag_heat.heatflag.flag_heat_short)
        {
            u8flag_heat.heatflag.flag_heat_short = 0;
            // if(u8DtcCondition)
            {
                AppDemmHeatShortFailureEvent(DTC_TEST_EVENT_PASSED);
            }
        }
        if (u8flag_heat.heatflag.flag_heat_open)
        {
            u8flag_heat.heatflag.flag_heat_open = 0;
            // if(u8DtcCondition)
            {
                AppDemmHeatOpenFailureEvent(DTC_TEST_EVENT_PASSED);
            }
        }
    }
}

/*
ntc 开路 短路诊断记录
*/
void vHeatDiag(void)
{
    /// uint8_t u8DtcCondition=0;
    ///  u8DtcCondition = u8GetMonitorDtcCondition();
    ////////////////////////////////////////////////////////////////////ntc short/open
    if (App_stWheelHeat.u16NtcValue > ADC_WHL_HEAT_NTC_OPEN_THRESD)
    {
        App_stWheelHeat.u16NtcShortCnt = 0;
        App_stWheelHeat.u16NtcOpenCnt++;
        if (App_stWheelHeat.u16NtcOpenCnt > WHEEL_HEAT_TIMER_2S)
        {
            // if(u8DtcCondition)
            {
                AppDemmNtcShortOrOpenFailureEvent(DTC_TEST_EVENT_FAILED);
            }
            App_stWheelHeat.u16NtcOpenCnt = WHEEL_HEAT_TIMER_2S;
        }
    }
    else if (App_stWheelHeat.u16NtcValue < ADC_WHL_HEAT_NTC_SHORT_THRESD)
    {

        App_stWheelHeat.u16NtcOpenCnt = 0;
        App_stWheelHeat.u16NtcShortCnt++;
        if (App_stWheelHeat.u16NtcShortCnt > WHEEL_HEAT_TIMER_2S)
        {
            // if(u8DtcCondition)
            {
                AppDemmNtcShortOrOpenFailureEvent(DTC_TEST_EVENT_FAILED);
            }
            App_stWheelHeat.u16NtcShortCnt = WHEEL_HEAT_TIMER_2S;
        }
    }
    else
    {
        App_stWheelHeat.u16NtcShortCnt = 0;
        App_stWheelHeat.u16NtcOpenCnt = 0;
        vHeatShortOpenDiag();
        AppDemmNtcShortOrOpenFailureEvent(DTC_TEST_EVENT_PASSED);
    }
}

uint8_t u8GetNTCFaultFlag(void)
{
    uint8_t u8result = 0;
    if (u8flag_heat.heatflag.flag_ntc_open || u8flag_heat.heatflag.flag_ntc_short)
    {
        u8result = 1;
    }
    return u8result;
}

static void vOverHeatDiag(void)
{
    uint8_t u8DtcCondition = 0;
    int16_t s16temp;
    do{
      if (App_stWheelHeat.u16NtcValue < ADC_WHL_HEAT_NTC_SHORT_THRESD)
      {
          break;
      }
    App_stWheelHeat.u8overheatperiodcntr++;
    if (App_stWheelHeat.u8overheatperiodcntr >= WHEEL_OVERHEAT_PERIOD_1S) // 1s进入一次
    {
        u8DtcCondition = u8GetMonitorDtcCondition();
        App_stWheelHeat.u8overheatperiodcntr = 0;
        if (0 == u8GetNTCFaultFlag() && 1 == u8DtcCondition)
        {

            s16temp = convertRimTemp(u16readntcvalue());
            if (s16temp > WHEEL_OVERHEAT_TEMP_37_5)
            {
                App_stWheelHeat.u16overheatfaultcntr++;
                App_stWheelHeat.u16overheatrecovercntr = 0;
                if (App_stWheelHeat.u16overheatfaultcntr > WHEEL_OVERHEAT_FAULT_15S)
                {
                    AppDemmTemOver37_5FailureEvent(DTC_TEST_EVENT_FAILED);
                    App_stWheelHeat.u16overheatfaultcntr = WHEEL_OVERHEAT_FAULT_15S;
                }
            }
            else
            {
                App_stWheelHeat.u16overheatfaultcntr = 0;
                App_stWheelHeat.u16overheatrecovercntr++;
                if (App_stWheelHeat.u16overheatrecovercntr > WHEEL_OVERHEAT_RECOVER_10S)
                {
                    AppDemmTemOver37_5FailureEvent(DTC_TEST_EVENT_PASSED);
                    App_stWheelHeat.u16overheatrecovercntr = WHEEL_OVERHEAT_RECOVER_10S;
                }
            }
        }
        else
        {
            App_stWheelHeat.u16overheatfaultcntr = 0;
            App_stWheelHeat.u16overheatrecovercntr = 0;
        }
    }
    }
    while(0);

}

/* check if heat fun ok*/
uint8_t u8CheckHeatCmd(void)
{
    uint8_t u8result = CMDHEAT_DISABLE_CMD;
    volatile uint8_t u8heatcmd;

    u8heatcmd = App_stWheelHeat.u8HeatSwitchStauts + 1;

    volatile uint8_t u8heatcondition = u8CheckCondition();
    u8HeatCmd_test = u8CheckCondition(); // danny

    if (u8flag_heat.heatflag.flag_heat_short || u8flag_heat.heatflag.flag_heat_open)
    {
        App_stWheelHeat.u8HeatEnable = CMDHEAT_DISABLE_CMD;
    }
    else
    {
        App_stWheelHeat.u8HeatEnable = u8heatcondition;
    }
    if (CANRXHEATCMD_ENABLE == u8heatcmd)
    {
        if (CMDHEAT_ENABLE == u8heatcondition)
            u8result = CMDHEAT_ENABLE;
        vHeatDiag();
    }
    return u8result;
}

/* heat task fun */
void App_WheelHeat_Task(void)
{
    uint8_t u8cmd = 0;
    vOverHeatDiag();
    u8cmd = u8CheckHeatCmd();

    switch (App_stWheelHeat.Machine)
    {
    case WHEEL_HEAT_STATE_OFF:
        u8HeatFeedBack = CANRXHEATCMD_DISABLE;
        if (CMDHEAT_ENABLE == u8cmd) /////if enable
        {
            u8HeatFeedBack = CANRXHEATCMD_ENABLE;
            App_stWheelHeat.Machine = WHEEL_HEAT_STATE_ON;

#ifdef PID
            PID_ResetIntegral(&pid);
#endif

            App_stWheelHeat.u8heatcntr = 0;
            App_stWheelHeat.u16pidcntr = WHEEL_HEAT_PID_TIMER3S;
        }
        else /////if  not ENABLE
        {
            vHeatOutputOff();
            App_stWheelHeat.u8OnFlag = 0;
        }
        break;

    case WHEEL_HEAT_STATE_ON:
        u8HeatFeedBack = CANRXHEATCMD_ENABLE;

        if (CMDHEAT_ENABLE == u8cmd) /////if enable
        {
            if (App_stWheelHeat.u16NtcValue + u16delat < (ADC_WHL_HAET_ALL_ON_ADC - 50)) //
            {
                App_stWheelHeat.flag = 1;
            }
            else
            {
                App_stWheelHeat.flag = 0;
            }
            if (App_stWheelHeat.u16NtcValue + u16delat > ADC_WHL_HAET_ALL_ON_ADC)
            // if((App_stWheelHeat.u16NtcValue+u16delat>ADC_WHL_HAET_ALL_ON_ADC)&&(App_stWheelHeat.u16ISValue<VALUE_THRESD_OVER_LOAD))
            {
#ifdef PID
                vHeatControl();
#endif
#ifdef UNPID
                if ((App_stWheelHeat.flag == 1) && (App_stWheelHeat.u32HeatTimer > 0))
                {
                    vHeatOutputOn();
                    App_stWheelHeat.u8OnFlag = 1;
                    App_stWheelHeat.u32HeatTimer--;
                    App_stWheelHeat.u32unHeatTimer = WHEEL_HEAT_TIMER_6S;
                }
                else
                {
                    if (App_stWheelHeat.flag == 1)
                    {
                        vHeatOutputOff();
                        App_stWheelHeat.u32unHeatTimer--;
                        if (App_stWheelHeat.u32unHeatTimer == 0)
                        {
                            App_stWheelHeat.u32HeatTimer = WHEEL_HEAT_TIMER_6S;
                        }
                    }
                    else
                    {
                        vHeatOutputOn();
                        App_stWheelHeat.u8OnFlag = 1; // danny
                    }
                }

#endif
            }
            else
            {
                if (0 == App_stWheelHeat.u16HeatShortCnt)
                {
                    vHeatOutputOff();
                    App_stWheelHeat.u32HeatTimer = WHEEL_HEAT_TIMER_6S;
                    App_stWheelHeat.u8OnFlag = 0;
                }
            }
        }
        else
        {
            u8HeatFeedBack = CMDHEAT_DISABLE_CMD;
            App_stWheelHeat.Machine = WHEEL_HEAT_STATE_OFF;
        }

        break;

    case WHEEL_HEAT_STATE_OVERLOAD:
        if (CMDHEAT_ENABLE == u8cmd) /////if enable
        {
            //////////在过载的情况下如果收到使能的命令是否继续打开�?
            vHeatOutputOff();
            App_stWheelHeat.u8HeatSwitchStauts = 0;         // 立即关闭加热
            u8HeatStatusSignal = 1;                         // 设置状态为1
            App_stWheelHeat.Machine = WHEEL_HEAT_STATE_OFF; // 回到关闭状态
        }
        else
        {
            vHeatOutputOff();
            App_stWheelHeat.u8HeatSwitchStauts = 0;         // 立即关闭加热
            u8HeatStatusSignal = 1;                         // 设置状态为1
            App_stWheelHeat.Machine = WHEEL_HEAT_STATE_OFF; // 回到关闭状态
        }
        break;

    default:
        vHeatOutputOff(); // vHeatPwmDuty(0);//MODIFY
        App_stWheelHeat.Machine = WHEEL_HEAT_STATE_OFF;
        // App_stWheelHeat.u32HeatTimer=WHEEL_HEAT_TIMER_30MIN;
        break;
    }

    if (CMDHEAT_ENABLE != u8HeatFeedBack)
    {
        u8HeatFeedBack = CMDHEAT_DISABLE_CMD; ///////closed
    }
}

/**
 * @brief Send heat status signal,period is 100ms
 *
 */
static void vSendHeatStatusSignal(void)
{
    Com_SendSignal(COM_TXSIGSWS_HEATSTATUS, &u8HeatStatusSignal);
}

uint8_t u8HeatGetStatus(void)
{
    return App_stWheelHeat.u8HeatSwitchStauts;
}

/**
 * @brief Check the heating switch/signal status
 *
 */
void App_HeatSignal_Task(void)
{
    static uint8_t u8HeatSwitchFlag = 0;
    static uint8_t u8sendsigcntr = 0;
    AppIpduAds_560_TimeoutCheck();
    uint32_t u32heatbtn;
    u32heatbtn = vHeatGetInputIO();

    /*
    if(CMDHEAT_ENABLE != App_stWheelHeat.u8HeatEnable)
    {
        if(u8HeatStatusSignal == 3)
        {
            u8HeatStatusSignal = 1;
        }
        u8flag_heat.heatflag.flag_heat_short=0;
        u8flag_heat.heatflag.flag_heat_open=0;
        App_stWheelHeat.u16HeatOpenCnt=0;
        App_stWheelHeat.u16HeatShortCnt=0;
        App_stWheelHeat.u8HeatSwitchStauts = 0;
        vLedSwitchOff();
        u32heatbtn = 0;
    }
    */

    // 连续检测到3次为1则认为按键触发
    if (u32heatbtn)
    {
        App_stWheelHeat.u16HeatSwitchCnt++;
        if (App_stWheelHeat.u16HeatSwitchCnt > 3)
        {
            App_stWheelHeat.u16HeatSwitchCnt = 3;

            if (u8HeatSwitchFlag == 0) // First time to trigger
            {
                if (App_stWheelHeat.u8HeatSwitchStauts == 0)
                {
                    App_stWheelHeat.u8HeatSwitchStauts = 1;
                    u8HeatStatusSignal = 3;
                }
                else
                {
                    App_stWheelHeat.u8HeatSwitchStauts = 0;
                    u8HeatStatusSignal = 1;
                    AppDemmHeatShortFailureEvent(DTC_TEST_EVENT_PASSED);
                }
            }
            u8HeatSwitchFlag = 1;
        }
    }
    else
    {
        App_stWheelHeat.u16HeatSwitchCnt = 0;
        u8HeatSwitchFlag = 0;
    }

    App_stWheelHeat.u8RemoteSwitchStauts = u8ReadRemoteHeatSwitchStatus();
    if (CMDHEAT_ENABLE == App_stWheelHeat.u8HeatEnable)
    {
        /*Remote control switch from CAN*/
        if (App_stWheelHeat.u8RemoteSwitchStauts == 1)
        {
            App_stWheelHeat.u8RemoteSwitchStauts = 0;
            App_stWheelHeat.u8HeatSwitchStauts = 1;
            u8HeatStatusSignal = 3;
        }
    }

    if (CMDHEAT_ENABLE != App_stWheelHeat.u8HeatEnable)
    {
        vLedSwitchOff();
        u8HeatStatusSignal = 1;
    }
    else
    {
        if (App_stWheelHeat.u8HeatSwitchStauts == 1u)
        {
            vLedSwitchOn();
            u8HeatStatusSignal = 3; // danny
        }
        else
        {
            vLedSwitchOff();
            u8HeatStatusSignal = 1; // danny
        }
    }

    u8sendsigcntr++;
    if (u8sendsigcntr >= 10)
    {
        u8sendsigcntr = 0;
        vSendHeatStatusSignal();
    }
}
