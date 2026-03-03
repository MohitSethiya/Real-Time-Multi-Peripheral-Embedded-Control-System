//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LCD.C
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
// LCD Module Driver API.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <Include/lcd.h>
#include <Include/systick.h>

//----------------------------------------------------------------------------
// FUNCTION : LCD_Init( void )
// PURPOSE  : Initializes the LCD module
//----------------------------------------------------------------------------

void LCD_Init( void )
{
    // GPIO Port A and E Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCGPIO ) |= 0x11;

    // Configure Data Bus (initially input)
    HWREG( GPIO_PORTA_BASE + GPIO_O_DEN  ) |= LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DIR  ) &= ~LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_PDR  ) |= LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DR4R ) |= LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( LCD_BUS << 2 ) ) = 0;

    // Configure Control Signals
    HWREG( GPIO_PORTE_BASE + GPIO_O_DEN  ) |= LCD_CONTROL;
    HWREG( GPIO_PORTE_BASE + GPIO_O_DIR  ) |= LCD_CONTROL;
    HWREG( GPIO_PORTE_BASE + GPIO_O_DR4R ) |= LCD_CONTROL;
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_CONTROL << 2 ) ) = 0;

    // Turn power off to the LCD module
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_PWR << 2 ) ) = 0;

    // Delay prior to power-up
    SYSTICK_Delay( 10 );

    // Power-up the LCD module
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_PWR << 2 ) ) = LCD_PWR;

    // Delay after power-up
    SYSTICK_Delay( 50 );

    // Set to 4-bit mode
    LCD_WriteNibble( 0, LCD_IC_FUNCTION );
    SYSTICK_Delay( 1 );

    // Configure LCD Module
    LCD_SendInstruction( LCD_IC_FUNCTION | LCD_IC_FUNCTION_2LINE | LCD_IC_FUNCTION_ON );
    LCD_SendInstruction( LCD_IC_DISPLAY | LCD_IC_DISPLAY_ON );
    LCD_SendInstruction( LCD_IC_CLEAR );
    LCD_SendInstruction( LCD_IC_ENTRYMODE | LCD_IC_ENTRYMODE_INC );

    // Set CGRAM address to store the custom character (e.g., address 0x00)
    LCD_Write(0, LCD_IC_CGRAMADDR + 0x00); // Degree Symbol (Accessible as 0x00)

    // Define the 5x8 pixel pattern for the raised degree symbol
    LCD_Write(1, 0x0E); // .###.
    LCD_Write(1, 0x11); // #...#
    LCD_Write(1, 0x0E); // .###.
    LCD_Write(1, 0x00); // .....
    LCD_Write(1, 0x00); // .....
    LCD_Write(1, 0x00); // .....
    LCD_Write(1, 0x00); // .....
    LCD_Write(1, 0x00); // .....

    // LCD Menu

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_SendMessage( char* sMessage )
// PURPOSE  : Writes a zero-terminated string to the LCD module
//----------------------------------------------------------------------------

void LCD_SendMessage( char* sMessage )
{
    // Send entire message at the current cursor position until
    // the NULL character is reached

    while( *sMessage )
    {
        LCD_Write( 1, ( uint8_t )*sMessage++ );
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_SendInstruction( uint8_t uiInstruction )
// PURPOSE  : Writes one 8-bit instruction to the LCD module
//----------------------------------------------------------------------------

void LCD_SendInstruction( uint8_t uiInstruction )
{
    LCD_Write( 0, uiInstruction );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_WaitForReady( void )
// PURPOSE  : Returns only when the LCD module is ready
//----------------------------------------------------------------------------

void LCD_WaitForReady( void )
{
    while( LCD_Read( 0 ) & LCD_IC_STATUS_BUSY );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_Write( uint8_t uiRS, uint8_t uiData )
// PURPOSE  : Writes one byte to the LCD module
//----------------------------------------------------------------------------

void LCD_Write( uint8_t uiRS, uint8_t uiData )
{
    LCD_WaitForReady();

    LCD_WriteNibble( uiRS, uiData );
    LCD_WriteNibble( uiRS, uiData << 4 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_Read( uint8_t uiRS )
// PURPOSE  : Reads one byte from the LCD module
//----------------------------------------------------------------------------

uint8_t LCD_Read( uint8_t uiRS )
{
    uint8_t uiData;

    uiData = LCD_ReadNibble( uiRS );
    uiData |= ( ( LCD_ReadNibble( uiRS ) >> 4 ) & 0x0F );

    return uiData;
}

//----------------------------------------------------------------------------
// FUNCTION : UpdateTransitionCount( int32_t iTC )
// PURPOSE  : Updates text on LCD using switch
//----------------------------------------------------------------------------

//void UpdateTransitionCount( int16_t iTC ) {
    /*switch(iTC) {
       case CSCRN_EL507:
           LCD_SendInstruction(LCD_IC_CLEAR);
           LCD_SendMessage("EL 507 LAB 8:");
           LCD_SendInstruction(LCD_IC_DDRAMADDR + 0x40);
           LCD_SendMessage("Embedded Systems");
           break;

       case CSCRN_VOLTAGE:
           LCD_SendInstruction(LCD_IC_CLEAR);
           LCD_SendMessage("Voltage: 19.1 V");
           break;

       case CSCRN_CURRENT:
           LCD_SendInstruction(LCD_IC_CLEAR);
           LCD_SendMessage("Current: 81.6 mA");
           break;

       case CSCRN_POWER:
           LCD_SendInstruction(LCD_IC_CLEAR);
           LCD_SendMessage("Power  : 42.7 mW");
           break;

       case CSCRN_TEMP:
           LCD_SendInstruction(LCD_IC_CLEAR);
           LCD_SendMessage("Temp   : 20.3\xDF C");
           LCD_SendInstruction(LCD_IC_DDRAMADDR + 0x40);
           LCD_SendMessage("         68.5\xDF F");
           break;
       }*/

//----------------------------------------------------------------------------
// FUNCTION : LCD_Menu( int32_t iTC )
// PURPOSE  : Show the LCD screen
//----------------------------------------------------------------------------
const char g_aLCDScreens[ 3 ][ 2 ][ 17 ] = {
{
 { "EL 507 : LAB 12  "},      // Screen 1, Line 1
 { "Embedded Systems"}       // Screen 1, Line 2
},
{
 { "SPEED: XXX.X RPM"},     // Screen 2, Line 1
 {}
},
{
 {"Time: ##:##:##  "},      // Screen 6, Line 1
 { "Date: ####:##:##"}      // Screen 6, Line 2
}
};

void LCD_Menu( int32_t iTC ) {
    LCD_SendInstruction(LCD_IC_CLEAR);
    LCD_SendMessage(( char* )g_aLCDScreens[iTC-1][0]);
    LCD_SendInstruction(LCD_IC_DDRAMADDR + 0x40);
    LCD_SendMessage(( char* )g_aLCDScreens[iTC-1][1]);
}

//----------------------------------------------------------------------------
// END LCD.C
//----------------------------------------------------------------------------
