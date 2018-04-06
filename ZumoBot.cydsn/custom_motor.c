/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <math.h>
#include "custom_motor.h"



/**
* @brief    Starting motor sensors
* @details  
*/
void cmotor_start()
{
    PWM_Start();
}


/**
* @brief    Stopping motor sensors
* @details
*/
void cmotor_stop()
{
    PWM_Stop();
}

int getInRange(int speed, int min, int max){
    if (speed > max)
        speed = min;
    
    if (speed < min)
        speed = min;
    
    return speed;
}

float calibration = 0.0f;

void calibrate(float rightSkew){
    calibration = rightSkew;   
}

void cmotor_speed(float left, float right, float scale){
    int leftDir = left >= 0.0f ? 0 : 1;
    int rightDir = right >= 0.0f ? 0 : 1;
    uint8 leftSpeed = scale * getInRange(fabs(left * 255), MIN_SPEED, MAX_SPEED);
    uint8 rightSpeed = scale * getInRange(fabs(right * 255), MIN_SPEED, MAX_SPEED);
    
    MotorDirLeft_Write(leftDir);    
    MotorDirRight_Write(rightDir);   
    PWM_WriteCompare1(leftSpeed * (1.0f - calibration)); 
    PWM_WriteCompare2(rightSpeed * (1.0f + calibration)); 
}

/* [] END OF FILE */
