#ifndef SYSTICK_H_ 
#define SYSTICK_H_ 

#include <stdint.h>

/*  Start systick timer   */
void Systick_Start(void);

/* Read current number of systicks (milliseconds) since boot. 
 * Systicks are incremented every 1 ms (1000 ticks / second) */
uint32_t GetTicks(void);

#endif
