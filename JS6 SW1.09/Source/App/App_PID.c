#include "App_PID.h"


static float fStepBuffer[PID_DELAY_STEPS];
/**
 * @brief PID控制器初始化
 * 
 * @param pid  PID控制器
 * @param Kp 比例
 * @param Ki 积分 
 * @param Kd 微分
 * @param max 输出最大限幅
 * @param min 输出最小限幅
 * @param integralmax 积分最大限幅 
 * @param integralmin 积分最小限幅 
 * @param integralrangehi 积分范围上限
 * @param integralrangelo 积分范围下限
 */
void PID_Init(PIDController *pid, float Kp, float Ki, float Kd, float max, float min, float integralmax, float integralmin,float integralrangehi,float integralrangelo)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->max = max;
    pid->min = min;
    pid->integralmax = integralmax;
    pid->integralmin = integralmin;
    pid->prev_error = 0.0;
    pid->integral = 0.0;
    pid->integralrangehi = integralrangehi;
    pid->integralrangelo = integralrangelo;
    pid->delay_buffer = fStepBuffer; 
    pid->buffer_index = 0;
    pid->delay_steps = PID_DELAY_STEPS;
    
}

float PID_Compute(PIDController *pid, float setpoint, float measured_value) 
{
    uint8_t u8inrange = 0;
    float error = setpoint - measured_value;

    //如果积分项超出范围，不再累积

    if(error <= pid->integralrangehi && error >= pid->integralrangelo)
    {
        if(error>8.0f)
        {
            pid->integral += error; // 积分项累积误差
        }        
        u8inrange = 1;
    }
    else
    {
        pid->integral = 0;
    }
    
    if (pid->integral > pid->integralmax) {
        pid->integral = pid->integralmax;
    } 
    else if (pid->integral < pid->integralmin) 
    {
        pid->integral = pid->integralmin;
    }


    float derivative = (error - pid->prev_error) ; // 误差变化率
    pid->prev_error = error;
    // PID控制器输出
    float output;
    if(u8inrange == 0)
    {
       output  = pid->Kp * error;// + pid->Ki * pid->integral + pid->Kd * derivative;
    }
    else  
    {
       output  = 0.3f * error + pid->Ki * pid->integral + pid->Kd * derivative;
    }
    
    if (output > pid->max) {
        output = pid->max;
    } else if (output < pid->min) {
        output = pid->min;
    }
    return output;
       // 处理延迟
 /*
    double delayed_output = pid->delay_buffer[pid->buffer_index];
    pid->delay_buffer[pid->buffer_index] = output;
    pid->buffer_index = (pid->buffer_index + 1) % pid->delay_steps;
    return delayed_output; 
*/
    
}

void PID_ResetIntegral(PIDController *pid) 
{    
    pid->integral = 0.0;
}