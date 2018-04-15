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
#include "Reflectance.h"
enum State {search, attack, reverse, turnR, turnL};
void doState( enum State *s, int attackDistance, float speedScale);
void check_if_inRing(enum State *s, struct sensors_ *dig);
#endif

/* [] END OF FILE */
