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

//Given a value and a range [min, max], returns a value in range [nmin, nmax]
//trimming the result if it falls out of the range
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

//Drive at fixed speed while the condition is true, with motor speeds being right*speed and left*speed
void driveFixedWhile(bool (* condition) (DriveState*), DriveState* state, float left, float right, float speed, void (*callback)()){
    driveReset(state); //Discard the turning and PID data
    
    while (condition(state)){
        cmotor_speed(left, right, speed);
        
        if (callback != NULL)
            callback();
            
        CyDelay(1);
    }
    
    cmotor_speed(0, 0, 0);
}

//Drive with PID while the condition is true, with motor speeds being right*speed and left*speed
void driveWhile(bool (* condition) (DriveState*), DriveState* state, float speed, float* coefficients, void (*callback)()){
    driveReset(state); //Discard the turning and PID data
    driveFetchData(state, 0.0f); //Fetch the initial data (will be used to check the condition the first time)
    
    while (condition(state)){
        driveFetchData(state, 0.0f);
        driveUpdateSpeed(state, speed, 0.0f, 1.0f, coefficients);
        
        if (callback != NULL)
            callback();
        
        CyDelay(1);
    }
    
    cmotor_speed(0, 0, 0);
}

//Check if every sensor is zero
bool driveDataIsZero(DriveState* state){
    float totalDarkness = 0;
    
    for (int i = 0; i < NSENSORS; i++){
     
        /*if (i == REF_L2 || i == REF_R2) //Ignore L2 and R2
            continue;*/
        
        totalDarkness += state->current[i] - state->displ;
        
    }
    
    //Small threshold below which we consider the sensors sum value to be zero
    //To prevent commong floating point misbehaviour in equality comparison
    //For example: we use value < 0.01f instead of value == 0;
    const float almostZero = 0.01f;
    
    return fabs(totalDarkness) < almostZero;
    
}

void driveReset(DriveState* state){
    
    state->emergencyTurnSum = 0.0f;
    state->filteredL2 = 0;
    state->filteredR2 = 0;
    state->prevL2 = 0;
    state->prevR2 = 0;
    state->resetEmergencySumCounter = 0;
    state->time = GetTicks();
    state->displ = 0;
    
    //Set the sensor reading to 0, as none has been carried out yet
    for (int i = 0; i < NSENSORS; i++){
        state->current[i] = 0;
        state->derivative[i] = 0;
        state->integral[i] = 0;
    }
    
}

//Start the drive
void driveStart(DriveState* state, float* reflectanceMin, float* reflectanceMax){
    //Start the reflectance sensors readings
    reflectance_start();
    
    driveReset(state);
    
    //Initialize the state to its initial value
    state->reflectanceMin = reflectanceMin;
    state->reflectanceMax = reflectanceMax;
    
    CyDelay(5); //Wait for the sensors to initialize
    
    
    //Start the motor
    //Note: the custom_motor.h custom library allows for sharp turns
    cmotor_start();
    
}

void driveFetchData(DriveState* state, float displ){
    
    struct sensors_ in_data;
    
    //Read the reflectance data from the sensors
    reflectance_read(&in_data);
    
    float data[NSENSORS], time;
    
    //Normalize the data in range 0 to 1, and add the displacement if needed, to shif the range for example to -0.5f...0.5f
    //Note: displ is 0 when using multiple sensors. It can be useful to set it to -0.5f if using one sensor
    data[REF_R1] = displ + normalize(in_data.r1, state->reflectanceMin[REF_R1], state->reflectanceMax[REF_R1], 0.0f, 1.0f);
    data[REF_R2] = displ + normalize(in_data.r2, state->reflectanceMin[REF_R2], state->reflectanceMax[REF_R2], 0.0f, 1.0f);
    data[REF_R3] = displ + normalize(in_data.r3, state->reflectanceMin[REF_R3], state->reflectanceMax[REF_R3], 0.0f, 1.0f);
    data[REF_L1] = displ + normalize(in_data.l1, state->reflectanceMin[REF_L1], state->reflectanceMax[REF_L1], 0.0f, 1.0f);
    data[REF_L2] = displ + normalize(in_data.l2, state->reflectanceMin[REF_L2], state->reflectanceMax[REF_L2], 0.0f, 1.0f);
    data[REF_L3] = displ + normalize(in_data.l3, state->reflectanceMin[REF_L3], state->reflectanceMax[REF_L3], 0.0f, 1.0f);
    
    //Time of the reading, in milliseconds
    time = GetTicks();
    
    //Show the data for debug
    //printf("%5d %5d %5d %5d %5d %5d\r\n", in_data.l3, in_data.l2, in_data.l1, in_data.r1, in_data.r2, in_data.r3);
    
    
    //*****************************************
    //FILTER l2 and r2 in case of random errors (see hardware bug with L2 and R2 reflectance sensors)
    
    //If the difference between the last and the current reading is greater than this number, discard it
    const float errorThreshold = 0.2f; 
    
    //If more than maximumMeasurementsToDiscard measurements in a row have a great difference from before,
    //accept the new value as real
    const int maximumMeasurementsToDiscard = 2;
    
    if (data[REF_L2] - state->prevL2 > errorThreshold && state->filteredL2 < maximumMeasurementsToDiscard){
        data[REF_L2] = 0;
        state->filteredL2++; //Take count of the amount of times in a row we filtered
    } else {
        state->filteredL2=0; //No filtering this time, reset the counter
    }
    if (data[REF_R2] - state->prevR2 > errorThreshold && state->filteredR2 < maximumMeasurementsToDiscard){
        data[REF_R2] = 0;
        state->filteredR2++; //Take count of the amount of times in a row we filtered
    } else {
        state->filteredR2=0; //No filtering this time, reset the counter
    }
    
    //END FILTER for l2 and r2
    //*****************************************
    
    
    //Compute the time difference from the last reading
    float dt = time - state->time;
    
    //printf("dt: %f\n", dt);
    
    //Set the current measurement, and its derivative and integral
    for (int i = 0; i < NSENSORS; i++){
        if (dt == 0){
            state->derivative[i] = 0;
        } else { 
            state->derivative[i] = (data[i] - state->current[i]) / dt;
        }
        state->integral[i] += data[i] * dt;
        state->current[i] = data[i];
    }
    
    //If a displacement has been applied to the measurement range, take note of it
    state->displ = displ;
    
    //Set the time of the measurement
    state->time = time;

}


void driveUpdateSpeed(DriveState* state, float maxSpeed, float aim, float correctionStrength, float coefficients[NCOEFF]){
    
    //The weightedSum for the PID, meaning the position in the space of the model
    //The value is computed by multiplying each input data with its own coefficient and summing
    float weightedSum = 0.0f;
        
    for (int i = 0; i < NSENSORS; i++){
        //Weight each value, its derivative and its integral by their respective coefficients and sum thems    
        weightedSum += coefficients[i] * state->current[i];
        weightedSum += coefficients[i+NSENSORS] * state->derivative[i];
        weightedSum += coefficients[i+NSENSORS*2] * state->integral[i];
    }
    
    //***************************************
    // START OF EMERGENCY TURN HANDLING CODE
    //***************************************
    
    //Note: Since we monitor L3 and R3 for emergency turn, this code also deals with recognizing the horizontal signal lines 
    //      at the start and end of the track
    
    //This type of turn is executed to handle 90 degrees or more non smooth turns.
    //If all the sensors end up on white, and one of external sensors (R3 or L3) in the previous resetEmergencySumAfter 
    //number of cycles has read more black than the other, assume that the path sharply turn that way,
    //so execute a full speed turn in that direction until either R1 or L1 go back on path
    
    //Are we starting an emergency turn?
    const int resetEmergencySumAfter = 20; //Reset the sum value after resetEmergencySumAfter white readings
    const float emergencyThreshold = 0.1f; //If the sum of all the sensors is less than this value, assume you lost the track
    const float black = 0.8f; //Assume a black measurement from L3 or R3 if their value is greater than this value
    const float emergencySumThreshold = 0.9f; //If the emergency sum absolute value is greater than this, assume R3 or L3 read the correct path turn
    
    //Fetch the values of the sensor used in the emergency turn
    const float l1 = state->current[REF_L1];
    const float r1 = state->current[REF_R1];
    const float l3 = state->current[REF_L3];
    const float r3 = state->current[REF_R3];
    
    //Total darkness from the sensors. If less than emergencyThreshold, execute emergency turn.
    const float totalDarkness = l1+l3+r1+r3; 
    
    //Both R3 and L3 are reading black, meaning we meet a trasnversal line
    if (r3 > black && l3 > black){
        state->emergencyTurnSum = 0.0; //Reset the sum, since we are in a horizontal line, not in a turn        
    } else if (r3 > black && l3 < black){ //R3 only reads black
        state->emergencyTurnSum -= 1.0f; //Make the sum more negative, to signal that the road probably goes right
        state->resetEmergencySumCounter = 0; //Stop counting for reset, because we have a reading
    } else if (l3 > black && r3 < black){
        state->emergencyTurnSum += 1.0f; //Make the sum more negative, to signal that the road probably goes left
        state->resetEmergencySumCounter = 0; //Stop counting for reset, because we have a reading
    } else { 
        //If we are moving on the path and both L3 and R3 are white, meaning the path is straight,
        //then increase the counter so soon it will erase the memory (state->emergencyTurnSum) of the older sharp turns
        state->resetEmergencySumCounter++;
    }
    
    //If we have been on a straight path long enough
    if (state->resetEmergencySumCounter > resetEmergencySumAfter){
        //Reset the counter and the memory of sharp turns
        state->resetEmergencySumCounter = 0;
        state->emergencyTurnSum = 0.0;
    }
    

    //If all the sensors read almost white, we lost the path
    if (totalDarkness < emergencyThreshold && fabs(state->emergencyTurnSum) > emergencySumThreshold){
        //If there is memory of a recent sharp turn in the path, start in place rotation towards that direction
        if (state->emergencyTurnSum > 0.0f){
            cmotor_speed(-1, +1, 1.0); //left engine full backwards, right engine full forward, scale speed maximum (1.0)
        } else if (state->emergencyTurnSum < 0.0f) {
            cmotor_speed(+1, -1, 1.0); //left engine full backwards, right engine full forward, scale speed maximum (1.0)
        }
        
        //Reset the sharp turn memory
        state->emergencyTurnSum = 0;
        
        do { //Keep on rotating until you find the path again
            driveFetchData(state, state->displ); 
        } while (state->current[REF_L1] + state->current[REF_R1] < 0.5f);
    }
    
    //***************************************
    // END OF EMERGENCY TURN HANDLING CODE
    //***************************************
    
    //Check if we are completely on white, in which case stop the motors
    if (driveDataIsZero(state)){
      cmotor_speed(0,0,0); 
      return;
    }
    
    //Compute the difference between the weightedSum from the input, and the optimum wanted value aim
    //aim is usually 0.0f
    float diff = weightedSum - aim;
    
    float rightSpeed = 1.0f;
    float leftSpeed = 1.0f;
    
    //Compute the speed of the motors, starting from full speed and removing the difference*correctionSthrength
    //from one of the motor (according to the sign of the difference)
    if (diff > 0){
        rightSpeed = 1.0f - diff*correctionStrength;
    } else if (diff < 0) {
        leftSpeed = 1.0f + diff*correctionStrength;
    }
    
    //Set the speeds
    //Note: cmotor_speed from custom_motor.h takes care to convert the 0...1 range values to 0...255, 
    //      trimming in case they are out of range
    cmotor_speed(leftSpeed, rightSpeed, maxSpeed);
    
}

//Given a threshold, get a digital value (0 or 1) for the specified sensor
int getDigitalSensor(DriveState* state, int sensor, float threshold){
    float value = 0;
    
    value = state->current[sensor];
    
    return (value > threshold ? 1 : 0);
}

void driveStop(){
    //Stop the motor (cmotor_stop() from custom_motor.h)
    cmotor_stop();
}


//*********************************
//DEFAULT CONDITIONS FOR driveWhile
//*********************************


bool isStillOnTransversalLine(DriveState* state){
    const float l3 = state->current[REF_L3];
    const float r3 = state->current[REF_R3];
    const float dl3 = state->derivative[REF_L3];
    const float dr3 = state->derivative[REF_R3];
    
    const float white = 0.0f;
    
    //We are still on the line if l3 or r3 read any black or grey value 
    return (l3 > white || r3 > white) || (dl3 > 0.0f || dr3 > 0.0f);
}

bool isNotYetOnTransversalLine(DriveState* state){
    const float l3 = state->current[REF_L3];
    const float r3 = state->current[REF_R3];
    const float dl3 = state->derivative[REF_L3];
    const float dr3 = state->derivative[REF_R3];
    const float black = 1.0f;
    
    //We not yet on the line if either l3 or r3 read a value that is less than full black
    
    return !((l3 >= black && r3 >= black) && (dl3 <= 0.0f && dr3 <= 0.0f));
}


/* [] END OF FILE */
