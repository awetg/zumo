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

//Return the value trimmed if out of range [min, max]
int getInRange(int speed, int min, int max){
    if (speed > max)
        speed = max;
    
    if (speed < min)
        speed = min;
    
    return speed;
}

//Introduce a skew in the speeds of the motors if one engine runs significantly faster than the other
float calibration = 0.0f;
void cmotor_calibrate(float rightSkew){
    calibration = rightSkew;   
}

//Spin the motor with the values left*scale and right*scale.
//If speed*scale is -1, the motor will spin backward full speed.
//If speed*scale is 1, the motor will spin forward full speed.
//If speed*scale is out of range, if will be trimmed to the maximum or minimum value.
void cmotor_speed(float left, float right, float scale){
    int leftDir = left >= 0.0f ? 0 : 1;
    int rightDir = right >= 0.0f ? 0 : 1;
    uint8 leftSpeed = scale * getInRange(fabs(left * (calibration < 0 ? 1.0f + calibration : 1.0f) * 255), MIN_SPEED, MAX_SPEED);
    uint8 rightSpeed = scale * getInRange(fabs(right * (calibration > 0 ? 1.0f - calibration : 1.0f) * 255), MIN_SPEED, MAX_SPEED);
    
    MotorDirLeft_Write(leftDir);    
    MotorDirRight_Write(rightDir);   
    PWM_WriteCompare1(leftSpeed); 
    PWM_WriteCompare2(rightSpeed); 
}

/* [] END OF FILE */
