#ifndef __EEPROM_CFG_H
#define __EEPROM_CFG_H


/* Includes ------------------------------------------------------------------*/
#include "mf_config.h"
#include "fm33lg0xx_fl.h"
#include "Dem_Cfg.h"
#include "eeprom.h"

#define EEPROM_FALSE			0			//返回错误
#define EEPROM_TRUE				1			//返回正确
#define EEPROM_ERR_DATA			2			//数据错误
#define EEPROM_ERR_CRC8			3			//校验错误
#define EEPROM_ERR_ADDR			4			//地址错误

typedef struct
{
    uint8_t* F183_boot_id;
    uint8_t* F187_facturepartnumber;
    uint8_t* F18C_ECUSerialNumber;

}boot_read_data_add_s;

typedef struct
{
    uint8_t* F195_software_version;
    uint8_t* F193_hardware_version;
    uint8_t* F187_facturepartnumber;

}app_read_data_add_s;

#define EOL_GETSTART_ADDRESS    ((uint32_t)0x0001BC00) 
#define EOL_F18B_ADDRESS  (EOL_GETSTART_ADDRESS + 16)
#define EOL_F18C_ADDRESS  EOL_GETSTART_ADDRESS
//#define EOL_F199_ADDRESS  EOL_GETSTART_ADDRESS+20
#if(0)
#define SIZE_OF_VIN_EE_FLASH_RAM    20u

#define VIN_BYTE0_EE_FLASH_RAM_ADDRESS  0u
#define VIN_BYTE1_EE_FLASH_RAM_ADDRESS  1u
#define VIN_BYTE2_EE_FLASH_RAM_ADDRESS  2u
#define VIN_BYTE3_EE_FLASH_RAM_ADDRESS  3u
#define VIN_BYTE4_EE_FLASH_RAM_ADDRESS  4u
#define VIN_BYTE5_EE_FLASH_RAM_ADDRESS  5u
#define VIN_BYTE6_EE_FLASH_RAM_ADDRESS  6u
#define VIN_BYTE7_EE_FLASH_RAM_ADDRESS  7u
#define VIN_BYTE8_EE_FLASH_RAM_ADDRESS  8u
#define VIN_BYTE9_EE_FLASH_RAM_ADDRESS  9u
#define VIN_BYTE10_EE_FLASH_RAM_ADDRESS 10u
#define VIN_BYTE11_EE_FLASH_RAM_ADDRESS 11u
#define VIN_BYTE12_EE_FLASH_RAM_ADDRESS 12u
#define VIN_BYTE13_EE_FLASH_RAM_ADDRESS 13u
#define VIN_BYTE14_EE_FLASH_RAM_ADDRESS 14u
#define VIN_BYTE15_EE_FLASH_RAM_ADDRESS 15u
#define VIN_BYTE16_EE_FLASH_RAM_ADDRESS 16u
#define VIN_RESERVED1_EE_FLASH_RAM_ADDRESS 17u
#define VIN_RESERVED2_EE_FLASH_RAM_ADDRESS 18u
#define VIN_RESERVED3_EE_FLASH_RAM_ADDRESS 19u

#define SIZE_OF_APPLICATION_SOFTWARE_FINGERPRINT_EE_FLASH_RAM   12u

#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE0_EE_FLASH_RAM_ADDRESS  20u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE1_EE_FLASH_RAM_ADDRESS  21u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE2_EE_FLASH_RAM_ADDRESS  22u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE3_EE_FLASH_RAM_ADDRESS  23u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE4_EE_FLASH_RAM_ADDRESS  24u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE5_EE_FLASH_RAM_ADDRESS  25u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE6_EE_FLASH_RAM_ADDRESS  26u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE7_EE_FLASH_RAM_ADDRESS  27u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE8_EE_FLASH_RAM_ADDRESS  28u
#define APPLICATION_SOFTWARE_FINGERPRINT_RESERVED1_EE_FLASH_RAM_ADDRESS  29u
#define APPLICATION_SOFTWARE_FINGERPRINT_RESERVED2_EE_FLASH_RAM_ADDRESS  30u
#define APPLICATION_SOFTWARE_FINGERPRINT_RESERVED3_EE_FLASH_RAM_ADDRESS  31u


#define SIZE_OF_DTC_EE_FLASH_EE_FLASH_RAM   sizeof(dtc_stored_data_t) * TOTAL_DTC_NUM
/* dtc 长度 1byte dtc status （2byte 快照（电源模式，电压） = 1byte 快照存1个  1Byte 快照写指针*/
#define DTC_EE_FLASH_RAM_START_ADDRESS  32u

#define DTC_LOSS_OF_COMMUNICATION_BETWEEN_MFS_AND_BCM_EE_FLASH_RAM_START_ADDRESS  32u
#define DTC_LOSS_OF_COMMUNICATION_BETWEEN_MFS_AND_ICC_EE_FLASH_RAM_START_ADDRESS  37u
#define DTC_LOSS_OF_COMMUNICATION_BETWEEN_MFS_AND_PEPS_EE_FLASH_RAM_START_ADDRESS  42u
#define DTC_LOSS_OF_COMMUNICATION_BETWEEN_MFS_AND_CIM_EE_FLASH_RAM_START_ADDRESS  47u
#define DTC_MFS_NM_LIMPHOME_FAULT_EE_FLASH_RAM_START_ADDRESS  52u
#define DTC_CAN_BUS_COMMUNICATION_FAULT_EE_FLASH_RAM_START_ADDRESS  57u
#define DTC_CUSTOM_SWITCH_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS  62u
#define DTC_RETURN_SWITCH_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS  67u
#define DTC_LEFT_SWITCH_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS    72u
#define DTC_RIGHT_SWITCH_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS   77u
#define DTC_LE_ROLL_PRESS_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS  82u
#define DTC_SRC_SWITCH_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS     87u
#define DTC_VOICE_SWITCH_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS   92u
#define DTC_PREV_SONG_SWITCH_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS   97u
#define DTC_NEXT_SONG_SWITCH_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS   102u
#define DTC_RI_ROLL_PRESS_STUCK_FAILURE_EE_FLASH_RAM_START_ADDRESS      107u
#define DTC_LOSS_OF_COMMUNICATION_BETWEEN_MFS_AND_ESP_EE_FLASH_RAM_START_ADDRESS      112u

#define VIN_BYTE0_3_EE_FLASH_ADDRESS   0u
#define VIN_BYTE4_7_EE_FLASH_ADDRESS   1u
#define VIN_BYTE8_11_EE_FLASH_ADDRESS  2u
#define VIN_BYTE12_15_EE_FLASH_ADDRESS 3u
#define VIN_BYTE16_19_EE_FLASH_ADDRESS 4u


#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE0_3_EE_FLASH_ADDRESS  5u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE4_7_EE_FLASH_ADDRESS  6u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE7_11_EE_FLASH_ADDRESS 7u


#define FAT_RAM_LENGTH (SIZE_OF_VIN_EE_FLASH_RAM + SIZE_OF_APPLICATION_SOFTWARE_FINGERPRINT_EE_FLASH_RAM + SIZE_OF_DTC_EE_FLASH_EE_FLASH_RAM)
#else
#define VIN_BYTE0_EE_FLASH_RAM_ADDRESS  0u    //F190
#define VIN_BYTE17_EE_FLASH_RAM_ADDRESS 17u   //F199
#define VIN_BYTE21_EE_FLASH_RAM_ADDRESS 21u   //F19D
#define VIN_BYTE25_EE_FLASH_RAM_ADDRESS 25u   //F1A8
#define VIN_BYTE45_EE_FLASH_RAM_ADDRESS 45u   //FLAG,F199 is writed by command or EOL
//#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE0_EE_FLASH_RAM_ADDRESS  46u   //FINGERPRINT
#define DTC_APP_BUSOFF_CNTR_EE_FLASH_RAM_ADDRESS  46u
#define DTC_EE_FLASH_RAM_START_ADDRESS  58u
#define SIZE_OF_VIN_EE_FLASH_RAM    46u
#define DTC_APP_EE_FLASH_RAM   12u
#define SIZE_OF_DTC_EE_FLASH_EE_FLASH_RAM   sizeof(dtc_stored_data_t) * TOTAL_DTC_NUM
#define FAT_RAM_LENGTH (SIZE_OF_VIN_EE_FLASH_RAM + DTC_APP_EE_FLASH_RAM + SIZE_OF_DTC_EE_FLASH_EE_FLASH_RAM)
#define EEPROM_ADDRESS EEPROM_START_ADDRESS
#endif

extern volatile uint16_t eeprom_write_status;
void EE_Flash_Task(void);
void EE_Flash_Init(void);
uint8_t Eeprom_RamRead(uint8_t *pBuf, uint16_t addr, uint16_t len);
uint8_t Eeprom_RamWrite(uint8_t *pBuf, uint16_t addr, uint16_t len);
#endif