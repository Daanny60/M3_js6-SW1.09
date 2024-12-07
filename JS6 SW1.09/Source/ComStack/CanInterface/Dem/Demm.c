/*
 * @copyright: KUWE technologies Co.,Ltd. 2013-2015.all rights reserved.
 * @file: DEMM.c
 * @brief:  DEMM.c
 * @author:
 * @version: V1.0.0
 * @date: 2014-08-01
 * @history:
 *	   1.
 *  author:
 *  version:
 *  date:
 *  modification:
 */
#include "Demm.h"
#include "fm33lg0xx_fl.h"
#include "string.h"
#include "types.h"
#include "memory.h"
#include "system_voltage_manage.h"
//#include "App_Mfs.h"
#include "dem_cfg.h"
#include "eeprom_cfg.h"
#include "App_Can.h"

#define DEMM_CPU_SR_ALLOC()			uint32_t primask
#define DEMM_ENTER_CRITICAL()        {primask = __get_PRIMASK();__disable_irq();}
#define DEMM_EXIT_CRITICAL()         __set_PRIMASK(primask)

#define DEM_EEPM_SR_ALLOC()
#define DEM_EEPROM_PRO()
#define DEM_EEPROM_RLS()






typedef struct {
	uint8_t storaged_unit_size; /* debug only */
	uint8_t storage_total_dtc_numbers;		//表示需要儲存到eeprom的 dtc的總數
	uint8_t result[TOTAL_TEST_RESULT_NUM];		//以位表示的方式存储每个 DTC 的结果。每 2 位表示一个 DTC 的结果
	uint8_t res_last[TOTAL_TEST_RESULT_NUM];		//同上，是上一次的結果---??? 作用是為了 dem_get_test_result_updated服務 ，還沒有 理解透；
	uint8_t need_to_be_written_to_eeprom[TOTAL_DTC_FLAG_NUM]; //DTC_MONITORING_CYCLE_NUM   每1位表示一個DTC
	uint8_t not_stored_since_last_test_passed[TOTAL_DTC_FLAG_NUM];   //power up時 不需要儲存的標志
	//uint8_t has_been_stored_since_power_up_flag[TOTAL_DTC_FLAG_NUM];
	//uint8_t self_recover_flag[TOTAL_MAX_STORED_DTC_NUM/8+1];
	uint8_t occurrence_counter[TOTAL_MAX_STORED_DTC_NUM]; //记录每个 DTC 发生的次数
//	uint8_t self_healing_counter[TOTAL_MAX_STORED_DTC_NUM];    //20240717 modify extenddatas do not need
    uint8_t aged_counter[TOTAL_MAX_STORED_DTC_NUM];       //add,extenddata need
    uint8_t aging_counter[TOTAL_MAX_STORED_DTC_NUM];      //add,extenddata need
	uint8_t samp_count[TOTAL_DTC_NUM];
	unsigned b_control_dtc_setting: 1;	//设置标志，表示是否设置了某个 DTC
	unsigned b_set_enabled:1;	//使能标志，表示是否启用了某个 DTC
	unsigned b_last_control_dtc_setting: 1;  //上一次未使能，当前使能，清除状态。用于处理 DTC 的使能状态变化
	unsigned b_last_set_enabled:1;  //？？
	unsigned :4;
}demm_ctrl_t;

STATIC demm_ctrl_t demm_dtc;	//總的結構體  

dtc_info_struct_t dtc_info_struct;   //快照和外部相關

STATIC dtc_record_t dtc_record[TOTAL_DTC_NUM];	//內部 11個id 的dtc

extern const dtc_drv_cfg_t dtc_cfg_tbl[NUM_OF_DTC];		//11個配置

#define Make32Bit(HiByte,MiByte,LoByte)  ((uint32)((((uint32)(HiByte)) << 16)|((uint32)(MiByte) << 8)|((uint32)(LoByte))))

#if (TEST_RESULT_STATE_BIT_LENGTH == (2U))
	#define TEST_ST_BYT_RR      (2U)
	#define TEST_ST_BIT_NUM     (0x3U)
	#define TEST_ST_BIT_MASK    (0x3U)
#else if(TEST_RESULT_STATE_BIT_LENGTH == (1U))
    #define TEST_ST_BYT_RR	     (3U)
    #define TEST_ST_BIT_NUM     (0x1U)
    #define TEST_ST_BIT_MASK    (0x7U)
#endif

#define DEM_TOTAL_STORED_DTC_NUM     (demm_dtc.storage_total_dtc_numbers)
#define DTC_STORED_UNIT_BYTE_LENGTHS ((uint16_t)sizeof(dtc_stored_data_t))
#define TOTAL_DTC_STORED_DATA_LENGTH (DTC_STORED_UNIT_BYTE_LENGTHS*DEM_TOTAL_STORED_DTC_NUM)

#if 1
#define DEM_NVM_READ(addr, pBuf, len)   Eeprom_RamRead((pBuf),(addr),(len))
#define DEM_NVM_WRITE(addr, pBuf, len)  Eeprom_RamWrite((pBuf),(addr),(len))
#else
#define DEM_NVM_READ(addr, pBuf, len)   nvm_read((pBuf),(addr),(len))
#define DEM_NVM_WRITE(addr, pBuf, len)  nvm_write((pBuf),(addr),(len))
#endif


#if(DTC_SNAP_SHOT_DATA_ENABLE!=STD_OFF)
static void dem_odmet_buffer(uint8_t *pbuf, uint32_t datas)
{
	/* odmet 只有低24位有效*/
	pbuf[0] = (uint8_t)((datas >> 16u) & 0xff);
	pbuf[1] = (uint8_t)((datas >> 8) & 0xff);
	pbuf[2] = (uint8_t)((datas) & 0xff);
	//pbuf[3] = (uint8_t)((datas >> 24) & 0xff);
}
#define _SnapShort_size_all   (sizeof(dtc_snap_shot_data_t))
#else
#define _SnapShort_size_all   (0U)
#endif

#if(DTC_EXTENDED_DATA_ENABLE!=STD_OFF)
#define _extdata_size_all   (sizeof(dtc_extended_data_t))
#else
#define _extdata_size_all   (0U)
#endif

#define  DTC_STORED_DATA_UNIT_SIZE  ((_SnapShort_size_all) + (_extdata_size_all))

////static bool dem_mem_compare(const uint8_t *pdata1, const uint8_t *pdata2, uint16_t len)
////{
////	uint16_t i;
////	for(i = 0; i < len; i++)
////	{
////		if(pdata1[i] != pdata2[i])
////		{
////			return FALSE;
////		}
////	}
////	return TRUE;
////}

static void dem_mem_clear(uint8_t *pdata, uint16_t len)
{
	uint16_t i;
	for(i = 0; i < len; i++)
	{
		pdata[i] = 0;
	}
}

static void dem_mem_set(uint8_t *pdata, uint8_t value, uint16_t len)
{
	uint16_t i;
	for(i = 0; i < len; i++)
	{
		pdata[i] = value;
	}
}

#if (DEM_DTC_STORAGE_TO_EEPROM_EN ==1)
/*  stred_dtc_ind  由调用函数判断有效范围----清除和儲存到EEPROM相關的DTC*/
static void dem_stored_dtc_clear(uint8_t stred_dtc_ind)
{
	uint8_t *pdata;
	uint8_t i;
#if((DTC_SNAP_SHOT_DATA_ENABLE!=STD_OFF) || (DTC_EXTENDED_DATA_ENABLE!=STD_OFF))   /* not used for BAIC */

#endif
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].dtc_status.status_byt = DTC_MASK_INIT_VALUE; // 0
#if(DTC_SNAP_SHOT_DATA_ENABLE!=STD_OFF) 
	/* 指向快照數據首地址*/
	for(i = 0; i < TOTAL_DTC_SNAP_SHOT_STORED_NUM; i ++)
	{
		pdata = &dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCSnapShotData[i].c_main_ecu_supply_voltage; //DTCSnapShotData
		dem_mem_set(pdata, 0x00, DTC_STORED_DATA_UNIT_SIZE);
	}
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].SnapShotWritePonit = 0;
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].NumOfSnapShot = 0;
#endif
#if	(DTC_EXTENDED_DATA_ENABLE!=STD_OFF)  /* not used for BAIC */
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.occurrence_counter = 0;
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.aging_counter = 0;
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.aged_counter = 0;
#endif
	demm_dtc.occurrence_counter[stred_dtc_ind] = 0;
	demm_dtc.aging_counter[stred_dtc_ind] = 0;
    demm_dtc.aged_counter[stred_dtc_ind] = 0;
}


/**
 * @brief  給dtc_info_struct.dtc_stored_dt 賦值
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
static void dem_stored_dtc_sets(uint8_t stred_dtc_ind, const uint8_t *psource_data)
{
#if((DTC_SNAP_SHOT_DATA_ENABLE!=STD_OFF) || (DTC_EXTENDED_DATA_ENABLE!=STD_OFF))   /* not used for BAIC */
	uint8_t i;
	uint8_t *pdata;
	uint8_t *pdata2;
	uint8_t extend_len=0;
#endif
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].dtc_status.status_byt = psource_data[0];
#if((DTC_EXTENDED_DATA_ENABLE!=STD_OFF)) /* not used for BAIC */
    extend_len= _extdata_size_all;
    pdata2=&dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.occurrence_counter;
    memory_copys(pdata2, &psource_data[1], _extdata_size_all); 
#endif
#if(DTC_SNAP_SHOT_DATA_ENABLE!=STD_OFF)
        
        for(i = 0; i < TOTAL_DTC_SNAP_SHOT_STORED_NUM; i ++)
        {
          pdata = &dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCSnapShotData[i].c_main_ecu_supply_voltage; //DTCSnapShotData 
		  memory_copys(pdata, &psource_data[1 + extend_len+i * _SnapShort_size_all], _SnapShort_size_all); 
         // memory_copys(pdata, &psource_data[1 + i * DTC_STORED_DATA_UNIT_SIZE], DTC_STORED_DATA_UNIT_SIZE);  //這一句是有疑問的  
        }
        dtc_info_struct.dtc_stored_dt[stred_dtc_ind].SnapShotWritePonit = psource_data[1 + extend_len+i * _SnapShort_size_all];
		dtc_info_struct.dtc_stored_dt[stred_dtc_ind].NumOfSnapShot = psource_data[2 + extend_len+i * _SnapShort_size_all];

#endif
}


/*  必須先調用了  dem_task_init () 或dem_stored_dtc_index_init()  */
uint8_t dem_get_stored_index(dtc_handle_t dtc_index)
{
	uint8_t index;
	if( dtc_cfg_tbl[dtc_index].story_mode != DTC_NOT_STORAGE)  //要儲存到eeprom
	{
		for(index = 0; index<demm_dtc.storage_total_dtc_numbers; index++)
		{
			if(dtc_info_struct.stored_dtc_index[index]  == dtc_index)
			{
				return index;
			}
		}
	}
	return DTC_NOT_STORED_FLAG;
}

static void dem_dtc_snap_short_record(uint8_t stred_dtc_ind)
{
	uint8_t *snap_shot_write_ponit;

	if(stred_dtc_ind < DEM_TOTAL_STORED_DTC_NUM)
	{
		snap_shot_write_ponit = &dtc_info_struct.dtc_stored_dt[stred_dtc_ind].SnapShotWritePonit;
		if(snap_shot_write_ponit[0] >= TOTAL_DTC_SNAP_SHOT_STORED_NUM)
		{
			snap_shot_write_ponit[0] = 0;
		}

		dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCSnapShotData[snap_shot_write_ponit[0]].c_main_ecu_supply_voltage = SystemVoltageGetForDtcSnapShot();
		dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCSnapShotData[snap_shot_write_ponit[0]].c_main_ecu_power_mode = App_PEPS_PwrMod_Get();
		dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCSnapShotData[snap_shot_write_ponit[0]].c_main_ecu_vehicle_speed= App_VehicleSpeed_Get();
		snap_shot_write_ponit[0] ++;
		if (dtc_info_struct.dtc_stored_dt[stred_dtc_ind].NumOfSnapShot < TOTAL_DTC_SNAP_SHOT_STORED_NUM)
		{
			dtc_info_struct.dtc_stored_dt[stred_dtc_ind].NumOfSnapShot ++;
		}
	}
}

static void dem_dtc_extended_data_record(uint8_t stred_dtc_ind)
{
#if (DTC_EXTENDED_DATA_ENABLE != STD_OFF)
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.occurrence_counter = demm_dtc.occurrence_counter[stred_dtc_ind];
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.aged_counter = demm_dtc.aged_counter[stred_dtc_ind];
	dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.aging_counter = demm_dtc.aging_counter[stred_dtc_ind];
#endif
}

//static void dem_dtc_record_storay(uint8_t stred_dtc_ind)
static void  dem_dtc_record_storay(dtc_handle_t dtc_index)
{
	uint16_t ee_addr;
	//uint8_t ee_result;
	//uint8_t ee_tmpdata[DTC_STORED_UNIT_BYTE_LENGTHS];
	uint8_t *pdata;
	uint8_t index = dem_get_stored_index(dtc_index);
	if(index != DTC_NOT_STORED_FLAG)
	{

		if(dtc_info_struct.dtc_stored_dt[index].dtc_status.status_byt != dtc_record[dtc_index].c_status.status_byt)
		{
			dtc_info_struct.dtc_stored_dt[index].dtc_status.status_byt = dtc_record[dtc_index].c_status.status_byt;
			pdata = &dtc_info_struct.dtc_stored_dt[index].dtc_status.status_byt;
			ee_addr = DEM_DTC_STORAGE_EEPROM_START_ADDR + ((DTC_STORED_UNIT_BYTE_LENGTHS)*index);
			(void)DEM_NVM_WRITE(ee_addr, pdata, DTC_STORED_UNIT_BYTE_LENGTHS);
		}
	}
}

static void dem_stored_dtc_status_read(void)
{
	dtc_handle_t index;
	uint16_t start_addr, ee_result;
	uint8_t ee_tmpdata[DTC_STORED_UNIT_BYTE_LENGTHS];

	for(index = 0; index < DEM_TOTAL_STORED_DTC_NUM; index++)
	{
		start_addr = DEM_DTC_STORAGE_EEPROM_START_ADDR + ((DTC_STORED_UNIT_BYTE_LENGTHS)*index);
		ee_result = DEM_NVM_READ(start_addr, ee_tmpdata, DTC_STORED_UNIT_BYTE_LENGTHS);
		if(ee_result == EEPROM_TRUE)
		{
			if((ee_tmpdata[0] == 0) || (ee_tmpdata[0] == 0xffu))	//记录状态为0，扩展数据清除
			{
				dem_mem_clear(&ee_tmpdata[0], sizeof(dtc_stored_data_t));
				ee_tmpdata[0] = DTC_MASK_INIT_VALUE;
#if (DTC_EXTENDED_DATA_ENABLE != STD_OFF)
				ee_tmpdata[1] = 0;
				ee_tmpdata[2] = 0;
				ee_tmpdata[3] = 0;
#endif
			}
			else
			{
				ee_tmpdata[0] &= 0xfcu;
				ee_tmpdata[0] |= 0x50u;
			}
#if((DTC_SNAP_SHOT_DATA_ENABLE == STD_OFF) && (DTC_EXTENDED_DATA_ENABLE == STD_OFF))   /* not used for BAIC */
			dtc_info_struct.dtc_stored_dt[index].dtc_status.status_byt = ee_tmpdata[0];
#else
			dem_stored_dtc_sets(index, ee_tmpdata);  /* 讀取存儲的快照數據*/
#endif
		}
		else  /*  讀取失敗, 應該記錄DTC_EEPROM 故障其它故障狀態如何初始化  */
		{
			dtc_info_struct.dtc_stored_dt[index].dtc_status.status_byt = DTC_MASK_INIT_VALUE; 
			dem_mem_set(&dtc_info_struct.dtc_stored_dt[index].DTCSnapShotData[0].c_main_ecu_supply_voltage, 0x00, DTC_STORED_DATA_UNIT_SIZE);
		}
#if (DTC_EXTENDED_DATA_ENABLE != STD_OFF)
		if((dtc_info_struct.dtc_stored_dt[index].DTCextended_data.occurrence_counter == 0xff)
				&&(dtc_info_struct.dtc_stored_dt[index].DTCextended_data.aging_counter == 0xff) 
				&&(dtc_info_struct.dtc_stored_dt[index].DTCextended_data.aged_counter == 0xff))    //未存储过
		{
			dtc_info_struct.dtc_stored_dt[index].DTCextended_data.occurrence_counter = 0;
			dtc_info_struct.dtc_stored_dt[index].DTCextended_data.aged_counter = 0;
			dtc_info_struct.dtc_stored_dt[index].DTCextended_data.aging_counter = 0;
		}
		if(dtc_info_struct.dtc_stored_dt[index].dtc_status.status_byt == DTC_MASK_INIT_VALUE)   //不存储，读取失败，擦除情况下临时取该值
		{
			demm_dtc.occurrence_counter[index] = 0;
			demm_dtc.aging_counter[index] = 0;
			demm_dtc.aged_counter[index] = 0;
		}
		else
		{
			demm_dtc.occurrence_counter[index] = dtc_info_struct.dtc_stored_dt[index].DTCextended_data.occurrence_counter;
			demm_dtc.aging_counter[index] = dtc_info_struct.dtc_stored_dt[index].DTCextended_data.aging_counter;
			demm_dtc.aged_counter[index] = dtc_info_struct.dtc_stored_dt[index].DTCextended_data.aged_counter;
		}
#endif
	}
	//cpu_nop();
}
#endif

static void dem_clear_all_test_result(void)     //读取存储前，初始化，DCM开关初始化和清除时
{
	DEMM_CPU_SR_ALLOC();			
	DEMM_ENTER_CRITICAL();        
	//dem_mem_clear(demm_dtc.self_recover_flag, DEM_TOTAL_STORED_DTC_NUM/8+1);     
	dem_mem_clear(demm_dtc.res_last, TOTAL_TEST_RESULT_NUM);
	dem_mem_clear(demm_dtc.result, TOTAL_TEST_RESULT_NUM);
	dem_mem_clear(demm_dtc.samp_count, TOTAL_DTC_NUM);

	DEMM_EXIT_CRITICAL();
}


/**
 * @brief  真正的外部接口；用於記錄DTC的結果
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
void dem_set_test_result(dtc_handle_t dtc_index, test_result_state_e result)
{

	uint8_t byte_number,tmp,mask,mask1;
	DEMM_CPU_SR_ALLOC();
	if(dtc_index < TOTAL_DTC_NUM)
	{
		byte_number = (dtc_index >> (TEST_ST_BYT_RR));
		tmp = (dtc_index & (TEST_ST_BIT_MASK)) * (TEST_RESULT_STATE_BIT_LENGTH);
		mask = ((uint8_t)~((TEST_ST_BIT_NUM)<< tmp));
		mask1 = (result&(TEST_ST_BIT_NUM)) << tmp;

		DEMM_ENTER_CRITICAL();
		demm_dtc.result[byte_number] &= mask;
		demm_dtc.result[byte_number] |= mask1;
		DEMM_EXIT_CRITICAL();
	}


}


void dem_clear_test_result(dtc_handle_t dtc_index)
{

	uint8_t byte_number,tmp;
	DEMM_CPU_SR_ALLOC();
	/*if(dtc_index < TOTAL_DTC_NUM)*/
	{
		byte_number = (dtc_index >> TEST_ST_BYT_RR);
		tmp = (dtc_index & TEST_ST_BIT_MASK) * (TEST_RESULT_STATE_BIT_LENGTH);
		tmp = ((uint8_t)~((TEST_ST_BIT_NUM) << tmp));
		DEMM_ENTER_CRITICAL();
		demm_dtc.res_last[byte_number] &= tmp;
		demm_dtc.result[byte_number] &= tmp;
		demm_dtc.samp_count[dtc_index] = 0;
		//demm_dtc.self_recover_flag&=~(1<<dtc_index);    //第n位置0，一位代表一个flag
		//demm_dtc.occurrence_counter[dtc_index] = 0;    //清存储的时候把临时已经清了
		DEMM_EXIT_CRITICAL();
	}

}
//
////static uint8_t dem_get_test_result(dtc_handle_t dtc_index)
////{
////	uint8_t tmprst = 0;
////
////	uint8_t byte_number = (dtc_index >> TEST_ST_BYT_RR);
////	uint8_t tmp = (dtc_index & TEST_ST_BIT_MASK) * (TEST_RESULT_STATE_BIT_LENGTH);
////	DEMM_CPU_SR_ALLOC();
////	DEMM_ENTER_CRITICAL();
////	tmprst = (demm_dtc.result[byte_number] >> tmp);
////	DEMM_EXIT_CRITICAL();
////	tmprst &= (TEST_ST_BIT_NUM);
////
////	return tmprst;
////}

static bool dem_get_test_result_updated(dtc_handle_t dtc_index, uint8_t *result)
{
	bool flag_changed = FALSE;

	///uint8_t mask1,tmprst = 0;
     uint8_t tmprst = 0;
	uint8_t byte_number = (dtc_index >> (TEST_ST_BYT_RR));
	uint8_t tmp = (dtc_index & (TEST_ST_BIT_MASK)) * (TEST_RESULT_STATE_BIT_LENGTH);
	uint8_t mask = ((uint8_t)~((TEST_ST_BIT_NUM)<< tmp));
	DEMM_CPU_SR_ALLOC();
	DEMM_ENTER_CRITICAL();
	tmprst = (demm_dtc.result[byte_number] >> tmp);
	DEMM_EXIT_CRITICAL();
	tmprst &= (TEST_ST_BIT_NUM);
	if(((demm_dtc.res_last[byte_number] >> tmp)&(TEST_ST_BIT_NUM)) != tmprst ) {
		if((tmprst == DTC_TEST_STATE_FAILED) || (tmprst == DTC_TEST_STATE_PASSED)) {
			flag_changed = TRUE;
		}
		demm_dtc.res_last[byte_number] &= mask;
		demm_dtc.res_last[byte_number] |= (tmprst << tmp);
	}

	*result = tmprst;
	return flag_changed;
}

//static uint8_t dem_get_and_clr_test_result(dtc_handle_t dtc_index)
//{
//	uint8_t tmprst;
//
//	uint8_t byte_number = (dtc_index >> TEST_ST_BYT_RR);
//	uint8_t tmp = (dtc_index & TEST_ST_BIT_MASK) * (TEST_RESULT_STATE_BIT_LENGTH);
//	uint8_t u8_result = ((uint8_t)~((TEST_ST_BIT_NUM) << tmp));
//	DEMM_CPU_SR_ALLOC();
//	DEMM_ENTER_CRITICAL();
//	tmprst = (demm_dtc.result[byte_number] >> tmp);
//	demm_dtc.result[byte_number] &= u8_result;
//	DEMM_EXIT_CRITICAL();
//	tmprst &= (TEST_ST_BIT_NUM);
//
//	return tmprst;
//}

static void dem_stored_dtc_index_init(void)
{
	dtc_handle_t i;
	demm_dtc.storage_total_dtc_numbers = 0;
	for(i=0;i<TOTAL_DTC_FLAG_NUM; i++)
	{
		demm_dtc.not_stored_since_last_test_passed[i] = 0xffu;
		demm_dtc.need_to_be_written_to_eeprom[i] = 0;
	}
	for(i=0;i<TOTAL_DTC_NUM; i++)
	{
		if( dtc_cfg_tbl[i].story_mode != DTC_NOT_STORAGE) /* get storage dtc total numbers */
		{
			dtc_info_struct.stored_dtc_index[demm_dtc.storage_total_dtc_numbers] = i;
			demm_dtc.storage_total_dtc_numbers++;
		}
	}
	//dem_mem_clear(demm_dtc.occurrence_counter , DEM_TOTAL_STORED_DTC_NUM);     //这里初始化也没用，后面会读出存储
}

static void dem_stored_dtc_status_init(void)
{
	dtc_handle_t dtc_index,index=0;
	uint8_t stred_dtc_ind;
	for(dtc_index = 0; dtc_index < TOTAL_DTC_NUM; dtc_index++)
	{
	#if (DEM_DTC_STORAGE_TO_EEPROM_EN != STD_OFF)
		if( dtc_cfg_tbl[dtc_index].story_mode != DTC_NOT_STORAGE) /* get storage dtc total numbers */
		{
			/* This DTC is stored in EEPROM */
			/* bit4:pending_dtc = 1;  bit1,0: test_failed = test_failed_this_monitoring_cycle = 0 */
			stred_dtc_ind = dem_get_stored_index(dtc_index);
			if(stred_dtc_ind == DTC_NOT_STORED_FLAG)
			{
				dtc_record[dtc_index].c_status.status_byt = DTC_MASK_INIT_VALUE; /* ACC ISO14229-1,  bit6,bit4 = 1, others = 0, after clear diagnotics service */
			}
			else
			{
				dtc_record[dtc_index].c_status.status_byt = dtc_info_struct.dtc_stored_dt[stred_dtc_ind].dtc_status.status_byt; ///(dtc_info_struct.dtc_stored_dt[stred_dtc_ind].dtc_status.status_byt & 0xFC) | 0x40;
			}
			index++;
		}
		else
		{
			dtc_record[dtc_index].c_status.status_byt = DTC_MASK_INIT_VALUE; /* ACC ISO14229-1,  bit6,bit4 = 1, others = 0, after clear diagnotics service */
		}
	#else
		dtc_record[dtc_index].c_status.status_byt = DTC_MASK_INIT_VALUE; /* ACC ISO14229-1,  bit6,bit4 = 1, others = 0, after clear diagnotics service */
	#endif
	}
}

//uint8_t g_test_input = 0;
static void dem_init(uint8_t c_eeprom_operate_state)
{

	#if (DEM_DTC_STORAGE_TO_EEPROM_EN != 0)
	uint8_t index;
	#endif
	dem_stored_dtc_index_init();  //初始化 儲存相關數組，且把要儲存的id，重新排列到儲存數組
	if(DEM_TOTAL_STORED_DTC_NUM > TOTAL_MAX_STORED_DTC_NUM)
	{
		//#error TOTAL_MAX_STORED_DTC_NUM macro define size is too small.
		while(1);
	}
	if(c_eeprom_operate_state == EEPROM_INITED)
	{
		//g_test_input++;
	#if (DEM_DTC_STORAGE_TO_EEPROM_EN != 0)
		dem_stored_dtc_status_read();
	#endif
		dem_stored_dtc_status_init();
	}
	else if( (c_eeprom_operate_state == EEPROM_UNINITED) ||  (c_eeprom_operate_state == EEPROM_DTC_CLEAR))
	{
		//g_test_input++;
		//dem_mem_clear(&dtc_record[0].c_status.status_byt, TOTAL_DTC_NUM);
		dem_mem_set(&dtc_record[0].c_status.status_byt, DTC_MASK_INIT_VALUE, TOTAL_DTC_NUM);
	#if (DEM_DTC_STORAGE_TO_EEPROM_EN != 0)
		for(index = 0; index < DEM_TOTAL_STORED_DTC_NUM; index++)
		{
		#if((DTC_SNAP_SHOT_DATA_ENABLE==STD_OFF) && (DTC_EXTENDED_DATA_ENABLE==STD_OFF))   /* not used for BAIC */
			dtc_info_struct.dtc_stored_dt[index].dtc_status.status_byt = DTC_MASK_INIT_VALUE;
		#else
			dem_stored_dtc_clear(index);
		#endif
			/* update EEPROM
			(void)DEM_NVM_WRITE(DEM_DTC_STORAGE_EEPROM_START_ADDR + ((DTC_STORED_UNIT_BYTE_LENGTHS)*index),
								&dtc_info_struct.dtc_stored_dt[index].dtc_status.status_byt,
								DTC_STORED_UNIT_BYTE_LENGTHS);
			*/
		}
		(void)DEM_NVM_WRITE(DEM_DTC_STORAGE_EEPROM_START_ADDR,
							&dtc_info_struct.dtc_stored_dt[0].dtc_status.status_byt,
							(DTC_STORED_UNIT_BYTE_LENGTHS)*(DEM_TOTAL_STORED_DTC_NUM));
		//cpu_nop();
	#endif
	}
	else
	{
		//g_test_input++;
	}

}

/*一个dtc分两位两位放入，检查该DTC是否是表格中的DTC*/
static dtc_handle_t dem_get_dtc_handle(const uint8_t *dtc)
{
	dtc_handle_t i;
	dtc_handle_t tmp=DTC_NO_EXIST;
	uint32_t dtcn = Make32Bit(dtc[0],dtc[1],dtc[2]);
	for(i=0;i<NUM_OF_DTC;i++)
	{
		/*if(Dtc==SupportedDTC[i])*/
		if( SupportedDTC[i]==dtcn)
		{
			tmp = i;
			break;
		}
		else
		{

		}
	}
	return tmp;
}

/**
 * @brief  清除 dtc_record
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
static void _dem_clear_dtc(dtc_handle_t dtc_index)
{
#if (DEM_DTC_STORAGE_TO_EEPROM_EN != 0)
	uint8_t stred_dtc_ind = dem_get_stored_index(dtc_index);
	if(dtc_cfg_tbl[dtc_index].story_mode != DTC_NOT_STORAGE)
	{

	#if((DTC_SNAP_SHOT_DATA_ENABLE == STD_OFF) && (DTC_EXTENDED_DATA_ENABLE == STD_OFF))    /* not used for BAIC */
		//dem_stored_dtc_clear(stred_dtc_ind);
		dtc_record[dtc_index].c_status.status_byt = DTC_MASK_INIT_VALUE; ///(dtc_info_struct.dtc_stored_dt[stred_dtc_ind].dtc_status.status_byt & 0xFC) | 0x40;
	#else
		if(stred_dtc_ind<DEM_TOTAL_STORED_DTC_NUM)
		{
			dem_stored_dtc_clear(stred_dtc_ind);
		}
	#endif
		dem_dtc_record_storay(dtc_index);/* story dtc info to eeprom */
	}
	else
	{
		dtc_record[dtc_index].c_status.status_byt = DTC_MASK_INIT_VALUE;
	}
#else
	dtc_record[dtc_index].c_status.status_byt = DTC_MASK_INIT_VALUE;
#endif
}

//err_flag_e  dem_clear_dtc( uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin )
/**
 * 此函数根据提供的DTC指向地址，清除相应的故障代码它支持清除单个故障代码、组故障代码或所有故障代码具体行为取决于输入的DTC值
 * @brief  根據dtc的id；清除； 根據三字節的id
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
err_flag_e  dem_clear_dtc(const uint8_t *dtc)
{
	dtc_handle_t i;
	uint32_t Dtc= Make32Bit(dtc[0],dtc[1],dtc[2]);
	if(Dtc == DTC_ALL_GROUPS)			  /* 清除All Groups (all DTC's) */
	{
		dem_clear_all_test_result();
		dem_init(EEPROM_DTC_CLEAR);  //dem_init(EEPROM_DTC_CLEAR);
	}
	else if( (Dtc == DTC_NETWORK_COMM_GROUPS) || (Dtc == DTC_BODY_GROUPS) ||
			 (Dtc == DTC_CHASSIS_GROUPS) || (Dtc == DTC_POWERTRAIN_GROUPS))
	{
		for(i = 0; i < NUM_OF_DTC; i++)
		{
			if(((SupportedDTC[i]>>16) & 0xc0u) == (dtc[0] & 0xc0u)) /*查表, 如果 类型相同 , 逐一清除*/
			{
				dem_clear_test_result(i);
				_dem_clear_dtc(i);  //清除 dtc_record
			}
		}
	}
	else
	{
		i = dem_get_dtc_handle(dtc);
		if(i < NUM_OF_DTC)  /* 清除单个DTC_RECORD  */
		{
			dem_clear_test_result(i);
			_dem_clear_dtc(i);   //清除 dtc_record
		}
		else
		{
			return ERR_FAULT;
		}
	}
	return ERR_OK;
}

/*
	app_desc_clear_all_dtc_information:  調用: dtc_init(EEPROM_DTC_CLEAR);
	main_init 中調用:
*/
void dem_task_init(void)
{
	dem_clear_all_test_result();
	dem_init(EEPROM_INITED);
	demm_dtc.storaged_unit_size = DTC_STORED_UNIT_BYTE_LENGTHS; /* debug only */
	dem_mem_clear(demm_dtc.samp_count, TOTAL_DTC_NUM);
	demm_dtc.b_last_control_dtc_setting	= TRUE;
	demm_dtc.b_last_set_enabled	= TRUE;
	demm_dtc.b_control_dtc_setting = TRUE; ///FALSE;
	demm_dtc.b_set_enabled = TRUE; ///FALSE;
}


static void dtc_process(dtc_handle_t dtc_index, uint8_t cTestResult)
{
	uint8_t byte_number = 0, stred_dtc_ind;
	uint8_t bit_mask = 0;
	if(dtc_index < NUM_OF_DTC)
	{
		byte_number = dtc_index/8u;
		bit_mask = (1u << (dtc_index & 0x7u));
		//需要儲存到eeprom中
		if(get_8bits_mask(demm_dtc.need_to_be_written_to_eeprom[byte_number], bit_mask) != 0)
		{
			if(cTestResult == DTC_TEST_FAILED)
			{
			#if((DTC_STATUS_USED_MASK & 0x04U) != 0) //好像沒有起作用
				dtc_record[dtc_index].c_status.status_bits.pending_dtc = 1;
			#endif

				dtc_record[dtc_index].c_status.status_bits.test_failed = 1;
				dtc_record[dtc_index].c_status.status_bits.test_failed_this_operation_cycle  = 1;

				dtc_record[dtc_index].c_status.status_bits.test_failed_since_last_clr = 1;
				dtc_record[dtc_index].c_status.status_bits.confirmed_dtc = 1;                       //是否要加判定次数

				if(dtc_record[dtc_index].c_status.status_bits.test_not_completed_since_last_clr)
				{
					dtc_record[dtc_index].c_status.status_bits.test_not_completed_since_last_clr = 0;
				}

				if(dtc_record[dtc_index].c_status.status_bits.test_not_completed_this_operation_cycle)
				{
					dtc_record[dtc_index].c_status.status_bits.test_not_completed_this_operation_cycle = 0;
				}
				
				if(((demm_dtc.not_stored_since_last_test_passed[byte_number] & bit_mask)))
				{
					/* The first time happen DTC, Save Sanp shot data and extended data */
					demm_dtc.not_stored_since_last_test_passed[byte_number] &= (~bit_mask);
					///demm_dtc.need_to_be_written_to_eeprom[byte_number] |= bit_mask;
				#if (DEM_DTC_STORAGE_TO_EEPROM_EN != STD_OFF)
					if( dtc_cfg_tbl[dtc_index].story_mode != DTC_NOT_STORAGE) /* get storage dtc total numbers */
					{
						stred_dtc_ind = dem_get_stored_index(dtc_index);//  uint8_t index = dem_get_stored_index(dtc_index);
						dem_dtc_snap_short_record(stred_dtc_ind);
						dem_dtc_extended_data_record(stred_dtc_ind);
						dem_dtc_record_storay(dtc_index);
						clear_8bits_mask(demm_dtc.need_to_be_written_to_eeprom[byte_number], bit_mask);
					}
				#endif
				}
			}
			else
			{
				demm_dtc.not_stored_since_last_test_passed[byte_number] |= bit_mask;
				/* DTC fault detection counter has reached its min value , '-128'. */
				dtc_record[dtc_index].c_status.status_bits.test_failed = 0;
				if(dtc_record[dtc_index].c_status.status_bits.test_failed_since_last_clr)
				{
					/* story dtc info */
					///demm_dtc.need_to_be_written_to_eeprom[byte_number] |= bit_mask;
				}
				if(dtc_record[dtc_index].c_status.status_bits.test_not_completed_since_last_clr)
				{
					dtc_record[dtc_index].c_status.status_bits.test_not_completed_since_last_clr = 0;
				}
				if(dtc_record[dtc_index].c_status.status_bits.test_not_completed_this_operation_cycle)
				{
					dtc_record[dtc_index].c_status.status_bits.test_not_completed_this_operation_cycle = 0;
				}
				if((dtc_record[dtc_index].c_status.status_bits.test_not_completed_this_operation_cycle == 0) &&
				   (dtc_record[dtc_index].c_status.status_bits.test_failed_this_operation_cycle  == 0))
				{
					dtc_record[dtc_index].c_status.status_bits.pending_dtc = 0;
				}
				if((dtc_record[dtc_index].c_status.status_bits.pending_dtc==0)&&(dtc_record[dtc_index].c_status.status_bits.confirmed_dtc ==1))
				{
                #if (DEM_DTC_STORAGE_TO_EEPROM_EN != 0)
                   #if ((DTC_EXTENDED_DATA_ENABLE != STD_OFF))
					if (dtc_cfg_tbl[dtc_index].story_mode != DTC_NOT_STORAGE) /* get storage dtc total numbers */
					{
						stred_dtc_ind = dem_get_stored_index(dtc_index); //	uint8_t index = dem_get_stored_index(dtc_index);
						demm_dtc.aging_counter[stred_dtc_ind]++;
						if (demm_dtc.aging_counter[stred_dtc_ind] == 40&&stred_dtc_ind!=2)
						{
							demm_dtc.aging_counter[stred_dtc_ind] = 0;
							demm_dtc.aged_counter[stred_dtc_ind]++;		
							dtc_record[dtc_index].c_status.status_bits.confirmed_dtc =0;
							dtc_record[dtc_index].c_status.status_bits.test_failed_since_last_clr=0;
						}
						else if(demm_dtc.aging_counter[stred_dtc_ind] == 80&&stred_dtc_ind==2)    //2是80次循环
						{
							demm_dtc.aging_counter[stred_dtc_ind] = 0;
							demm_dtc.aged_counter[stred_dtc_ind]++;		
							dtc_record[dtc_index].c_status.status_bits.confirmed_dtc =0;
							dtc_record[dtc_index].c_status.status_bits.test_failed_since_last_clr=0;
						}
						
						dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.aging_counter = demm_dtc.aging_counter[stred_dtc_ind];
						dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.aged_counter = demm_dtc.aged_counter[stred_dtc_ind];
					}
                   #endif
                #endif
				  
				}
				/* bit4:pending_dtc = 1;  bit1,0: test_failed = test_failed_this_monitoring_cycle = 0 */
			#if (DEM_DTC_STORAGE_TO_EEPROM_EN != 0)
				if( dtc_cfg_tbl[dtc_index].story_mode != DTC_NOT_STORAGE) /* get storage dtc total numbers */
				{
					//dem_dtc_extended_data_record(stred_dtc_ind);
					dem_dtc_record_storay(dtc_index);                    
					clear_8bits_mask(demm_dtc.need_to_be_written_to_eeprom[byte_number], bit_mask);
				}
			#endif
			}
		}
	}
}

/**
 * @brief  用于通过本地条件是否使能DTC 检测;
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
void dem_enable_dtc_record_update(void)
{
	uint8_t i;
	//當上次是使能時，要清除 除了高低壓的dtc
	if(demm_dtc.b_last_set_enabled	== FALSE)
	{
		for(i = 0; i < NUM_OF_DTC; i++)
		{

			dem_clear_test_result(i);

		}
	}
	demm_dtc.b_set_enabled  = TRUE;
	demm_dtc.b_last_set_enabled	= TRUE;

}

void dem_disable_dtc_record_update(void)
{
	demm_dtc.b_set_enabled = FALSE;
	demm_dtc.b_last_set_enabled	= FALSE;

}

/**
 * @brief  用于通过来自网络的UDS 命令, 控制DTC 是否使能DTC检测
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
void dem_enable_dtc_setting(void)
{
	//當設置由禁用變成使能時，清所有DTC
	if(demm_dtc.b_last_control_dtc_setting	== FALSE)
	{
		dem_clear_all_test_result();
	}
	demm_dtc.b_last_control_dtc_setting	= TRUE;
	demm_dtc.b_control_dtc_setting  = TRUE;
}

void dem_disable_dtc_setting(void)
{
	demm_dtc.b_control_dtc_setting = FALSE;
	demm_dtc.b_last_control_dtc_setting	= FALSE;
}

#if 0
/*
返回DTC_RECORD.STATE
*/
uint8_t dtc_get_state(uint8_t dtc_index, uint8_t *state)
{
	if(dtc_index >= TOTAL_DTC_NUM)
		return FALSE;
	*state = dtc_record[dtc_index].c_status.status_byt;
	return TRUE;
}
#else

/**
 * @brief  返回DTC_RECORD.STATE
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
uint8_t _dem_get_state(dtc_handle_t dtc_index)
{
	return(dtc_record[dtc_index].c_status.status_byt);
}

#endif

#if 0
uint8_t test_buff[40] = {0};
void dem_test_fun(void)
{
	uint16_t j;
	uint8_t i;
	uint8_t t_test_dem_set = 0;
	dem_task_init();
	while(1)
	{
		dem_set_test_result(7, DTC_TEST_STATE_FAILED);
		for (j = 0; j<10; j++)
		{
			dem_task();
		}
		t_test_dem_set = demm_copy_snap_short_data_to_dcm_txbuff(7, test_buff);
		if(t_test_dem_set)
		{
			cpu_nop();
		}
		cpu_nop();
	}
	for (i = 0;i<TOTAL_DTC_NUM; i++)
	{
		dem_set_test_result(i, DTC_TEST_STATE_FAILED);
		t_test_dem_set = dem_get_and_clr_test_result(i);
		dem_set_test_result(i, DTC_TEST_STATE_PASSED);
		t_test_dem_set = dem_get_and_clr_test_result(i);
	}
}
#endif

/**
 * @brief  這個函數功能？？
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
static void dem_check_sub(dtc_handle_t dtc_index)
{
#if 1
	uint8_t stred_dtc_ind;
	uint8_t tmp = 0; /// = dem_get_test_result(dtc_index);
	bool changed = dem_get_test_result_updated(dtc_index, &tmp);	//好像是更新結果，且res_last也更新
#else
	uint8_t tmp = dem_get_and_clr_test_result(dtc_index);
#endif
	//好像是 有變化
	if(changed != FALSE)
	{  //
		if(tmp == DTC_TEST_STATE_FAILED) 
		{
		#if (DEM_DTC_STORAGE_TO_EEPROM_EN != 0)
			if( dtc_cfg_tbl[dtc_index].story_mode != DTC_NOT_STORAGE) /* get storage dtc total numbers */
			{
				stred_dtc_ind = dem_get_stored_index(dtc_index);//	uint8_t index = dem_get_stored_index(dtc_index);
				demm_dtc.occurrence_counter[stred_dtc_ind]++;
				demm_dtc.samp_count[dtc_index] = 0;

				set_8bits_mask(demm_dtc.need_to_be_written_to_eeprom[dtc_index/8u], (1u << (dtc_index & 0x7u)));
			}
		#endif
		}
		else if(tmp == DTC_TEST_STATE_PASSED)
		{
	#if (DEM_DTC_STORAGE_TO_EEPROM_EN != 0)
		//#if((DTC_EXTENDED_DATA_ENABLE != STD_OFF))
			if( dtc_cfg_tbl[dtc_index].story_mode != DTC_NOT_STORAGE) /* get storage dtc total numbers */
			{
				if((dtc_record[dtc_index].c_status.status_bits.test_failed) || (dtc_record[dtc_index].c_status.status_bits.test_not_completed_this_operation_cycle))
				{
					stred_dtc_ind = dem_get_stored_index(dtc_index);//	uint8_t index = dem_get_stored_index(dtc_index);
					///demm_dtc.self_healing_counter[stred_dtc_ind]++;
					demm_dtc.samp_count[dtc_index] = dtc_cfg_tbl[dtc_index].confirm_debounce_time;
					set_8bits_mask(demm_dtc.need_to_be_written_to_eeprom[dtc_index/8u], (1u << (dtc_index & 0x7u)));
				}
			}
		//#endif
	#endif
		}
	}
	//時時檢測
	if(tmp == DTC_TEST_STATE_FAILED)
	{
		if(demm_dtc.samp_count[dtc_index] < dtc_cfg_tbl[dtc_index].confirm_debounce_time)
		{
			demm_dtc.samp_count[dtc_index]++;
		}
		if(demm_dtc.samp_count[dtc_index] >= dtc_cfg_tbl[dtc_index].confirm_debounce_time)
		{
			dtc_process(dtc_index, DTC_TEST_FAILED);	 /* process DTC result */
			demm_dtc.samp_count[dtc_index] = 0;
		}
	}
	else if(tmp == DTC_TEST_STATE_PASSED)
	{
		if(dtc_cfg_tbl[dtc_index].debounce_mode == DEM_DTC_DEBOUNCE_RST)
		{
			if(demm_dtc.samp_count[dtc_index] != 0)
			{
				dtc_process(dtc_index, DTC_TEST_PASSED);
				demm_dtc.samp_count[dtc_index] = dtc_cfg_tbl[dtc_index].confirm_debounce_time;
				return;
			}
		}
		//如果是DEM_DTC_DEBOUNCE_ACC  才考虙消抖
		if(demm_dtc.samp_count[dtc_index] == 0)
		{
			dtc_process(dtc_index, DTC_TEST_PASSED);
			demm_dtc.samp_count[dtc_index] = dtc_cfg_tbl[dtc_index].confirm_debounce_time;
		}
		else
		{
			demm_dtc.samp_count[dtc_index]--;
			if(demm_dtc.samp_count[dtc_index] == 0)
			{
				dtc_process(dtc_index, DTC_TEST_PASSED);
				demm_dtc.samp_count[dtc_index] = dtc_cfg_tbl[dtc_index].confirm_debounce_time;
			}
		}
	}
	else /* Test Not Completed */
	{

	}
}

/**
* @brief dtc_task 任务, 50ms周期任务---目前放在了10ms的任務裡
* @retval none
*/
void dem_task(void)
{

	dtc_handle_t dtc_index = 0;
	if(demm_dtc.b_set_enabled && (demm_dtc.b_control_dtc_setting == DESC_CONTROL_DTC_SETTING_ON))
	{
		for(dtc_index = 0u; dtc_index < TOTAL_DTC_NUM; dtc_index++)
		{
			dem_check_sub(dtc_index);
		}
	}


}


/**
 * @brief  19 04  函数调用, dtc index 范围由调用函数处理,  此函数内部不检查有效范围
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
bool demm_copy_snap_short_data_to_dcm_txbuff(dtc_handle_t dtc_index, uint8_t *dcm_txbuff, uint8_t dtc_number)
{
	uint8_t stred_dtc_ind, i,j;
	uint8_t *pdest, *psr;
	stred_dtc_ind = dem_get_stored_index(dtc_index);
	if(stred_dtc_ind != DTC_NOT_STORED_FLAG)
	{
		pdest = dcm_txbuff;
		//pdata = &pMsgContext->resData[8];
		psr = &dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCSnapShotData[dtc_number].c_main_ecu_supply_voltage; //DTCSnapShotData
		for(i=0;i<TOTAL_DTC_SNAP_SHOT_ID_NUM;i++)
		{
			*pdest = c_dtc_snap_shot_data_id[i][0];  /*Dataidentifier#1  byte#1(MSB)*/
			pdest++;
			*pdest = c_dtc_snap_shot_data_id[i][1];  /*Dataidentifier#1  byte#1(LSB)*/
			pdest++;
			for(j=0; j < c_dtc_snap_shot_data_size[i]; j++)   /*load record data by Dataidentifier#1   */
			{
				*pdest = *psr;
				psr++;
				pdest++;
			}
		}
		return TRUE;
	}
	else
	{

	}
	return FALSE;
}
#if((DTC_EXTENDED_DATA_ENABLE != STD_OFF))
/* 19 06  函数调用, dtc index 范围由调用函数处理,  此函数内部不检查有效范围*/
bool demm_copy_extdata_to_dcm_txbuff(dtc_handle_t dtc_index, uint8_t *dcm_txbuff)   //没有调用。直接19 06里面赋值
{
	uint8_t stred_dtc_ind, i,j;
	uint8_t *pdest, *psr;
	stred_dtc_ind = dem_get_stored_index(dtc_index);
	if(stred_dtc_ind != DTC_NOT_STORED_FLAG)
	{
		pdest = dcm_txbuff;
		//pdata = &pMsgContext->resData[8];
		psr = &dtc_info_struct.dtc_stored_dt[stred_dtc_ind].DTCextended_data.occurrence_counter; //DTCSnapShotData
		for(i=0;i<TOTAL_DTC_EXTENDED_DATA_NUM;i++)
		{
			for(j=0; j < 2; j++)   /*load record data by Dataidentifier#1   */
			{
				*pdest = *psr;
				psr++;
				pdest++;
			}
		}
		return TRUE;
	}
	else
	{

	}
	return FALSE;
}
#endif
/**
 * @brief  IGN變為ON 重新喚醒時，需要清除
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
void demm_operation_cycle_chaned_event_proc(void)
{
	uint8_t i,stred_dtc_ind;
	for(i = 0; i < NUM_OF_DTC; i++)
	{
		dtc_record[i].c_status.status_bits.test_failed = 0;      
		dtc_record[i].c_status.status_bits.test_failed_this_operation_cycle = 0;
		dtc_record[i].c_status.status_bits.test_not_completed_this_operation_cycle = 1;
		if( dtc_cfg_tbl[i].story_mode != DTC_NOT_STORAGE) /* get storage dtc total numbers */
		{
			stred_dtc_ind = dem_get_stored_index(i);
		//	demm_dtc.self_healing_counter[stred_dtc_ind]++;
		}
	}
	dem_mem_clear(demm_dtc.res_last, TOTAL_TEST_RESULT_NUM);
	dem_mem_clear(demm_dtc.result, TOTAL_TEST_RESULT_NUM);
	dem_mem_clear(demm_dtc.samp_count, TOTAL_DTC_NUM);
}

