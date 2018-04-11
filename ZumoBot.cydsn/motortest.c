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

/* [] END OF FILE */

#include "custom_motor.h"

void do_motortest(){
    /*
    CyDelay(2000);
    cmotor_start();
    cmotor_speed(1, -1, 1);
    CyDelay(182);
    cmotor_stop();*/
    CyDelay(2000);
    cmotor_start();
    cmotor_speed(1, 1, 0.3);
    CyDelay(3600);
    cmotor_stop();
    /*
    cmotor_start();
    cmotor_speed(1, -1, 1);
    CyDelay(182);
    cmotor_stop();
    
    cmotor_start();
    cmotor_speed(1, 1, 1);
    CyDelay(1100);
    cmotor_stop();
    
    cmotor_start();
    cmotor_speed(1, -1, 1);
    CyDelay(182);
    cmotor_stop();
    
    cmotor_start();
    cmotor_speed(1, 1, 1);
    CyDelay(1200);
    cmotor_stop();*/
}