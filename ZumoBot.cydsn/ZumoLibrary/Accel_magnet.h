/**
 * @file    Accel_magnet.h
 * @brief   Accelerometer and Magnetometer header file.
 * @details If you want to use Accelerometer methods, you need to include Accel_magnet.h file. Defining register address for basic setting up and reading sensor output values.
*/
#include <project.h>
#include <stdio.h>
#include <math.h>


uint16 value_convert_magnet(uint8 AXIS_H, uint8 AXIS_L);
void heading(double X_AXIS, double Y_AXIS);
void value_convert_accel(uint16 X_AXIS, uint16 Y_AXIS, uint16 Z_AXIS);


#define WHO_AM_I_ACCEL      0x0F
#define ACCEL_MAG_ADDR      0x1D
#define ACCEL_CTRL1_REG     0x20                
#define ACCEL_CTRL7_REG     0x26

#define OUT_X_L_M           0x08            // Magnetometer output
#define OUT_X_H_M           0x09
#define OUT_Y_L_M           0x0A
#define OUT_Y_H_M           0x0B
#define OUT_Z_L_M           0x0C
#define OUT_Z_H_M           0x0D

#define OUT_X_L_A           0x28            // Accelerometer output
#define OUT_X_H_A           0x29
#define OUT_Y_L_A           0x2A
#define OUT_Y_H_A           0x2B
#define OUT_Z_L_A           0x2C
#define OUT_Z_H_A           0x2D


