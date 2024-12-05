/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "mf_config.h"
#include "fm33lg0xx_fl.h"
#include "eeprom_cfg.h"

#define PAGE_SIZE  (uint16_t)0x2000  /* Page size = 8KByte */
#define FLASH_SECTOR_SIZE (uint16_t)0x0800

/* EEPROM start address in Flash */
#define EEPROM_START_ADDRESS    ((uint32_t)0x0001C000) /* EEPROM emulation start address:
                                                    after 64KByte of used Flash memory */

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + 0x000))
#define PAGE0_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

/* Used Flash pages for EEPROM emulation */
#define PAGE0                   ((uint16_t)0x0000)
#define PAGE1                   ((uint16_t)0x0001)

/* No valid page define */
#define NO_VALID_PAGE           ((uint16_t)0x00AB)

/* Page status definitions */
#define ERASED                  ((uint16_t)0xFFFF)     /* PAGE is empty */
#define RECEIVE_DATA            ((uint16_t)0xEEEE)     /* PAGE is marked to receive data */
#define VALID_PAGE              ((uint16_t)0x0000)     /* PAGE containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE    ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE     ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL               ((uint8_t)0x80)

/* Variables' number */
#define NumbOfVar               ((FAT_RAM_LENGTH % 4) ? (FAT_RAM_LENGTH / 4 + 1) : (FAT_RAM_LENGTH / 4)) //((uint16_t)0x42)

typedef uint32_t flashOperation_t;  //flash operation occupy

#define FLASH_OPERATION_LEN             4   //flash operation lenth, 4 bytes
#define PAGE_START_INFO_LEN             8   //page start information, 4 bytes for page status, 4 bytes reserved
#define PAGE_FIRST_VARIABLE_ADDRESS     (PAGE_START_INFO_LEN + FLASH_OPERATION_LEN)
#define ELEMENT_LEN                     8   //page element lenth, 8 bytes{4 for data + 4 for virtual address}

/* Exported constants --------------------------------------------------------*/
/** 
  * @brief  FLASH Status  
  */

typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

#define FLASH_Lock()
#define FLASH_Unlock()

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t EE_Init(void);
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint32_t* Data);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint32_t Data);

#endif /* __EEPROM_H */

/*******************************END OF FILE************************************/
