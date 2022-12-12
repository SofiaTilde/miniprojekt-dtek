/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h> /* Declarations of uint_32 and the like */
//#include <stdbool.h>
//#include "pic32mx.h" /* Declarations of system-specific addresses etc */
//#include "mipslab.h" /* Declatations for these labs */
//#include "mipslabdata.h" /* Drawing declarations */
//#include "mipslabfunc.h"
//#include "displayitems.c"
//#include "accelerometer.h"

/* int main(void)
{
	// display setup
	initiate_spi();
	display_init();
	display_update();
	screen_init();
	ChangeScreen(MainMenu);
	update_screen();

	// START OF I2C
	I2C_setup();

	// Set up input pins
	TRISDSET = 1 << 8;
	TRISFSET = 1 << 1;

	// acceletometer configurations setup
	ACCEL_config_I2C();

	// start reading accl data
	// Send start to intialize I2C message
	// start_I2C();

	// getWHOAMI();

	while (1)
	{
		delay_I2C();
		doScreenStuff();
	}

	return 0;
} */