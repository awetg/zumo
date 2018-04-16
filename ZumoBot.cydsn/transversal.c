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

#include "transversal.h"

int trCount = 0;
bool onTransversal = false;

//Takes the values of r3 and l3 and returns true if we are on a transversal line
bool transversalDetect(float r3, float l3){
    const float black = 1.0f;
    const bool bothOnBlack = r3 >= black && l3 >= black;
    
    //If we were on a transversal line but now the sensors read white...
    if (onTransversal && !bothOnBlack) {
        //... set that we are not on the line anymore
        onTransversal = false;
    } else if (!onTransversal && bothOnBlack) { //If we were not on a transversal line but both sensors are on black
        onTransversal = true;   
        trCount++;
    }
    
    return onTransversal;
}
    
//Takes the values of r3 and l3 and returns the number of transversal lines we passed
int transversalCount(float r3, float l3){
    transversalDetect(r3, l3);
    return trCount;
}
    
//Reset the count of transversal lines passed
void transversalReset(){
    trCount = 0;
}

/* [] END OF FILE */
