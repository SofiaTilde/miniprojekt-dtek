#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* Declarations of uint_32 and the like */
#include <stdbool.h>
#include "pic32mx.h" /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include "mipslabfunc.h"
#include "accelerometer.h"

/* Global variables */
// declare array to hold the high and low 8 bits (total 16 bits) for X, Y and Z.
uint8_t accel_data[6];
uint8_t gyro_data[6];
// variables to hold the full X, Y and Z results.
uint16_t x_data;
uint16_t y_data;
uint16_t z_data;
uint16_t x_gyro_data;
uint16_t y_gyro_data;
uint16_t z_gyro_data;

// oled communication init
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

// check if bus is free or not
void check_if_idle()
{
    // Check lowest 5 bits of I2C1CON to check if idle
    // Check bit 14 of I2CSTAT to make sure no master transmit is in progress
    // if both or one is 1 keep waiting to finish
    // if both are 0 then proceed
    while ((I2C1CON & 0x1F) || (I2C1STAT & (1 << 14)))
        ;
}

// send data
bool send_I2C(uint8_t tobBeSent)
{
    check_if_idle();
    I2C1TRN = tobBeSent;
    check_if_idle();
    // return status of ack
    return !(I2C1STAT & (1 << 15));
}

// recv data from peripherial
uint8_t recv_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 3; // enable recv
    check_if_idle();
    I2C1STATCLR = 1 << 6; // set bit 6 to 0
    return I2C1RCV;
}

// set start condition
void start_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 0; // SEN: start condition enable bit
    check_if_idle();
}

// set restart condition
void restart_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 1; // set restart condition
    check_if_idle();
}

// setup before starting I2C message
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

// set no acknowledgement
void NACK_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 5; // ACKDT
    I2C1CONSET = 1 << 4; // ACKEN
}

// set acknowledge
void ACK_I2C()
{
    check_if_idle();
    I2C1CONCLR = 1 << 5; // ACKDT
    I2C1CONSET = 1 << 4; // ACKEN
}

// set stop
void stop_I2C()
{
    check_if_idle();
    I2C1CONSET = 1 << 2; // PEN: stop condition enable bit
    check_if_idle();
}

// get acceleration data from peripheral
xyz_data getACCL_XYZ_I2C(int adress)
{

    // send start until address and write bit to peripheral is acknowledged
    do
    {
        start_I2C();

    } while (!send_I2C(ACCELEROMETER_ADDR << 1));

    // send register address
    send_I2C(adress);

    // send restart until address with read bit is acknowledged by peripheral
    do
    {
        restart_I2C();

    } while (!send_I2C((ACCELEROMETER_ADDR << 1) | 1));

    // recv data from peripheral
    // will read from low bits of the x register all the way through to the high bits of the z register
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

    return current;
}

// get gyro data from peripheral
gyro_data_xyz getGYRO_XYZ_I2C(int adress)
{

    // send start until address and write bit to peripheral is acknowledged
    do
    {
        start_I2C();

    } while (!send_I2C(ACCELEROMETER_ADDR << 1));

    // send register address
    send_I2C(adress);

    // send restart until address with read bit is acknowledged by peripheral
    do
    {
        restart_I2C();

    } while (!send_I2C((ACCELEROMETER_ADDR << 1) | 1));

    // recv data from peripheral
    // will read from low bits of the x register all the way through to the high bits of the z register
    int j;
    for (j = 0; j < 5; j++)
    {
        gyro_data[j] = recv_I2C(); // grab 8 bits of data at a time
        ACK_I2C();                 // sends master ACK to confirm more data can be sent
    }

    // recv last 8 bits outside of loop to send Nack instead of mack after
    gyro_data[5] = recv_I2C();

    // send NACK
    NACK_I2C();

    // send stop
    stop_I2C();

    gyro_data_xyz this_gyro;
    // formatting high and low bits for each value to one complete X, Y and Z result
    this_gyro.x_gyro_data = (gyro_data[1] << 8) | gyro_data[0];
    this_gyro.y_gyro_data = (gyro_data[3] << 8) | gyro_data[2];
    this_gyro.z_gyro_data = (gyro_data[5] << 8) | gyro_data[4];

    return this_gyro;
}

// delay
void delay_I2C()
{
    int k;
    for (k = 0; k < 1000000; k++)
        ;
}

// to test if contact with Pmod is achieved
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

// before message transmission start setup for accelerometer
void ACCEL_config_I2C()
{
    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG5_XL);
    send_I2C(0x38); // all register bits are set to default values
    stop_I2C();

    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG6_XL);
    send_I2C(0x09); // set to +-16g, 211 Hz
    stop_I2C();

    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG7_XL);
    send_I2C(0x00); // set to disabled, internal and filter bypassed
    stop_I2C();
}

void GYRO_config_I2C()
{
    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG1_G);
    send_I2C(0x58); // ODR[Hz] = 238 and Cutoff[Hz] = 76
    // send_I2C(0x80); // ODR[Hz] = 238 (100) and Cutoff[Hz] = 14 (00)  (rest of bits 0)
    stop_I2C();

    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG2_G);
    send_I2C(0x0); // all bits set to default (00000000)
    stop_I2C();

    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG3_G);
    send_I2C(0x0); // default
    stop_I2C();

    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(ORIENT_CFG_G);
    send_I2C(0x0); // set to default (00000000)
    stop_I2C();

    do
    {
        start_I2C();
    } while (!send_I2C((ACCELEROMETER_ADDR << 1)));
    send_I2C(CTRL_REG4);
    send_I2C(0x38); // activate gyroscope yaw, roll and pitch axis (00111000)
    stop_I2C();
}