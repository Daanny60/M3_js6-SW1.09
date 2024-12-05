#ifndef __PLL_H__
#define __PLL_H__


#include "mf_config.h"
#include "types.h"

#ifndef MFANG
    #include <stdio.h>
#endif
void CMU_PLL_ConfigDomain_SYSTEM(uint32_t Source, uint32_t PLL_REFPSC, uint32_t PLL_DB, uint32_t PLL_OSEL);

void SelRCHFToPLL(uint32_t rchf, uint32_t clock);
void SelXTHFToPLL(uint32_t div, uint32_t clock);

void RCHFInit(uint32_t clock);
void XTHFInit(void);
#endif
