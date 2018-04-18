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
#define LEFT -1
#define FORWARD 1
#define REVERSE -1
#include "Reflectance.h"
enum State {search, attack, reverse, turnR, turnL};
void doState( enum State *state, int attackDistance, float speedScale);
void check_if_inRing(enum State *state, struct sensors_ *dig);
void checkForEnemy( int attackDistance, enum State *state);
void turn(int direction, int speedScale);
void driveSumo(int direction, int speedScale);
void searchEnemy(int speedScale);
void startTime();
#endif

/* [] END OF FILE */
