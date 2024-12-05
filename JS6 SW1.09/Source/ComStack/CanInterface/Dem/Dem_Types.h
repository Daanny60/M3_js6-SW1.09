#ifndef _DEM_TYPES_H_
#define _DEM_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

#define DEM_DTC_DEBOUNCE_RST	(0U) /* 0: 仅test failed 去抖; */
#define DEM_DTC_DEBOUNCE_ACC	(1U)	/* 累加/减法去抖 test failed&passed  都采样去抖 */

#define DTC_NOT_STORAGE	  (0U)  /* 0: DTC 不存储*/
#define DTC_STORAGE_TO_EE (1U)	/* 1: DTC 存储*/
#define DTC_NOSELFRECOVER  (0u)
#define DTC_SELFRECOVER  (1u)

typedef union
{
	uint8_t status_byt;
  	struct{
		unsigned char test_failed                       :1;		//用以確定故障是否存在
		unsigned char test_failed_this_operation_cycle     :1;	//在這周期中，任何失敗的測試都曾發生
		unsigned char pending_dtc                       :1;		//故障狀態--當test至少通過一次且從未發生故障時，
															//在一個操作周期完成後才會清除。
		unsigned char confirmed_dtc                     :1;		//經過確認，--它並不總是表明，請求時麻煩就出現了
		unsigned char test_not_completed_since_last_clr   :1;	//測試未完成
		unsigned char test_failed_since_last_clr         :1;		//latch failure of bit1 ,only can be clear by cleardtc
		unsigned char test_not_completed_this_operation_cycle  :1;	//如果相關的DTC測試曾經運行過，在這個操作中完成
		unsigned char warning_indicator_requested        :1;		//當警告指示是需要的          
	} status_bits;
} dtc_status_t;

typedef enum
{
	DTC_RECORD_STORY_TO_RAM = 0,
	DTC_RECORD_STORY_TO_INT_FLASH,
	DTC_RECORD_STORY_TO_INT_EEPROM,
	DTC_RECORD_STORY_TO_EXT_FLASH,
	DTC_RECORD_STORY_TO_EXT_EEPROM
} dtc_story_mode_t;


typedef enum
{
	DTC_TEST_STATE_NO_RESULT = 0,
	DTC_TEST_STATE_FAILED,
	DTC_TEST_STATE_PASSED,
	DTC_TEST_STATE_UNKNOW
} test_result_state_e;


typedef uint8_t  dtc_freq_t; /* DTC  frequency(s)  */
typedef uint8_t  dtc_stry_start_addr_t; /* DTC  addres(s)  */

typedef  void (*dtc_callback)(void);

typedef struct
{
	dtc_freq_t  confirm_debounce_time;  /* Confirm Debounce delay Time ,  >= 1, < 255*/
	bool  debounce_mode; /* 0: 仅test failed 去抖; 1: test failed&passed	都采样去抖  見宏 DEM_DTC_DEBOUNCE_RST 和 DEM_DTC_DEBOUNCE_ACC */
	bool  story_mode;   /* 配置是否存储到EEPROM, 1 存储到EEPROM 見宏 DTC_NOT_STORAGE 和 宏 DTC_STORAGE_TO_EE */
	bool  recover_age;
}dtc_drv_cfg_t;


typedef struct{
	uint8_t c_main_ecu_supply_voltage;
	uint8_t c_main_ecu_power_mode;
	uint16_t c_main_ecu_vehicle_speed;
} dtc_snap_shot_data_t;

typedef struct{
	//uint8_t c_monitoring_cycle_counter[4];
	uint8_t  occurrence_counter;
	uint8_t  aged_counter;
	uint8_t  aging_counter;
	/* signed char cDTCFaultDetectionCounter; */
} dtc_extended_data_t;

typedef struct{
	/*  const dword *pDTCNumber; */
	dtc_status_t c_status;
	/* dtc_snap_shot_data_t SnapShotData[TOTAL_DTC_NUM]; */
	/* dtc_extended_data_t ExtendedData[TOTAL_DTC_NUM]; */
	/*uint8_t dtc_fault_detection_counter;*/
} dtc_record_t;



#define fld_sizeof(s, m) sizeof(((s *)0)->m)  //取s結構體中m的成員的長度
//#define offsetof(s,m)    (( (uint8_tnt8_t) &( ( (s*)0 )->m )) - (size_t)((s*)0))
#define offsetof1(s,m)   ( (uint8_t) &( ( (s*)0 )->m ))  //取s結構體中m的地址


#endif
