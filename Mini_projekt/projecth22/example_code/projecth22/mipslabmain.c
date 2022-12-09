/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h> /* Declarations of uint_32 and the like */
//#include "pic32mx.h"	 /* Declarations of system-specific addresses etc */
#include "p32mx340f512h.h"
#include "p32_defs.h" /* more defines, ports/regs */
#include "stdbool.h"
#include "mipslab.h"	 /* Declatations for these labs */
#include "mipslabdata.h" /* Drawing declarations */
#include "mipslabmain.h"

// useful variables between functions, will be defined by seupi2c() and in main
p32_i2c *pi2c;				   // i2c struct for easier handling of things
volatile p32_ioport *iop;	   // io port representation
uint8_t accl_chip_i2c_address; // address to LSM9D

int main(void)
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

	display_init();
	display_string(0, "KTH/ICT lab");
	display_string(1, "in Computah");
	display_string(2, "Engineering");
	display_string(3, "Welcome!");
	display_update();

	// display_image(96, icon);
	// display_image(48, shape1);

	// labinit(); /* Do any lab-specific initialization */

	// Below code to initiate I2C

	accl_chip_i2c_address = ACCELEROMETER_ADDR; // address of the i2c component we want to use from h file

	setupi2c();
	display_string(2, " Done! Setup");
	display_update();
	// Now i2c should be initialized and we can attempt reading WHO_AM_I_XM of accelerometer to verify

	uint8_t who_ami_i = readACCWhoAmI();

	char tmp[10] = {'W', 'H', 'O', ':', ' ', ' ', '\n'};
	tmp[4] = (char)who_ami_i;
	display_string(3, tmp);

	display_string(2, " Done!");
	display_update();

	while (1)
	{
		// labwork(); /* Do lab-specific things again and again */
	}
	return 0;
}

uint8_t readACCWhoAmI()
{
	// From LSM9D doc: The next byte of data
	// transmitted after the start condition contains the address of the slave in the first 7 bits and
	// the eighth bit tells whether the master is receiving data from the slave or transmitting data to
	// the slave.  (bit shifting in 0 for writing to come, and 1 for reading to come)
	do
	{
		setStartCondition();
	} while (!sendOneByte(accl_chip_i2c_address << 1));
	// writng the register we are interested in
	sendOneByte(WHO_AM_I_XM);

	do
	{
		setRestartCondition();
	} while (!sendOneByte(((accl_chip_i2c_address << 1) | 1)));

	uint8_t data = receiveOneByte();

	// SendAck();  No acc according to LSM9D doc

	setStopCondition();

	return data;
}

void setupi2c()
{
	pi2c = (p32_i2c *)_I2C1_BASE_ADDRESS; // address of i2c base adress

	// we initiate in master mode on the default 100kHz
	int frequency = 100000; // Hz, see BRG use later in this method

	// make sure i2c is off
	// ON: I2C Enable bit(2)
	// 1 = Enables the I2C module and configures the SDAx and SCLx pins as serial port pins
	// 0 = Disables I2C module; all I2C pins are controlled by PORT functions
	pi2c->i2cCon.ON = 0;

	// set pins to INPUT
	// uc32 pinSDA1 = A5 RG3 = bit3
	// uc32 pinscl1 = A4 RG2 = bit2
	// PORTG       /* 0xBF886190 */
	// PORTGCLR    /* 0xBF886194 */
	// PORTGSET    /* 0xBF886198 */
	// PORTGINV    /* 0xBF88619C */

	// iop defines the PORTG
	// iop=(p32_ioport *)0xBF886190;   // PORTG has pins A4 and A5
	TRISGSET = 1 << 3;

	// SDA1 as INPUT by setting bit 3
	// iop->tris.set = 1<<3;  //bit 3 on TRIS defines RG3/A5 as input
	// iop->odc.clr  = 1<<3;  //apparently required to make sure it isn't open drain

	// SCL1 as INPUT by setting bit 2
	// iop->tris.set = 1<<2;  //bit 2 on TRIS defines RG2/A4 as input
	// iop->odc.clr  = 1<<2;  //apparently required to make sure it isn't open drain
	TRISGSET = 1 << 2;

	// The I2C module contains a Baud Rate Generator (BRG)
	// calculate the BRG and set the frequency of the master

	// I2CxBRG<15:0>: I2C Baud Rate Generator Value bits(1,2)
	// These bits control the divider function of the Peripheral Clock.
	// This formula comes from the documentation
	// (__PIC32_pbClk/frequency/2) - (((__PIC32_pbClk / 10000000) * 26) / 25) - 2;
	// and gives 0x018A from the documentation for 80MHz CPU and 100kHz i2c frequency
	// pi2c->ixBrg.reg = 0x018A;
	I2C1BRGSET = 0x018A;

	// turn on the I2C controller, see doc earlier in method
	pi2c->i2cCon.ON = 1;
}

// Helper methods below

/* Check if bus in use or wait */
void check_If_Idle()
{
	while (I2C1CON & 0x1F || I2C1STAT & (1 << 14))
		; // TRSTAT
}

/* Send one byte on I2C bus, return ack/nack status of transaction */
bool sendOneByte(uint8_t data)
{
	check_If_Idle();
	pi2c->ixTrn.reg = data; // load byte onto txreg
	check_If_Idle();
	// 1 = Not Acknowledge (NACK) was received
	// 0 = Acknowledge (ACK) was received
	return !(pi2c->i2cStat.ACKSTAT);
}

/* Attempt receive one byte from the i2c device */
uint8_t receiveOneByte()
{
	check_If_Idle();
	// RCEN
	// 1 = Enables Receive mode for I2C, automatically cleared by the I2C module
	// after reception of the eighth bit of the received data byte
	// 0 = Receive sequence not in progress
	pi2c->i2cCon.RCEN = 1;
	check_If_Idle();
	//   I2COV: I2C Receive Overflow Status bit
	// 1 = A byte was received while the I2CxRCV register was still holding the previous byte.
	// This bit must be cleared in software.
	// 0 = No overflow
	pi2c->i2cStat.I2COV = 0;
	// data should now be in the I2CxRCV register
	return pi2c->ixRcv.reg;
}

/* Acknowledge */
void SendAck()
{
	check_If_Idle();
	//   ACKDT: Acknowledge Data bit
	// This bit determines the value of the acknowledgment bit that will be transmitted during acknowledge
	// sequence (the ninth clock cycle) after receiving a byte in either slave or master operation. In Master mode,
	// the value of this bit will be transmitted after setting the ACKEN bit. In Slave mode, if AHEN or DHEN = 1,
	// software must wait the appropriate setup time after writing this bit before setting the SCLREL bit to begin
	// the acknowledge sequence.
	// 1 = A NACK is sent
	// 0 = ACK is sent
	pi2c->i2cCon.ACKDT = 0;
	// ACKEN: Acknowledge Sequence Enable bit(4)
	// In I2C Master mode only; applicable during master receive.
	// 1 = Initiate Acknowledge sequence on SDAx and SCLx pins, and transmit ACKDT data bit; automatically
	// cleared by the I2C module when completed.
	// 0 = Acknowledge sequence idle
	pi2c->i2cCon.ACKEN = 1;
}

/* no acknowledge */
void sendNoAck()
{
	check_If_Idle();
	// as comment above
	pi2c->i2cCon.ACKDT = 1;
	pi2c->i2cCon.ACKEN = 1;
}

/* Set stop conditon on i2c */
void setStopCondition()
{
	check_If_Idle();
	// PEN: Stop Condition Enable bit(4)
	// In I2C Master mode only.
	// 1 = Initiate Stop condition on SDAx and SCLx pins;
	// automatically cleared by the I2C module when completed.
	// 0 = Stop condition is idle
	pi2c->i2cCon.PEN = 1;
	check_If_Idle();
}

/* Set start conditon on i2c */
void setStartCondition()
{
	check_If_Idle();
	//   SEN: Start Condition Enable bit(4)
	// In I2C Master mode only.
	// 1 = Initiate Start condition on SDAx and SCLx pins;
	// automatically cleared by the I2C module when completed.
	// 0 = Start condition is idle
	pi2c->i2cCon.SEN = 1;
	check_If_Idle();
}

/* Set restart conditon on i2c */
void setRestartCondition()
{
	check_If_Idle();
	// RSEN: Restart Condition Enable bit(4)
	// In I2C Master mode only.
	// 1 = Initiate Restart condition on SDAx and SCLx pins;
	// automatically cleared by the I2C module when completed.
	// 0 = Restart condition is idle
	pi2c->i2cCon.RSEN = 1;
	check_If_Idle();
}
