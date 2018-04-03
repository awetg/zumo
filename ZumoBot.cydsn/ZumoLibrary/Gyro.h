/**
 * @file    Gyro.h
 * @brief   Gyroscope header file
 * @details If you want to use Gyroscope methods, you need to include Gyro.h file. Defining register address for reading gyroscope output values.
*/

#include "project.h"

uint16 value_convert_gyro(uint16 raw);

#define WHO_AM_I_GYRO       0x0F   
#define GYRO_ADDR           0x6B
#define GYRO_CTRL1_REG      0x20  
#define GYRO_CTRL4_REG      0x23
#define OUT_X_AXIS_L        0x28            // Gyroscope output
#define OUT_X_AXIS_H        0x29
#define OUT_Y_AXIS_L        0x2A
#define OUT_Y_AXIS_H        0x2B
#define OUT_Z_AXIS_L        0x2C
#define OUT_Z_AXIS_H        0x2D

