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
#ifndef BATTERY_H
#define BATTERY_H

#include <stdint.h>

void initBattery();
void checkBattery(uint32_t batteryMeasurementDelay, float minVoltage);


#endif
/* [] END OF FILE */
