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
#include "sumo.h"
#include "Ultra.h"
#include "CyLib.h"
#include "custom_motor.h"
#include "Systick.h"
#include <stdlib.h>
#include <stdio.h>



/*
@check robbot is inside ring using refelectance sensor
*/
void check_if_inRing(enum State *state, struct sensors_ *dig)
{
    
    reflectance_digital(dig);
    
    int sumofsensors = dig->l1 + dig->l2 + dig->l3 + dig->r1 + dig->r2 + dig->r3;
     
    //check if any sensor is on black
    if(sumofsensors > 0 )
    {
        printf("N sensors: %d\n", sumofsensors);
        // if leftmost sensor is on black turn right
        if(dig->l3>0)
        {
            printf("L3 black\n");
            *state = TURN_R;
        }
        // if rightmost sensor is on black turn left
        else if ( dig->r3>0) 
        {
            printf("R3 black\n");
            *state = TURN_L;
        }
        else if(dig->r1 ==1 || dig->l1 ==1) // if middle sensor is on black go backward
        {
            *state = REVERSE;
        }
    }
}


/*
@execute each state of sumo robot
*/

void doState( enum State *state, int attackDistance, float speedScale, float *searchTime )
{   
    switch(*state)
    {
        case SEARCH:
            
            searchEnemy(speedScale, searchTime);
            checkForEnemy(attackDistance, state);
            ShieldLed_Write(0);
            
        break;
        
        case ATTACK:
            ShieldLed_Write(1);
            driveSumo(FORWARD, speedScale); // attack until reach end of ring
                        
        break;
        
        case TURN_L:
             ShieldLed_Write(0);
            turn(LEFT, speedScale);
            delayCallback(300); // turn duration
            driveSumo(FORWARD,speedScale);
            delayCallback(300); // forward duration
            checkForEnemy(attackDistance, state);
           
        
        break;
        
        case TURN_R:
            ShieldLed_Write(0);
            turn(RIGHT, speedScale);
            delayCallback(300); // turn duration
            driveSumo(FORWARD,speedScale);
            delayCallback(300); // forward duration
            checkForEnemy(attackDistance, state);
            
            
        break;
        
        case REVERSE:
        
            ShieldLed_Write(0);
            driveSumo(BACKWARD, speedScale);
            delayCallback(300); // reverse duration
            checkForEnemy(attackDistance, state);
            
            
        break;
    }
}

/*
@check if object exist inside given range using ultrasonic sensor
*/
void checkForEnemy( int attackDistance, enum State *state)
{
    int distance = Ultra_GetDistance(); 
    
    if(distance<attackDistance){
    *state = ATTACK;
    }
    else {
    *state = SEARCH;
    }
}

/*
@turn robot to right or left direction
*/

void turn(int direction, float speedScale)
{
    cmotor_speed(1 * direction, -1 * direction, speedScale);
}

/*
@drive robot in straight line to forward or backward direction
*/
void driveSumo(int direction, float speedScale)
{
    cmotor_speed(1 * direction, 1*direction, speedScale);
}


/*
@Do random search patterns when robot is in search state
*/
void searchEnemy(float speedScale, float *searchTime)
{
    if(GetTicks() > *searchTime)
    {
        int randomNumber = random() % 3;
        int randomTime = 500 + random() % 3000;
        
        *searchTime = GetTicks() + randomTime;
        
        if(randomNumber == 0)
        {
            driveSumo(FORWARD, speedScale);
            *searchTime = GetTicks() + randomTime / 3;
        }
        else if (randomNumber % 2 == 1)
        {
            turn(RIGHT,speedScale*0.3);
        }
        else
        {
            turn(LEFT,speedScale*0.3);
        }
        
    }
    
    

}

//Function pointer that will be called at every cycle during a delay
void (*delayCallbackFunc)() = NULL;

//Set the function that will be called at every cycle during a delay
void setDelayCallback(void (*callback)()){
    delayCallbackFunc = callback;
}

//Delay, but keep on calling a callback function during the delay
//The function to call is set by setDelayCallback
void delayCallback(int delay){
    uint32 end = GetTicks() + delay;
    while (GetTicks() < end){
        if (delayCallbackFunc != NULL)
         delayCallbackFunc();
    }
}

/* [] END OF FILE */
