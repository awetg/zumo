/**
 * @file    Nunchuk.c
 * @brief   Basic methods for operating wii nunchuck. For more details, please refer to Nunchuk.h file. 
 * @details 
*/
#include "Nunchuk.h"

static int x,y,z;

/**
* @brief    Start Nunchuk
* @details
*/
void nunchuk_start()
{
    I2C_Start();
}


/**
* @brief    Initializing Nunchuk
* @details  write value(0x00) to register(address - 0x40) to initialize nunchuk
*/
void nunchuk_init()
{
    uint8_t buf[2] = { 0x40, 0x00 };
    uint8_t status;
    I2C_MasterClearStatus();
    CyDelay(100);
    status = I2C_MasterWriteBuf(0x52, buf, 2, I2C_MODE_COMPLETE_XFER);
    while(((status = I2C_MasterStatus()) & I2C_MSTAT_WR_CMPLT) == 0);
    CyDelay(100);
}    


/**
* @brief    Reading Nunchuk value
* @details  Reading nunchuk value and getting joystick and accelerometer values
*/
void nunchuk_read()
{
    uint8_t status;
    uint8_t buf[6];
    
    // send conversion command    buf[0] = 0x00;
    I2C_MasterClearStatus();
    status = I2C_MasterWriteBuf(0x52, buf, 1, I2C_MODE_COMPLETE_XFER);
    while(((status = I2C_MasterStatus()) & I2C_MSTAT_WR_CMPLT) == 0);
    
    // wait for conversion to complete
    CyDelay(50); // todo: check if shorter wait would work
   
    // read values
    I2C_MasterClearStatus();
    status = I2C_MasterReadBuf(0x52, buf, 6, I2C_MODE_COMPLETE_XFER);
    while(((status = I2C_MasterStatus()) & I2C_MSTAT_RD_CMPLT) == 0);
    
    int i;
    // decrypt data
    for(i = 0; i < 6; i++) buf[i] = (buf[i] ^ 0x17) + 0x17;
    // accelerometer values
    x = (buf[2] << 2) | ((buf[5] >> 2) & 0x03);
    y = (buf[3] << 2) | ((buf[5] >> 4) & 0x03);
    z = (buf[4] << 2) | ((buf[5] >> 6) & 0x03);
    
    //If you want to print out the value  
    //printf("%3d %3d, %d, %d, %4d %4d %4d\r\n", buf[0], buf[1], (buf[5] >> 1) & 1, buf[5] & 1, x, y, z);
}

#if 0
//nunchuk//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
  
    nunchuk_start();
    nunchuk_init();
    
    for(;;)
    {    
        nunchuk_read();
    }
}   
#endif


