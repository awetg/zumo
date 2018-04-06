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

void calibrate(float rightSkew);
void cmotor_speed(float left, float right, float scale);


#endif