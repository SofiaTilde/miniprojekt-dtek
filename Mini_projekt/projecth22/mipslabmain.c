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
// #include "mipslabdata.h" /* Drawing declarations */
#include "mipslabfunc.h"
// #include "mipslabfunc.c"
// #include "displayitems.c"

/*
// void translate_image(char[][]);
void render_image(char *data);
char my_little_picture[32][128];

// array to put results in
char picture[4 * 128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// create image here to send to translate image
void create_image()
{
	/* int i, j;
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
	} */
/*
// add a little ball
my_little_picture[0][0] = 1;
my_little_picture[1][0] = 1;
my_little_picture[2][0] = 1;
my_little_picture[3][0] = 1;
my_little_picture[4][0] = 1;
my_little_picture[5][0] = 1;
my_little_picture[6][0] = 1;
my_little_picture[7][0] = 1;
* /
/* my_little_picture[11][99] = 0;
my_little_picture[11][100] = 0;
my_little_picture[11][101] = 0;
my_little_picture[12][100] = 0; */
//}
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
	PORTBSET = num << 12;
}
void setSDA(int num)
{
	clockTick();
	PORTBSET = num << 14;
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
/*
void startI2C()
{
	//Set up I2C as master
I2C1CONSET = 1 << 15; // start condition
I2C1CONSET = 1 << 0;
I2C1STATSET = 1 << 3;

/*
SCL -> J11 SCL -> 19/A5 -> RB14
SDA -> J11 SDA -> 18/A4 -> RB12
*/
// check if bus is idle
/*
int returnvalue = PORTG;
returnvalue &= 0b 0001 0000;
returnvalue = returnvalue >> 4;
if (returnvalue ==)
*/
/*
// start condition
setSCL(1);
setSDA(1);
setSDA(0);
setSCL(0);
*/
// 0001 1101
// 0011101
/*
int slave_address = 0b00111010; // 8 bit slave address
slave_address |= 0 << 0;		// setting last bit to write (0)
I2C1TRNSET = slave_address;		// slave address with write bit
// I2C1STATSET = 0 << 2;  // setting bit to write
// I2C1ADDSET = 0011101;  // setting the slave address
isAcknowledged();
display_string(0, "Acknowledged!!!!!");
display_update();

I2C1TRNSET = 0b00000000;
isAcknowledged();
display_string(1, "Acknowledged!!!!!");
display_update();

I2C1TRNSET = 0x0F;
isAcknowledged();
display_string(2, "Acknowledged!!!!!");
display_update();

I2C1CONSET = 1 << 1;

slave_address |= 1 << 0;	// setting last bit to read (1)
I2C1TRNSET = slave_address; // slave address with read bit
isAcknowledged();

I2C1CONSET = 1 << 3;
I2C1CONSET = 1 << 5; // sends a NACK
I2C1CONSET = 1 << 2; // stop

// display_debug(&I2C1RCV);
if (I2C1RCV == 0) // 0xD4)
{
	display_string(3, "00000000");
	display_update();
}
else if (I2C1RCV == 0xD4)
{
	display_string(3, "11010100");
	display_update();
}
display_string(0, "Is here");
display_update();
}
*/
int main(void)
{
	/*
	initiate_spi();
	display_init();
	display_update();
	screen_init();
	//*/
	// startI2C();
	testStuff();
	/*
	ChangeScreen();
	update_screen();
	while (1)
	{
		doScreenStuff();
	}
	//*/
	return 0;
}
