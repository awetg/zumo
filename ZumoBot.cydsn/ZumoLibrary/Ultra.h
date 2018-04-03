/**
 * @file    Ultra.h
 * @brief   Ultra sonic sensor header file
 * @details If you want to use Ultra sonic sensor methods, you need to include Ultra.h file.
*/
#ifndef ULTRA_H_
#define ULTRA_H_

void ultrasonic_handler(void);

// start ultrasonic sensor
void Ultra_Start();

// returns the distance in centimeters
int Ultra_GetDistance(void);

#endif
