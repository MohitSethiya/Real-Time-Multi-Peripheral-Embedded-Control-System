//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : PCF8574A.C
// FILE VERSION : 1.0
// PROGRAMMER   : Programmer Name
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, YYYY-MM-DD, Programmer Name
//   - Initial release
//
//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
// Remote 8-Bit I/O Expander
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "Include/pcf8574a.h"
#include "Include/i2c.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Init( void )
// PURPOSE  : Initializes the I/O expander
//----------------------------------------------------------------------------

void PCF8574A_Init( void )
{
    uint8_t uiData;

    // Configure LEDs and switches
    uiData = 0xE7;
    PCF8574A_Write( PCF8574A_SA, uiData );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Write( uint8_t uiSA, uint8_t uiData )
// PURPOSE  : Writes data to the I/O expander
//----------------------------------------------------------------------------

void PCF8574A_Write( uint8_t uiSA, uint8_t uiData )
{
    uint8_t uiMCS;

    // Write slave address and R/W = 0
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( uiSA << 1 );

    // Write data to transmit register
    HWREG( I2C0_BASE + I2C_O_MDR ) = uiData;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    I2C_WaitForControllerReady();

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Read( uint8_t uiSA, uint8_t* puiData )
// PURPOSE  : Reads data from the I/O expander
//----------------------------------------------------------------------------

void PCF8574A_Read( uint8_t uiSA, uint8_t* puiData )
{
    uint8_t uiMCS;

    // Write slave address and R/W = 1
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( uiSA << 1 ) | 0x01;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    I2C_WaitForControllerReady();

    // Read data from receive register
    *puiData = HWREG( I2C0_BASE + I2C_O_MDR );

    return;
}

//----------------------------------------------------------------------------
// END PCF8574A.C
//----------------------------------------------------------------------------
