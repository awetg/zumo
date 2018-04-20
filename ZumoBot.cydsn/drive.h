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
    
//Set this to 0 to disable emergency turns (turns to handles sharp angles)
#define EMERGENCY_TURN 1
    
#define NCOEFF 18
    
#define NSENSORS 6
    
#define REF_R1 3
#define REF_R2 4
#define REF_R3 5
#define REF_L1 2
#define REF_L2 1
#define REF_L3 0
    

typedef struct drivestate {
    //Measurements (by default in range 0...1), and their derivative and integrals
    float current[NSENSORS];
    float derivative[NSENSORS];
    float integral[NSENSORS];
    
    //Keep track of the harsh turns, by using the far away sensors and remembering their sum value
    //for an amount of resetEmergencySumCounter cycles
    float emergencyTurnSum;
    int resetEmergencySumCounter;
    
    //Arrays of minimum and maximum reflectance values for the reflectance sensors
    float *reflectanceMin;
    float *reflectanceMax;
    
    //Time at which the measurements have been acquired
    float time;
    
    //Have the measurements been displaced? If so, by how much?
    //For example: if the values in current[] have a range -0.5...0.5 instead of 0...1, displ will be -0.5
    float displ; 
    
} DriveState;
    
//Start the motors and set the initial values for state
void driveStart(DriveState* state, float* reflectanceMin, float* reflectanceMax);

//Reset the temporary state of the drive, keeping the original initialization parameters
void driveReset(DriveState* state);

//Drive at fixed speed while the condition is true, with motor speeds being right*speed and left*speed
//At each cycle, the callback function is called, if not null
void driveFixedWhile(bool (* condition) (DriveState*), DriveState* state, float left, float right, float scale, void (*callback)());

//Drive with PID while the condition is true, with motor speeds being right*speed and left*speed
//At each cycle, the callback function is called, if not null
void driveWhile(bool (* condition) (DriveState*), DriveState* state, float speed, float* coefficients, void (*callback)());

//Return true if the data from all the sensors is zero
bool driveDataIsZero(DriveState* state);

//Fetch data from the reflectance sensors, and put them in range [0.0 + displ, 1.0 0 displ], storing
//them in state. Store also their integrals and derivatives fro each sensor.
void driveFetchData(DriveState* state, float displ);

//Use the data from the sensors hold in state to update the speed of the motors accordingly.
//state     -> the state of the sensors and motor controller
//maxSpeed  -> the speed at which the motors are going to run if in a straight line, in range [0.0, 1.0]
//aim       -> what the sum of sensor data weighted over coefficients should aim to be, usually aim = 0.0
//correctionSpeed -> the strength of the correction. The difference between the weighted sum and the aim will be multiplied by this value.
//coefficients -> the coefficients that weight each sensors. They are based on Kp, Kd and Ki for the PID
void driveUpdateSpeed(DriveState* state, float maxSpeed, float aim, float correctionStrength, float coefficients[NCOEFF]);

//Given a threshold, get a digital value (0 or 1) for the specified sensor
int getDigitalSensor(DriveState* state, int sensor, float threshold);

//Stop the motor
void driveStop();


//*********************************
//DEFAULT CONDITIONS FOR driveWhile
//*********************************

bool isStillOnTransversalLine(DriveState* state);
bool isNotYetOnTransversalLine(DriveState* state);


#endif

/* [] END OF FILE */
