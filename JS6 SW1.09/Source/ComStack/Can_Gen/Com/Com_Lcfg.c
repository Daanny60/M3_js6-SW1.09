/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       Com_Lcfg.c
********************************************************************************
*   Project/Product: COM programme
*   Title:           Com_Lcfg.c
*   Author:          bo.liu        2010-2011
*                    donger.yang   2012-
********************************************************************************
*   Description:     Define configure data for Com
* 
********************************************************************************
*   Limitations:      None
*
********************************************************************************
*
********************************************************************************
*   Revision History:
* 
*   Version       Date    Initials      CR#     Descriptions
*   -------   ----------  ----------    ------  -------------------
*   03.00.xx
********************************************************************************
*END_FILE_HDR*/

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "Com.h"

/*******************************************************************************
*   for Com_SendSignal
*******************************************************************************/
void  COM_SendSigSWS_SubId(COM_CONST void* SigValue)
{
  //TxIpduSWS_4A8.IpduSWS_4A8.SWS_SubId=(*(uint8 *)SigValue);
  //Com_SetSendRequest(COM_TXSIGSWS_SUBID);//
}
void  COM_SendSigSWS_MediKey(COM_CONST void* SigValue)
{
  //TxIpduSWS_4A8.IpduSWS_4A8.SWS_MediaKey=(*(uint8 *)SigValue);
  //Com_SetSendRequest(COM_TXSIGSWS_MEDIAKEY);//
}

void  COM_SendSigSWS_InstMenu(COM_CONST void* SigValue)
{
  //TxIpduSWS_4A8.IpduSWS_4A8.SWS_InstMenu=(*(uint8 *)SigValue);
  //Com_SetSendRequest(COM_TXSIGSWS_INSTMENU);//
}

void  COM_SendSigSWS_DriverInformation(COM_CONST void* SigValue)
{
  //TxIpduSWS_4A8.IpduSWS_4A8.SWS_DriverInformation=(*(uint8 *)SigValue);
  //Com_SetSendRequest(COM_TXSIGSWS_DRIVERINFORMATION);//
}

void  COM_SendSigSWS_CrossCountry(COM_CONST void* SigValue)
{
  //TxIpduSWS_4A8.IpduSWS_4A8.SWS_CrossCountry=(*(uint8 *)SigValue);
  //Com_SetSendRequest(COM_TXSIGSWS_CROSSCOUNTRY);//
}

void  COM_SendSigSWS_CvsModeIn(COM_CONST void* SigValue)
{
  //TxIpduSWS_4A8.IpduSWS_4A8.SWS_CvsModeIn=(*(uint8 *)SigValue);
  //Com_SetSendRequest(COM_TXSIGSWS_CVSMODEIN);//
}

void  COM_SendSigSWS_CvsModeOut(COM_CONST void* SigValue)
{
  //TxIpduSWS_4A8.IpduSWS_4A8.SWS_CvsModeOut=(*(uint8 *)SigValue);
  //Com_SetSendRequest(COM_TXSIGSWS_CVSMODEOUT);//
}
////////////////////////////////////////////////////////////////////
void  COM_SendSigSWS_DistanceSet(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_DistaneSet=(*(uint8 *)SigValue);
  ////Com_SetSendRequest(COM_TXSIGSWS_DISTANCESET);//
}
void  COM_SendSigSWS_SpeedSet(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_SpeedSet=(*(uint8 *)SigValue);
  ////Com_SetSendRequest(COM_TXSIGSWS_SPEEDSET);//
}
void  COM_SendSigSWS_Dipilot(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_Dipliot=(*(uint8 *)SigValue);
  ////Com_SetSendRequest(COM_TXSIGSWS_DIPILOT);//
}
void  COM_SendSigSWS_CruiseLimitMode(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_CruiseLimitMode=(*(uint8 *)SigValue);
 //// Com_SetSendRequest(COM_TXSIGSWS_CRUISELIMITMODE);//
}
void  COM_SendSigSWS_ResetSignal(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_ReSetSignal=(*(uint8 *)SigValue);
  ///Com_SetSendRequest(COM_TXSIGSWS_RESETSIGNAL);//
}
void  COM_SendSigSWS_CruiseFaultStatus(COM_CONST void* SigValue)
{
 // TxIpduSWS_3B0.IpduSWS_3B0.SWS_CruiseFaultStatus=(*(uint8 *)SigValue);
 //// Com_SetSendRequest(COM_TXSIGSWS_CRUISEFAULTSTATUS);//
}
void  COM_SendSigSWS_SetSignal(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_SetSignal=(*(uint8 *)SigValue);
  ////Com_SetSendRequest(COM_TXSIGSWS_SETSIGNAL);//
}
void  COM_SendSigSWS_DisanceDec(COM_CONST void* SigValue)
{
 // TxIpduSWS_3B0.IpduSWS_3B0.SWS_DisanceDec=(*(uint8 *)SigValue);
 //// Com_SetSendRequest(COM_TXSIGSWS_DISANCEDEC);//
}
void  COM_SendSigSWS_DisanceAdd(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_DisanceAdd=(*(uint8 *)SigValue);
 //// Com_SetSendRequest(COM_TXSIGSWS_DISANCEADD);//
}
void  COM_SendSigSWS_AccSignal(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_AccSignal=(*(uint8 *)SigValue);
 //// Com_SetSendRequest(COM_TXSIGSWS_ACCSIGNAL);//
}
void  COM_SendSigSWS_AccLeft(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_AccLeft=(*(uint8 *)SigValue);
  ////Com_SetSendRequest(COM_TXSIGSWS_ACCLEFT);//
}
void  COM_SendSigSWS_AccRight(COM_CONST void* SigValue)
{
 // TxIpduSWS_3B0.IpduSWS_3B0.SWS_AccRight=(*(uint8 *)SigValue); 
  ////Com_SetSendRequest(COM_TXSIGSWS_ACCRIGHT);//
}
void  COM_SendSigSWS_PackLossCnt(COM_CONST void* SigValue)
{
  //TxIpduSWS_3B0.IpduSWS_3B0.SWS_PackLossCnt=(*(uint8 *)SigValue);

 //// Com_SetSendRequest(COM_TXSIGSWS_PACKLOSSCNT);//
}
///////////////////////////////////////////////////////////////////
void  COM_SendSigSWS_HeatOrNot(COM_CONST void* SigValue)
{
  //TxIpduSWS_429.IpduSWS_429.SWS_HeatOrNot=(*(uint8 *)SigValue);
 //Com_SetSendRequest(COM_TXSIGSWS_HeatOrNot);//
}
void  COM_SendSigSWS_HeatStatus(COM_CONST void* SigValue)
{
  TxIpduSWS_36A.IpduSWS_36A.HEAT_STATUS=(*(uint8 *)SigValue);
  Com_SetSendRequest(COM_TXSIGSWS_HEATSTATUS);
 // TxIpduSWS_429.IpduSWS_429.SWS_HeatStatus=(*(uint8 *)SigValue);
  /////Com_SetSendRequest(COM_TXSIGSWS_HeatSTATUS);//
}

void  COM_SendSigSWS_LeftModeEncoder(COM_CONST void* SigValue)
{
  // TxIpduSWS_429.IpduSWS_429.SWS_LeftModeMore=1;
  //TxIpduSWS_429.IpduSWS_429.SWS_LeftModeEncoder=(*(uint8 *)SigValue);
 //Com_SetSendRequest(COM_TXSIGSWS_LEFTMODEENCODER);//
}
void  COM_SendSigSWS_LeftModeMore(COM_CONST void* SigValue)
{
  //TxIpduSWS_429.IpduSWS_429.SWS_LeftModeMore=(*(uint8 *)SigValue);
////  Com_SetSendRequest(COM_TXSIGSWS_LEFTMODEMORE);//
}

void  COM_SendSigSWS_RightModeEncoder(COM_CONST void* SigValue)
{
 // TxIpduSWS_429.IpduSWS_429.SWS_RightModeMore=1;
  //TxIpduSWS_429.IpduSWS_429.SWS_RightModeEncoder=(*(uint8 *)SigValue);
  //Com_SetSendRequest(COM_TXSIGSWS_RIGHTMODEENCODER);//
}

void  COM_SendSigSWS_RightModeMore(COM_CONST void* SigValue)
{
 // TxIpduSWS_429.IpduSWS_429.SWS_RightModeMore=(*(uint8 *)SigValue);
////  Com_SetSendRequest(COM_TXSIGSWS_RIGHTMODEMORE);//
}



#if (COM_TXIPDUNUM>=1)
COM_CONST COM_SendSignalFun COM_SendSignalFunPrt[COM_TXSIGNUM]={
  COM_SendSigSWS_HeatStatus,
};
#endif
/*******************************************************************************
*   for Com_ReceiveSignal
*******************************************************************************/

void COM_ReceiveSigADS_HeatCommand(void* SigValue)
{
(*(uint8 *)SigValue) = RxIpduADS_560.IpduADS_560.HEAT_Command;
}

void COM_ReceiveSig_EngineState(void* SigValue)
{
(*(uint8 *)SigValue) = RxIpdu_117.Ipdu_117.EngineState;
}

//Receive signal HvtSts
void  COM_ReceiveSigHvtSts(void* SigValue)
{
(*(uint8 *)SigValue) = RxIpdu_193.Ipdu_193.HvtSts;
}

void  COM_ReceiveSigKeyState(void* SigValue)
{
(*(uint8 *)SigValue) = RxIpdu_2AB.Ipdu_2AB.KeyState;
}

void COM_ReceiveSigVehSpd(void* SigValue)
{
  uint16_t u16VehSpd = 0;
  u16VehSpd = RxIpdu_23C.Ipdu_23C.VehSpdMsb<<8;
  u16VehSpd |= RxIpdu_23C.Ipdu_23C.VehSpdLsb;
  (*(uint16 *)SigValue) = u16VehSpd;
}

void  COM_ReceiveSigADS_AccMode(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduADS_29C.IpduADS_29C.ADS_AccMode;
}
void  COM_ReceiveSigADS_DnpStatus(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduADS_29C.IpduADS_29C.ADS_DnpStatus;
}
void  COM_ReceiveSigADS_AdsTakeOverReq(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduADS_29C.IpduADS_29C.ADS_AdsTakeOverReq;
}
void  COM_ReceiveSigADS_DnpTakeOverReq(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduADS_29C.IpduADS_29C.ADS_DnpTakeOverReq;
}
////////////////////////////////////////////////////////////////////////////
void  COM_ReceiveSigBCM_VehMode(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduBCM_12D.IpduBCM_12D.BCM_VehMode;
}
void  COM_ReceiveSigBCM_PwrGear(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduBCM_12D.IpduBCM_12D.BCM_PwrGear;
}
void  COM_ReceiveSigBCM_VehStatus(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduBCM_12D.IpduBCM_12D.BCM_VehStatus;
}
//////////////////////////////////////////////////
void  COM_ReceiveSigLBC_BackLightBright(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduLBC_4BF.IpduLBC_4BF.LBC_BackLightBright;
}
/////////////////////////////////////////////////////////
void  COM_ReceiveSigMPC_TakeOverReq(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduMPC_316.IpduMPC_316.MPC_TakeOverReq;
}
//////////////////////////////////////////////////////////////
void  COM_ReceiveSigMED_SteelHeat(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduMED_385.IpduMED_385.MED_SteelHeat;
}
////////////////////////////////////////////////////////////
void  COM_ReceiveSigFGM_Ecl(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduFGM_49A.IpduFGM_49A.FGM_Ecl;
}
void  COM_ReceiveSigFGM_OtaLight(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduFGM_49A.IpduFGM_49A.FGM_OtaLight;
}
void  COM_ReceiveSigFGM_OtaReq(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduFGM_49A.IpduFGM_49A.FGM_OtaReq;
}
void  COM_ReceiveSigFGM_OtaBootCmd(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduFGM_49A.IpduFGM_49A.FGM_OtaBootCmd;
}
void  COM_ReceiveSigESP_FGM_CrcXor(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduFGM_49A.IpduFGM_49A.FGM_CrcXor;
}
////////////////////////////////////////////////////////////////////////////////////////
void  COM_ReceiveSigCSM_RemoteCmd(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduCSM_0A2.IpduCSM_0A2.CSM_RemoteCmd;
}
void  COM_ReceiveSigCSM_SteelHeatCmd(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduCSM_0A2.IpduCSM_0A2.CSM_SteelHeatCmd;
}
///////////////////////////////////////////////////////////
void  COM_ReceiveSigRBC_SteelHeat(void* SigValue)
{
//(*(uint8 *)SigValue) = RxIpduRBC_4E6.IpduRBC_4E6.RBC_SteelHeat;

}
///////////////////////////////////////////////////////////////////
#if (COM_RXIPDUNUM>=1)
COM_CONST COM_ReceivedSignalFun COM_ReceivedSignalFunPrt[COM_RXSIGNUM]={
  COM_ReceiveSigADS_HeatCommand,
  COM_ReceiveSig_EngineState,
  COM_ReceiveSigHvtSts,
  COM_ReceiveSigKeyState,
  COM_ReceiveSigVehSpd,

};
#endif
