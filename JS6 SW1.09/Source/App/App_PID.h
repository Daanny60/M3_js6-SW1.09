#ifndef __APP_PID_H
#define __APP_PID_H
#include "types.h"

#define PID_DELAY_STEPS 3

typedef struct 
{
    float Kp; // 比例增益
    float Ki; // 积分增益
    float Kd; // 微分增益
    float max; // 输出限幅
    float min; // 输出限幅    
    float integralmax; // 积分限幅    
    float integralmin; // 积分限幅 
    float prev_error; // 上一次误差
    float integral; // 积分项
    float integralrangehi; // 积分控制范围上限
    float integralrangelo; // 积分控制范围下限
    float *delay_buffer; // 延迟队列
    int delay_steps; // 延迟步数
    int buffer_index; // 队列索引

} PIDController;


extern void PID_Init(PIDController *pid, float Kp, float Ki, float Kd, float max, float min, float integralmax, float integralmin,float integralrangehi,float integralrangelo);
extern float PID_Compute(PIDController *pid, float setpoint, float measured_value);
extern void PID_ResetIntegral(PIDController *pid);
#endif
