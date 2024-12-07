#include "DEM_Cfg.h"
#include "DCM_CFG.h"
#include "Dem_Types.h"
#include "keyboard_config.h"

const uint32_t SupportedDTC[TOTAL_DTC_NUM] = {
{0x900216},// Voltage Low         电压低
{0x900117},//Voltage Over         电压高
{0xC07388},//bus off              连续6次检测到busoff 故障，且中间无成功通信
{0xC14087},// COM TimeOut         持续500ms时间内，未收到BCM发送的0x2CD报文
{0xA5C013},//Heat R  open         加热丝开路
{0xA5C011}, //Heat R  connect GND 加热丝对地短路
{0xA5102A},//NTC open or short    NTC开路或者短路
{0xA5C1F0},// HeatOver.           NTC检测温度>35+2.5℃
{0xC10087}, //未收到EMS发送的0x117报文
{0xA5P001}, //IS管脚检测到反馈电流<0
};
const dtc_drv_cfg_t dtc_cfg_tbl[TOTAL_DTC_NUM] = 
{
/*0*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* voltage low */
/*1*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* voltage high*/
/*2*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* bus off */
/*3*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* bcm time out */
/*4*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* heat wire open */
/*5*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* heat wire short */
/*6*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* NTC open or short */
/*7*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* steering wheel is overheating*/
/*8*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/*  COM TimeOut_EMS */
/*9*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* IS < 0*/
};


/*M3 无快照/扩展*/
#if (DTC_SNAP_SHOT_DATA_ENABLE)
const uint8_t c_dtc_snap_shot_data_id[TOTAL_DTC_SNAP_SHOT_ID_NUM][2] = { 
  {0x04, 0x0E}, {0x04, 0x0F}, {0x04,0x10},
};

//各快照的內容長度
const uint8_t c_dtc_snap_shot_data_size[TOTAL_DTC_SNAP_SHOT_ID_NUM] = {
 _SnapShort_size_0, _SnapShort_size_1, _SnapShort_size_2,
};

#endif


#if (DTC_EXTENDED_DATA_ENABLE)
const uint8_t c_dtc_extended_data_id[TOTAL_DTC_EXTENDED_DATA_NUM] = {
  0x01, 0x02, 0x03, //0x04, 0x10,
};
#endif
