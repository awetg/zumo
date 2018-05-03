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


//Awet's personal version of the PID drive

#ifndef DR_H
#define DR_H
#include "Reflectance.h"
#include<stdbool.h>
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
