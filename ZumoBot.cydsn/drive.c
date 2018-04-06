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

#include "drive.h"
#include "Reflectance.h"
#include "Systick.h"
#include "custom_motor.h"

#include <math.h>
#include <stdio.h>

//Utils
float normalize(float value, float min, float max, float nmin, float nmax){
    float interval = max - min;
    float ninterval = nmax - nmin;
    float trimmed = value;
   
    if (trimmed > max)
        trimmed = max;
    
    if (trimmed < min)
        trimmed = min;
    
    float scaled = ((trimmed - min) / interval) * (ninterval) + nmin;
    
    return scaled;
}

void scalePairTo(float aim, float* value1, float* value2){
    float abs1 = fabs(*value1);
    float abs2 = fabs(*value2);
    float max = abs1 > abs2 ? abs1 : abs2;
    float ratio = aim / max;
    
    *value1 = *value1 * ratio;
    *value2 = *value2 * ratio;

}

float denoise(float signal, float threshold){   
    if (signal < threshold)
        return 0;
    else
        return signal;
}

void driveStart(DriveState* state, float reflectanceMin, float reflectanceMax){
    reflectance_start();
    state->reflectanceMin = reflectanceMin;
    state->reflectanceMax = reflectanceMax;
    
    state->current.l1 = 0;
    state->current.l2 = 0;
    state->current.l3 = 0;
    state->current.r1 = 0;
    state->current.r2 = 0;
    state->current.r3 = 0;
    state->current.time = 0;
    
    state->derivative.l1 = 0;
    state->derivative.l2 = 0;
    state->derivative.l3 = 0;
    state->derivative.r1 = 0;
    state->derivative.r2 = 0;
    state->derivative.r3 = 0;
    state->derivative.time = 0;
    
    state->integral.l1 = 0;
    state->integral.l2 = 0;
    state->integral.l3 = 0;
    state->integral.r1 = 0;
    state->integral.r2 = 0;
    state->integral.r3 = 0;
    state->integral.time = 0;
    
    cmotor_start();              // motor start
    
}

void driveFetchData(DriveState* state){
    
    struct sensors_ in_data;
    
    reflectance_read(&in_data);
    
    float min = state->reflectanceMin;
    float max = state->reflectanceMax;
    float r1, r2, r3, l1, l2, l3, time;
    
    r1 = normalize(in_data.r1, min, max, 0.0f, 1.0f);
    r2 = normalize(in_data.r2, min, max, 0.0f, 1.0f);
    r3 = normalize(in_data.r3, min, max, 0.0f, 1.0f);
    l1 = normalize(in_data.l1, min, max, 0.0f, 1.0f);
    l2 = normalize(in_data.l2, min, max, 0.0f, 1.0f);
    l3 = normalize(in_data.l3, min, max, 0.0f, 1.0f);
    time = GetTicks();
    
    //Compute derivative
    float dt = time - state->current.time;
    
    printf("dt: %f\n", dt);
    
    state->derivative.r1 = (r1 - state->current.r1) / (dt);
    state->derivative.r2 = (r2 - state->current.r2) / (dt);
    state->derivative.r3 = (r3 - state->current.r3) / (dt);
    state->derivative.l1 = (l1 - state->current.l1) / (dt);
    state->derivative.l2 = (l2 - state->current.l2) / (dt);
    state->derivative.l3 = (l3 - state->current.l3) / (dt);
    state->derivative.time = time;
    
    //Compute integral
    state->integral.r1 = state->integral.r1 + r1 * dt;
    state->integral.r2 = state->integral.r2 + r2 * dt;
    state->integral.r3 = state->integral.r3 + r3 * dt;
    state->integral.l1 = state->integral.l1 + l1 * dt;
    state->integral.l2 = state->integral.l2 + l2 * dt;
    state->integral.l3 = state->integral.l3 + l3 * dt;
    state->integral.time = time;
    
    //Update current
    state->current.r1 = r1;
    state->current.r2 = r2;
    state->current.r3 = r3;
    state->current.l1 = l1;
    state->current.l2 = l2;
    state->current.l3 = l3;
    state->current.time = time;
}

void driveResetIntegral(DriveState* state){
    state->integral.l1 = 0;
    state->integral.l2 = 0;
    state->integral.l3 = 0;
    state->integral.r1 = 0;
    state->integral.r2 = 0;
    state->integral.r3 = 0;
    state->integral.time = GetTicks();
}

void driveUpdateSpeed(DriveState* state, float maxSpeed, float rCoefficients[NCOEFF], float lCoefficients[NCOEFF]){
    
    float rightSpeed = 0.0f;
    float leftSpeed = 0.0f;
    
    
    rightSpeed += rCoefficients[0] * state->current.r1;
    rightSpeed += rCoefficients[1] * state->current.r2;
    rightSpeed += rCoefficients[2] * state->current.r3;
    rightSpeed += rCoefficients[3] * state->current.l1;
    rightSpeed += rCoefficients[4] * state->current.l2;
    rightSpeed += rCoefficients[5] * state->current.l3;
    
    rightSpeed += rCoefficients[6] * state->derivative.r1;
    rightSpeed += rCoefficients[7] * state->derivative.r2;
    rightSpeed += rCoefficients[8] * state->derivative.r3;
    rightSpeed += rCoefficients[9] * state->derivative.l1;
    rightSpeed += rCoefficients[10] * state->derivative.l2;
    rightSpeed += rCoefficients[11] * state->derivative.l3;
    
    rightSpeed += rCoefficients[12] * state->integral.r1;
    rightSpeed += rCoefficients[13] * state->integral.r2;
    rightSpeed += rCoefficients[14] * state->integral.r3;
    rightSpeed += rCoefficients[15] * state->integral.l1;
    rightSpeed += rCoefficients[16] * state->integral.l2;
    rightSpeed += rCoefficients[17] * state->integral.l3;
    
    
    leftSpeed += lCoefficients[0] * state->current.r1;
    leftSpeed += lCoefficients[1] * state->current.r2;
    leftSpeed += lCoefficients[2] * state->current.r3;
    leftSpeed += lCoefficients[3] * state->current.l1;
    leftSpeed += lCoefficients[4] * state->current.l2;
    leftSpeed += lCoefficients[5] * state->current.l3;
    
    leftSpeed += lCoefficients[6] * state->derivative.r1;
    leftSpeed += lCoefficients[7] * state->derivative.r2;
    leftSpeed += lCoefficients[8] * state->derivative.r3;
    leftSpeed += lCoefficients[9] * state->derivative.l1;
    leftSpeed += lCoefficients[10] * state->derivative.l2;
    leftSpeed += lCoefficients[11] * state->derivative.l3;
    
    leftSpeed += lCoefficients[12] * state->integral.r1;
    leftSpeed += lCoefficients[13] * state->integral.r2;
    leftSpeed += lCoefficients[14] * state->integral.r3;
    leftSpeed += lCoefficients[15] * state->integral.l1;
    leftSpeed += lCoefficients[16] * state->integral.l2;
    leftSpeed += lCoefficients[17] * state->integral.l3;
    

    float sum = fabs(leftSpeed) + fabs(rightSpeed);
    
    if (sum <= 0.01f){
        //Stop
        cmotor_speed(0, 0, maxSpeed);
        return;
    }
    
    float leftProportion = leftSpeed / sum;
    float rightProportion = rightSpeed / sum;
    
    scalePairTo(1.0f, &leftProportion, &rightProportion);
    
    uint8 right = (int) (rightProportion * 255);
    uint8 left = (int) (leftProportion * 255);
    
        
    cmotor_speed(leftProportion, rightProportion, maxSpeed);
}

void driveStop(){

       
    cmotor_stop();               // motor stop   
}


/* [] END OF FILE */
