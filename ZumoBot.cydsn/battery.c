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

#include <stdbool.h>
#include "Systick.h"
#include "ADC_Battery.h"
#include "ShieldLed.h"
#include "BatteryLed.h"
#include "Beep.h"

//State variable to keep count of the last time we measure the battery (so we can repeat the measurement after a certain time)
uint32_t lastBatteryTime = 0;
//State variable to keep count of the state of the battery sensing light, to alternate it
bool battery_lighton = false;

void initBattery(){
    battery_lighton = false;
    lastBatteryTime = 0; 
}

//Check the battery status
void checkBattery(uint32_t batteryMeasurementDelay, float minVoltage){
    //Parameters for the battery measurement
    const float divider_ratio = 1.5f;
    const float adc_max_voltage = 5.0f;
    const float bits_max = 4095.0f;
    
    //If enough time has passed from the previous measurement
    if (GetTicks() > lastBatteryTime + batteryMeasurementDelay){
        //Start the battery measurement
        ADC_Battery_StartConvert();
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // Wait to get ADC converted value
            const uint16 adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            
            
            //Convert value to Volts
            const float volts = (adcresult / bits_max) * (adc_max_voltage * divider_ratio);
            
            //Switch the battery sensing light, to signal that the code is correctly sensing the battery
            battery_lighton = !battery_lighton;
            ShieldLed_Write(battery_lighton?1:0);
            
            //If the voltage is critical
            if(volts<minVoltage)
            {
                BatteryLed_Write(1); // Switch critical battery led on
                Beep(300, 200); //Emit a short beep
            }
            
            //Record the last time the battery has been checked
            lastBatteryTime = GetTicks();
        }
    }
}

void checkBatteryWithDefaults(){
    checkBattery(5000, 4.2);   
}

/* [] END OF FILE */
