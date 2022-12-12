#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* Declarations of uint_32 and the like */
#include <stdbool.h>
#include "pic32mx.h" /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
//#include "mipslabdata.h" /* Drawing declarations */
#include "mipslabfunc.h"
//#include "displayitems.c"
#include "accelerometer.h"

/////////////////////////////////////////
// LSM9DS1 Accel/Gyro (XL/G) Registers //
/////////////////////////////////////////
#define ACCELEROMETER_ADDR 0x6B
#define ACT_THS 0x04
#define ACT_DUR 0x05
#define INT_GEN_CFG_XL 0x06
#define INT_GEN_THS_X_XL 0x07
#define INT_GEN_THS_Y_XL 0x08
#define INT_GEN_THS_Z_XL 0x09
#define INT_GEN_DUR_XL 0x0A
#define REFERENCE_G 0x0B
#define INT1_CTRL 0x0C
#define INT2_CTRL 0x0D
#define WHO_AM_I_XG 0x0F
#define CTRL_REG1_G 0x10
#define CTRL_REG2_G 0x11
#define CTRL_REG3_G 0x12
#define ORIENT_CFG_G 0x13
#define INT_GEN_SRC_G 0x14
#define OUT_TEMP_L 0x15
#define OUT_TEMP_H 0x16
#define STATUS_REG_0 0x17
#define OUT_X_L_G 0x18
#define OUT_X_H_G 0x19
#define OUT_Y_L_G 0x1A
#define OUT_Y_H_G 0x1B
#define OUT_Z_L_G 0x1C
#define OUT_Z_H_G 0x1D
#define CTRL_REG4 0x1E
#define CTRL_REG5_XL 0x1F
#define CTRL_REG6_XL 0x20
#define CTRL_REG7_XL 0x21
#define CTRL_REG8 0x22
#define CTRL_REG9 0x23
#define CTRL_REG10 0x24
#define INT_GEN_SRC_XL 0x26
#define STATUS_REG_1 0x27
#define OUT_X_L_XL 0x28
#define OUT_X_H_XL 0x29
#define OUT_Y_L_XL 0x2A
#define OUT_Y_H_XL 0x2B
#define OUT_Z_L_XL 0x2C
#define OUT_Z_H_XL 0x2D
#define FIFO_CTRL 0x2E
#define FIFO_SRC 0x2F
#define INT_GEN_CFG_G 0x30
#define INT_GEN_THS_XH_G 0x31
#define INT_GEN_THS_XL_G 0x32
#define INT_GEN_THS_YH_G 0x33
#define INT_GEN_THS_YL_G 0x34
#define INT_GEN_THS_ZH_G 0x35
#define INT_GEN_THS_ZL_G 0x36
#define INT_GEN_DUR_G 0x37

/* Global variables */
// declare array to hold the high and low 8 bits (total 16 bits) for X, Y and Z.
uint8_t accel_data[6];
// variables to hold the full X, Y and Z results.
uint16_t x_data;
uint16_t y_data;
uint16_t z_data;

void initiate_spi()
{
    /*
This will set the peripheral bus clock to the same frequency
as the sysclock. That means 80 MHz, when the microcontroller
is running at 80 MHz. Changed 2017, as recommended by Axel.
*/
    SYSKEY = 0xAA996655; /* Unlock OSCCON, step 1 */
    SYSKEY = 0x556699AA; /* Unlock OSCCON, step 2 */
    while (OSCCON & (1 << 21))
        ;                 /* Wait until PBDIV ready */
    OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
    while (OSCCON & (1 << 21))
        ;         /* Wait until PBDIV ready */
    SYSKEY = 0x0; /* Lock OSCCON */

    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up input pins */
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    /* SPI2STAT bit SPIROV = 0; */
    SPI2STATCLR = 0x40;
    /* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
    /* SPI2CON bit MSTEN = 1; */
    SPI2CONSET = 0x20;
    /* SPI2CON bit ON = 1; */
    SPI2CONSET = 0x8000;
}

void check_if_idle()
{
    // Check lowest 5 bits of I2C1CON to check if idle
    // Check bit 14 of I2CSTAT to make sure no master transmit is in progress
    // if both or one is 1 keep waiting to finish
    // if both are 0 then proceed
    while ((I2C1CON & 0x1F) || (I2C1STAT & (1 << 14)))
        ;
}

// send i2c
bool send_I2C(uint8_t tobBeSent)
{
    check_if_idle();
    I2C1TRN = tobBeSent;
    check_if_idle();
    // return status of ack
    return !(I2C1STAT & (1 << 15));
}

uint8_t recv_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 3; // enable recv
    check_if_idle();
    I2C1STATCLR = 1 << 6; // set bit 6 to 0
    return I2C1RCV;
}

void start_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 0; // SEN: start condition enable bit
    check_if_idle();
}

void restart_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 1; // set restart condition
    check_if_idle();
}

void I2C_setup()
{
    /* Set up I2C as master and clean before begin */
    I2C1CON = 0x0;          // clean-up
    I2C1STAT = 0x0;         // clean-up
    I2C1BRG = 0x184;        // setting BRG to recommended 100 kHz clock frequency for 80 MHz CPU
    I2C1CONSET = 1 << 13;   // SIDL: Stop module operation when idle
    I2C1CONSET = 1 << 15;   // ON: start condition
    uint8_t data = I2C1RCV; // Clean recieve data before using
}

void NACK_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 5; // ACKDT
    I2C1CONSET = 1 << 4; // ACKEN
}

void ACK_I2C()
{
    check_if_idle();
    I2C1CONCLR = 1 << 5; // ACKDT
    I2C1CONSET = 1 << 4; // ACKEN
}

void stop_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 2; // PEN: stop condition enable bit
    check_if_idle();
}

xyz_data getACCL_XYZ_I2C()
{

    do
    {
        start_I2C();

    } while (!send_I2C(ACCELEROMETER_ADDR << 1));

    // send register address
    send_I2C(OUT_X_L_XL);

    // send slave address with read bit
    do
    {
        restart_I2C();

    } while (!send_I2C((ACCELEROMETER_ADDR << 1) | 1));

    // recv data
    int j;
    for (j = 0; j < 5; j++)
    {
        accel_data[j] = recv_I2C(); // grab 8 bits of data at a time
        ACK_I2C();                  // sends master ACK to confirm more data can be sent
    }

    // recv last 8 bits outside of loop to send Nack instead of mack after
    accel_data[5] = recv_I2C();

    // send NACK
    NACK_I2C();

    // send stop
    stop_I2C();

    xyz_data current;
    // formatting high and low bits for each value to one complete X, Y and Z result
    current.x_data = (accel_data[1] << 8) | accel_data[0];
    current.y_data = (accel_data[3] << 8) | accel_data[2];
    current.z_data = (accel_data[5] << 8) | accel_data[4];

    // print the value from uint16_t to char array
    /*  char xyz_data;
     char thebits[17] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
     int s;
     for (s = 0; s < 3; s++)
     {

         int i;
         int onebit;
         for (i = 0; i < 16; i++)
         {
             // uint8_t onebit = mydata & 1;
             // onebit = (mydata & (1 << (7 - i)));
             if (mydata & (1 << (15 - i)))
             {
                 thebits[i] = '1';
             }
         }
     } */

    /* char thebits[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '\0'};
    int i;
    int onebit;
    for (i = 0; i < 8; i++)
    {
        // uint8_t onebit = mydata & 1;
        // onebit = (mydata & (1 << (7 - i)));
        if (mydata & (1 << (7 - i)))
        {
            thebits[i] = '1';
        }
    } */
    // display_string(1, thebits);
    // display_update();
    return current;
}

void delay_I2C()
{
    int k;
    for (k = 0; k < 1000000; k++)
        ;
}

/* void getWHOAMI()
{
    do
    {
        start_I2C();

    } while (!send_I2C(ACCELEROMETER_ADDR << 1));

    // send register address
    send_I2C(WHO_AM_I_XM);

    // send slave address with read bit
    do
    {
        restart_I2C();

    } while (!send_I2C((ACCELEROMETER_ADDR << 1) | 1));

    // recv data from slave
    uint8_t mydata1 = recv_I2C();

    // send NACK
    NACK_I2C();

    // send stop
    stop_I2C();

    // print the value from uint8_t to char array
    char thebits1[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '\0'};
    int i;
    int onebit1;
    for (i = 0; i < 8; i++)
    {
        // uint8_t onebit = mydata & 1;
        // onebit = (mydata & (1 << (7 - i)));
        if (mydata1 & (1 << (7 - i)))
        {
            thebits1[i] = '1';
        }
    }
    display_string(0, thebits1);
    display_update();
} */

void ACCEL_config_I2C()
{
    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG5_XL);
    send_I2C(0x38);
    stop_I2C();

    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG6_XL);
    send_I2C(0x78);
    stop_I2C();

    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG7_XL);
    send_I2C(0x00);
    stop_I2C();
}