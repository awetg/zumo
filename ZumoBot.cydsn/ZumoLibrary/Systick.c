#include <stdbool.h>
#include "project.h"
#include "Ultra.h"
#include "Reflectance.h"

static volatile uint32_t systicks;
/* ISR prototype declaration */
CY_ISR_PROTO(USER_ISR); 
/* User ISR function definition */
CY_ISR(USER_ISR) {
    /* User ISR Code*/
    ++systicks;
    ultrasonic_handler();
    reflectance_handler();
}  

void Systick_Start(void)
{
    static bool init = false;
    if(!init) {
        init = true;
        // Map systick ISR to the user defined ISR. SysTick_IRQn is already defined in core_cm0.h file 
        CyIntSetSysVector((SysTick_IRQn + 16), USER_ISR);  
        // Enable Systick timer with desired period/number of ticks
        SysTick_Config(BCLK__BUS_CLK__HZ / 1000);  
    }
}

uint32_t GetTicks(void)
{
    return systicks;
}

