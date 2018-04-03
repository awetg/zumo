/**
 * @file    Accel.c
 * @brief   Basic methods for operating accelerometer. For more details, please refer to Accel_magnet.h file. 
 * @details part number: LSM303D (simultaneously used with magnetometer and included in Zumo shield)
*/
#include "Accel_magnet.h"

/**
* @brief    Converting raw value
* @details  Converting raw value
* @param    uint16 X_AXIS : X axis value
* @param    uint16 Y_AXIS : Y axis value
* @param    uint16 Z_AXIS : Z axis value
*/
void value_convert_accel(uint16 X_AXIS, uint16 Y_AXIS, uint16 Z_AXIS)
{
    double AccXangle, AccYangle;
   
    AccXangle = (float) (atan2(Y_AXIS, Z_AXIS)+M_PI) *180 / M_PI;
    AccYangle = (float) (atan2(Z_AXIS, X_AXIS)+M_PI) *180 / M_PI;
    
    (void) AccXangle; // just to get rid of warning about unused variable
    (void) AccYangle; // just to get rid of warning about unused variable
  //If you want to print out the value  
  //printf("%7.3f %7.3f \r\n", AccXangle, AccYangle);
}

#if 0
//accelerometer//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
  
    I2C_Start();
  
    uint8 X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A;
    int16 X_AXIS_A, Y_AXIS_A, Z_AXIS_A;
    
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x22);
    
    
    for(;;)
    {
        //print out accelerometer output
        X_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_A);
        X_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_A);
        X_AXIS_A = convert_raw(X_L_A, X_H_A);
        
        Y_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_A);
        Y_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_A);
        Y_AXIS_A = convert_raw(Y_L_A, Y_H_A);
        
        Z_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_A);
        Z_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_A);
        Z_AXIS_A = convert_raw(Z_L_A, Z_H_A);
        
        printf("ACCEL: %d %d %d %d %d %d \r\n", X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A);
        value_convert_accel(X_AXIS_A, Y_AXIS_A, Z_AXIS_A);
        printf("\n");
        
        CyDelay(50);
    }
}   
#endif



