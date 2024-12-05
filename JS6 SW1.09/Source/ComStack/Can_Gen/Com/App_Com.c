/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name:       App_Com.c
********************************************************************************
*   Project/Product: COM programme
*   Title:           App_Com.c
*   Author:          bo.liu        2010-2011
*                    donger.yang   2012-
********************************************************************************
*   Description:     Defination of Interface for Application Layer
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
#include "demm.h"
#include "App_Can.h"
#include "App_Keyboard.h"
#include "App_WheelHeat.h"
void Com_TxIpduCallout(PduIdType PduId)
{
    /*
  static uint8_t  u8ErrCnt=0;
  uint8_t sum=0;
  uint8_t i;  

  if(COM_TXIPDUSWS_3B0==PduId)
    {     
       TxIpduSWS_3B0._c[6]=(u8ErrCnt<<4)&0xf0;
      for(i=0;i<7;i++) sum+= TxIpduSWS_3B0._c[i];      
      sum=sum^0x0ff;
      TxIpduSWS_3B0._c[7]= sum;
      u8ErrCnt++;
      u8ErrCnt%=16;
    }  
*/
}

void Com_RxIpduCallout(PduIdType PduId)
{
}

void AppIpdu_Message1_TxTOIndication(void)
{

}

//////////////////////////////////////////////////////////////////

void AppIpduSWS_4A8_TxTOIndication(void)
{
  AppIpduSWS_4A8_TOProcess();
}

void AppIpduSWS_3B0_TxTOIndication(void)
{
    AppIpduSWS_3B0_TOProcess();
}

void AppIpduSWS_429_TxTOIndication(void)
{
    AppIpduSWS_429_TOProcess();
}
///////////////////////////////////////////////


void AppTxIpdu_Message1_Conf(void)
{
  AppIpduSWS_36A_ConfProcess();
}



void AppIpduSWS_4A8_Conf(void)
{
  //AppIpduSWS_4A8_ConfProcess();
}

void AppIpduSWS_3B0_Conf(void)
{
//AppIpduSWS_3B0_ConfProcess();
}

void AppIpduSWS_429_Conf(void)
{
//AppIpduSWS_429_ConfProcess();
}
//////////////////////////////////////



//接收加热按键是否按下
void AppIpdu_Message1_Ind(void)
{
    AppIpduADS_2CD_Ind_Process();
}
//接收发动机状态
void AppIpdu_Message2_Ind(void)
{
    AppIpdu_117_Ind_Process();
}
void AppIpdu_193_Ind(void)
{
    AppIpdu_193_Ind_Process();
}
void AppIpdu_1F4_Ind(void)
{
    AppIpdu_1F4_Ind_Process();
}

void AppIpdu_23C_Ind(void)
{
    AppIpdu_23C_Ind_Process();
}




void AppIpduADS_29C_Ind(void)
{
  (void)AppIpduADS_29C_Ind_Process();
}

void AppIpduBCM_12D_Ind(void)
{
  (void)AppIpduBCM_12D_Ind_Process();
}

void AppIpduLBC_4BF_Ind(void)
{
  (void)AppIpduLBC_4BF_Ind_Process();
}

void AppIpduMPC_316_Ind(void)
{
  (void)AppIpduMPC_316_Ind_Process();
}

void AppIpduMED_385_Ind(void)
{
  (void)AppIpduMED_385_Ind_Process();
}

void AppIpduFGM_49A_Ind(void)
{
  (void)AppIpduFGM_49A_Ind_Process();
}


void AppIpduCSM_0A2_Ind(void)
{
  (void)AppIpduCSM_0A2_Ind_Process();
}

void AppIpduRBC_4E6_Ind(void)
{
  (void)AppIpduRBC_4E6_Ind_Process();
}

//////////////////////////////////////////////////////


void AppRxIpdu_Message1_TimeOutInd(void)
{
    
}
void AppRxIpdu_Message2_TimeOutInd(void)
{
    AppRxIpdu_117_TimeOutProcess();
}

void AppRxIpdu_193_TimeOutInd(void)
{
    AppRxIpdu_193_TimeOutProcess();
}

void AppRxIpdu_1F4_TimeOutInd(void)
{
    AppRxIpdu_1F4_TimeOutProcess();
}

void AppRxIpdu_23C_TimeOutInd(void)
{
    AppRxIpdu_23C_TimeOutProcess();
}


void AppRxIpduBCM_12D_TimeOutInd(void)
{
  (void)AppRxIpduBCM_12D_TimeOutProcess();
}

void AppRxIpduLBC_4BF_TimeOutInd(void)
{
  (void)AppRxIpduLBC_4BF_TimeOutProcess();
}

void AppRxIpduMPC_316_TimeOutInd(void)
{
  (void)AppRxIpduMPC_316_TimeOutProcess();
}

void AppRxIpduMED_385_TimeOutInd(void)
{
  (void)AppRxIpduMED_385_TimeOutProcess();
}

void AppRxIpduFGM_49A_TimeOutInd(void)
{
  (void)AppRxIpduFGM_49A_TimeOutProcess();
}

void AppRxIpduCSM_0A2_TimeOutInd(void)
{
  (void)AppRxIpduCSM_0A2_TimeOutProcess();
}

void AppRxIpduRBC_4E6_TimeOutInd(void)
{
  (void)AppRxIpduRBC_4E6_TimeOutProcess();
}
/////////////////////////////////////////////////////////////////////