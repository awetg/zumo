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
#include "dr.h"
#include "Systick.h"
#include "custom_motor.h"
#include "transversal.h"

int count = 0; // for checking horizontal lines
/*
@brief start sensor data
*/
void startSensor(sensorData* d)
{
    d->s.l1 = 0;
    d->s.l2 = 0;
    d->s.l3 = 0;
    d->s.r1 = 0;
    d->s.r2 = 0;
    d->s.r3 = 0;
    d->time= 0;
    d->lastproportional= 0;
    d->derivative= 0;
    reflectance_start();
    CyDelay(2);
}


/*
@brief get data and drive
*/
void drive(sensorData* d, float *min,float *max, float kp, float kd, float speedScale) 
{
    float r1, r2, r3, l1, l2, l3, dt, time, proportional, newSpeed;
    
    reflectance_read(&d->s);
   
    // Scale sensor values from 0 - 1
    l1 = scale(d->s.l1,min[0],max[0]);
    l2 = scale(d->s.l2,min[1],max[1]);
    l3 = scale(d->s.l3,min[2],max[2]);
    r1 = scale(d->s.r1,min[3],max[3]);
    r2 = scale(d->s.r2,min[4],max[4]);
    r3 = scale(d->s.r3,min[5],max[5]);
    
    //check for horizontal lines
    if(l3+r3 >=2)
    {
        count = transversalCount(r3, l3);
        if(count >=3)
        {
            cmotor_speed(0, 0, 0);
            return;
        }
    }
    
    // calulate robot deviation from line
    proportional = (l1 * -kp) + (r1 * kp) +  (l3 * -kp*100) + (r3 * kp*100);
    
    
    // check if sensor l1 , l3, r1, r3 are on white , then stop or make turn
     if(l1+r1+l3+r3<0.01){
        
        // check if l2, r2 are black, if yes sharp turn
        if(l2+r2>0.0)
        {
            proportional = (l2 * -kp*100) + (r2 * kp*100); // sharp turn
        }
        else // if all are white stop
        {
        cmotor_speed(0,0,0);
        return;
        }
        
    }
    
    
    // calculate change of deviation from line over time
    time =GetTicks();
    dt =time - d->time;
    d->derivative = (proportional - d->lastproportional)/dt;
    
    // update time and proportional values
    d->time = time;
    d->lastproportional = proportional;
    
    
    //calculate deviation as speed
    newSpeed = proportional + d->derivative*kd ;
    
    
     if(newSpeed<=0)
        {
            cmotor_speed(1.0 + newSpeed, 1.0, speedScale);
        }
        else 
        {
            cmotor_speed(1.0, 1.0 - newSpeed, speedScale);
            
        }     
}


/*
@brief scale sensor data from 0 to 1
*/
float scale(float data, float min, float max)
{
    if(data<min)
    {
        data=min;
    }
    else if(data>max)
    {
        data = max;
    }
    
    //scale and return
    return (data -min )/(max - min);
}


/* [] END OF FILE */
