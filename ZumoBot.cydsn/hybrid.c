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

#include "Beep.h"
#include "hybrid.h"
#include "drive.h"
#include "custom_motor.h"

#define PRACTICAL_ZERO 0.01f

void hybridSetState(HybridState* state, int direction, int mode){
    state->hybState = mode * HYBRID_STATES + direction;
}

void hybridError(int code){
    cmotor_speed(0, 0, 0);
    cmotor_stop();
    
    Beep(1000, 200);
    CyDelay(1000);
    for (int i = 0; i < code; i++){
        Beep(200, 200);
        CyDelay(200);
    }
    
    CyDelay(10000);
}

void hybridNonPIDCorrect(HybridState *state, HybridParams *params){
    int direction = state->hybState % HYBRID_STATES;
    int mode = state->hybState / HYBRID_STATES;
    
    float amount = 0.0f;
    
    if (mode == HYBRID_MODE_SOFT){
        amount = params->softCorrection;
    } else if (mode == HYBRID_MODE_HARD){
        amount = params->hardCorrection;
    } else {
        amount = 0.0f; //No need to turn
    }
    
    float rightSpeed = 1.0f;
    float leftSpeed = 1.0f;
    
    if (direction == HYBRID_RIGHT){
        rightSpeed = 1.0f - amount;
    } else if (direction == HYBRID_LEFT) {
        leftSpeed = 1.0f - amount;
    } else {
        //No need to turn   
    }
    
    cmotor_speed(leftSpeed, rightSpeed, params->speed);
    
}

void hybridStart(HybridState* hstate, DriveState* dstate, float* reflectanceMin, float* reflectanceMax){
    hstate->dstate = dstate;
    driveStart(hstate->dstate, reflectanceMin, reflectanceMax);
    hstate->hybState = HYBRID_MODE_LINE * HYBRID_STATES + HYBRID_STRAIGHT;
    hstate->postCorrection = 0.0f;
}

void hybridFetch(HybridState* state){
    driveFetchData(state->dstate, 0.0f);
}


void hybridUpdate(HybridState* state, HybridParams* params){

    
    int direction = state->hybState % HYBRID_STATES;
    int mode = state->hybState / HYBRID_STATES;
   
    const float l1 = state->dstate->current[REF_L1];
    const float r1 = state->dstate->current[REF_R1];
    
    const bool r1_zero = r1 <= PRACTICAL_ZERO;
    const bool l1_zero = l1 <= PRACTICAL_ZERO;
    
    const bool r1_line = r1 >= params->softThreshold;
    const bool l1_line = l1 >= params->softThreshold;
    
    const bool r1_soft = r1 < params->softThreshold;
    const bool l1_soft = l1 < params->softThreshold;
    
    const bool r1_hard = false;// r1 < params->hardThreshold;
    const bool l1_hard = false;// l1 < params->hardThreshold;
    
    const bool r1_pid = r1_zero;
    const bool l1_pid = l1_zero;
    
    if (r1_zero && l1_zero){
        cmotor_speed(0, 0, 0);
        return;
    }
    
    if (mode != HYBRID_MODE_PID || direction == HYBRID_STRAIGHT){
        //Execute non-PID logic

        if (r1_pid){
            hybridSetState(state, HYBRID_LEFT, HYBRID_MODE_PID);
        } else if (l1_line && r1_hard){
            hybridSetState(state, HYBRID_LEFT, HYBRID_MODE_HARD);
        } else if (l1_line && r1_soft){
            hybridSetState(state, HYBRID_LEFT, HYBRID_MODE_SOFT);
        } else if (l1_line && r1_line) {
            hybridSetState(state, HYBRID_STRAIGHT, HYBRID_MODE_LINE);
        } else if (l1_pid){
            hybridSetState(state, HYBRID_RIGHT, HYBRID_MODE_PID);
        } else if (r1_line && l1_hard){
            hybridSetState(state, HYBRID_RIGHT, HYBRID_MODE_HARD);
        } else if (r1_line && l1_soft){
            hybridSetState(state, HYBRID_RIGHT, HYBRID_MODE_SOFT);
        } else if (r1_line && l1_line) {
            hybridSetState(state, HYBRID_STRAIGHT, HYBRID_MODE_LINE);
        } else { //This should not happen, stop the engine and signal an error (code 1)
            hybridError(1);
        }
        
        //Apply the appropriate correction for the currenta state
        hybridNonPIDCorrect(state, params);
        
    } else { //HYBRID_MODE_PID
        
        //Do we need to exit pid mode?
        bool exit = (direction == HYBRID_LEFT && r1_line) || (direction == HYBRID_RIGHT && l1_line);
        
        if (!exit){
        
            float coeffs[NSENSORS*3];
        
            for (int i = 0; i < NSENSORS*3; i++){
                coeffs[i] = 0;   
            }
        
            int sensor = direction == HYBRID_LEFT ? REF_L1 : REF_R1;
            int sign = direction == HYBRID_LEFT ? -1 : 1;
            
            //Proportional
            coeffs[sensor] = sign * params->Kp;
            //Derivative
            coeffs[NSENSORS*2 + sensor] = sign * params->Td;
            //Integral
            coeffs[NSENSORS*3 + sensor] = sign * params->Ti;
            
            driveUpdateSpeedOpt(state->dstate, params->speed, 0.0f,params->pidCorrection, coeffs);
        
        } else { //exit pid mode
            hybridSetState(state, HYBRID_STRAIGHT, HYBRID_MODE_LINE);
        }
    }
    

}


/* [] END OF FILE */
