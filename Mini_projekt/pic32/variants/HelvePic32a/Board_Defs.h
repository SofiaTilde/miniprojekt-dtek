/************************************************************************/
/*																		*/
/*	Board_Defs.h --	CmodCK1 Board Customization Declarations			*/
/*																		*/
/************************************************************************/
/*	Author: Gene Apperson												*/
/*	Copyright 2011, Digilent. All rights reserved						*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/* This file contains the board specific declartions and data structure	*/
/* to customize the chipKIT MPIDE for use with a CmodCK1 board using a	*/
/* PIC32 part in a 44-pin package.										*/
/*																		*/
/* This code is based on earlier work:									*/
/*		Copyright (c) 2010, 2011 by Mark Sproul							*/
/*		Copyright (c) 2005, 2006 by David A. Mellis						*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	10/07/2011(GeneA): Created											*/
/*	11/28/2011(GeneA): Moved data definitions and configuration			*/
/*		functions to Board_Data.c										*/
/*	11/29/2011(GeneA): Moved int priority definitions to System_Defs.h	*/
/*  01/23/2013(KeithV): Modified for CK1 board                          */
/*  03/26/2013(KeithV): Modified for DP32 board                         */
/*	05/27/2013(ClaudiaGoga): Fixed MISO, MOSI definitions.       		*/
/*							Added definitions to support PPS for SPI1	*/
/*																		*/
/************************************************************************/
//*	This library is free software; you can redistribute it and/or
//*	modify it under the terms of the GNU Lesser General Public
//*	License as published by the Free Software Foundation; either
//*	version 2.1 of the License, or (at your option) any later version.
//*	
//*	This library is distributed in the hope that it will be useful,
//*	but WITHOUT ANY WARRANTY; without even the implied warranty of
//*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//*	Lesser General Public License for more details.
//*	
//*	You should have received a copy of the GNU Lesser General
//*	Public License along with this library; if not, write to the
//*	Free Software Foundation, Inc., 59 Temple Place, Suite 330,
//*	Boston, MA  02111-1307  USA
/************************************************************************/

#if !defined(BOARD_DEFS_H)
#define BOARD_DEFS_H

#include <inttypes.h>

/* ------------------------------------------------------------ */
/*				Public Board Declarations						*/
/* ------------------------------------------------------------ */
/* The following define symbols that can be used in a sketch to
** refer to periperhals on the board generically.
*/

#define	_BOARD_NAME_	"HelvePic32A"
#define _USB

/* Define the peripherals available on the board.
*/
#define	NUM_DIGITAL_PINS	15
#define	NUM_ANALOG_PINS		9
#define NUM_OC_PINS			5
#define	NUM_IC_PINS			5
#define	NUM_TCK_PINS		5
#define	NUM_INT_PINS		5

#define	NUM_SERIAL_PORTS	2       
#define	NUM_SPI_PORTS		1
#define	NUM_I2C_PORTS		1       

#define NUM_DSPI_PORTS		1
#define NUM_DTWI_PORTS		1       

/* Define I/O devices on the board.
*/
#define	NUM_LED				1
#define NUM_BTN				1
#define	NUM_SWT				0
#define NUM_SERVO			0

/* ------------------------------------------------------------ */
/*						LED Declarations						*/
/* ------------------------------------------------------------ */

/* Define the pin numbers for the LEDs
*/
#define	PIN_LED1	10

/* ------------------------------------------------------------ */
/*					Button Declarations							*/
/* ------------------------------------------------------------ */

/* One button (PRG) for this board
*/
#define	PIN_BTN1	1 

/* Also define the virutal program button for soft reset */
#define USE_VIRTUAL_PROGRAM_BUTTON      1
#define VIRTUAL_PROGRAM_BUTTON_TRIS     TRISBbits.TRISB4
#define VIRTUAL_PROGRAM_BUTTON          LATBbits.LATB4

/* ------------------------------------------------------------ */
/*					Switch Declarations							*/
/* ------------------------------------------------------------ */

/* No switches on this board.
*/

/* ------------------------------------------------------------ */
/*					Servo Pin Declarations						*/
/* ------------------------------------------------------------ */

/* No servo connectors on this board.
*/

/* ------------------------------------------------------------ */
/*					Timer Pin Declarations						*/
/* ------------------------------------------------------------ */

#define PIN_OC1		 8      // B15   RPB15R  = 5  
#define	PIN_OC2		 2      // B8    RPB8R   = 5
#define	PIN_OC3		 3      // B9    RPB9R   = 5
#define	PIN_OC4		13      // B2    RPB2R   = 5
#define	PIN_OC5		 6      // B13   RPB13R  = 6

#define PIN_IC1		 6      // RB13 IC1R = RPB13 = 3
#define PIN_IC2		11      // B0   IC2R = RPB0  = 2
#define PIN_IC3		 2      // B8   IC3R = RPB8  = 4
#define PIN_IC4		14      // B3   IC4R = RPB3  = 1
#define	PIN_IC5		13      // RB2  IC5R = RPB2  = 4

#define	PIN_TCK1	18      // A4, non PPS
#define	PIN_TCK2	14      // B3   T2CKR = RPB3  = 1
#define	PIN_TCK3	12      // B1   T3CKR = RPB1  = 2
#define	PIN_TCK4	 6      // B13  T4CKR = RPB13 = 3
#define	PIN_TCK5	 3      // B9   T5CKR = RPB9  = 4

/* ------------------------------------------------------------ */
/*					Interrupt Pin Declarations					*/
/* ------------------------------------------------------------ */

#define	PIN_INT0	 1      // RB7 non-PPS
#define	PIN_INT1	 3      // B9  INT1R = RPB9 = 4
#define PIN_INT2	13      // B2  INT2R = RPB2 = 4
#define	PIN_INT3	 2      // B8  INT3R = RPB8 = 4
#define	PIN_INT4	17      // B4  INT4R = RPB4 = 2

/* ------------------------------------------------------------ */
/*					SPI Pin Declarations						*/
/* ------------------------------------------------------------ */
/* These symbols are defined for compatibility with the original
** SPI library and the original pins_arduino.h. 
*/
const static uint8_t SS   = 2;
const static uint8_t MISO = 3;
const static uint8_t MOSI =	8;
const static uint8_t SCK  = 14;

/* The Digilent DSPI library uses these ports.
*/
#define	PIN_DSPI0_SS	2
/* ------------------------------------------------------------ */
/*					Analog Pins									*/
/* ------------------------------------------------------------ */
/* Define symbols for accessing the analog pins. This table is  
** used to map an analog pin number to the corresponding digital
** pin number.
*/
#define	A0		 13
#define	A1		 14
#define A2		 15
#define A3		 2
#define A4		 3
#define A5		 4
#define A6		 5
#define A7		 6
#define A8		 7

/* ------------------------------------------------------------ */
/*					Change Notice Pins							*/
/* ------------------------------------------------------------ */
/* These define the pin numbers for the various change notice
** pins.
*/
#define	PIN_CN0	     0  //  0   J2-1    RB5     TMS/RPB5/USBID/RB5
#define	PIN_CN1	     1  //  1   J2-2    RB4     SOSCI/RPB4/RB4 
#define	PIN_CN2	     2  //  2   J2-3    RA0     PGED3/VREF+/CVREF+/AN0/C3INC/RPA0/CTED1/PMD7/RA0
#define	PIN_CN3	     3  //  3   J2-4    RA1     
#define	PIN_CN4      4  //  4   J2-5    RB0    
#define	PIN_CN5      5  //  5   J2-6    RB1    
#define	PIN_CN6      6  //  6   J2-7    RB2    
#define	PIN_CN7      7  //  7   J2-8    RB3    
#define	PIN_CN8      8  //  8   J4-1    RA4    
#define	PIN_CN9      9  //  9   J4-2    RB4    
#define	PIN_CN10    10  // 10   J4-3    RB7     
#define	PIN_CN11    11  // 11   J4-4    RB8     
#define	PIN_CN12    12  // 12   J4-5    RB9    
#define	PIN_CN13    13  // 13   J4-6    RB13    
#define	PIN_CN14    14  // 14   J4-7    RB14     
#define	PIN_CN15    15  // 15   J4-8    RB15     
#define	PIN_CN16    16  // 16    NC     RA2     OSC1/CLKI/RPA2/RA2     
#define	PIN_CN17    17  // 17    NC     RA3     OSC2/CLKO/RPA3/PMA0/RA3 

/* ------------------------------------------------------------ */
/*					Pin Mapping Macros							*/
/* ------------------------------------------------------------ */
/* Macros used to access the port and pin mapping tables.
** These are mostly generic, but some of them may be board specific.
** These perform slightly better as macros compared to inline functions
*/
#undef digitalPinToAnalog
#define	digitalPinToAnalog(P) ( digital_pin_to_analog_PGM[P] )

#undef analogInPinToChannel
#define analogInPinToChannel(P) ( analog_pin_to_channel_PGM[P]  )

/* ------------------------------------------------------------ */
/*					Data Definitions							*/
/* ------------------------------------------------------------ */

/* The following declare externals to access the pin mapping
** tables. These tables are defined in Board_Data.c.
*/

#if !defined(OPT_BOARD_DATA)

extern const uint32_t	port_to_tris_PGM[];
extern const uint8_t	digital_pin_to_port_PGM[];
extern const uint16_t	digital_pin_to_bit_mask_PGM[];
extern const uint16_t	digital_pin_to_timer_PGM[];
extern const uint8_t	digital_pin_to_pps_out_PGM[];
extern const uint8_t	digital_pin_to_pps_in_PGM[];
extern const uint8_t 	digital_pin_to_analog_PGM[];
extern const uint8_t	analog_pin_to_channel_PGM[];

extern const uint8_t	output_compare_to_digital_pin_PGM[];
extern const uint8_t	external_int_to_digital_pin_PGM[];

#endif

/* ------------------------------------------------------------ */
/*				Internal Declarations							*/
/* ------------------------------------------------------------ */
/* The following declarations are used to map peripherals for	*/
/* the core and libraries and to provide configuration options	*/
/* for the core. They are not normally needed by a user sketch.	*/
/* ------------------------------------------------------------ */

#if defined(OPT_BOARD_INTERNAL)

/* ------------------------------------------------------------ */
/*				Core Configuration Declarations					*/
/* ------------------------------------------------------------ */
/*																*/
/* These are conditional compilation switches that control the	*/
/* board core configuration functions. These functions provide	*/
/* hooks that can call from some of the core functions into		*/
/* functions defined below that can be used to extend or		*/
/* replace the default behavior of the core function. To use	*/
/* this, enter the appropriate code into the appropriate		*/
/* function skeleton below and then set the appropriate switch	*/
/* value to 1. This will cause the configuration function to be	*/
/* compiled into the build and will cause the code to call the	*/
/* hook function to be compiled into the core function.			*/
/*																*/
/* ------------------------------------------------------------ */

#define	OPT_BOARD_INIT			1	//board needs special init code
#define	OPT_BOARD_DIGITAL_IO	0	//board does not extend digital i/o functions
#define	OPT_BOARD_ANALOG_READ	0	//board does not extend analogRead
#define	OPT_BOARD_ANALOG_WRITE	0	//board does not extend analogWrite

/* ------------------------------------------------------------ */
/*					Serial Port Declarations					*/
/* ------------------------------------------------------------ */

/* Serial port 0 uses UART1 ? for the serial monitor
*/
#define       _SER0_BASE           _UART1_BASE_ADDRESS
#define       _SER0_IRQ            _UART1_ERR_IRQ
#define       _SER0_VECTOR         _UART_1_VECTOR
#define       _SER0_IPL_ISR        _UART1_IPL_ISR
#define       _SER0_IPL            _UART1_IPL_IPC
#define       _SER0_SPL            _UART1_SPL_IPC
#define       _SER0_TX_OUT         PPS_OUT_U1TX     // RPB3R = U1TX = 1   
#define       _SER0_TX_PIN         7
#define       _SER0_RX_IN          PPS_IN_U1RX      // U1RXR = RPB13 = 3
#define       _SER0_RX_PIN         13


/* Serial port 1 uses UART2
*/
#define       _SER1_BASE           _UART2_BASE_ADDRESS
#define       _SER1_IRQ            _UART2_ERR_IRQ
#define       _SER1_VECTOR         _UART_2_VECTOR
#define       _SER1_IPL_ISR        _UART2_IPL_ISR
#define       _SER1_IPL            _UART2_IPL_IPC
#define       _SER1_SPL            _UART2_SPL_IPC
#define       _SER1_TX_OUT         PPS_OUT_U2TX     // RPB14R = U2TX = 2
#define       _SER1_TX_PIN         14
#define       _SER1_RX_IN          PPS_IN_U2RX      // U2RXR = RPA1 = 0
#define       _SER1_RX_PIN         3


/* ------------------------------------------------------------ */
/*					SPI Port Declarations						*/
/* ------------------------------------------------------------ */

/* The default SPI port uses SPI1.
*/
#define	_SPI_BASE		_SPI1_BASE_ADDRESS
#define _SPI_ERR_IRQ	_SPI1_ERR_IRQ
#define	_SPI_RX_IRQ		_SPI1_RX_IRQ
#define	_SPI_TX_IRQ		_SPI1_TX_IRQ
#define	_SPI_VECTOR		_SPI_1_VECTOR
#define	_SPI_IPL_ISR	_SPI1_IPL_ISR
#define	_SPI_IPL		_SPI1_IPL_IPC
#define	_SPI_SPL		_SPI1_SPL_IPC

/* SPI pin declarations
*/
#define _SPI_MISO_IN	PPS_IN_SDI1
#define	_SPI_MISO_PIN	MISO
#define _SPI_MOSI_OUT	PPS_OUT_SDO1
#define	_SPI_MOSI_PIN	MOSI

/* SPI1 
*/
// RA0  CS1     PGED3/VREF+/CVREF+/AN0/C3INC/RPA0/CTED1/PMD7/RA0 
// RA4  SDO1    RPA4R = SDO1 = 3    
// RA1  SDI1    SDI1R = RPA1 = 0 
// RB14 SCK1    CVREF/AN10/C3INB/RPB14/VBUSON/SCK1/CTED5/RB14
#define	_DSPI0_BASE			_SPI1_BASE_ADDRESS
#define	_DSPI0_ERR_IRQ		_SPI1_ERR_IRQ
#define	_DSPI0_RX_IRQ		_SPI1_RX_IRQ
#define	_DSPI0_TX_IRQ		_SPI1_TX_IRQ
#define	_DSPI0_VECTOR		_SPI_1_VECTOR
#define	_DSPI0_IPL_ISR		_SPI1_IPL_ISR
#define	_DSPI0_IPL			_SPI1_IPL_IPC
#define	_DSPI0_SPL			_SPI1_SPL_IPC

#define _DSPI0_MISO_IN		PPS_IN_SDI1
#define _DSPI0_MISO_PIN		MISO
#define _DSPI0_MOSI_OUT		PPS_OUT_SDO1
#define _DSPI0_MOSI_PIN		MOSI


/* ------------------------------------------------------------ */
/*					I2C Port Declarations						*/
/* ------------------------------------------------------------ */

/* The standard I2C1 port uses I2C1 (SCL1/SDA1). 
** RB8/RB9 pins 38/4
*/
#define	_TWI_BASE		_I2C1_BASE_ADDRESS
#define	_TWI_BUS_IRQ	_I2C1_BUS_IRQ
#define	_TWI_SLV_IRQ	_I2C1_SLAVE_IRQ
#define	_TWI_MST_IRQ	_I2C1_MASTER_IRQ
#define	_TWI_VECTOR		_I2C_1_VECTOR
#define	_TWI_IPL_ISR	_I2C1_IPL_ISR
#define _TWI_IPL		_I2C1_IPL_IPC
#define	_TWI_SPL		_I2C1_SPL_IPC

/* Declarations for Digilent DTWI library.
**		DTWI0 is on RB8/RB9 pins 38/4 
*/
#define	_DTWI0_BASE		_I2C1_BASE_ADDRESS
#define	_DTWI0_BUS_IRQ	_I2C1_BUS_IRQ
#define	_DTWI0_SLV_IRQ	_I2C1_SLAVE_IRQ
#define	_DTWI0_MST_IRQ	_I2C1_MASTER_IRQ
#define	_DTWI0_VECTOR	_I2C_1_VECTOR
#define	_DTWI0_IPL_ISR	_I2C1_IPL_ISR
#define	_DTWI0_IPL		_I2C1_IPL_IPC
#define	_DTWI0_SPL		_I2C1_SPL_IPC

#define _DTWI0_SCL_PIN  2
#define _DTWI0_SDA_PIN  3

/* ------------------------------------------------------------ */
/*					A/D Converter Declarations					*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */

#endif	// OPT_BOARD_INTERNAL

/* ------------------------------------------------------------ */

#endif	// BOARD_DEFS_H

/************************************************************************/
