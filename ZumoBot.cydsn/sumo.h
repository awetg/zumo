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

#ifndef SUMO_H
#define SUMO_H
#define RIGHT 1
#define LEFT (-1)
#define FORWARD 1
#define BACKWARD (-1)
#include "Reflectance.h"
enum State {SEARCH, ATTACK, REVERSE, TURN_R, TURN_L};
void doState( enum State *state, int attackDistance, float speedScale, float *searchTime);
void check_if_inRing(enum State *state, struct sensors_ *dig);
void checkForEnemy( int attackDistance, enum State *state);
void turn(int direction, float speedScale);
void driveSumo(int direction, float speedScale);
void searchEnemy(float speedScale, float *searchTime);
void startTime();

//Set the function that will be called at every cycle during a delay
void setDelayCallback(void (*callback)());

//Delay, but keep on calling a callback function during the delay
//The function to call is set by setDelayCallback
void delayCallback(int delay);
#endif

/* [] END OF FILE */
