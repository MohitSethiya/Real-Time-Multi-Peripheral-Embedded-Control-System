/*
 * max518bcpa.c
 *
 *  Created on: Nov 25, 2025
 *      Author: msethiya
 */

#include <Include/global.h>
#include <Include/i2c.h>
#include <Include/max518bcpa.h>

void contrast_adj(uint8_t uidata) {
    // Write slave address
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( MAX518_SLAVEADD << 1 );

    // Write register address to start reading
    HWREG( I2C0_BASE + I2C_O_MDR ) = 00;

    // Initiate I2C transaction
    HWREG( I2C0_BASE + I2C_O_MCS ) = 0x03;

    // Wait until the controller is no longer busy
    I2C_WaitForControllerReady();

    // Write register address to the transmit register
    HWREG( I2C0_BASE + I2C_O_MDR ) = uidata;

    // Initiate I2C transaction
    HWREG( I2C0_BASE + I2C_O_MCS ) = 0x05;

    // Wait until the controller is no longer busy
    I2C_WaitForControllerReady();
}
