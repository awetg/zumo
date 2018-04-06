/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include <project.h>
#include <stdio.h>
#include "Systick.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "I2C_made.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "IR.h"
#include "Ambient.h"
#include "Beep.h"
#include <time.h>
#include <sys/time.h>
#include "music.h"
#include "custom_motor.h"
#include "drive.h"

int rread(void);

/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

#if 1
//battery level//
int main()
{
    bool lighton = false;
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    ADC_Battery_Start();        

    uint16 adcresult =0;
    float volts = 0.0;
    const float divider_ratio = 1.5f;
    const float adc_max_voltage = 5.0f;
    const float bits_max = 4095.0f;

    printf("\nBoot\n");
    
    struct sensors_ data;
    
 /*
    play_music("2 C D D e F F e D C b b C D .D -C oC", 300);
    play_music("3 E =EeEeEBDC A =S 2 =CE 3 =A B =S 2 =E 3 =aB C 2 E", 500);   

    play_music_with_base("4 CACBCACBCACBCADB CACBCACBCACBCADB CACBCACBCACBCADB CACBCACBCACBCADB", 
                         "3 S C CS CS C CS C CS CS C CS  A AS AS A AS A AS AS A AS  F FS FS F FS F FS FS F FS  G GS GS G GS G GS GS G GS" , 200);

*/
   
    //play_music("1 A A A A 0-.F1 =C A 0-.F1 =C A", 500);
     
    //play_music_with_base("2 -.A =S -.A =S -.A =S -.A =S 1-.F2 =C A 1-.F2 =C oA"
  //      , "1 A A A A a A a oA", 500);
    
    BatteryLed_Write(0); // Switch led off 
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    DriveState dstate;
    
    /*
    float lCoefficients[NCOEFF] =
    {
        1, 0.8, 2, //r1, r2, r3
        -1, -0.8, -2, //l1, l2, l3
        0, 0.0, 0, //derivatives r1, r2, r3
        -25, 0, 0, //derivatives l1, l2, l3
        0, 0, 0, //integrals r1, r2, r3
        0.001, 0, 0 //integrals l1, l2, l3
    };
    
    float rCoefficients[NCOEFF] =
    {
        -1, -0.8, -2, //r1, r2, r3
        1, 0.8, 2, //l1, l2, l3
        -25, 0, 0, //derivatives r1, r2, r3
        0, 0.00, 0,      //derivatives l1, l2, l3
        0.001, 0, 0, //integrals r1, r2, r3
        0, 0, 0 //integrals l1, l2, l3
    };
    */
    
float lCoefficients[NCOEFF] =
    {
        1, 0.8, 2, //r1, r2, r3
        0, -0.8, -2, //l1, l2, l3
        0, 0, 0, //derivatives r1, r2, r3
        0, 0, 0, //derivatives l1, l2, l3
        0, 0, 0, //integrals r1, r2, r3
        0, 0, 0 //integrals l1, l2, l3
    };
    
    float rCoefficients[NCOEFF] =
    {
        0, -0.8, -2, //r1, r2, r3
        1, 0.8, 2, //l1, l2, l3
        0, 0, 0, //derivatives r1, r2, r3
        0, 0, 0, //derivatives l1, l2, l3
        0, 0, 0, //integrals r1, r2, r3
        0, 0, 0 //integrals l1, l2, l3
    };
    
    
    
    driveStart(&dstate, 6000, 10000);
    //reflectance_start();
    
    int counter = 0;
    
    for(;;)
    {
        
        ADC_Battery_StartConvert();
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            
            volts = (adcresult / bits_max) * (adc_max_voltage * divider_ratio);
            
            //reflectance_read(&data);
            
            driveFetchData(&dstate);
            driveUpdateSpeed(&dstate, 0.6f, rCoefficients, lCoefficients);
            
            if (counter % 10 == 0)
                driveResetIntegral(&dstate);
                
            counter++;
            
            printf("L1: %f\n", dstate.current.l1);
            printf("L2: %f\n", dstate.current.l2);
            printf("L3: %f\n", dstate.current.l3);
            printf("R1: %f\n", dstate.current.r1);
            printf("R2: %f\n", dstate.current.r2);
            printf("R3: %f\n", dstate.current.r3);
            
            printf("DL1: %f\n", dstate.integral.l1);
            printf("DL2: %f\n", dstate.integral.l2);
            printf("DL3: %f\n", dstate.integral.l3);
            printf("DR1: %f\n", dstate.integral.r1);
            printf("DR2: %f\n", dstate.integral.r2);
            printf("DR3: %f\n", dstate.integral.r3);
            
            // Print both ADC results and converted value
            printf("%d %f\r\n",adcresult, volts);
            
            if(volts<4.2)
            {
                lighton = !lighton; 
                BatteryLed_Write(lighton?1:0); // Switch led on
                ShieldLed_Write(lighton?1:0);
                //Beep(1000, 150);
            }
        }
        CyDelay(20);
        
       
        
    }
 }   
#endif

#if 0
// button
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    for(;;)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            ShieldLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) CyDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif


#if 0
//ultrasonic sensor//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    Ultra_Start();                          // Ultra Sonic Start function
    while(1) {
        int d = Ultra_GetDistance();
        //If you want to print out the value  
        printf("distance = %d\r\n", d);
        CyDelay(200);
    }
}   
#endif


#if 0
//IR receiver//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    IR_wait(); // wait for IR command
    printf("IR command received\n");
    
    // print received IR pulses and their lengths
    for(;;)
    {
        if(IR_get(&IR_val)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
            //printf("%d %lu\r\n",IR_val & IR_SIGNAL_HIGH ? 1 : 0, (unsigned long) (IR_val & IR_SIGNAL_MASK));
        }
    }    
 }   
#endif


#if 0
//reflectance//
int main()
{
    struct sensors_ ref;
    struct sensors_ dig;

    Systick_Start();

    CyGlobalIntEnable; 
    UART_1_Start();
  
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    

    for(;;)
    {
        // read raw sensor values
        reflectance_read(&ref);
        printf("%5d %5d %5d %5d %5d %5d\r\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       // print out each period of reflectance sensors
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
        printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);        //print out 0 or 1 according to results of reflectance period
        
        CyDelay(200);
    }
}   
#endif


#if 0
//motor//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();

    motor_start();              // motor start

    motor_forward(100,2000);     // moving forward
    motor_turn(200,50,2000);     // turn
    motor_turn(50,200,2000);     // turn
    motor_backward(100,2000);    // movinb backward
       
    motor_stop();               // motor stop
    
    for(;;)
    {

    }
}
#endif


/* [] END OF FILE */
