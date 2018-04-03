/**
 * @file Ambient.c
 * @brief basic methods of operating ambient light sensor. For more details, please refer to Ambient.h file. 
 * @details part number: APDS-9301
*/
#include "Ambient.h"
#include <math.h>


/**
* @brief    Get lux from raw value
* @details  get lux using values of Channel 0 and Channel 1
* @param    double Channel 0
* @param    double Channel 1
* @return   double return calculated value
*/
double getLux(double CH0, double CH1)
{
    double result = 0;
    double ChannelRatio = (double)CH1/CH0;
    
    if((0<ChannelRatio) && (ChannelRatio <= 0.5))
        result = (0.0304 * CH0) - (0.062 * CH0 * pow(ChannelRatio, 1.4));
    else if((0.5<ChannelRatio)&&(ChannelRatio <= 0.61))
        result = (0.0224 * CH0) - (0.031 * CH1);
    else if((0.61<ChannelRatio)&&(ChannelRatio <= 0.80))
        result = (0.0128 * CH0) - (0.0153 * CH1);
    else if((0.80<ChannelRatio)&&(ChannelRatio <= 1.30))
        result = (0.00146 * CH0) - (0.00112 * CH1);
    else 
        result = 0;

    return result;
}

#if 0
//Ambient light sensor//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    
    I2C_Start();
    
    I2C_write(0x29,0x80,0x00);          // set to power down
    I2C_write(0x29,0x80,0x03);          // set to power on
    
    for(;;)
    {    
        uint8 Data0Low,Data0High,Data1Low,Data1High;
        Data0Low = I2C_read(0x29,CH0_L);
        Data0High = I2C_read(0x29,CH0_H);
        Data1Low = I2C_read(0x29,CH1_L);
        Data1High = I2C_read(0x29,CH1_H);
        
        uint8 CH0, CH1;
        CH0 = convert_raw(Data0Low,Data0High);      // combine Data0
        CH1 = convert_raw(Data1Low,Data1High);      // combine Data1

        double Ch0 = CH0;
        double Ch1 = CH1;
        
        double data = 0;
        data = getLux(Ch0,Ch1);
        
        // If you want to print out data
        //printf("%lf\r\n",data);    
    }    
 }   
#endif


