#ifndef __APP_BOOT_CFG_H
#define __APP_BOOT_CFG_H

#include <stdint.h>
#include "types.h"



#define FLAG_REPROGRAM_ID		0x01U
#define FLAG_RESET_ID			0x02U
#define FLAG_FINGERPRINT_ID		0x03U



#define FLAG_BASE_SIZE			0x04U
#define FLAG_BASE_ADDR			0x20007FC0


#define FLAG_REPROGRAM_ADDR		(FLAG_BASE_ADDR + 0)

#define FLAG_REPROGRAM_DATA		0x34E5A5A5U //0xA5A5E534U

#define ResetMCUHandle()  NVIC_SystemReset()
#define FlagWriteHandle(addr,size,buf)			Boot_MemCpy((uint8_t *)addr,(const uint8_t *)buf,size)

#endif /* endif of __APP_BOOT_CFG_H */
