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

void check_if_inRing(enum State *s, struct sensors_ *dig)
{
    int sumofsensors=0;
    
    reflectance_digital(dig);
    
    sumofsensors = dig->l1 + dig->l2 + dig->l3 + dig->r1 + dig->r2 + dig->r3;
     
    //check if any sensor is on black
    if(sumofsensors > 0 )
    {
        if(dig->l3>0)
        {
            *s = turnR;
        }
        else if ( dig->r3>0)
        {
            *s = turnL;
        }
        else if(dig->r1 ==1 || dig->l1 ==1)
        {
            *s = reverse;
        }
    }
}

void doState( enum State *s, int attackDistance, float speedScale )
{
      int distance = Ultra_GetDistance();
        printf("distance = %d\r\n", distance);
        
    switch(*s)
        {
            case search:
                if(distance<attackDistance)
                {
                    *s = attack;
                    return;
                }
                else
                {
                   cmotor_speed(-1,1, 0.7);
                }
            break;
            
            case attack:
            
                cmotor_speed(1,1,speedScale);
                
                if(distance> attackDistance)
                {
                    *s = search;
                }
            break;
            
            case turnL:
           
            cmotor_speed(-1, 1, speedScale);
            
            if(distance<attackDistance)
                {
                    *s = attack;
                }
                else
                {
                    *s = search;
                }
            
            break;
            
            case turnR:
                
                cmotor_speed(1, -1, speedScale);
                
                if(distance<attackDistance)
                {
                    *s = attack;
                }
                else
                {
                    *s = search;
                }
            
            break;
            
            case reverse:
            
                cmotor_speed(-1,-1, speedScale);
                
               
            
            break;
        }
}

/* [] END OF FILE */
