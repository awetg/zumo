/**
 * @file    I2C_made.h
 * @brief   I2C header file
 * @details If you want to use I2C methods, Include I2C_made.h file.
*/
#include "project.h"
#include <stdio.h>


uint8 I2C_read(uint8 device_addr, uint8 Reg);
void I2C_write(uint8 device_addr, uint8 Reg, uint8 value);
uint16 convert_raw(uint8 L, uint8 H);
