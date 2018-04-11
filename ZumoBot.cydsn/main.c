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

#include "motortest.h"
#include "hybrid.h"


#if 0
//battery level//
int main()
{
    bool lighton = false;
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    ADC_Battery_Start();        

    uint16 adcresult =0;
    const float divider_ratio = 1.5f;
    const float adc_max_voltage = 5.0f;
    const float bits_max = 4095.0f;

    printf("\nBoot\n");
    
    //cmotor_calibrate(-0.01);
    //do_motortest();
    
    //for(;;){}
    
    //struct sensors_ data;
    
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
    
    //Two sensors working values
    /*float coefficients[NCOEFF] =
    {
        0.0f, 0, -0.8f, 0.8f, 0, 0.0f, //l3, l2, l1, r1, r2, r3
        0,0,-9.0f,9.0f,0,0,
        0,0,0.0f,0.0f,0,0
    };*/
    //NOTE: Speed 1, correction=1.5
    //**********************
    
    float coefficients[NCOEFF] =
    {
        0.0f, 0, -0.5f, 0, 0, 0.0f, //l3, l2, l1, r1, r2, r3
        0, 0,0, 9.0f,0.0f, 0, //derivatives
        0,0,0.0f,0.0f,0,0
    };

    //float thresholdMin[NSENSORS] = {4200, 4800, 4500, 4800, 5500, 7000};
    
    //White track
    //float thresholdMin[NSENSORS] = {7000, 5500, 5000, 4500, 5000, 5000};
    
    //White table
    float thresholdMin[NSENSORS] = {11000, 9000, 8000, 7500, 8200, 8100};
    
    
    float thresholdMax[NSENSORS] = {20000, 20000, 20000, 20000, 20000, 20000};
    
    driveStart(&dstate, thresholdMin, thresholdMax);
    //reflectance_start();
    
    //int counter = 0;
    const uint32_t batteryMeasurementDelay = 5000;
    uint32_t lastBatteryTime = 0;
    
    for(;;)
    {
        
        if (GetTicks() > lastBatteryTime + batteryMeasurementDelay){
            ADC_Battery_StartConvert();
            if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
                adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
                // convert value to Volts
                // you need to implement the conversion
                
                const float volts = (adcresult / bits_max) * (adc_max_voltage * divider_ratio);
                
                lighton = !lighton;
                ShieldLed_Write(lighton?1:0);
                
                if(volts<4.2)
                { 
                    BatteryLed_Write(1); // Switch led on
                    Beep(500, 200);
                }
                
                lastBatteryTime = GetTicks();
            }
        }
        
        //reflectance_read(&data);
        
        driveFetchData(&dstate, -0.5f);
        
        float speed = 1.0f;
        //float correction = fmax(speed * 20, 1.0f);
        
        driveUpdateSpeedOpt(&dstate, speed, 0.0f, 6.0f, coefficients);
    
        CyDelay(1);
        
       
        
    }
 }   
#endif

//Two sensors curve
#if  1
//battery level//
int main()
{
    bool lighton = false;
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    ADC_Battery_Start();        

    uint16 adcresult =0;
    const float divider_ratio = 1.5f;
    const float adc_max_voltage = 5.0f;
    const float bits_max = 4095.0f;

    printf("\nBoot\n");
    
    
    //cmotor_calibrate(-0.01);
    //do_motortest();
    
    //for(;;){}
    
    //struct sensors_ data;
    
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
    
    float KdpRatio = 19.0;
    float Kpe = 100.0;
    float Kpm = 1.0;
    
    float Kp = 1.0;
    float Kd = KdpRatio;
    
    //Two sensors working values
    float coefficients[NCOEFF] =
    {
        -Kpe, -Kpm, -Kp, Kp, Kpm, Kpe, //l3, l2, l1, r1, r2, r3
        0,0,-Kd,Kd,0,0,
        0,0,0.0f,0.0f,0,0
    };
    //NOTE: Speed 1, correction=1.5
    //**********************
    
    /*float coefficients[NCOEFF] =
    {
        0.0f, 0, -0.8f, 0.8f, 0, 0.0f, //l3, l2, l1, r1, r2, r3
        0, 0,2.375, 2.375, 0, 0, //derivatives
        0,0,0.0f,0.0f,0,0
    };*/

    //float thresholdMin[NSENSORS] = {4200, 4800, 4500, 4800, 5500, 7000};
    
    //White track
    //float thresholdMin[NSENSORS] = {7000, 5500, 5000, 4500, 5000, 5000};
    
    //White table
    float thresholdMin[NSENSORS] = {11000, 9000, 8000, 7500, 8200, 8100};
    
    
    float thresholdMax[NSENSORS] = {20000, 20000, 20000, 20000, 20000, 20000};
    
    driveStart(&dstate, thresholdMin, thresholdMax);
    //reflectance_start();
    
    //int counter = 0;
    const uint32_t batteryMeasurementDelay = 5000;
    uint32_t lastBatteryTime = 0;
    
    for(;;)
    {
        
        if (GetTicks() > lastBatteryTime + batteryMeasurementDelay){
            ADC_Battery_StartConvert();
            if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
                adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
                // convert value to Volts
                // you need to implement the conversion
                
                const float volts = (adcresult / bits_max) * (adc_max_voltage * divider_ratio);
                
                lighton = !lighton;
                ShieldLed_Write(lighton?1:0);
                
                if(volts<4.2)
                { 
                    BatteryLed_Write(1); // Switch led on
                    Beep(500, 200);
                }
                
                lastBatteryTime = GetTicks();
            }
        }
        
        //reflectance_read(&data);
        
        driveFetchData(&dstate, 0.0f);
        
        float speed = 1.0f;
        //float correction = fmax(speed * 20, 1.0f);
        
        driveUpdateSpeedOpt(&dstate, speed, 0.0f, 1.0f, coefficients);
    
        CyDelay(1);
        
       
        
    }
 }   
#endif

//Hybrid mode
#if 0
//battery level//
int main()
{
    bool lighton = false;
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    ADC_Battery_Start();        

    uint16 adcresult =0;
    const float divider_ratio = 1.5f;
    const float adc_max_voltage = 5.0f;
    const float bits_max = 4095.0f;

    printf("\nBoot\n");
    
    
    BatteryLed_Write(0); // Switch led off 
    
    DriveState dstate;
    HybridState hstate;
    HybridParams hparams;
    
    hparams.speed = 0.5f;
    
    hparams.Kp = 0.8f;
    hparams.Td = 2.375f;
    hparams.Ti = 0;
    hparams.pidCorrection = 0.5f;
    
    hparams.softThreshold = 0.6f;
    hparams.hardThreshold = 0.6f;
    hparams.softCorrection = 0.30f;
    hparams.hardCorrection = 0.60f;
    

    //float thresholdMin[NSENSORS] = {4200, 4800, 4500, 4800, 5500, 7000};
    
    //White track
    //float thresholdMin[NSENSORS] = {7000, 5500, 5000, 4500, 5000, 5000};
    
    //White table
    float thresholdMin[NSENSORS] = {11000, 9000, 8000, 7500, 8200, 8100};
    
    
    float thresholdMax[NSENSORS] = {20000, 20000, 20000, 20000, 20000, 20000};
    
    //driveStart(&dstate, thresholdMin, thresholdMax);
    hybridStart(&hstate, &dstate, thresholdMin, thresholdMax);

    const uint32_t batteryMeasurementDelay = 5000;
    uint32_t lastBatteryTime = 0;
    
    for(;;)
    {
        
        if (GetTicks() > lastBatteryTime + batteryMeasurementDelay){
            ADC_Battery_StartConvert();
            if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
                adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
                // convert value to Volts
                // you need to implement the conversion
                
                const float volts = (adcresult / bits_max) * (adc_max_voltage * divider_ratio);
                
                lighton = !lighton;
                ShieldLed_Write(lighton?1:0);
                
                if(volts<4.2)
                { 
                    BatteryLed_Write(1); // Switch led on
                    Beep(500, 200);
                }
                
                lastBatteryTime = GetTicks();
            }
        }
        
        //reflectance_read(&data);
        
        //driveFetchData(&dstate, 0.0f);
        hybridFetch(&hstate);
        
        float speed = 0.5f;
        float correction = fmax(speed * 20, 1.0f);
        
        //driveUpdateSpeedOpt(&dstate, speed, 0.0f, 1.5f, coefficients);
        hybridUpdate(&hstate, &hparams);
        
        /*if (counter % integralPeriod == 0)
            driveResetIntegral(&dstate);
            
        counter++;*/
        /*
        printf("L1: %f\n", dstate.current[REF_L1]);
        printf("L2: %f\n", dstate.current[REF_L2]);
        printf("L3: %f\n", dstate.current[REF_L3]);
        printf("R1: %f\n", dstate.current[REF_R1]);
        printf("R2: %f\n", dstate.current[REF_R2]);
        printf("R3: %f\n", dstate.current[REF_R3]);
        */
        // Print both ADC results and converted value
        //printf("%d %f\r\n", adcresult, volts);
        

    
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


/* [] END OF FILE */
