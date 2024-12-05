/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE
*   This software is the property of HiRain Technologies. Any information
*   contained in this doc should not be reproduced, or used, or disclosed
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Can_Pl.h
********************************************************************************
*   Project/Product : AUTOSAR 4.0
*   Title           : CAN Driver platform head File
*   Author          : HeYang
********************************************************************************
*   Description     : The header file Can_Pl.h contains type definitions and
*                      macros which relate to specific MCU .
*
********************************************************************************
*   Limitations     : only used for MSCAN
*
********************************************************************************
*
********************************************************************************
*   Revision History:
*
*   Version     Date          Initials      CR#          Descriptions
*   ---------   ----------    ------------  ----------   ---------------
*   01.00.00    06/05/2012    HeYang        N/A          Original
*   01.01.00    12/05/2012    NingChen      N/A          first test feed back ;
*   02.00.00    29/07/2013    NingChen      N/A          DPN_MSCAN_130729_1
*   02.00.01    15/08/2013    NingChen      N/A          D10_MSCAN_130815_1
*   02.00.02    23/09/2013    NingChen      N/A          D10_MSCAN_130923_1   
*   02.00.03    23/01/2014    SuyunYing     N/A          D10_MSCAN_140123_1                                                    
********************************************************************************
* END_FILE_HDR*/
#ifndef _CAN_PL_H_
#define _CAN_PL_H_

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "ComStack_Types.h"
#include "Can_Cfg.h"

/*******************************************************************************
*   Macro
*******************************************************************************/
#define CAN_HARDWARE_TIMEOUT_CHECK      STD_OFF
#define CAN_ENTER_CRITICAL_SECTION()    Can_DisableGlobalInterrupts()
#define CAN_LEAVE_CRITICAL_SECTION()    Can_EnableGlobalInterrupts()
#define CAN_NOT_MATCH                   (0u)
#define CAN_MATCH                       (1u)
#define CAN_CONTROLLER_NOT_HOH          (0xffu)
#define TOTAL_CAN_MODULE_NUM            (0x1u)
#define MSK_EXT_ID_MSB                  ((uint32)(0x10000000))
#define HOH_NUM_PER_CONTROLLER          (4u)
#define HTH_NUM_PER_CONTROLLER          (1u)  ///////20231018
#define CAN_MULTIPLEXED_HTH_MBNUM       (3u)

/*******************************************************************************
*   ISR enroll related & vetor numbers
*******************************************************************************/
#define CAN_AUTOENROLL_ISR            STD_OFF

#if(CAN_AUTOENROLL_ISR == STD_OFF)
#define CAN_INTERRUPT_ISR
#else
#define CAN_INTERRUPT_ISR interrupt
#endif

#if(CAN_AUTOENROLL_ISR == STD_OFF)
#define CAN0WAKEUP_VECTOR_NUMBER 
#define CAN0ERR_VECTOR_NUMBER 
#define CAN0RX_VECTOR_NUMBER 
#define CAN0TX_VECTOR_NUMBER 
#else
/* notice: these vector numbers may be different in different MCU , here is the 
xep100 demo. When used in other MCU, pls check these vectors  */
#define CAN0WAKEUP_VECTOR_NUMBER      (36u)
#define CAN0ERR_VECTOR_NUMBER         (37u)
#define CAN0RX_VECTOR_NUMBER          (38u)
#define CAN0TX_VECTOR_NUMBER          (39u)
#endif

/*******************************************************************************
*   Typedef
*******************************************************************************/
typedef volatile struct
{
    #if (STD_ON == CAN_ENABLE_EXTERNED_ID)
    volatile uint32 Id;
    #else
    volatile uint16 Id;             /* Identifier Register 0,1                */
    volatile uint16 IdExt;          /* Identifier Register 2,3                */
    #endif
    volatile uint8  DataSeg[8];     /* Data 0 .. 7                            */
    volatile uint8  Dlc;            /* Data length reg.:  X X X X DLC3 DLC2
                                                                   DLC1 DLC0  */
    volatile uint8  Priority;       /* TxBuf priority reg.                    */
    volatile uint16 TimeStamp;
}Can_MsgBufType;

typedef volatile struct
{
  /* CAN Control Register */
  volatile uint32      CAN_CR;
  /* CAN mode select register */
  volatile uint32      CAN_MSR;
  /* CAN Baud rate Prescaler Register */
  volatile uint32      CAN_BRPR;
  /* CAN Bit Timing Register */
  volatile uint32      CAN_BTR;
  /* CAN Error Counter Register */  
  volatile uint32      CAN_ECR; 
  /* CAN Error Status Register */
  volatile uint32      CAN_ESR;
  /* CAN Status Register */ 
  volatile uint32      CAN_SR;
  /* CAN Interrupt Status Register */ 
  volatile uint32      CAN_ISR;
  /* CAN Interrupt Enable Register */ 
  volatile uint32      CAN_IER;
  /* CAN Interrupt Clear Register */ 
  volatile uint32      CAN_ICR;
  volatile uint32      RESERVED_0;  
  volatile uint32      RESERVED_1; 
  /* CAN TX FIFO ID Register */ 
  volatile uint32      CAN_TXF_IDR; 
  /* CAN TX FIFO DLC Register */ 
  volatile uint32      CAN_TXF_DLCR; 
  /* CAN TX FIFO Data Word1 Register */ 
  volatile uint32      CAN_TXF_DW1R;
  /* CAN TX FIFO Data Word2 Register */ 
  volatile uint32      CAN_TXF_DW2R;
  /* CAN TX HPB ID Register */ 
  volatile uint32      CAN_HPB_IDR;  
  /* CAN TX HPB DLC Register */ 
  volatile uint32      CAN_HPB_DLCR;
  /* CAN TX HPB Data Word1 Register */ 
  volatile uint32      CAN_HPB_DW1R;
  /* CAN TX HPB Data Word2 Register */
  volatile uint32      CAN_HPB_DW2R; 
  /* CAN RX FIFO ID Register */
  volatile uint32      CAN_RXF_IDR; 
  /* CAN RX FIFO DLC Register */
  volatile uint32      CAN_RXF_DLCR; 
  /* CAN RX FIFO Data Word1 Register */
  volatile uint32      CAN_RXF_DW1R; 
  /* CAN RX FIFO Data Word2 Register */
  volatile uint32      CAN_RXF_DW2R; 
  /* Acceptance Filter Register */
  volatile uint32      CAN_AFR; 
  /* Acceptance Filter Mask Register1 */
  volatile uint32      CAN_AFMR1; 
  /* Acceptance Filter ID Register1 */
  volatile uint32      CAN_AFIR1; 
  /* Acceptance Filter Mask Register2 */
  volatile uint32      CAN_AFMR2; 
  /* Acceptance Filter ID Register2 */
  volatile uint32      CAN_AFIR2; 
  /* Acceptance Filter Mask Register3 */
  volatile uint32      CAN_AFMR3; 
  /* Acceptance Filter ID Register3 */
  volatile uint32      CAN_AFIR3;
  /* Acceptance Filter Mask Register4 */
  volatile uint32      CAN_AFMR4; 
  /* Acceptance Filter ID Register4 */
  volatile uint32      CAN_AFIR4;  
} Can_RegType;


typedef volatile struct
{
  uint32      CanInitBRPR;
  uint32      CanInitBTR;
  uint32      CanInitIER;
  uint32      CanInitAFR;
  uint32      CanInitAFMR1;
  uint32      CanInitAFIR1;
  uint32      CanInitAFMR2;
  uint32      CanInitAFIR2;
  uint32      CanInitAFMR3;
  uint32      CanInitAFIR3;
  uint32      CanInitAFMR4;
  uint32      CanInitAFIR4;
} Can_RegInitType;

typedef struct
{
    uint32      oldCanIER;
}Can_OldIERType;

/*******************************************************************************
*  Register definitions
*******************************************************************************/


#define CAN_CONTROLLER_BASIS_ADR(controllerID)     \
                                ((Can_RegType *)(CanBasisAddress[controllerID]))
#define CAN_CR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_CR)
#define CAN_MSR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_MSR)
#define CAN_BRPR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_BRPR)
#define CAN_BTR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_BTR)
#define CAN_ECR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_ECR)
#define CAN_ESR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_ESR)
#define CAN_SR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_SR)
#define CAN_ISR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_ISR)
#define CAN_IER(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_IER)
#define CAN_ICR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_ICR)
#define CAN_TXF_IDR(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_TXF_IDR)
#define CAN_TXF_DLCR(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_TXF_DLCR)
#define CAN_TXF_DW1R(controllerID)     (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_TXF_DW1R)
#define CAN_TXF_DW2R(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_TXF_DW2R)
#define CAN_HPB_IDR(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_HPB_IDR)
#define CAN_HPB_DLCR(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_HPB_DLCR)
#define CAN_HPB_DW1R(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_HPB_DW1R)
#define CAN_HPB_DW2R(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_HPB_DW2R)
#define CAN_RXF_IDR(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_RXF_IDR)
#define CAN_RXF_DLCR(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_RXF_DLCR)
#define CAN_RXF_DW1R(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_RXF_DW1R)
#define CAN_RXF_DW2R(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_RXF_DW2R)
#define CAN_AFR(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFR)
#define CAN_AFMR1(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFMR1)
#define CAN_AFIR1(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFIR1)
#define CAN_AFMR2(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFMR2)
#define CAN_AFIR2(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFIR2)
#define CAN_AFMR3(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFMR3)
#define CAN_AFIR3(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFIR3)
#define CAN_AFMR4(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFMR4)
#define CAN_AFIR4(controllerID)   (CAN_CONTROLLER_BASIS_ADR(controllerID)->CAN_AFIR4)



/*******************************************************************************
*  Register Mask Value define
*******************************************************************************/

/* Bitmask of CR: */
#define CAN_INITRQ   ((uint32)0x02)    /* initialized mode request                 */

/* Bitmask of SR: */
#define CAN_INITAK   ((uint32)0x01)    /* initialized mode ack                    */
#define CAN_NORMALAK ((uint32)0x08)    /* normal mode ack                    */
/* Bitmask of CANCTL1: */

#define CAN_SLPAK    ((uint8)0x02)    /* sleep mode acknowledge                   */
#define CAN_WUPM     ((uint8)0x04)    /* wakeup mode                              */
#define CAN_BORM     ((uint8)0x08)    /* bus-off Recovery upon user request       */
#define CAN_LISTEN   ((uint8)0x10)    /* listen only                              */
#define CAN_LOOPB    ((uint8)0x20)    /* loopback mode                            */
#define CAN_CLKSRC   ((uint8)0x40)    /* clocksource is bus clock                 */
#define CAN_CANE     ((uint8)0x80)    /* can enabled                              */

/* Bitmasks of CANRFLG: */
#define CAN_RXF      ((uint8)0x01)    /* receive buffer full  flag                */
#define CAN_OVRIF    ((uint8)0x02)    /* overrun  flag                            */
#define CAN_TSTAT0   ((uint8)0x04)    /* transmitter status bits TSTAT[0:1]       */
#define CAN_TSTAT1   ((uint8)0x08)    /* TSTAT[0:1] =11,bus off                   */
#define CAN_RSTAT0   ((uint8)0x10)    /* receiver status bits RSTAT[0:1]          */
#define CAN_RSTAT1   ((uint8)0x20)    /*                                          */
#define CAN_CSCIF    ((uint8)0x40)    /* can status change  flag                  */
#define CAN_WUPIF    ((uint8)0x80)    /* wake-up  flag                            */
#define CAN_BOFFTIF  ((uint8)(CAN_TSTAT0|CAN_TSTAT1))  /* BusOff flag                     */
#define CAN_BOFFRIF  ((uint8)(CAN_RSTAT0|CAN_RSTAT1))  /* BusOff flag                     */

/* Bitmasks of CRIER:   */

#define CAN_RXFIE    ((uint8)0x01)    /* receive buffer full enable               */
#define CAN_OVRIE    ((uint8)0x02)    /* overrun interrupt enable                 */
#define CAN_TSTAT0E  ((uint8)0x04)    /* transmitter status change enable
                                                                  CAN_TSTAT0E[0:1]*/
#define CAN_TSTAT1E  ((uint8)0x08)    /*                                          */
#define CAN_RSTAT0E  ((uint8)0x10)    /* receiver statu change enable CAN_RSTAT0E[0:1]*/
#define CAN_RSTAT1E  ((uint8)0x20)    /*                                          */
#define CAN_CSCIE    ((uint8)0x40)    /* can status change interrupt enable       */
#define CAN_WUPIE    ((uint8)0x80)    /* wake-up interrupt enable                 */
#define CAN_BOFFIE   ((uint8)(CAN_TSTAT1E|CAN_RSTAT1E))  /* BusOff flag*/

 /* Bitmasks of CTARQ:   */
#define CAN_ABTRQ0   ((uint8)0x01)    /* abort request                            */
#define CAN_ABTRQ1   ((uint8)0x02)
#define CAN_ABTRQ2   ((uint8)0x04)

/* Bitmasks of CTAAK:    */
#define CAN_ABTAK0   ((uint8)0x01)    /* abort acknowledge                        */
#define CAN_ABTAK1   ((uint8)0x02)
#define CAN_ABTAK2   ((uint8)0x04)

/* Bitmasks of CTBSEL:   */
#define CAN_TX0           ((uint8)0x01)
#define CAN_TX1           ((uint8)0x02)
#define CAN_TX1TX2        ((uint8)0x04)
#define CAN_TXIE          ((uint8)(CAN_TX0|CAN_TX1|CAN_TX1TX2))
#define CAN_TXMASKALL     ((uint8)(CAN_TX0|CAN_TX1|CAN_TX1TX2))
#define CAN_TXMASK(hth)   ((uint8)(0x02))

/* Bitmasks of CMISC:    */
#define CAN_BOHOLD   ((uint8)0x01)

/*******************************************************************************
*  Controller related  
*******************************************************************************/
#if (CAN_USED_CONTROLLER_NUM >= 1)

    #if (CAN0_BUSOFF_PROCESSING == CAN_INTERRUPT)
        #define CAN0_CSCIE_INT_CFG  CAN_CSCIE
    #elif (CAN0_BUSOFF_PROCESSING == CAN_POLLING)
        #define CAN0_CSCIE_INT_CFG  ((uint8)0x00U)
    #else
        #error "!!!ERROR FOR CAN0_BUSOFF_CFG!!!"
    #endif

    #if (CAN0_TX_PROCESSING == CAN_INTERRUPT)
        #define CAN0_TX_INT_CFG  CAN_TXIE
    #elif (CAN0_TX_PROCESSING == CAN_POLLING)
        #define CAN0_TX_INT_CFG  ((uint8)0x00)
    #else
        #error "!!!ERROR FOR CAN0_TX_CFG!!!"
    #endif

    #if (CAN0_RX_PROCESSING == CAN_INTERRUPT)
        #define CAN0_RX_INT_CFG  CAN_RXFIE
    #elif (CAN0_RX_PROCESSING == CAN_POLLING)
        #define CAN0_RX_INT_CFG  ((uint8)0x00)
    #else
        #error "!!!ERROR FOR CAN0_RX_CFG!!!"
    #endif

    #if (CAN0_WAKEUP_PROCESSING == CAN_INTERRUPT)
        #define CAN0_WAKEUP_INT_CFG  CAN_WUPIE
    #elif (CAN0_WAKEUP_PROCESSING == CAN_POLLING)
        #define CAN0_WAKEUP_INT_CFG  ((uint8)0x00)
    #else
        #error "!!!ERROR FOR CAN0_WAKEUP_CFG!!!"
    #endif

    #if (CAN0_CLKSRC_BUSCLK == STD_ON)
        #define CAN0_CLKSRC_CFG  ((uint8)0x40)
    #elif (CAN0_CLKSRC_BUSCLK == STD_OFF)
        #define CAN0_CLKSRC_CFG  ((uint8)0x00)
    #else
        #error "!!!ERROR FOR CAN0_WAKEUP_CFG!!!"
    #endif

#endif/* #if (CAN_USED_CONTROLLER_NUM>=1) */


/*******************************************************************************
*   Macro Fuctions definitions
*******************************************************************************/


/*Used for transmit*/
#if (STD_ON == CAN_ENABLE_EXTERNED_ID)
    #define GET_REG_STDID(id)     (((uint32)(((uint32)(id)) << 21)))
#else
    #define GET_REG_STDID(id)     ((uint16)(((uint16)(id)) & 0x7ff))
#endif


#define GET_REG_EXTID(id)         (uint32)(((uint32)(id) << 13) | ((uint32)1 << 12) | ((uint32)1 << 11) | (((uint32)(id) & 0x1ffc0000) >> 18))


/*Used for receive*/
#define GET_EXTID(canID)     (((uint32)(canID  \
                                  & ((uint32)0x7FFul)) << 18)    \
                                  + ((uint32)((canID >> 13) \
                                  & ((uint32)0x3FFFF)) ))

#if (STD_ON == CAN_ENABLE_EXTERNED_ID)
    #define GET_STDID(controller)   ((uint16)((CAN_RxBuf(controller).Id) >> 21))
#else
    #define GET_STDID(canID)   ((uint32)(canID) & 0x7ff)
#endif

#if (STD_ON == CAN_ENABLE_EXTERNED_ID)
    #define GET_IDE(controller)     ((uint32)((CAN_RxBuf(controller).Id) \
                                    &((uint32)0x00080000)))
#else
    #define GET_IDE(canID)     ((uint32)((canID) \
                                    &((uint32)1ul << 12)))
#endif

#define GET_DLC(dlc)   ((uint8)((dlc) & ((uint8)0x0f)))

/*******************************************************************************
*   Loop timer define 
*******************************************************************************/
#if (STD_ON == CAN_HARDWARE_TIMEOUT_CHECK) 
    #define CAN_LOOP_BEGIN(i)      ApplTimerBegin(i)
    #define CAN_LOOP_TIMEOUT(i)    ApplTimerOutCheck(i)
    #define CAN_LOOP_END(i)        ApplTimerEnd(i) 
#else
    #define CAN_LOOP_BEGIN(i)  
    #define CAN_LOOP_TIMEOUT(i)    CAN_NOT_OK
    #define CAN_LOOP_END(i)  
#endif/* #if (STD_ON == CAN_HARDWARE_TIMEOUT_CHECK) */
        
#define INIT_ENTER_TIMEOUT        (0x0u)
#define INIT_LEAVE_TIMEOUT        (0x1u)
#define SLEEP_ENTER_TIMEOUT       (0x2u)
#define SLEEP_LEAVE_TIMEOUT       (0x3u)

#endif/* _CAN_PL_H_*/
