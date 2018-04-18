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

int randomNumber = 0;
float time, searchDuration = 50;


void check_if_inRing(enum State *state, struct sensors_ *dig)
{
    int sumofsensors=0;
    
    reflectance_digital(dig);
    
    sumofsensors = dig->l1 + dig->l2 + dig->l3 + dig->r1 + dig->r2 + dig->r3;
     
    //check if any sensor is on black
    if(sumofsensors > 0 )
    {
        if(dig->l3>0)
        {
            *state = turnR;
        }
        else if ( dig->r3>0)
        {
            *state = turnL;
        }
        else if(dig->r1 ==1 || dig->l1 ==1)
        {
            *state = reverse;
        }
    }
}




void doState( enum State *state, int attackDistance, float speedScale )
{   
    switch(*state)
    {
        case search:
            
            searchEnemy(speedScale/2);
            checkForEnemy(attackDistance, state);
            
        break;
        
        case attack:
        
            driveSumo(FORWARD, speedScale);
            checkForEnemy(attackDistance, state);
            
        break;
        
        case turnL:
            turn(LEFT, speedScale);
            CyDelay(300); // turn duration
            driveSumo(FORWARD,speedScale);
            CyDelay(300); // forward duration
            checkForEnemy(attackDistance, state);
        
        break;
        
        case turnR:
            turn(RIGHT, speedScale);
            CyDelay(300); // turn duration
            driveSumo(FORWARD,speedScale);
            CyDelay(300); // forward duration
            checkForEnemy(attackDistance, state);
            
        break;
        
        case reverse:
        
            driveSumo(REVERSE, speedScale);
            CyDelay(300); // reverse duration
            checkForEnemy(attackDistance, state);
            
        break;
    }
}


void checkForEnemy( int attackDistance, enum State *state)
{
    int distance = Ultra_GetDistance(); 
    
    if(distance<attackDistance)
    *state = attack;
    else 
    *state = search;
}



void turn(int direction, int speedScale)
{
    cmotor_speed(1 * direction, -1 * direction, speedScale);
}


void driveSumo(int direction, int speedScale)
{
    cmotor_speed(1 * direction, 1*direction, speedScale);
}

void searchEnemy(int speedScale)
{
    if(GetTicks() - time > searchDuration)
    {
        randomNumber = random() % 5;
        time = GetTicks();
        searchDuration = (randomNumber * 100) + 300;
    }
    
    
    if(randomNumber == 0)
    {
        driveSumo(FORWARD, speedScale);
    }
    else if ( randomNumber % 2)
    {
        turn(RIGHT,speedScale);
    }
    else
    {
        turn(LEFT,speedScale);
    }
}


void startTime()
{
    time = GetTicks();
}
/* [] END OF FILE */
