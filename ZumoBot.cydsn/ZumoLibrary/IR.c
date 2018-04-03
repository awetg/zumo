/**
 * @file IR.c
 * @brief Basic methods for operating IR receiver. For more details, please refer to IR.h file. 
 * @details part number: TSOP-2236
*/
#include "project.h"
#include "IR.h"

#define IRB_SIZE 10 /* size in power of 2 */
#define IRB_MASK (0xFFFFFFFF >> (32 - IRB_SIZE))


struct ir_buffer {
    uint32_t data[1 << IRB_SIZE];
    uint32_t head;
    uint32_t tail;
};

volatile struct ir_buffer irb;

CY_ISR(IR_capture_isr)
{
    static uint32_t old;
    uint32_t status = Timer_IR_ReadStatusRegister();
    if(status & Timer_IR_STATUS_CAPTURE) {
        uint32_t capture = Timer_IR_ReadCapture();
        uint32_t value  = (old - capture) & 0xFFFFFFUL;
        old = capture;
        //Pin_1_Write(IR_receiver_Read()); // for debugging with scope
        if(IR_receiver_Read()==0) { // signal was low --> we captured falling edge
            value |= IR_SIGNAL_HIGH;
        }
        irb.data[irb.head] = value;
        irb.head = (irb.head + 1) & IRB_MASK;
    }
}

bool IR_get(uint32_t *value) {
    if(irb.head != irb.tail) {
        *value = irb.data[irb.tail];
        irb.tail = (irb.tail + 1) & IRB_MASK;
        return true;
    }
    return false;
}

void IR_flush(void)
{
    uint32_t tmp;
    while(IR_get(&tmp)); // flush IR rcv buffer
}


void IR_wait(void)
{
    IR_flush();

    while(true) {
        uint32_t tmp;
        if(IR_get(&tmp) && (tmp & IR_SIGNAL_HIGH)) {
            tmp &= IR_SIGNAL_MASK;
            if(tmp > IR_LOWER_LIMIT && tmp < IR_UPPER_LIMIT) {
                break;
            }
        }
    }
}    

/**
* @brief    Starting Ultra Sonic Sensor
* @details  
*/
void IR_Start(void)
{
    IR_isr_StartEx(IR_capture_isr);               // Start ultra sonic interrupt
    Timer_IR_Start();                                      // Start Timer
}


#if 0 /* old (obsolete) stuff */
/**
* @brief Waiting until signal goes up
* @details continue looping before IR_receiver value becomes 1
*/
void wait_going_up()
{
    while(IR_receiver_Read()==0);
}    


/**
* @brief Waiting until signal goes down
* @details continue looping before IR_receiver value becomes 0
*/
void wait_going_down()
{
    while(IR_receiver_Read()==0);
}


/**
* @brief Counting to signal down
* @details Measuring the time until IR_receiver value goes up
* @return int
*   - returns count time
*/
int count_downtime()
{
    int cnt =0;
    while(IR_receiver_Read()==0)
        cnt++;
    
    return cnt;
}


/**
* @brief    Counting to signal up 
* @details  Measuring the time until IR_receiver value goes down
* @return   int
*   - returns count time
*/
int count_uptime()
{
    int cnt =0;
    while(IR_receiver_Read()==1)
        cnt++;
    
    return cnt;
}


/**
* @brief    Getting remote controller value
* @details  Getting each of the value sent by remote controller
* @return   int
    - returns hex value
*/
int get_IR()
{
    uint8 bit[33]={0};
    unsigned int val=0;
    uint t1, t2, i;
    
    wait_going_down();
    
    for(i = 0; i < 33; i++)     // loop 33times ( 1bit - Start bit, 32bits - Databit)
    {
        t1 = 0;
        t2 = 0;
        
        t1 = count_downtime();
        t2 = count_uptime();
        
        // If up time is longer than down time, value of the bit becomes 1
        if(t2 > t1)
            bit[i]=1;
        else
            bit[i]=0; 
    } 
   
    /* // If you want to see binary value
    int cnt;
    for(cnt=0; cnt<33; cnt++)
        printf("%d ",bit[cnt]);
    printf("\r\n\n");
    */
   
    // compress bits to single int, ignore start bit

    for( i=0; i<32; i++)
        val |= bit[i+1] << i;
    
    return val;
}

#endif
