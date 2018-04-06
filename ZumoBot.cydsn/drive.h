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
    
    
#define NCOEFF 18
    
typedef struct reflectancedata {
    float r1, r2, r3;
    float l1, l2, l3;
    float time;
} ReflectanceData;

typedef struct drivestate {
    ReflectanceData current;
    ReflectanceData derivative;
    ReflectanceData integral;
    float reflectanceMin;
    float reflectanceMax;
} DriveState;
    
void driveResetIntegral(DriveState* state);
void driveStart(DriveState* state, float reflectanceMin, float reflectanceMax);
void driveFetchData(DriveState* state);
void driveUpdateSpeed(DriveState* state, float maxSpeed, float rCoefficients[NCOEFF], float lCoefficients[NCOEFF]);
void driveStop();

#endif

/* [] END OF FILE */
