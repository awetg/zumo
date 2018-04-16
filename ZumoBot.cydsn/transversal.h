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

#ifndef TRANSVERSAL_H
#define TRANSVERSAL_H
    
    
    
    
    //Takes the values of r3 and l3 and returns true if we are on a transversal line
    bool transversalDetect(float r3, float l3);
    
    //Takes the values of r3 and l3 and returns the number of transversal lines we passed
    int transversalCount(float r3, float l3);    
    
    //Reset the count of transversal lines passed
    void transversalReset();    

#endif

/* [] END OF FILE */
