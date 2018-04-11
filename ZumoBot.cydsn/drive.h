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

#ifndef DRIVE_H
#define DRIVE_H
    
#include <stdbool.h>
#include <stdint.h>
    
#define NCOEFF 18
    
#define NSENSORS 6
#define SENS_R1 0
#define SENS_R2 1
#define SENS_R3 2
#define SENS_L1 3
#define SENS_L2 4
#define SENS_L3 5
    
#define REF_R1 3
#define REF_R2 4
#define REF_R3 5
#define REF_L1 2
#define REF_L2 1
#define REF_L3 0
    

typedef struct drivestate {
    float current[NSENSORS];
    float derivative[NSENSORS];
    float integral[NSENSORS];
    
    float emergencyTurnSum;
    int resetEmergencySumCounter;
    
    //Filter for r2 and l2
    float prevL2, prevR2;
    int filteredL2, filteredR2;
    
    float *reflectanceMin;
    float *reflectanceMax;
    float time;
    float displ; 
    
} DriveState;
    
void driveResetIntegral(DriveState* state);
void driveStart(DriveState* state, float* reflectanceMin, float* reflectanceMax);
bool driveDataIsZero(DriveState* state);
void driveFetchData(DriveState* state, float displ);
//void driveUpdateSpeed(DriveState* state, float maxSpeed, float rCoefficients[NCOEFF], float lCoefficients[NCOEFF]);
void driveUpdateSpeedOpt(DriveState* state, float maxSpeed, float aim, float correctionSpeed, float coefficients[NCOEFF]);
int getDigitalSensor(DriveState* state, int sensor, float threshold);
void driveStop();

#endif

/* [] END OF FILE */
