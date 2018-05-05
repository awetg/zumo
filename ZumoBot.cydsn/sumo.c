

/* ========================================
 *
 * @file    sumo.c
 * @brief   Basic methods for Robot-sumo fight. For more details, please refer to sumo.h file. 
 *
 * ========================================
*/


#include "sumo.h"
#include "Ultra.h"
#include "CyLib.h"
#include "custom_motor.h"
#include "Systick.h"
#include<stdlib.h>







/*
@brief      Check if robbot is inside ring using refelectance sensor.
@details    The function check if any of the Refelectance sensors are reading black, and change robots state depending sensor values.
@param      enum State* state : state of robot
@param      struct sensors_* dig : to read digital values of Refelectance sensor
*/
void check_if_insideRing(enum State *state, struct sensors_ *dig)
{
    
    reflectance_digital(dig);
    
    int sumofsensors = dig->l1 + dig->l2 + dig->l3 + dig->r1 + dig->r2 + dig->r3;
     
    //check if any sensor is on black
    if(sumofsensors > 0 )
    {
        // if leftmost sensor is on black turn right
        if(dig->l3>0)
        {
            *state = TURN_R;
        }
        // if rightmost sensor is on black turn left
        else if ( dig->r3>0) 
        {
            *state = TURN_L;
        }
        else if(dig->r1 ==1 || dig->l1 ==1) // if middle sensor is on black go backward
        {
            *state = REVERSE;
        }
    }
}








/*
@brief      Execute each state of sumo robot.
@details    The method do a switch of robot's state, and do current state of robot.
@param      enum State* state : to read current state
@param      int attackDistance : if ultrasonic sensor pick up an object less or equal to this distance, robot will change to attack state
@param      float speedscale : speed scale of robot's motor, value is 0-1
@param      float* searchTime : time the robot will spend on each search behaviour
*/
void doState( enum State *state, int attackDistance, float speedScale, float *searchTime )
{   
    switch(*state)
    {
        case SEARCH:
            
            searchEnemy(speedScale, searchTime);
            checkForEnemy(attackDistance, state);
            
        break;
        
        case ATTACK:
            driveSumo(FORWARD, speedScale); // attack until end of ring is reached
                        
        break;
        
        case TURN_L:
            turn(LEFT, speedScale);
            delayCallback(300); // turn duration
            driveSumo(FORWARD,speedScale);
            delayCallback(300); // forward duration
            checkForEnemy(attackDistance, state);
           
        
        break;
        
        case TURN_R:
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
@brief      check if object exist on a given range using ultrasonic sensor.
@details    The method read ultrasonic sensor to see objects infront of the robot, and change state of robot if object is found with in given range.
@param      int attackDistance : if ultrasonic sensor pick up an object less or equal to this distance, robot will change to attack state
@param      enum State* state : to change current state of robot to attack if opponent is found
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
@brief     Turn robot to right or left direction.
@param     int direction : pre defined LEFT or RIGHT values
@param     float speedscale : speed scale the robot will turn, value is 0-1
*/
void turn(int direction, float speedScale)
{
    cmotor_speed(1 * direction, -1 * direction, speedScale);
}







/*
@brief      Drive robot in straight line to forward or backward direction.
@param     int direction : pre defined FORWARD or BACKWARD values
@param      float speedscale : speed scale of robot's motor, value is 0-1
*/
void driveSumo(int direction, float speedScale)
{
    cmotor_speed(1 * direction, 1*direction, speedScale);
}






/*
@brief      Do random search patterns when robot is in search state.
@details    The method do a random search behaviour for random amount of time.
@details    This search behaviour include go forward, turn left and turn right.
@param      float speedscale : speed scale of robot's motor, value is 0-1
@param      float* searchTime : time the robot will spend on each search behaviour
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





/*
@brief      Set the function that will be called at every cycle during a delay.
@param      void (*calback)() : a function to be set as delayCallbackFunc, this function will be call on delays
*/
void setDelayCallback(void (*callback)()){
    delayCallbackFunc = callback;
}






//Delay, but keep on calling a callback function during the delay
//The function to call is set by setDelayCallback
/*
@brief      Calls a function for given milliseconds of time.
@details    This method is used when a delay is needed, instead of waiting without performing any usefull operaion a previously set function can be called for a given milliseconds.
@param      int delay : amount of millisecond a function will be called continally
*/
void delayCallback(int delay){
    uint32 end = GetTicks() + delay;
    while (GetTicks() < end){
        if (delayCallbackFunc != NULL)
         delayCallbackFunc();
    }
}

/* [] END OF FILE */
