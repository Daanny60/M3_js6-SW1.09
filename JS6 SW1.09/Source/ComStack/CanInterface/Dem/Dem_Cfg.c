#include "DEM_Cfg.h"
#include "DCM_CFG.h"
#include "Dem_Types.h"
#include "keyboard_config.h"
#if(0)
const uint32_t SupportedDTC[TOTAL_DTC_NUM] = {

/*0*/   {0x9E0507u},/* VOICE */
/*1*/   {0x9E0107u},/* RIGHT BACK  */
/*2*/   {0x9E1907u},/* SILENCE */
/*3*/   {0x9E1A07u},/* RIGHT STAR       右自定义 */
/*4*/   {0x9E5C00u},/* CROSS COUNTRY    越野大师 */
/*5*/   {0x9E0607u},/* 360 MEDIA */
/*6*/   {0x9E2F07u},/* SPEED ADD */
/*7*/   {0x9E3407u},/* DISTANCE  ADD */
/*8*/   {0x9E3307u},/* DISTANCE DEC */
/*9*/   {0x9E3007u},/* SPEED DEC */
/*10*/  {0x9E4500u},/* INFORMATION      行车信息 */
#ifdef  SWSCONFIG_HIGH
/*11*/  {0x9E2C07u},/* left star        左自定义*/
#else 
/*11*/  {0x9E2D07u},/* DIPILOT */
#endif
/*12*/  {0x9E0207u},/* PHONE */
/*13*/  {0x9E0807u},/* RIGHT NEXT  */
/*14*/  {0x9E1C07u},/* RIGHT MENU */
#ifdef SWSCONFIG_HIGH
/*15*/  {0x9E4307u},/* LEFT PICK  */
/*16*/  {0x9E4407u},/* RIGHT PICK  */
/*17*/   {0x9E4113u},/* HEAT SHORT */
/*18*/   {0x9E4114u},/* HEAT OPEN */
/*19*/   {0x9E4213u},/* NTC SHORT */
/*20*/   {0x9E4214u},/* NTC OPEN */
#endif
#ifdef SWSCONFIG_MID
/*15*/   {0x9E4113u},/* HEAT SHORT */
/*16*/   {0x9E4114u},/* HEAT OPEN */
/*17*/   {0x9E4213u},/* NTC SHORT */
/*18*/   {0x9E4214u},/* NTC OPEN */
#endif
};

const dtc_drv_cfg_t dtc_cfg_tbl[TOTAL_DTC_NUM] = {

/*0*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* voice struck */
/*1*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right back struck */
/*2*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* silence struck */
/*3*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right star  struck */
/*4*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* cross country struck */
/*5*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* media struck */
/*6*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* speed add struck */
/*7*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* distance add struck */
/*8*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* dis dec struck */
/*9*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* speed dec struck */
/*10*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* driver information struck */
/*11*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/*left star struck */
/*12*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* phone struck */
/*13*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right next  struck */
/*14*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right menu struck */
#ifdef  SWSCONFIG_HIGH
/*15*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* left pick struck */
/*16*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right pick struck */
/*17*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right pick struck */
/*18*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right pick struck */
/*19*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right pick struck */
/*20*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right pick struck */
#endif
#ifdef  SWSCONFIG_MID
/*15*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* left pick struck */
/*16*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right pick struck */
/*17*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right pick struck */
/*18*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE},/* right pick struck */
#endif
};
#else
const uint32_t SupportedDTC[TOTAL_DTC_NUM] = {
{0x900216}, // Voltage Low
{0x900117},//Voltage Over
{0xC07388},//bus off
{0xC14087},// COM TimeOut
{0xA5C013},//Heat R  open 
{0xA5C011}, //Heat R  connect GND
{0xA5102A},//NTC open or short .
{0xA5C1F0},// HeatOver
};
const dtc_drv_cfg_t dtc_cfg_tbl[TOTAL_DTC_NUM] = {
/*0*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* voltage low */
/*1*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* voltage high*/
/*2*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* bus off */
/*3*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* bcm time out */
/*4*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* heat wire open */
/*5*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* heat wire short */
/*6*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* NTC open or short */
/*7*/   {1, DEM_DTC_DEBOUNCE_RST, DTC_STORAGE_TO_EE,DTC_SELFRECOVER},/* steering wheel is overheating*/


};


#endif




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
