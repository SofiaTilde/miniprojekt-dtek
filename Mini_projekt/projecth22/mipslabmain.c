/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include "pic32mx.h" /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
//#include "mipslabdata.h" /* Drawing declarations */
#include "mipslabfunc.h"
#include "displayitems.c"
#include "LSM9DS0.h" /* Register defs */

// void translate_image(char[][]);
void render_image(char *data);
// char my_little_picture[32][128];

// array to put results in
// char picture[4 * 128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// create image here to send to translate image
/*void create_image()
{
	int i, j;
	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 128; j + 2)
		{
			my_little_picture[i][j] = (char)0;
		}
	}

	for (i = 0; i < 32; i++)
	{
		for (j = 1; j < 128; j + 2)
		{
			my_little_picture[i][j] = (char)1;
		}
	}

	// add a little ball
	my_little_picture[0][0] = 1;
	my_little_picture[1][0] = 1;
	my_little_picture[2][0] = 1;
	my_little_picture[3][0] = 1;
	my_little_picture[4][0] = 1;
	my_little_picture[5][0] = 1;
	my_little_picture[6][0] = 1;
	my_little_picture[7][0] = 1;
	my_little_picture[11][99] = 0;
	my_little_picture[11][100] = 0;
	my_little_picture[11][101] = 0;
	my_little_picture[12][100] = 0;
} */

/* void debug_inputimage()
{
	int i, j;
	printf("input-image:(%c", '\n');
	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 128; j++)
		{
			printf("%d, ", my_little_picture[i][j]);
		}
		// printf("(ENDROW %c)", i);
	}
	// printf("\n)");
}

void debug_outputimage()
{
	int j;

	printf("output-image:\n(");
	for (j = 0; j < (4 * 128); j++)
	{
		printf("%d, ", picture[j]);
	}
	printf("\n)");
} */

// translate into format render_image can understand

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

void clockTick() //!!!! TODO FIX
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

int check_if_acknowledged()
{
	// check if slave acknowledged
	while (!(I2C1STAT & (1 << 15)))
		; // ACKSTAT: acknowledge status
	return 1;
}

// send i2c
int send(uint8_t tobBeSent)
{
	check_if_idle();
	I2C1TRN = tobBeSent;
	check_if_idle();
	// return status of ack
	return (!(I2C1STAT & (1 << 15)));
}

void start_I2C()
{
	check_if_idle();
	I2C1CONSET = 1 << 0; // SEN: start condition enable bit
	check_if_idle();
}

void startI2C()
{
	display_string(0, "hello1");
	/* Set up I2C as master and clean before begin */
	I2C1CON = 0;				 // clean-up
	I2C1STAT = 0;				 // clean-up
	I2C1BRG = 0x184;			 // setting BRG to recommended 100 kHz clock frequency for 80 MHz CPU
	I2C1CONSET = 1 << 13;		 // SIDL: Stop module operation when idle
	I2C1CONSET = 1 << 15;		 // ON: start condition
	uint8_t gyro_data = I2C1RCV; // Clean recieve data before using
	display_string(1, "hello2");

	// Set up input pins
	TRISDSET = 1 << 8;
	TRISFSET = 1 << 1;
	display_string(2, "hello3");

	// Send start to intialize I2C message
	// first check if idle before doing anything
	check_if_idle();
	I2C1CONSET = 1 << 0; // SEN: start condition enable bit
	check_if_idle();
	display_string(3, "hello4");

	// send slave address with write bit
	I2C1TRN = (ACCELEROMETER_ADDR << 1); // TRN: transmit accelerometer address onto I2C bus
	check_if_idle();
	while (check_if_acknowledged() != 1)
	{
		I2C1TRN = (ACCELEROMETER_ADDR << 1);
	}

	// send register address
	check_if_idle();
	I2C1TRN = WHO_AM_I_XM; // send register
	check_if_idle();
	while (check_if_acknowledged() != 1)
	{
		I2C1TRN = WHO_AM_I_XM;
	}

	// send restart condition
	check_if_idle();
	I2C1CONSET = 1 << 1; // set restart condition
	check_if_idle();

	// send slave address with read bit
	I2C1TRN = ((ACCELEROMETER_ADDR << 1) | 1); // TRN: transmit accelerometer address onto I2C bus
	check_if_idle();
	while (check_if_acknowledged() != 1)
	{
		I2C1TRN = ((ACCELEROMETER_ADDR << 1) | 1);
	}

	// recv data from slave
	check_if_idle();
	I2C1CONSET = 1 << 3; // enable recv
	check_if_idle();
	I2C1STATCLR = 1 << 6; // set bit 6 to 0
	uint8_t mydata = I2C1RCV;

	// send NACK
	I2C1CON = 1 << 5; // ACKDT
	// stop
	I2C1CON = 1 << 2;

	// print the value from uint8_t to char array
	char thebits[8];
	int i;
	for (i = 0; i < 7; i++)
	{
		uint8_t onebit = mydata & 1;
		if (onebit == (uint8_t)1)
		{
			thebits[i] = 1;
		}
		else
		{
			thebits[i] = 0;
		}
	}

	display_string(1, "hello_world");
	display_string(0, thebits);
	display_update;

	// SCL -> J11 SCL -> 19/A5 -> RB14
	// SDA -> J11 SDA -> 18/A4 -> RB12
}

int main(void)
{
	initiate_spi();

	display_init();

	display_string(0, "sup, i am alive");

	display_update();

	// create_image(); // this creates the internal screen with demo data

	// debug_inputimage(); // this debug prints the internal screen data unformatted

	// translate_image(my_little_picture); // this translates internal screen to oled data
	//  debug_outputimage();				// this prints debug of translate result

	// render_image(picture);

	display_string(1, "i made it here");
	display_update();

	// START OF I2C
	display_string(0, "hello1");
	display_update();
	/* Set up I2C as master and clean before begin */
	I2C1CON = 0x0;			// clean-up
	I2C1STAT = 0x0;			// clean-up
	I2C1BRG = 0x184;		// setting BRG to recommended 100 kHz clock frequency for 80 MHz CPU
	I2C1CONSET = 1 << 13;	// SIDL: Stop module operation when idle
	I2C1CONSET = 1 << 15;	// ON: start condition
	uint8_t data = I2C1RCV; // Clean recieve data before using
	display_string(1, "hello2");
	display_update();

	// Set up input pins
	TRISDSET = 1 << 8;
	TRISFSET = 1 << 1;
	display_string(2, "hello3");
	display_update();

	// Send start to intialize I2C message
	// first check if idle before doing anything
	check_if_idle();
	I2C1CONSET = 1 << 0; // SEN: start condition enable bit
	check_if_idle();
	display_string(3, "hello4");
	display_update();

	// send slave address with write bit
	/* I2C1TRN = (ACCELEROMETER_ADDR << 1); // TRN: transmit accelerometer address onto I2C bus
	check_if_idle(); */
	// keep sending until ack is received
	while (send((ACCELEROMETER_ADDR << 1)) != 1)
	{
		start_I2C();
	};
	display_string(0, "hello5");
	display_update();

	// send register address
	send(WHO_AM_I_XM);
	display_string(1, "hello6");
	display_update();

	// send restart condition
	check_if_idle();
	I2C1CONSET = 1 << 1; // set restart condition
	check_if_idle();

	// send slave address with read bit
	while (send(((ACCELEROMETER_ADDR << 1) | 1)) != 1)
	{
		check_if_idle();
		I2C1CONSET = 1 << 1; // set restart condition
		check_if_idle();
	};

	display_string(2, "hello7");
	display_update();

	// recv data from slave
	check_if_idle();
	I2C1CONSET = 1 << 3; // enable recv
	check_if_idle();
	I2C1STATCLR = 1 << 6; // set bit 6 to 0
	uint8_t mydata = I2C1RCV;
	display_string(3, "hello8");
	display_update();

	// send NACK
	check_if_idle();
	I2C1CONSET = 1 << 5; // ACKDT
	I2C1CONSET = 1 << 4; // ACKEN
	display_string(0, "hello9");
	display_update();

	// stop
	check_if_idle();
	I2C1CONSET = 1 << 2; // PEN: stop condition enable bit
	check_if_idle();

	// print the value from uint8_t to char array
	char thebits[9];
	int i;
	for (i = 0; i < 8; i++)
	{
		// uint8_t onebit = mydata & 1;
		int onebit = (mydata & (1 << i));
		if (onebit == 1)
		{
			thebits[i] = '1';
		}
		else if (onebit == 0)
		{
			thebits[i] = '0';
		}
	}
	thebits[9] = '\0';

	display_string(1, "hello_world");
	/* if (thebits[7] == 1)
	{
		display_string(0, "one");
	}
	else if (thebits[7] == 0)
	{
		display_string(0, "zero");
	} */
	display_string(0, thebits);
	display_update();

	// SCL -> J11 SCL -> 19/A5 -> RB14
	// SDA -> J11 SDA -> 18/A4 -> RB12

	// startI2C();

	/*display_string(0, "KTH/ICT lab");
	display_string(1, "in Computah");
	display_string(2, "Engineering");
	display_string(3, "Welcome!");*/

	// display_image(96, icon);

	// labinit(); Do any lab-specific initialization

	/* while (1)
	{
		// labwork(); Do lab-specific things again and again
	} */

	return 0;
}
