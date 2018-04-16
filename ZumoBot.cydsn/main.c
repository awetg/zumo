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
#include "motortest.h"
#include "battery.h"
#include "transversal.h"

int rread(void);

/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/


//Two sensors curve
#if  1
//battery level//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    IR_Start();
    ADC_Battery_Start(); 
    Systick_Start();      
    
    printf("\nBoot\n");
    
    

    
    //9th Beethoven, Fur Elise, Polyphonic chords
    /*
    play_music("2 C D D e F F e D C b b C D .D -C oC", 300);
    play_music("3 E =EeEeEBDC A =S 2 =CE 3 =A B =S 2 =E 3 =aB C 2 E", 500);   

    play_music_with_base("4 CACBCACBCACBCADB CACBCACBCACBCADB CACBCACBCACBCADB CACBCACBCACBCADB", 
                         "3 S C CS CS C CS C CS CS C CS  A AS AS A AS A AS AS A AS  F FS FS F FS F FS FS F FS  G GS GS G GS G GS GS G GS" , 200);

    */
    
    //Imperial march
    /* 
    play_music("1 A A A A 0-.F1 =C A 0-.F1 =C A", 500);
     
    play_music_with_base("2 -.A =S -.A =S -.A =S -.A =S 1-.F2 =C A 1-.F2 =C oA"
          , "1 A A A A a A a oA", 500);
    */
    
    BatteryLed_Write(0); // Switch battery led off 
    
    DriveState dstate;
    
    //Parameters of PID for the motors
    float speed = 0.3f;
    
    const float KdpRatio = 19.0;
    const float Kpe = 100.0;
    const float Kpm = 2.0;
    
    const float Kp = 1.0;
    const float Kd = KdpRatio;
    
    //Status of the track
    bool firstLineReached = false;
    bool startTriggered = false;
    bool trackEnded = false;
    bool paused = false;
    
    //Coefficients to pass to the motors, reflecting the PID parameters symmetrically
    float coefficients[NCOEFF] =
    {
        -Kpe, -Kpm, -Kp, Kp, Kpm, Kpe, //l3, l2, l1, r1, r2, r3
        0,0,-Kd,Kd,0,0,
        0,0,0.0f,0.0f,0,0
    };

    //Thresholds for the reflectance sensors
    
    //White track (without margin)
    //float thresholdMin[NSENSORS] = {4200, 4800, 4500, 4800, 5500, 7000};
    
    //White track (with margin)
    //float thresholdMin[NSENSORS] = {7000, 5500, 5000, 4500, 5000, 5000};
    
    //White table
    float thresholdMin[NSENSORS] = {11000, 9000, 8000, 7500, 8200, 8100};
   
    //Black with margin
    float thresholdMax[NSENSORS] = {20000, 20000, 20000, 20000, 20000, 20000};
    
    initBattery();
    
    //Start the motors and the PID
    driveStart(&dstate, thresholdMin, thresholdMax);
    
    for(;;)
    {
        //Check that the battery has more than 4.2 V every 5 seconds
        checkBattery(5000, 4.2);
        
        //Fetch the data from the reflectance sensor, in the range 0..1 + displ, where
        //displ = 0.0f in this case
        //Note: You can use displ=-0.5f to obtain a range -0.5f(white) to 0.5f(black)
        driveFetchData(&dstate, 0.0f);
        
        //Fetch the values of R3 and L3 for the transversal line check
        float r3 = dstate.current[REF_R3];
        float l3 = dstate.current[REF_L3];
        
        if (paused){
            //Set the speed to zero if paused
            cmotor_speed(0,0,0);
        } else {
            //Update the speed of the motors so that:
            //- speed represents the full speed
            //- one of the two motors is always at full speed
            //- the function value is computed from matrix product of coefficients and the sensor values
            //- the difference between the function value and the aim (0.0f in this case) is computed
            //- the difference is then multiplied by the correction speed (1.0f in this case)
            //- the value is subctracted from the right engine full speed if positive, or from the left engine full speed if negative
            driveUpdateSpeed(&dstate, speed, 0.0f, 1.0f, coefficients);
        }
        
        if (!firstLineReached){
            //If we reach the first transversal line, take note of it...
            firstLineReached = transversalDetect(r3, l3);
            //...and pause the robot
            paused = firstLineReached;
        } else if (!startTriggered && firstLineReached){
            IR_flush(); // clear IR receive buffer
            IR_wait(); // wait for IR command
            startTriggered = true;
            transversalReset(); //Bring back to zero the count of transversl lines passed
            speed = 1.0f; //Set full speed for the race
            cmotor_speed(1,1,speed); //Go straight for 50 ms to pass the first transversal line
            CyDelay(50);
            paused = false;
        } else if (!trackEnded && startTriggered){
            if (transversalCount(r3, l3) >= 2){
                cmotor_stop(); //Stop at the end, after reading two tranversal lines
                paused = true; 
            }
        }
    
        //1 ms delay. The totale execution time for a cycle is around 6 ms
        CyDelay(1);
        
    }
 }   
#endif


//Awet's PID
#if 0
#include "dr.h"
//reflectance//
int main()
{
    //struct sensors_ dig;
    //truct sensors_ threshhold;
    
    sensorData d;

    Systick_Start();

    CyGlobalIntEnable; 
    UART_1_Start();
    
    //float min[6] = {6000, 5500, 4800, 4200, 4800, 4500};
    float min[6] = {11000, 9000, 8000, 7500, 8200, 8100};
    float max[6] = {20000, 20000, 20000, 20000, 20000, 20000};
    //float max[6] = {23000, 23000, 23000, 23000, 23000, 23000};
    //float dif[6] = {17000, 17500, 18200, 18800, 18200, 18500};
    
    float speedScale= 1;
    
    float kp=1.2;
    float kd=19;
    
   
    startSensor(&d);
    cmotor_start();
    
    for(;;)
    {
        drive(&d, min, max, kp, kd, speedScale);
        
        CyDelay(1);
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
        
        CyDelay(20);
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
