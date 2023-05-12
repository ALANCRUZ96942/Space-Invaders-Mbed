#ifndef I2C_ACCEL_H_
#define I2C_ACCEL_H_

#include <Driver_I2C.h>

// Constants for the accelerometer
#define ACCEL_ADDR 0x4C   // Accelerator device address

#define ACCEL_AXIS_X 0x00 // Accelerator X axis - XOUT Register
#define ACCEL_AXIS_Y 0x01 // Accelerator Y axis - YOUT Register
#define ACCEL_AXIS_Z 0x02 // Accelerator Z axis - ZOUT Register


#define INTSU_REGISTER 0x06 // registro intsu
#define TILT_REGISTER 0x03 // Registro TILT

// Initialize the accelerometer
void I2CACCEL_Init(ARM_DRIVER_I2C* i2cDrv);

// Set value 'regValue' to register 'regId' from device with
// address 'deviceAddr'
void I2CACCEL_SetRegister(ARM_DRIVER_I2C* i2cDrv, uint32_t deviceAddr,
                          uint8_t regId, uint8_t regValue);

// Get value from register 'regId' from device with
// address 'deviceAddr'
uint8_t I2CACCEL_GetRegister(ARM_DRIVER_I2C* i2cDrv, uint32_t deviceAddr,
                             uint8_t regId);

// Configure the accelerometer
void I2CACCEL_Configure(ARM_DRIVER_I2C* i2cDrv);

// Get the measurement for axis 'axis' from the accelerometer
int8_t I2CACCEL_GetValue(ARM_DRIVER_I2C* i2cDrv, uint8_t axis);

#endif /* I2C_ACCEL_H_ */
