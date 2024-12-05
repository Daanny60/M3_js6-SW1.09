#include "EncoderDrv.h"
#include "Gpio.h"





#define ENCODER_STATE_IDLE       0U
#define ENCODER_STATE_ROLL_UP    1U
#define ENCODER_STATE_ROLL_DOWN  2U
#define ENCODER_STATE_ERROR      3U

#define LEFT_ENCODER_INPUT_A  FL_GPIO_GetInputPin(LEFT_SIDE_ENCODER_A_GPIO, LEFT_SIDE_ENCODER_A_PIN)
#define LEFT_ENCODER_INPUT_B  FL_GPIO_GetInputPin(LEFT_SIDE_ENCODER_B_GPIO, LEFT_SIDE_ENCODER_B_PIN)

#define RIGHT_ENCODER_INPUT_A  FL_GPIO_GetInputPin(RIGHT_SIDE_ENCODER_A_GPIO, RIGHT_SIDE_ENCODER_A_PIN)
#define RIGHT_ENCODER_INPUT_B  FL_GPIO_GetInputPin(RIGHT_SIDE_ENCODER_B_GPIO, RIGHT_SIDE_ENCODER_B_PIN)


#define MIDDLE_ENCODER_INPUT_A  FL_GPIO_GetInputPin(MIDDLE_SIDE_ENCODER_A_GPIO, MIDDLE_SIDE_ENCODER_A_PIN)
#define MIDDLE_ENCODER_INPUT_B  FL_GPIO_GetInputPin(MIDDLE_SIDE_ENCODER_B_GPIO, MIDDLE_SIDE_ENCODER_B_PIN)



typedef struct {
  unsigned char LastInput_A;
  unsigned char LastInput_B;
  unsigned char NewInput_A;
  unsigned char NewInput_B;
  unsigned short int Timer;
  unsigned char Event;
  unsigned char State;
}EncoderDrvType;


EncoderDrvType LeftEncoderDrv;
EncoderDrvType RightEncoderDrv;
EncoderDrvType MiddleEncoderDrv;

void EncoderDrv_Init(void)
{
  LeftEncoderDrv.LastInput_A = LEFT_ENCODER_INPUT_A;
  LeftEncoderDrv.NewInput_A  = LEFT_ENCODER_INPUT_A;
  LeftEncoderDrv.LastInput_B = LEFT_ENCODER_INPUT_B;
  LeftEncoderDrv.NewInput_B  = LEFT_ENCODER_INPUT_B;
  LeftEncoderDrv.Event       = ENCODER_EVENT_NULL;
  LeftEncoderDrv.Timer       = 0;


  RightEncoderDrv.LastInput_A = RIGHT_ENCODER_INPUT_A;
  RightEncoderDrv.NewInput_A  = RIGHT_ENCODER_INPUT_A;
  RightEncoderDrv.LastInput_B = RIGHT_ENCODER_INPUT_B;
  RightEncoderDrv.NewInput_B  = RIGHT_ENCODER_INPUT_B;
  RightEncoderDrv.Event       = ENCODER_EVENT_NULL;
  RightEncoderDrv.Timer       = 0;
  
   MiddleEncoderDrv.LastInput_A = MIDDLE_ENCODER_INPUT_A;
  MiddleEncoderDrv.NewInput_A  = MIDDLE_ENCODER_INPUT_A;
  MiddleEncoderDrv.LastInput_B = MIDDLE_ENCODER_INPUT_B;
  MiddleEncoderDrv.NewInput_B  = MIDDLE_ENCODER_INPUT_B;
  MiddleEncoderDrv.Event       = ENCODER_EVENT_NULL;
  MiddleEncoderDrv.Timer       = 0;

  if(LeftEncoderDrv.LastInput_A == LeftEncoderDrv.LastInput_B)
  {
    LeftEncoderDrv.State = ENCODER_STATE_IDLE;
  }
  else
  {
    LeftEncoderDrv.State = ENCODER_STATE_ERROR;
  }
/////////////////////////////////////////////////////////////////////////
  if(RightEncoderDrv.LastInput_A == RightEncoderDrv.LastInput_B)
  {
    RightEncoderDrv.State = ENCODER_STATE_IDLE;
  }
  else
  {
    RightEncoderDrv.State = ENCODER_STATE_ERROR;
  }
  //////////////////////////////////////////////////////////////////
  if(MiddleEncoderDrv.LastInput_A == MiddleEncoderDrv.LastInput_B)
  {
    MiddleEncoderDrv.State = ENCODER_STATE_IDLE;
  }
  else
  {
    MiddleEncoderDrv.State = ENCODER_STATE_ERROR;
  }
}


void EncoderDrvEventSet(EncoderDrvType *pEncoderDrv, uint8_t Event)
{
  pEncoderDrv->Event = Event;
}


uint8_t LeftEncoderDrvEventGet(void)
{
  uint8_t Event;

  Event = LeftEncoderDrv.Event;
  LeftEncoderDrv.Event = ENCODER_EVENT_NULL;

  return (Event);
}

uint8_t RightEncoderDrvEventGet(void)
{
  uint8_t Event;

  Event = RightEncoderDrv.Event;
  RightEncoderDrv.Event = ENCODER_EVENT_NULL;

  return (Event);
}


uint8_t MiddleEncoderDrvEventGet(void)
{
  uint8_t Event;

  Event = MiddleEncoderDrv.Event;
  MiddleEncoderDrv.Event = ENCODER_EVENT_NULL;

  return (Event);
}



void EncoderDrvProcess(EncoderDrvType *pEncoderDrv)
{
  switch(pEncoderDrv->State)
  {
    case ENCODER_STATE_IDLE:
      if((pEncoderDrv->LastInput_A != pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B == pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->State       = ENCODER_STATE_ROLL_DOWN;
        pEncoderDrv->LastInput_A = pEncoderDrv->NewInput_A;
      }
      else if((pEncoderDrv->LastInput_A == pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B != pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->State       = ENCODER_STATE_ROLL_UP;
        pEncoderDrv->LastInput_B = pEncoderDrv->NewInput_B;
      }
      else if((pEncoderDrv->LastInput_A != pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B != pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->State       = ENCODER_STATE_ERROR;
        pEncoderDrv->LastInput_B = pEncoderDrv->NewInput_B;       
        pEncoderDrv->LastInput_A = pEncoderDrv->NewInput_A; 
      }
      else
      {
        if(pEncoderDrv->LastInput_A != pEncoderDrv->LastInput_B)
        {
          pEncoderDrv->State      = ENCODER_STATE_ERROR;
        }
      }
      break;
    case ENCODER_STATE_ROLL_DOWN:
      if((pEncoderDrv->LastInput_A != pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B == pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->State       = ENCODER_STATE_ERROR;
        pEncoderDrv->LastInput_A = pEncoderDrv->NewInput_A;
      }
      else if((pEncoderDrv->LastInput_A == pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B != pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->LastInput_B = pEncoderDrv->NewInput_B;
        if(pEncoderDrv->LastInput_B == pEncoderDrv->LastInput_A)
        {
          EncoderDrvEventSet(pEncoderDrv, ENCODER_EVENT_ROLL_DOWN );
          pEncoderDrv->State     = ENCODER_STATE_IDLE;
        }
        else
        {
          pEncoderDrv->State     = ENCODER_STATE_ERROR;
        }
      }
      else if((pEncoderDrv->LastInput_A != pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B != pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->State       = ENCODER_STATE_ERROR;
        pEncoderDrv->LastInput_B = pEncoderDrv->NewInput_B;       
        pEncoderDrv->LastInput_A = pEncoderDrv->NewInput_A; 
      }
      else
      {
      }
      break;
    case ENCODER_STATE_ROLL_UP :
      if((pEncoderDrv->LastInput_A != pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B == pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->LastInput_A = pEncoderDrv->NewInput_A;
        if(pEncoderDrv->LastInput_B == pEncoderDrv->LastInput_A)
        {
          EncoderDrvEventSet(pEncoderDrv, ENCODER_EVENT_ROLL_UP);
          pEncoderDrv->State     = ENCODER_STATE_IDLE;
        }
        else
        {
          pEncoderDrv->State     = ENCODER_STATE_ERROR;
        }
      }
      else if((pEncoderDrv->LastInput_A == pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B != pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->State       = ENCODER_STATE_ERROR;
        pEncoderDrv->LastInput_A = pEncoderDrv->NewInput_A;
      }
      else if((pEncoderDrv->LastInput_A != pEncoderDrv->NewInput_A) && (pEncoderDrv->LastInput_B != pEncoderDrv->NewInput_B))
      {
        pEncoderDrv->State       = ENCODER_STATE_ERROR;
        pEncoderDrv->LastInput_B = pEncoderDrv->NewInput_B;       
        pEncoderDrv->LastInput_A = pEncoderDrv->NewInput_A; 
      }
      else
      {
      }
      break;
    case ENCODER_STATE_ERROR:
      pEncoderDrv->LastInput_B = pEncoderDrv->NewInput_B;       
      pEncoderDrv->LastInput_A = pEncoderDrv->NewInput_A;       
      if(pEncoderDrv->LastInput_B == pEncoderDrv->LastInput_A)
      {
        pEncoderDrv->State     = ENCODER_STATE_IDLE;
      }
      break;
    default:
      EncoderDrv_Init();
  }
}


void EncoderDrv_Task(void)
{
  LeftEncoderDrv.NewInput_A = LEFT_ENCODER_INPUT_A;
  LeftEncoderDrv.NewInput_B = LEFT_ENCODER_INPUT_B;
  EncoderDrvProcess(&LeftEncoderDrv);
  RightEncoderDrv.NewInput_A = RIGHT_ENCODER_INPUT_A;
  RightEncoderDrv.NewInput_B = RIGHT_ENCODER_INPUT_B;
  EncoderDrvProcess(&RightEncoderDrv);
  MiddleEncoderDrv.NewInput_A = MIDDLE_ENCODER_INPUT_A;
  MiddleEncoderDrv.NewInput_B = MIDDLE_ENCODER_INPUT_B;
  EncoderDrvProcess(&MiddleEncoderDrv); 
  
}