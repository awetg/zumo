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
        else
        {
            *s = reverse;
        }
    }
}

void doState( enum State *s)
{
      int distance = Ultra_GetDistance();
        printf("distance = %d\r\n", distance);
        CyDelay(200);
        
    switch(*s)
        {
            case search:
            break;
            
            case attack:
            break;
            
            case turnL:
            break;
            
            case turnR:
            break;
            
            case reverse:
            break;
        }
}

/* [] END OF FILE */
