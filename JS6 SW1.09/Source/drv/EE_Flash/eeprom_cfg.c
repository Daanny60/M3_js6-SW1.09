#include "eeprom_cfg.h"
#include "eeprom.h"



__root uint8_t u8EE_Flash_Ram[4096] __attribute__((section(".EE_Flash_Ram")));

    uint16_t i;
    volatile uint16_t eeprom_write_status=FLASH_COMPLETE;

void EE_Flash_Init(void)
{

    uint32_t data;
	data = NumbOfVar;
    for(i = 0; i < NumbOfVar; i ++)
    {
        if(EE_ReadVariable(i , &data) == 1)
        {
            u8EE_Flash_Ram[i * 4] = 0xffu;
            u8EE_Flash_Ram[i * 4 + 1] = 0xffu;
            u8EE_Flash_Ram[i * 4 + 2] = 0xffu;
            u8EE_Flash_Ram[i * 4 + 3] = 0xffu;
        }
        else
        {
            u8EE_Flash_Ram[i * 4] = (uint8_t)data;
            u8EE_Flash_Ram[i * 4 + 1] = (uint8_t)(data >> 8);
            u8EE_Flash_Ram[i * 4 + 2] = (uint8_t)(data >> 16);
            u8EE_Flash_Ram[i * 4 + 3] = (uint8_t)(data >> 24);
        }
    }
}


void EE_Flash_Task(void)
{
    static uint16_t ee_flash_addr = 0;
    uint32_t data;
    uint8_t buf[4];
    
    if(ee_flash_addr >= NumbOfVar)
    {
        ee_flash_addr = 0;
    }
    if(EE_ReadVariable(ee_flash_addr , &data) == 1)
    {
        data = 0xffffffffu;
    }
    buf[0] = (uint8_t)data;
    buf[1] = (uint8_t)(data >> 8);
    buf[2] = (uint8_t)(data >> 16);
    buf[3] = (uint8_t)(data >> 24);

    if((u8EE_Flash_Ram[ee_flash_addr * 4] != buf[0]) || (u8EE_Flash_Ram[ee_flash_addr * 4 + 1] != buf[1]) \
    || (u8EE_Flash_Ram[ee_flash_addr * 4 + 2] != buf[2]) || (u8EE_Flash_Ram[ee_flash_addr * 4 + 3] != buf[3]))
    {
        data = u8EE_Flash_Ram[ee_flash_addr * 4 + 3];
        data <<= 8;
        data |= u8EE_Flash_Ram[ee_flash_addr * 4 + 2];
        data <<= 8;
        data |= u8EE_Flash_Ram[ee_flash_addr * 4 + 1];
        data <<= 8;
        data |= u8EE_Flash_Ram[ee_flash_addr * 4];
        eeprom_write_status=EE_WriteVariable(ee_flash_addr, data);
    }
    ee_flash_addr ++;

}


/*****************************************************************************
|Prototype         : 
|Called by         : 
|Preconditions     : -
|Input parameters  : None
|Output parameters : None
|Return value      : None
|Description       : 
*****************************************************************************/
uint8_t Eeprom_RamRead(uint8_t *pBuf, uint16_t addr, uint16_t len)
{

    uint32_t primask;
    uint8_t *RamBuf;

	if((len)==0)
    {
        return EEPROM_ERR_ADDR;
    }								
	if(((uint32_t)addr+(uint32_t)len) > FAT_RAM_LENGTH)	
    {
        return EEPROM_ERR_ADDR;
    }

    primask = __get_PRIMASK();

    __disable_irq();


	RamBuf = &u8EE_Flash_Ram[addr];
	while(len--)
	{
		*pBuf = *RamBuf;
		pBuf ++;
		RamBuf++;
	}
	__set_PRIMASK(primask);
   return EEPROM_TRUE;
}



/*****************************************************************************
|Prototype         : 
|Called by         : 
|Preconditions     : -
|Input parameters  : None
|Output parameters : None
|Return value      : None
|Description       : 
*****************************************************************************/
uint8_t Eeprom_RamWrite(uint8_t *pBuf, uint16_t addr, uint16_t len)
{
	uint16_t tlen = len;
    uint32_t primask;

	uint8_t *RamBuf,cRet=0;



	if((tlen)==0)								
    {
        return EEPROM_ERR_ADDR;
    }
	if(((uint32_t)addr+(uint32_t)tlen)>FAT_RAM_LENGTH)	
    {
        return EEPROM_ERR_ADDR;
    }
    

    primask = __get_PRIMASK();
    __disable_irq();
	RamBuf = &u8EE_Flash_Ram[addr];
	while(tlen--)
	{
		*RamBuf = *pBuf;
		pBuf++;
		RamBuf++;
	}

	__set_PRIMASK(primask);
	return cRet;
}