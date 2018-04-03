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
#include "Beep.h"

void Beep(uint32 length, uint8 pitch)
{
    uint8 cmp = pitch / 2;
    Buzzer_PWM_Start();
    Buzzer_PWM_WriteCompare(cmp);
    Buzzer_PWM_WritePeriod(pitch);
    CyDelay(length);
    Buzzer_PWM_Stop();
}

/* [] END OF FILE */
