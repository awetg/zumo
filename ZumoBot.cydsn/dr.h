


/* ========================================
 *
 * @file    dr.h
 * @brief   Line follower with PID controller header file
 * @details A set of mthods for line follower, this methods implement the PID controller. If you want to use drive methods, you need to include dr.h file.
 *
 * ========================================
*/





#ifndef DR_H
#define DR_H
#include "Reflectance.h"
/**
* @brief    sensor data for PID controller
* @details  raw value of Reflectance Sensor, time, proportional(error), and rate of of error over time(derivative)
*/
typedef struct {
   struct sensors_ s;
   float time, lastproportional, derivative;
}sensorData;
void startSensor(sensorData*);
float scale(float , float, float);
void drive(sensorData* d, float *min,float *max, float kp, float kd,float speedSccale, int *hrLineCount);
void gotoStartingLine( float speedScale);
#endif

/* [] END OF FILE */
