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
#include <stdio.h>
#include "Ultra.h"
#include "CyLib.h"
#include "custom_motor.h"


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
            if(enemyFound(attackDistance))
            {
                *state = attack;
                return;
            }
            else
            {
               cmotor_speed(-1,1, speedScale/2);
            }
        break;
        
        case attack:
        
            cmotor_speed(1,1,speedScale);
            
            if(!enemyFound(attackDistance)) //if enemy not found, start search
                *state = search;
        break;
        
        case turnL:
            turn(LEFT, speedScale);
            if(enemyFound(attackDistance))
                *state = attack;
            else
                *state = search;
        
        break;
        
        case turnR:
            turn(RIGHT, speedScale);
            
            if(enemyFound(attackDistance))
                *state = attack;
            else
                *state = search;
        
        break;
        
        case reverse:
        
            cmotor_speed(-1,-1, speedScale);
            CyDelay(300);
            if(enemyFound(attackDistance))
                *state = attack;
            else
                *state = search;
            
        break;
    }
}


bool enemyFound( int attackDistance)
{
    int distance = Ultra_GetDistance();
    printf("distance = %d\r\n", distance); 
    
    if(distance<attackDistance && distance!=5)
    return true;
    else 
    return false;
}

void turn(int direction, int speedScale)
{
    //turn
    cmotor_speed(1 * direction, -1 * direction, speedScale);
    CyDelay(300);
    
    //go forward
    cmotor_speed(1, 1, speedScale);
    CyDelay(300);
}
/* [] END OF FILE */
