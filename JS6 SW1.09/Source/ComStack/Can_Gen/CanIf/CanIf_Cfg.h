/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : CanIf_Cfg.h
*
********************************************************************************
*   Project/Product : AUTOSAR 4.0
*   Title           : CAN Interface module configuration File
*   Author          : Hirain
********************************************************************************
*   Description     : Contain the pre-compile-time configuration parameters.  
*
********************************************************************************
*   Limitations     : None
*
********************************************************************************
*
********************************************************************************
*   Revision History:
* 
*   Version     Date          Initials      CR#          Descriptions
*   ---------   ----------    ------------  ----------   ---------------
*   02.02.04    15/08/2013    NingChen      N/A          D10_CANIF_130815_01
*
********************************************************************************
* END_FILE_HDR*/



#ifndef _CANIF_CFG_H_
#define _CANIF_CFG_H_


#define CANIF_FILTER_TYPE_INDEX        (0x02u)
#define CANIF_FILTER_TYPE_LINEAR       (0x03u)

#define CANIF_DISPATCH_UL_CDD          (0u)
#define CANIF_DISPATCH_UL_CAN_SM       (1u)
#define CANIF_DISPATCH_UL_ECUM         (2u)  

/*******************************************************************************
*   CanIfPrivateCfg  
*******************************************************************************/
/*SWS Item        CANIF245_Conf :     
Container Name    CanIfPrivateCfg{CanInterfacePrivateConfiguration}   
Description       This container contains the private configuration 
                  (parameters) of the CAN Interface.*/

/*CANIF617_Conf   Selects whether the DLC check is supported. 
                  True: Enabled False: Disabled  */
#define CANIF_PRIVATE_DLC_CHECK          STD_ON

/*CANIF619_Conf   Selects the desired software filter mechanism for reception  
                  only.Each implemented software filtering method is identified 
                  by this enumeration number. 
                  Range: Types implemented software filtering methods*/
#define CANIF_PRIVATE_SOFTWARE_FILTER_TYPE      CANIF_FILTER_TYPE_INDEX

/*******************************************************************************
*    CanIfPublicCfg  
*******************************************************************************/
/*SWS Item        CANIF246_Conf :  
Container Name    CanIfPublicCfg{CanInterfacePublicConfiguration}  
Description       This container contains the public configuration (parameters) 
                  of the CAN Interface*/

/*CANIF614_Conf   Enables and disables the development error detection and 
                  notification mechanism. */
#define CANIF_PUBLIC_DEV_ERROR_DETECT        STD_OFF
#define CANIF_PUBLIC_TX_BUFFERING            STD_ON
#define CANIF_PUBLIC_CANCEL_TRANSMIT_SUPPORT STD_ON
#define CANIF_PUBLIC_SETDYNAMICTXID_API      STD_OFF
#define CANIF_PUBLIC_READRXPDU_DATA_API      STD_OFF
#define CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API     STD_OFF
#define CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API     STD_OFF
#define CANIF_PUBLIC_VERSION_INFO_API                STD_OFF
#define CANIF_USED_CONTROLLER_NUM         (1u)
#define CANIF_UPPER_LAYER_HANDLE          STD_ON

#define CANIF_PDU_CHANNEL_MODE_CONTROL    STD_ON

#define CANIF_RX_PDU_RANGE_CANID          STD_ON


#define CANIF_HRH_RANGE_FILTER            STD_OFF

#define CANIF_VENDOR_ID_VALUE             ((uint16)26)
#define CANIF_MODULE_ID_VALUE             ((uint16)60)
#define CANIF_INSTANCE_ID_VALUE_0         ((uint8)0) 

/*******************************************************************************
*   CanIfDispatchCfg 
*******************************************************************************/

/*SWS Item        CANIF250_Conf :  
Container Name    CanIfDispatchCfg{CanInterfaceDispatcherConfiguration}
Description       Callback functions provided by upper layer modules of the
                  CanIf. The callback functions defined in this container are 
                  common to all configured CAN Driver / CAN Transceiver 
                  Driver modules. */


/*add for OSEK NM*/
#define CANIF_SUPPORT_OSEK_NM                STD_ON

/*CANIF547_Conf*/
/*This parameter defines the upper layer (UL) module to which the notifications 
of all ControllerBusOff events from the CAN Driver modules have to be routed via
<User_ControllerBusOff>. There is no possibility to configure no upper layer 
(UL) module as the provider of <User_ControllerBusOff>.*/
#define CANIF_DISPATCH_USERCTRLBUSOFF_UL            CANIF_DISPATCH_UL_CDD

/*CANIF548_Conf*/
#define CANIF_DISPATCH_USERSETWAKEUPEVENT_UL        CANIF_DISPATCH_UL_CDD

#define CANIF_DISPATCH_USERCTRLBUSOFF_DEFINED_NAME  /*user define*/

#define CANIF_DISPATCH_USERSETWAKEUPEVENT_DEFINED_NAME /*user define*/
#endif  /* _CANIF_CFG_H_ */
