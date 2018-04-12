/**
 * @file    Motor.h
 * @brief   Motor header file
 * @details If you want to use Motor methods, Include Motor.h file.
*/
#ifndef CUSTOM_MOTOR_H_ 
#define CUSTOM_MOTOR_H_ 

#include "project.h"
    
#define MIN_SPEED 0
#define MAX_SPEED 255

void cmotor_start(); // start motor PWM timers
void cmotor_stop();  // stop motor PWM timers

//Introduce a skew in the speeds of the motors if one engine runs significantly faster than the other
void cmotor_calibrate(float rightSkew);

//Spin the motor with the values left*scale and right*scale.
//If speed*scale is -1, the motor will spin backward full speed.
//If speed*scale is 1, the motor will spin forward full speed.
//If speed*scale is out of range, if will be trimmed to the maximum or minimum value.
void cmotor_speed(float left, float right, float scale);


#endif