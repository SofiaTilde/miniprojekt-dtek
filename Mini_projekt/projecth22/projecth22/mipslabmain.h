// #include "pic32mx.h"	 /* Declarations of system-specific addresses etc */
// #include "p32_defs.h"	/* more defines, ports/regs */
#include "stdbool.h"

// #define _I2C1_BASE_ADDRESS 0xBF805000
// #define _I2C1_BUS_IRQ 29
// #define _I2C1_SLAVE_IRQ 30
// #define _I2C1_MASTER_IRQ 31
// #define _I2C_1_VECTOR 25

// #define	_DTWI0_BASE		_I2C1_BASE_ADDRESS
// #define	_DTWI0_BUS_IRQ	_I2C1_BUS_IRQ
// #define	_DTWI0_VECTOR	_I2C_1_VECTOR
// #define	_DTWI0_IPL_ISR	IPL3SOFT
// #define	_DTWI0_IPL		3
// #define	_DTWI0_SPL		0
// #define _DTWI0_SCL_PIN  46 
// #define _DTWI0_SDA_PIN  45

// F_CPU comes in through build time
#define CORETIMER_TICKS_PER_MICROSECOND		(F_CPU / 2 / 1000000UL)


// Functions

uint8_t readACCWhoAmI();
void setupi2c();
void check_If_Idle();
bool sendOneByte(uint8_t data);
uint8_t receiveOneByte();
void SendAck();
void sendNoAck();
void setStopCondition();
void setStartCondition();
void setRestartCondition();







// these are masks that can be used on I2C_STATUS.status dword
uint32_t static const   SLAVEADDR           = 0x0000FFFF;
uint32_t static const   SESSION_ID          = 0x000F0000;
uint32_t static const   GEN_CALL            = 0x10000000;
uint32_t static const   BUS_IN_USE          = 0x08000000;
uint32_t static const   MY_BUS              = 0x04000000;
uint32_t static const   MASTER_MODE         = 0x02000000;
uint32_t static const   SLAVE_MODE          = 0x01000000;
uint32_t static const   BUS_ERROR           = 0x00800000;
uint32_t static const   NAKING              = 0x00400000;
uint32_t static const   WRITE               = 0x00200000;
uint32_t static const   READ                = 0x00100000;

// This struct to hold the representation of status that can be read from i2c 
typedef struct 
{
	union
	{
		uint32_t            status;             // 32 bit status word for faster access
		struct 
		{
			uint16_t         addr;              // current slave address on the bus; only valid if fMyBus == true
			union 
			{
				uint16_t flags;
				struct
				{
					unsigned int    iSession    : 4;    // This is the current session ID, will change if another stop/start has occured
					bool            fRead       : 1;    // currently bytes are being recieved off of the bus
					bool            fWrite      : 1;    // currently bytes are being transmitted on the bus
					bool            fNacking    : 1;    // On read, a NAK will be asserted on the 9th bit
					bool            fBusError   : 1;    // a bus error has occured; interrupts are probably disabled
					bool            fSlave      : 1;    // I2C controller is in Slave mode
					bool            fMaster     : 1;    // I2C controller is in Master mode
					bool            fMyBus      : 1;    // You currently own the bus
					bool            fBusInUse   : 1;    // the bus is in use by you or someone else, there is a start conditon on the bus
					bool            fGenCall    : 1;    // In slave mode, the General call address will be accepted
					bool            resv        : 1;    // was deined 3, too large for bool
				};
			};
		};
	};
} I2C_STATUS;



// documentation: LSM9DS0 9-Axis IMU (0x1D or 0x1E for Accel/Mag, 0x6A or 0x6B for Gyro)
#define ACCELEROMETER_ADDR  0x1D;


// sub-"addresses" of registers in chip LSM9DS0
//////////////////////////////////////////
// LSM9DS0 Accel/Magneto (XM) Registers //
//////////////////////////////////////////
#define OUT_TEMP_L_XM		0x05
#define OUT_TEMP_H_XM		0x06
#define STATUS_REG_M		0x07
#define OUT_X_L_M			0x08
#define OUT_X_H_M			0x09
#define OUT_Y_L_M			0x0A
#define OUT_Y_H_M			0x0B
#define OUT_Z_L_M			0x0C
#define OUT_Z_H_M			0x0D
#define WHO_AM_I_XM			0x0F
#define INT_CTRL_REG_M		0x12
#define INT_SRC_REG_M		0x13
#define INT_THS_L_M			0x14
#define INT_THS_H_M			0x15
#define OFFSET_X_L_M		0x16
#define OFFSET_X_H_M		0x17
#define OFFSET_Y_L_M		0x18
#define OFFSET_Y_H_M		0x19
#define OFFSET_Z_L_M		0x1A
#define OFFSET_Z_H_M		0x1B
#define REFERENCE_X			0x1C
#define REFERENCE_Y			0x1D
#define REFERENCE_Z			0x1E
#define CTRL_REG0_XM		0x1F
#define CTRL_REG1_XM		0x20
#define CTRL_REG2_XM		0x21
#define CTRL_REG3_XM		0x22
#define CTRL_REG4_XM		0x23
#define CTRL_REG5_XM		0x24
#define CTRL_REG6_XM		0x25
#define CTRL_REG7_XM		0x26
#define STATUS_REG_A		0x27
#define OUT_X_L_A			0x28
#define OUT_X_H_A			0x29
#define OUT_Y_L_A			0x2A
#define OUT_Y_H_A			0x2B
#define OUT_Z_L_A			0x2C
#define OUT_Z_H_A			0x2D
#define FIFO_CTRL_REG		0x2E
#define FIFO_SRC_REG		0x2F
#define INT_GEN_1_REG		0x30
#define INT_GEN_1_SRC		0x31
#define INT_GEN_1_THS		0x32
#define INT_GEN_1_DURATION	0x33
#define INT_GEN_2_REG		0x34
#define INT_GEN_2_SRC		0x35
#define INT_GEN_2_THS		0x36
#define INT_GEN_2_DURATION	0x37
#define CLICK_CFG			0x38
#define CLICK_SRC			0x39
#define CLICK_THS			0x3A
#define TIME_LIMIT			0x3B
#define TIME_LATENCY		0x3C
#define TIME_WINDOW			0x3D
#define ACT_THS				0x3E
#define ACT_DUR				0x3F



