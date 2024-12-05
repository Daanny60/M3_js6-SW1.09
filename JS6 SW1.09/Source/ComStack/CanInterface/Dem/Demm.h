#ifndef _DEMM_H_
#define _DEMM_H_

#include "DEM_types.h"
#include "DEM_Cfg.h"
#include "error.h"
#include <stdint.h>


#define DTC_NOT_STORED_FLAG  (0XFFU)
#define DEM_DTC_INITED_FLAG  (0x55AAU)
#define DTC_MASK_INIT_VALUE   (0x50U)

#define DEM_ENTER_CRITICAL_METHOD  (3u)
#define DEM_DEV_ERR_CHECK (0u)  

#define DESC_TOTAL_ROUTINE_CONTROL_NUM          1u
#define DESC_TOTAL_ROUTINE_CONTROL_TYPE_NUM     3u 

#define DEM_EEPROM_RES_PROTECTED  1u   /* DEM �õ��Ĵ洢EPROM �����Ƿ�������Դ����*/

/* Define ControlDTCSetting sub service */
#define DESC_CONTROL_DTC_SETTING_ON                               (TRUE) //  1
#define DESC_CONTROL_DTC_SETTING_OFF                              (FALSE) //  2

/* Define Read Memory BY Address Sub Service */
#define DESC_READ_MEMORY_BY_ADDRESS_SHORT_ADDR                      0x14
#define DESC_READ_MEMORY_BY_ADDRESS_LONG_ADDR                       0x24

/* Define Read Memory BY Address Sub Service */
#define DESC_WRITE_MEMORY_BY_ADDRESS_SHORT_ADDR                     0x14
#define DESC_WRITE_MEMORY_BY_ADDRESS_LONG_ADDR                      0x24


#define DESC_SERVICE_REQ_LENGTH_NEED_CHECK                          0xff

#define DESC_TIMER_100MS                                            20 /* unit: can_desc_task Call Cycle Time * Value, here 5 ms * 20 =  100ms */
#define DESC_TESTER_PRESENT_TIME_OUT_DELAY_TIME                     140 /* unit: 100ms, here 50 * 100 ms =  5s */

/* Ӧ�ó���ӿ�*/
//void dem_set_test_result(dtc_handle_t dtc_index, test_result_state_e result);
//void dem_clear_test_result(dtc_handle_t dtc_index);
//void dem_task_init(void);
extern dtc_info_struct_t dtc_info_struct;   //快照和外部相關
void dem_task_init(void);
/**
* @brief dtc_task ����
* @retval none
*/
void dem_task(void);

/* 
����ͨ�����������Ƿ�ʹ��DTC ���*/
void dem_enable_dtc_record_update(void);
void dem_disable_dtc_record_update(void);


/* DCM ģ��ӿ�*/

/* 
����ͨ�����������UDS ����, ����DTC �Ƿ�ʹ��DTC���
*/
void dem_enable_dtc_setting(void);
void dem_disable_dtc_setting(void);


void dem_set_test_result(dtc_handle_t dtc_index, test_result_state_e result);


void dem_clear_test_result(dtc_handle_t dtc_index);

/*
void dem_get_status_of_dtc(void);
void dem_dcm_cancel_operation(void);
void dem_clear_all_dtc_record_information(void);
*/
/* ��DCM����*/
err_flag_e  dem_clear_dtc(const uint8_t *dtc);
/* 
����DTC_RECORD.STATE
*/
/*bool_t dtc_get_state(uint8_t dtc_index, uint8_t *state);*/
uint8_t  _dem_get_state(dtc_handle_t dtc_index);
void dem_test_fun(void);

void demm_operation_cycle_chaned_event_proc(void);
uint8_t dem_get_stored_index(dtc_handle_t dtc_index);
#endif
