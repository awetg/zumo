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

bool driveDataIsZero(DriveState* state){
    float totalDarkness = 0;
    
    for (int i = 0; i < NSENSORS; i++){
     
        /*if (i == REF_L2 || i == REF_R2)
            continue;*/
        
        totalDarkness += state->current[i] - state->displ;
        
    }
    
    return fabs(totalDarkness) < 0.01f;
    
}

void driveStart(DriveState* state, float* reflectanceMin, float* reflectanceMax){
    reflectance_start();
    state->reflectanceMin = reflectanceMin;
    state->reflectanceMax = reflectanceMax;
    state->emergencyTurnSum = 0.0f;
    state->filteredL2 = 0;
    state->filteredR2 = 0;
    state->prevL2 = 0;
    state-> prevR2 = 0;
    
    for (int i = 0; i < NSENSORS; i++){
        state->current[i] = 0;
        state->derivative[i] = 0;
        state->integral[i] = 0;
    }
    
    state->resetEmergencySumCounter = 0;
    state->time = GetTicks();
    state->displ = 0;
    
    cmotor_start();              // motor start
    
}

void driveFetchData(DriveState* state, float displ){
    
    struct sensors_ in_data;
    
    reflectance_read(&in_data);
    
    float data[NSENSORS], time;
    
    data[REF_R1] = displ + normalize(in_data.r1, state->reflectanceMin[REF_R1], state->reflectanceMax[REF_R1], 0.0f, 1.0f);
    data[REF_R2] = displ + normalize(in_data.r2, state->reflectanceMin[REF_R2], state->reflectanceMax[REF_R2], 0.0f, 1.0f);
    data[REF_R3] = displ + normalize(in_data.r3, state->reflectanceMin[REF_R3], state->reflectanceMax[REF_R3], 0.0f, 1.0f);
    data[REF_L1] = displ + normalize(in_data.l1, state->reflectanceMin[REF_L1], state->reflectanceMax[REF_L1], 0.0f, 1.0f);
    data[REF_L2] = displ + normalize(in_data.l2, state->reflectanceMin[REF_L2], state->reflectanceMax[REF_L2], 0.0f, 1.0f);
    data[REF_L3] = displ + normalize(in_data.l3, state->reflectanceMin[REF_L3], state->reflectanceMax[REF_L3], 0.0f, 1.0f);
    time = GetTicks();
    
    printf("%5d %5d %5d %5d %5d %5d\r\n", in_data.l3, in_data.l2, in_data.l1, in_data.r1, in_data.r2, in_data.r3);
    
    //Filter l2 and r2 in case of random errors
    if (data[REF_L2] - state->prevL2 > 0.2f && state->filteredL2 < 2){
        data[REF_L2] = 0;
        state->filteredL2++;
    } else {
        state->filteredL2=0;   
    }
    if (data[REF_R2] - state->prevR2 > 0.2f && state->filteredR2 < 2){
        data[REF_R2] = 0;
        state->filteredR2++;
    } else {
        state->filteredR2=0; 
    }
    
    //Compute derivative
    float dt = time - state->time;
    float derivatives[NSENSORS];
    
    printf("dt: %f\n", dt);
    
    //const float derivativeFilter = 0.9f;
    
    for (int i = 0; i < NSENSORS; i++){
        derivatives[i] = (data[i] - state->current[i]) / dt;
        
        //printf("filtered: %d\n", state->filtered);
        //printf("d[i]: %f\n", derivatives[i]);
        
        //if (state->filtered >= 1 && fabs(derivatives[i]) < derivativeFilter){
            state->derivative[i] = derivatives[i];
            state->integral[i] += data[i] * dt;
            state->current[i] = data[i];
        /*} else {
            state->filtered++;   
            return;
        }*/
    }
    state->displ = displ;
    state->time = time;

}

void driveResetIntegral(DriveState* state){
    
    for (int i = 0; i < NSENSORS; i++){
        state->integral[i] = 0;
    }

    state->time = GetTicks();
}

#if 0
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
    
        
    cmotor_speed(leftProportion, rightProportion, maxSpeed);
}
#endif

void driveUpdateSpeedOpt(DriveState* state, float maxSpeed, float aim, float correctionSpeed, float coefficients[NCOEFF]){
    
    float optvalue = 0.0f;

        
    for (int i = 0; i < NSENSORS; i++){
        
        /*if (i%NSENSORS == REF_R2)
            continue;*/
        
        optvalue += coefficients[i] * state->current[i];
        optvalue += coefficients[i+NSENSORS] * state->derivative[i];
        optvalue += coefficients[i+NSENSORS*2] * state->integral[i];
    }
    
    //Are we starting an emergency turn?
    const int resetEmergencySumAfter = 10;
    const float emergencyThreshold = 0.1f;
    const float black = 0.8f;
    const float emergencySumThreshold = 0.9f;
    const float l1 = state->current[REF_L1];
    const float r1 = state->current[REF_R1];
    const float l3 = state->current[REF_L3];
    const float r3 = state->current[REF_R3];
    const float totalDarkness = l1+l3+r1+r3;
    
    if (r3 > black && l3 > black){
        state->emergencyTurnSum = 0.0;
        
        //Signal horizontal line
        
    } else if (r3 > black && l3 < black){
        state->emergencyTurnSum -= 1.0f;
        state->resetEmergencySumCounter = 0;
    } else if (l3 > black && r3 < black){
        state->emergencyTurnSum += 1.0f;
        state->resetEmergencySumCounter = 0;
    } else {
        state->resetEmergencySumCounter++;
    }
    
    if (state->resetEmergencySumCounter > resetEmergencySumAfter){
        state->resetEmergencySumCounter = 0;
        state->emergencyTurnSum = 0.0;
    }
    
    
    //If all the sensors read almost white
    if (totalDarkness < emergencyThreshold && fabs(state->emergencyTurnSum) > emergencySumThreshold){
        if (state->emergencyTurnSum > 0.0f){
            cmotor_speed(-1, +1, 1.0);
        } else if (state->emergencyTurnSum < 0.0f) {
            cmotor_speed(+1, -1, 1.0);
        }
        state->emergencyTurnSum = 0;
        do {
            driveFetchData(state, state->displ); 
        } while (state->current[REF_L1] + state->current[REF_R1] < 0.5f);
    }
    
    if (driveDataIsZero(state)){
      cmotor_speed(0,0,0); 
      return;
    }
    
    float diff = optvalue - aim;
    float rightSpeed = 1.0f;
    float leftSpeed = 1.0f;
    
    if (diff > 0){
        rightSpeed = 1.0f - diff*correctionSpeed; //fmax(1.0f - diff * correctionSpeed, 0.0f);
    } else if (diff < 0) {
        leftSpeed = 1.0f + diff*correctionSpeed; //fmax(1.0f + diff * correctionSpeed, 0.0f);
    }
    
    /*
    printf("optvalue: %f\n", diff);
    printf("left: %f\n", leftSpeed);
    printf("right: %f\n", rightSpeed);
    */
    cmotor_speed(leftSpeed, rightSpeed, maxSpeed);
    
}

int getDigitalSensor(DriveState* state, int sensor, float threshold){
    float value = 0;
    
    value = state->current[sensor];
    
    return (value > threshold ? 1 : 0);
}

void driveStop(){

       
    cmotor_stop();               // motor stop   
}


/* [] END OF FILE */
