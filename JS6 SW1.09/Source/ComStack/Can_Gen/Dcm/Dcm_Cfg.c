/*  BEGIN_FILE_HDR
********************************************************************************
*   NOTICE                              
*   This software is the property of HiRain Technologies. Any information 
*   contained in this doc should not be reproduced, or used, or disclosed 
*   without the written authorization from HiRain Technologies.
********************************************************************************
*   File Name       : Dcm_Cfg.c
********************************************************************************
*   Project/Product : Dcm module
*   Title           : Dcm module Source File 
*   Author          : Hirain
********************************************************************************
*   Description     : Main source file of the AUTOSAR Diagnositc communcation 
*   manager, according to:AUTOSAR_SWS_DiagnosticCommunicationManager.pdf (Release
*   4.0) and ISO14229-1.pdf.
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
*   02.01.00    03/09/2013    jinbiao.li    N/A          HrAsrDcm130903-01
********************************************************************************
* END_FILE_HDR*/

/*******************************************************************************
* Include files
********************************************************************************/
#include "Dcm_Cfg.h"
#include "Dcm.h"

CONST(uint8, DCM_VARIABLE)  gSidMap_1[72]=
{
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,      //00-07
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,      //08-0F
    0x00,0x01,0xFF,0xFF,0x02,0xFF,0xFF,0xFF,      //10-17
    0xFF,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,      //18-1F
    0xFF,0xFF,0x04,0xFF,0xFF,0xFF,0xFF,0x05,      //20-27
    0x06,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0xFF,      //28-2F    将2F去除
    0xFF,0x08,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,      //30-37
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x09,0xFF,      //37-3F
    0xFF,0xFF,0xFF,0xFF,0xFF,0x0a,0xFF,0xFF       //40-47，需+0x40=0x80-087
};
static CONST(DcmDsdServiceTable, DCM_VARIABLE)   gServiceTable_1[DCM_NUM_OF_SERVICE]=
{
    #if(DCM_SERVICE_10_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_DiagnosticSessionControl,        UDS_SERVICE_WITH_SUB_FUNCTION,   (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING)}, 
      DCM_SUBSERVICESTART,                      (NUM_OF_10_SUB_FUNCTION-1u),PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},/* 0x10 */
    #endif
    
    #if(DCM_SERVICE_11_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_EcuReset,                        UDS_SERVICE_WITH_SUB_FUNCTION,   (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC)},
      NUM_OF_10_SUB_FUNCTION,  (NUM_OF_10_SUB_FUNCTION+ NUM_OF_11_SUB_FUNCTION-1u),PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},/* 0x11 */
    #endif
    
    #if(DCM_SERVICE_14_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_ClearDiagnosticInformation,      UDS_SERVICE_WITHOUT_SUB_FUNCTION,(DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING) },
      (DCM_NUM_OF_SUB_SERVICE-1u),(DCM_NUM_OF_SUB_SERVICE-1u),PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},/* 0x14 */
    #endif
    
    #if(DCM_SERVICE_19_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_ReadDTCInformation,              UDS_SERVICE_WITH_SUB_FUNCTION,   (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING) }, 
    (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION),(NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION-1u),PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},/* 0x19 */
    #endif
   
    #if(DCM_SERVICE_22_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_ReadDataByIdentifier,            UDS_SERVICE_WITHOUT_SUB_FUNCTION,(DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC)},
      (DCM_NUM_OF_SUB_SERVICE-1u),(DCM_NUM_OF_SUB_SERVICE-1u),PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},/* 0x22 */
    #endif
    
    #if(DCM_SERVICE_27_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_SecurityAccess,                  UDS_SERVICE_WITH_SUB_FUNCTION,   (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_EXTENDED_DIAGNOSTIC) },
      (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION),(NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION-1u),
      PHYSICAL_REQ_SUPPORTED},/* 0x27 */
    #endif
    
    #if(DCM_SERVICE_28_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_CommunicationControl,            UDS_SERVICE_WITH_SUB_FUNCTION,   (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_EXTENDED_DIAGNOSTIC) },
      (NUM_OF_10_SUB_FUNCTION+ NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION),(NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION+NUM_OF_28_SUB_FUNCTION-1u),
      PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},/* 0x28 */
    #endif
    
    #if(DCM_SERVICE_2E_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_WriteDataByIdentifier,           UDS_SERVICE_WITHOUT_SUB_FUNCTION,(DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING)},  //programming?
      (DCM_NUM_OF_SUB_SERVICE-1u),(DCM_NUM_OF_SUB_SERVICE-1u),PHYSICAL_REQ_SUPPORTED},/* 0x2E */
    #endif
    
    #if(DCM_SERVICE_2F_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_InputOutputControlByIdentifier,  UDS_SERVICE_WITHOUT_SUB_FUNCTION,DCM_SEC_LEV_L1,(DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_USERDEFINED1)},
      (DCM_NUM_OF_SUB_SERVICE-1),(DCM_NUM_OF_SUB_SERVICE-1u),PHYSICAL_REQ_SUPPORTED},               /* 0x2F */
    #endif
    
    #if(DCM_SERVICE_31_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_RoutineControl,                  UDS_SERVICE_WITH_SUB_FUNCTION,   (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1|DCM_SEC_LEV_L2),(DCM_SESSION_EXTENDED_DIAGNOSTIC)},//programming?
      (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION+NUM_OF_28_SUB_FUNCTION+NUM_OF_2C_SUB_FUNCTION),
      (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION+NUM_OF_28_SUB_FUNCTION+NUM_OF_2C_SUB_FUNCTION+NUM_OF_31_SUB_FUNCTION-1u),PHYSICAL_REQ_SUPPORTED},               /* 0x31 */
    #endif
    
    #if(DCM_SERVICE_3E_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_TesterPresent,                   UDS_SERVICE_WITH_SUB_FUNCTION,   (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING)},
    (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION+NUM_OF_28_SUB_FUNCTION+NUM_OF_2C_SUB_FUNCTION+NUM_OF_31_SUB_FUNCTION),
    (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION+NUM_OF_28_SUB_FUNCTION+NUM_OF_2C_SUB_FUNCTION+NUM_OF_31_SUB_FUNCTION+NUM_OF_3E_SUB_FUNCTION-1u),
    PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},/* 0x3E */
    #endif
    
    #if(DCM_SERVICE_85_ENABLED==STD_ON)
    { DCM_DSDSIDTABID,{&DspInternal_Uds_ControlDTCSetting,               UDS_SERVICE_WITH_SUB_FUNCTION,   (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),(DCM_SESSION_EXTENDED_DIAGNOSTIC)},
    (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION+NUM_OF_28_SUB_FUNCTION+NUM_OF_2C_SUB_FUNCTION+NUM_OF_31_SUB_FUNCTION+NUM_OF_3E_SUB_FUNCTION),
    (NUM_OF_10_SUB_FUNCTION+NUM_OF_11_SUB_FUNCTION+NUM_OF_19_SUB_FUNCTION+NUM_OF_27_SUB_FUNCTION+NUM_OF_28_SUB_FUNCTION+NUM_OF_2C_SUB_FUNCTION+NUM_OF_31_SUB_FUNCTION+NUM_OF_3E_SUB_FUNCTION+NUM_OF_85_SUB_FUNCTION-1u),PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},/* 0x85 */
    #endif
    
    
    { 1,{DCM_NULL,                                        UDS_SERVICE_WITHOUT_SUB_FUNCTION,DCM_SEC_LEV_LOCK,DEFAULT_AND_EXTENDED_SEESION_HANDLE},(DCM_NUM_OF_SUB_SERVICE-1u),(DCM_NUM_OF_SUB_SERVICE-1u),PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED} /* 0xFF */
}; 

P2CONST(CurrentSidMapType, DCM_VARIABLE, DCM_VARIABLE)  gCurrentSidMap=gSidMap_1;
P2CONST(DcmDsdServiceTable, DCM_VARIABLE, DCM_VARIABLE)  gCurrentDcmDsdServiceTable=gServiceTable_1;
CONST(Dcm_CurentDcmDsdServiceTableSizeType, DCM_VARIABLE)  gCurentDcmDsdServiceTableSize=DCM_NUM_OF_SERVICE;
CONST(DcmDsdSubService, DCM_VARIABLE)  gDcmDsdSubService[DCM_NUM_OF_SUB_SERVICE]=
{                                                           
  #if(DCM_SERVICE_10_ENABLED==STD_ON)
  #if(DCM_SERVICE_10_Default==STD_ON)
  {&App_Default,                                    &App_DefaultPost,               DCM_DEFAULT_SESSION,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #if(DCM_SERVICE_10_Programming==STD_ON)
  {&App_Programming,                                    &App_ProgrammingPost,               DCM_PROGRAMMING_SESSION,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING), PHYSICAL_REQ_SUPPORTED}, 
  #endif
  
  #if(DCM_SERVICE_10_Extended_Diagnostic==STD_ON)
  {&App_Extended_Diagnostic,                                    &App_Extended_DiagnosticPost,               DCM_EXTENDED_DIAGNOSTIC_SESSION,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #if(DCM_SERVICE_10_UserDefined1==STD_ON)
  {&App_Kuwe_Session,                                    &App_Kuwe_SessionPost,               DCM_Kuwe_Session_SESSION,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_USERDEFINED1), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #endif
  #if(DCM_SERVICE_11_ENABLED==STD_ON)
  #if(DCM_SERVICE_11_HardReset==STD_ON)
  {&App_HardReset,                                    &App_HardResetPost,               DCM_HARD_RESET,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  #if(DCM_SERVICE_11_SoftReset==STD_ON)
  {&App_SoftReset,                                    &App_SoftResetPost,               DCM_SOFT_RESET,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_USERDEFINED1|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif  
  #endif
  #if(DCM_SERVICE_19_ENABLED==STD_ON)
  #if(reportNumberOfDTCByStatusMask==STD_ON)
  {&App_ReportNumberOfDTCByStatusMask,                                    DCM_NULL,               DCM_REPORTNUMBEROFDTCBYSTATUSMASK,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #if(reportDTCByStatusMask==STD_ON)
  {&App_ReportDTCByStatusMask,                                    DCM_NULL,               DCM_REPORTDTCBYSTATUSMASK,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #if(reportSupportedDTC==STD_ON)
  {&App_ReportSupportedDTC,                                    DCM_NULL,               DCM_REPORTSUPPORTEDDTC,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif

  #if(reportDTCSnapshotRecordByDTCNumber == STD_ON)
  {&App_ReportDTCSnapshotRecordByDTCNumber,                     DCM_NULL,               DCM_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},    
  #endif
  #if(reportDTCExtendedDataRecordByDTCNumber==STD_ON)
  {&App_ReportDTCExtendedDataRecordByDTCNumber,                     DCM_NULL,               DCM_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},   
  #endif
  #if(reportDTCSnapshotIdentification == STD_ON)
  {&App_ReportDTCSnapshotIdentification,                         DCM_NULL,               DCM_REPORTDTCSNAPSHOTIDENTIFICATION,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_USERDEFINED1), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED},    
  #endif
#endif
  #if(DCM_SERVICE_27_ENABLED==STD_ON)
  #if(DCM_SERVICE_27_Request_Seed_L1==STD_ON)
  {&App_Request_Seed_L1,                                    DCM_NULL,               REQUEST_SEED_L1,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC), PHYSICAL_REQ_SUPPORTED}, 
  #endif
  
  #if(DCM_SERVICE_27_Send_Key_L1==STD_ON)
  {&App_Send_Key_L1,                                    DCM_NULL,               SEND_KEY_L1,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC), PHYSICAL_REQ_SUPPORTED}, 
  #endif
  
  #endif
  #if(DCM_SERVICE_28_ENABLED==STD_ON)
  #if(DCM_SERVICE_28_EnableRxAndTx==STD_ON)
  {&App_EnableRxAndTx,                                    DCM_NULL,               ENABLE_RX_AND_TX,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #if(DCM_SERVICE_28_DisableRxAndTx==STD_ON)
  {&App_DisableRxAndTx,                                    DCM_NULL,               DISABLE_RX_AND_TX,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_USERDEFINED1|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  #if(DCM_SERVICE_28_EnableRxAndDisableTx==STD_ON)
  {&App_EnableRxAndDisableTx,                                    DCM_NULL,               ENBALE_RX_AND_DISABLE_TX,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_USERDEFINED1|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  #if(DCM_SERVICE_28_DisableRxAndEnableTx==STD_ON)
  {&App_DisableRxAndEnableTx,                                    DCM_NULL,               DISABLE_RX_AND_ENBALE_TX,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  #endif
  #if(DCM_SERVICE_2C_ENABLED==STD_ON)
  #endif
#if(DCM_SERVICE_31_ENABLED==STD_ON)
  #if(DCM_SERVICE_31_startRoutine==STD_ON)
  {DCM_NULL,                                              DCM_NULL,                         START_ROUTINE,                      
     SUB_FUNCTION_SUPPORTED,(DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1|DCM_SEC_LEV_L2),(DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING),                               PHYSICAL_REQ_SUPPORTED}, /* 40,0x31 01 */
  #endif
  
  #if(DCM_SERVICE_31_stopRoutine==STD_ON)
  {DCM_NULL,                                              DCM_NULL,                         STOP_ROUTINE,                         
  SUB_FUNCTION_SUPPORTED,(DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1|DCM_SEC_LEV_L2),(DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING),                               PHYSICAL_REQ_SUPPORTED}, /* 40,0x31 02 */
  #endif
  
  #if(DCM_SERVICE_31_requestRoutineResults==STD_ON)
  {DCM_NULL,                                              DCM_NULL,                         REQUEST_ROUTINE_RESULT,                       
    SUB_FUNCTION_SUPPORTED,(DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1|DCM_SEC_LEV_L2),(DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING),                               PHYSICAL_REQ_SUPPORTED}, /* 40,0x31 03 */
  #endif
  #endif
  #if(DCM_SERVICE_3E_ENABLED==STD_ON)
  {DCM_NULL,                                    DCM_NULL,               ZERO_SUB_FUNCTION,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_DEFAULT|DCM_SESSION_EXTENDED_DIAGNOSTIC|DCM_SESSION_PROGRAMMING), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #if(DCM_SERVICE_85_ENABLED==STD_ON)
  #if(DCM_SERVICE_85_DTCRecordOn==STD_ON)
  {&App_DTCRecordOn,                                    DCM_NULL,               DTC_RECORD_ON,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #if(DCM_SERVICE_85_DTCRecordOff==STD_ON)
  {&App_DTCRecordOff,                                    DCM_NULL,               DTC_RECORD_OFF,                         SUB_FUNCTION_SUPPORTED,    (DCM_SEC_LEV_LOCK|DCM_SEC_LEV_L1),
  (DCM_SESSION_EXTENDED_DIAGNOSTIC), PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}, 
  #endif
  
  #endif
  #if(DCM_SERVICE_86_ENABLED==STD_ON)
  #endif
  
  {DCM_NULL,             								DCM_NULL,                         0,                             SUB_FUNCTION_SUPPORTED,DCM_SEC_LEV_LOCK,DEFAULT_AND_EXTENDED_SEESION_HANDLE,PHYSICAL_AND_FUNCTIONAL_REQ_SUPPORTED}  /* 54,0xFF */
};

