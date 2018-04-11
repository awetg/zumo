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

#ifndef HYBRID_H    
#define HYBRID_H
    
#include "drive.h"
    
#define HYBRID_STATES 3 
    
#define HYBRID_STRAIGHT 0
#define HYBRID_LEFT 1    
#define HYBRID_RIGHT 2
    
#define HYBRID_MODE_LINE 0
#define HYBRID_MODE_SOFT 1
#define HYBRID_MODE_HARD 2
#define HYBRID_MODE_PID 3
    
typedef struct hybridstate {
    DriveState* dstate;
    int hybState;
    float postCorrection;
} HybridState;

typedef struct hybridparams {
    float Kp, Td, Ti;
    float speed;
    float pidCorrection;
    float softThreshold, softCorrection;
    float hardThreshold, hardCorrection;
    
} HybridParams;

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

void hybridStart(HybridState* state, DriveState* dstate, float* reflectanceMin, float* reflectanceMax);
void hybridFetch(HybridState* state);
void hybridUpdate(HybridState* state, HybridParams* params);


/* [] END OF FILE */

    
#endif

/* [] END OF FILE */
