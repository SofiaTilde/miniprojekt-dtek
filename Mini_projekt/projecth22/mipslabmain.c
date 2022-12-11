/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* Declarations of uint_32 and the like */
#include <stdbool.h>
#include "pic32mx.h" /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
//#include "mipslabdata.h" /* Drawing declarations */
#include "mipslabfunc.h"
#include "displayitems.c"
//#include "LSM9DS0.h" /* Register defs */

// doc: LSM9DS0 9-axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for gyro)
#define ACCELEROMETER_ADDR 0x1E
#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D
#define WHO_AM_I_XM 0x0F
#define CTRL_REG0_XM 0x1F
#define CTRL_REG1_XM 0x20
#define CTRL_REG2_XM 0x21
#define FIFO_CTRL_REG 0x2E // bypass mode

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
		;				  /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while (OSCCON & (1 << 21))
		;		  /* Wait until PBDIV ready */
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

/* void clockTick() //!!!! TODO FIX
{
	int i;
	for (i = 0; i > 500; i++)
	{
	}
}
void setSCL(int num)
{
	clockTick();
	// PORTBSET = num << 12;
	PORTB = num << 12;
}
void setSDA(int num)
{
	clockTick();
	// PORTBSET = num << 14;
	PORTB = num << 14;
}
void isAcknowledged()
{
	//(I2CxSTAT<15>)
	int toCheck = 1;
	while (toCheck != 0)
	{
		toCheck = I2C1STAT;
		toCheck = toCheck >> 15;
		toCheck &= 0x0001;
	}
} */

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
	I2C1CON = 0x0;			// clean-up
	I2C1STAT = 0x0;			// clean-up
	I2C1BRG = 0x184;		// setting BRG to recommended 100 kHz clock frequency for 80 MHz CPU
	I2C1CONSET = 1 << 13;	// SIDL: Stop module operation when idle
	I2C1CONSET = 1 << 15;	// ON: start condition
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

void getACCLX_I2C()
{
	do
	{
		start_I2C();

	} while (!send_I2C(ACCELEROMETER_ADDR << 1));

	// send register address
	send_I2C(OUT_X_L_A);

	// send slave address with read bit
	do
	{
		restart_I2C();

	} while (!send_I2C((ACCELEROMETER_ADDR << 1) | 1));

	// recv data from slave
	uint8_t mydata = recv_I2C();

	// send NACK
	NACK_I2C();

	// send stop
	stop_I2C();

	// print the value from uint8_t to char array
	char thebits[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '\0'};
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
	}
	display_string(1, thebits);
	display_update();
}

void delay_I2C()
{
	int k;
	for (k = 0; k < 1000000; k++)
		;
}

void getWHOAMI()
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
}

void ACCEL_config()
{
	do
	{
		start_I2C();
	} while (send_I2C((ACCELEROMETER_ADDR << 1)) != 1);
	send_I2C(CTRL_REG0_XM);
	send_I2C(0x00);
	stop_I2C();

	do
	{
		start_I2C();
	} while (send_I2C((ACCELEROMETER_ADDR << 1)) != 1);
	send_I2C(CTRL_REG1_XM);
	send_I2C(0x67);
	stop_I2C();

	do
	{
		start_I2C();
	} while (send_I2C((ACCELEROMETER_ADDR << 1)) != 1);
	send_I2C(CTRL_REG2_XM);
	send_I2C(0x20); // setup how many g's (2 g)
	stop_I2C();

	// setup bypass mode
	do
	{
		start_I2C();
	} while (send_I2C((ACCELEROMETER_ADDR << 1)) != 1);
	send_I2C(FIFO_CTRL_REG);
	send_I2C(0x4); // setup how many g's (2 g)
	stop_I2C();
}

int main(void)
{
	// display setup
	initiate_spi();
	display_init();
	display_update();

	// START OF I2C
	I2C_setup();

	// Set up input pins
	TRISDSET = 1 << 8;
	TRISFSET = 1 << 1;

	// acceletometer configurations setup
	ACCEL_config();

	// start reading accl data
	// Send start to intialize I2C message
	// start_I2C();

	getWHOAMI();

	while (1)
	{
		getACCLX_I2C();
		delay_I2C();
	}

	// send slave address with write bit
	// keep sending until ack is received
	/*do
	{
		start_I2C();

	} while (!send_I2C(ACCELEROMETER_ADDR << 1));
	display_string(3, "hello3");
	display_update();

	// send register address
	send_I2C(OUT_X_L_A);

	// send slave address with read bit
	do
	{
		restart_I2C();

	} while (!send_I2C((ACCELEROMETER_ADDR << 1) | 1));
	display_string(1, "hello5");
	display_update();

	// recv data from slave
	uint8_t mydata = recv_I2C();
	display_string(2, "hello6");
	display_update();

	// send NACK
	NACK_I2C();
	display_string(3, "hello7");
	display_update();

	// send stop
	stop_I2C();
	display_string(0, "hello8");
	display_update();

	// print the value from uint8_t to char array
	char thebits[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '\0'};
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
	}
	display_string(0, thebits);
	display_update();
	display_string(1, "im here");
	display_update();*/
	return 0;
}
