#include "I2C_ACCEL.h"

// Initialize the accelerometer
void I2CACCEL_Init(ARM_DRIVER_I2C* i2cDrv) {
	// Initilize I2C
	i2cDrv->Initialize(NULL);

	// Enable I2C at full performance, configure the I2C speed to
	// Fast Speed (400kHz), and execute a bus clear operation.
	i2cDrv->PowerControl(ARM_POWER_FULL);
	i2cDrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
	i2cDrv->Control(ARM_I2C_BUS_CLEAR, 0);
}

// Set value 'regValue' to register 'regId' from device with
// address 'deviceAddr'
void I2CACCEL_SetRegister(ARM_DRIVER_I2C* i2cDrv, uint32_t deviceAddr,
                          uint8_t regId, uint8_t regValue) {
	ARM_I2C_STATUS status;
	static uint8_t buffer[2];

	// Fill in a buffer with the register number and value to be set
	buffer[0] = regId;
	buffer[1] = regValue;

	// Send the buffer to the device specifying that no chained
	// operations will follow, and wait until operation is completed.
	i2cDrv->MasterTransmit(deviceAddr, buffer, 2, false);
	do { status = i2cDrv->GetStatus(); } while(status.busy);
}

// Get value from register 'regId' from device with
// address 'deviceAddr'
uint8_t I2CACCEL_GetRegister(ARM_DRIVER_I2C* i2cDrv, uint32_t deviceAddr,
                             uint8_t regId) {
	ARM_I2C_STATUS status;
	uint8_t result;

	// Send a buffer (implicit with 1 byte specifying the register
	// to be read) to the device specifying that a chained operation
	// will follow, and wait until operation is completed.
	i2cDrv->MasterTransmit(deviceAddr, &regId, 1, true);
	do { status = i2cDrv->GetStatus(); } while(status.busy);
	
	// Send a read request and place result in 'result' variable
	// specifying that no chained operations will follow, and wait
	// until operation is completed.
	i2cDrv->MasterReceive(deviceAddr, &result, 1, false);
	do { status = i2cDrv->GetStatus(); } while(status.busy);
	
	return(result);
}

// Configure the accelerometer
void I2CACCEL_Configure(ARM_DRIVER_I2C* i2cDrv) {
	// Check the Accelerometer Datasheet for further information
	// File in Moodle: "Acelerómetro 3 ejes MMA7660FC Archivo"
	// Page 14: REGISTER DEFINITIONS
	// Note that commands are written as hex bytes here, but as binary
	// words in the document.
	I2CACCEL_SetRegister(i2cDrv, ACCEL_ADDR, 0x07, 0x00);
	I2CACCEL_SetRegister(i2cDrv, ACCEL_ADDR, 0x05, 0x00);
	I2CACCEL_SetRegister(i2cDrv, ACCEL_ADDR, 0x06, 0x10);
	I2CACCEL_SetRegister(i2cDrv, ACCEL_ADDR, 0x09, 0xE0);
	I2CACCEL_SetRegister(i2cDrv, ACCEL_ADDR, 0x08, 0x62);
	I2CACCEL_SetRegister(i2cDrv, ACCEL_ADDR, 0x0A, 0x00);
	I2CACCEL_SetRegister(i2cDrv, ACCEL_ADDR, 0x07, 0x41);
}

// Get the measurement for axis 'axis' from the accelerometer
int8_t I2CACCEL_GetValue(ARM_DRIVER_I2C* i2cDrv, uint8_t axis) {
	// ------------------------------------------------------------
	// Add your code here ...
	// Note: you can use function I2CACCEL_GetRegister() to get the
	// raw values from the register and then process the values
	
	
	uint8_t reg, ch = 0x40;

	reg = I2CACCEL_GetRegister(i2cDrv,ACCEL_ADDR,axis);

	while((reg & (ch)) == 1){
		reg = I2CACCEL_GetRegister(i2cDrv,ACCEL_ADDR,axis);
	}
	if((reg & (0x20)) == 0){ //es positivo //0x05
		reg &= 0x1F; //completar los 8 bits con 0`s
	}
	else{ //es negativo
	 reg |= 0xE0; //cmpletar los 8 bits con 1`s
	}
	
	return((int8_t)reg);
	
	
	// ------------------------------------------------------------
}


bool I2CACCEL_Shake(ARM_DRIVER_I2C* i2cDrv) { //funcion para usar la interrupcion de la funcion shake
	// ------------------------------------------------------------

	uint8_t tilt; //variable para acceder al registro tilt
	bool flag_shake = 0; //flag de la interrupcion shake

	I2CACCEL_SetRegister(i2cDrv, ACCEL_ADDR, INTSU_REGISTER, 0xE0); //habilitar la funcion shake en el reistro intsu (habilitar bits 7:5, es decir, 
																																	//habilitar el shake en los 3 ejes
	tilt = I2CACCEL_GetRegister(i2cDrv,ACCEL_ADDR,TILT_REGISTER);	//acceder el registro tilt

	if((tilt & (0x80)) == 1){  //leer el registro tilt en el bit 7 para saber si ocurrio una interrupcion del tipo shake
		flag_shake = 1; //si lo hizo, flag shake positiva
		return flag_shake; //regresar el valor shake 
	}
	// ------------------------------------------------------------
}
