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
#include "pdm/track_bbb.h"
#include "pdm/trackmarch.h"

#define MAIN_PID 0
#define ALT_PID  1
#define SUMO     2

#define ROBOT_MODE SUMO

int rread(void);

/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/


//Callbacks and conditions for driving loops, used both in race and sumo.

//To be called at every cycle. Performs battery check and plays the previously selected music.
void checkBatteryWithDefaultsAndMusic(){
    checkBatteryWithDefaults();
    play_music_async();
}
    
//Check if the end of track has been reached, and update the count of transversal lines passed.
bool endOfTrackNotReached(DriveState* state){        
    float r3 = state->current[REF_R3];
    float l3 = state->current[REF_L3];
       
    return transversalCount(r3, l3) < 2;
}


//PID main
#if ROBOT_MODE == MAIN_PID

int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    IR_Start();
    ADC_Battery_Start(); 
    Systick_Start();      
    
    printf("\nBoot\n");
    
    //Slow down one of the two motors if it is normally faster than the other (small corrections not necessary with PID)
    //cmotor_calibrate(0.03f);

    //Musics to choose from:
    
    //9th Beethoven, Fur Elise, Polyphonic chords
    
    /*
    play_music("2 C D D e F F e D C b b C D .D -C oC", 300);
    
    
    play_music("3 E =EeEeEBDC A =S 2 =CE 3 =A B =S 2 =E 3 =aB C 2 E", 500);   

    play_music_with_base("4 CACBCACBCACBCADB CACBCACBCACBCADB CACBCACBCACBCADB CACBCACBCACBCADB", 
                         "3 S C CS CS C CS C CS CS C CS  A AS AS A AS A AS AS A AS  F FS FS F FS F FS FS F FS  G GS GS G GS G GS GS G GS" , 200);
    
    //Imperial march
    
    play_music("1 A A A A 0-.F1 =C A 0-.F1 =C A", 500);
     
    play_music_with_base("2 -.A =S -.A =S -.A =S -.A =S 1-.F2 =C A 1-.F2 =C oA"
          , "1 A A A A a A a oA", 500);
    */

    BatteryLed_Write(0); // Switch battery led off 
    
    DriveState dstate;
    
    //Initial speed to reach the starting line
    const float walkSpeed = 0.3f;
    
    //Parameters of PID for the motors
    const float speed = 1.0f;
    
    const float KdpRatio = 19.0;
    const float Kpe = 1.5;
    const float Kpm = 1.2;
    const float Kp = 1.0;
    const float Kd = KdpRatio * Kp;// KdpRatio*Kp;
    const float Kpd = 0;// KdpRatio*Kp;
    
    //Coefficients to pass to the motors, reflecting the PID parameters symmetrically
    float walkCoefficients[NCOEFF] =
    {
        0, -Kpm, -Kp, Kp, Kpm, 0, //l3, l2, l1, r1, r2, r3
        0,-Kpd,-Kd,Kd,Kpd,0,
        0,0,0.0f,0.0f,0,0
    };

    
    //Coefficients to pass to the motors, reflecting the PID parameters symmetrically
    float coefficients[NCOEFF] =
    {
        -Kpe, -Kpm, -Kp, Kp, Kpm, Kpe, //l3, l2, l1, r1, r2, r3
        0,-Kpd,-Kd,Kd,Kpd,0,
        0,0,0.0f,0.0f,0,0
    };

    //Thresholds for the reflectance sensors
    
    //White track (without margin)
    //float thresholdMin[NSENSORS] = {4200, 4800, 4500, 4800, 5500, 7000};
    
    //White track (with margin)
    //float thresholdMin[NSENSORS] = {7000, 5500, 5000, 4500, 5000, 5000};
    
    //White table
    float thresholdMin[NSENSORS] = {11000, 9000, 8000, 7500, 8200, 8100};
    //float thresholdMin[NSENSORS] = {13000, 11000, 9000, 9500, 10200, 10100};
   
    //Black with margin
    float thresholdMax[NSENSORS] = {20000, 20000, 20000, 20000, 20000, 20000};
    
    //Set the music to play during the race (Pirates of Caribbean)
    set_music_async("3 -SAAACDSD -SDEFSF -SFGESE -SDCCD -.S -SACDSD -SDEFSF -SFGESE -SDCD S -SACDSD -SDFGSG -SG 4 -AbSb -S -A3G4A3D -.S -SDEFSFSGS4A3D -.S -SDFESESFD E -S     -SGG 4-ASASAS -b -.A oS 3-GSGSGS -G 4-.A oS 4-ASASAS -b -.A oS -3GSFSESDS oS   4A oS b 3-bDF4b -4ASASASA3G oS G -2G3bDG F -2C3ADF -ESFSESDS oS", 250);
    
    //Init the battery measuring procedures
    initBattery();
    
    //Check the battery in the beginning
    checkBattery(0, 4.2); //Must be > 4.2V
    
    //Play the initial music
    PlayPDM((uint16*)trackmarch, MARCH_SIZE);
    
    //Start the motors and the PID
    driveStart(&dstate, thresholdMin, thresholdMax);
    
    //Drive until the first transversal line is not reached
    driveWhile(isNotYetOnTransversalLine, &dstate, walkSpeed, walkCoefficients, checkBatteryWithDefaults);
    
    //Play "Bad Bad Boys" recording
    PlayPDM((uint16*)trackbbb, BBB_SIZE);

    IR_flush(); // Clear IR receive buffer
    IR_wait(); // Wait for IR command
    
    transversalReset(); //Reset the count of transversal lines passed (to esclude the initial one)
    
    //Race
    driveWhile(endOfTrackNotReached, &dstate, speed, coefficients, checkBatteryWithDefaultsAndMusic);
    
    //Stop the race music
    stop_music_async();
    
    //Play the final music
    play_music_with_base("4 CACBCACBCACBCADB CACBCACBCACBCADB CACBCACBCACBCADB CACBCACBCACBCADB", 
                         "3 S C CS CS C CS C CS CS C CS  A AS AS A AS A AS AS A AS  F FS FS F FS F FS FS F FS  G GS GS G GS G GS GS G GS" , 200);
  
 }   
#endif






//Awet's PID
#if ROBOT_MODE == ALT_PID
#include "dr.h"

int main()
{
    CyGlobalIntEnable; 
    ADC_Battery_Start(); 
    Systick_Start();
    UART_1_Start();
    IR_Start();
    
    initBattery();
    
    // all variables used, no globals
    float min[6] = {11000, 9000, 8000, 7500, 8200, 8100};
    float max[6] = {20000, 20000, 20000, 20000, 20000, 20000};
    sensorData d;
    float speedScale= 1;
    float kp=1.2;
    float kd=19;
    int hrLinesCount = 0; // horizontal lines encountered after race start
    
    
    startSensor(&d);
    cmotor_start();
    
    
    // go to first horizontal line
    gotoStartingLine(speedScale);
    
    
    // main loop
    for(;;)
    {
        checkBattery(5000, 4.2);
        drive(&d, min, max, kp, kd, speedScale, &hrLinesCount);
        
        CyDelay(1);
    }
}   
#endif





//Sumo battle
#if ROBOT_MODE == SUMO
#include "sumo.h"
#include "dr.h"

int main()
{
    CyGlobalIntEnable; 
    ADC_Battery_Start(); 
    UART_1_Start();
    Systick_Start();
    Ultra_Start();
    IR_Start();
    
    initBattery();
    
    // all variables used, no globals
    struct sensors_ dig;
    enum State state = SEARCH;
    int attackDistance = 40;
    float speedScale = 1;
    float searchTime = GetTicks();
    
    
    
    set_music_async("3 -SAAACDSD -SDEFSF -SFGESE -SDCCD -.S -SACDSD -SDEFSF -SFGESE -SDCD S -SACDSD -SDFGSG -SG 4 -AbSb -S -A3G4A3D -.S -SDEFSFSGS4A3D -.S -SDFESESFD E -S     -SGG 4-ASASAS -b -.A oS 3-GSGSGS -G 4-.A oS 4-ASASAS -b -.A oS -3GSFSESDS oS   4A oS b 3-bDF4b -4ASASASA3G oS G -2G3bDG F -2C3ADF -ESFSESDS oS", 250);
    
    //Play the initial music
    PlayPDM((uint16*)trackmarch, MARCH_SIZE);
    
    //Play "Bad Bad Boys" recording
    PlayPDM((uint16*)trackbbb, BBB_SIZE);
    
    
    
    
    reflectance_start();
    CyDelay(2);
    cmotor_start();
    
    
    
    // got to ring entrance
    gotoStartingLine(speedScale);
    
    // got to center of ring
    cmotor_speed(1, 1, speedScale);
    CyDelay(300); 
    
    
    
    // setting funtion to be called during delays
    setDelayCallback(checkBatteryWithDefaultsAndMusic);
    
    
    //main loop
    for(;;)
    {
        checkBatteryWithDefaultsAndMusic();
        check_if_insideRing(&state, &dig);
        doState(&state, attackDistance, speedScale, &searchTime);
        
        CyDelay(1);
    }
}   
#endif


/* [] END OF FILE */
