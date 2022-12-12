#include <stdint.h>

typedef struct XYZ
{
    volatile uint16_t x_data;
    volatile uint16_t y_data;
    volatile uint16_t z_data;
} xyz_data;

void initiate_spi();
void I2C_setup();
xyz_data getACCL_XYZ_I2C();
void delay_I2C();
void ACCEL_config_I2C();
