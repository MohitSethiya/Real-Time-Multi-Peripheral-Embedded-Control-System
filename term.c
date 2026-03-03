//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : TERM.C
// FILE VERSION : 1.0
// PROGRAMMER   : Mohit Sethiya
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, 2025-01-28, Mohit Sethiya
//   - Initial release
//
//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
// Code to support a VT100 compatible terminal.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "Include/term.h"
#include "stdio.h"
#include "Include/led.h"
#include "Include/lcd.h"


//----------------------------------------------------------------------------
// Global Definition
//----------------------------------------------------------------------------

/*
typedef enum {
    CSCRN_EL507 = 1,
    CSCRN_VOLTAGE,
    CSCRN_CURRENT,
    CSCRN_POWER,
    CSCRN_TEMP,
} LCDMENU;
*/


//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : TERM_Init( void )
// PURPOSE  : Initializes the terminal interface
//----------------------------------------------------------------------------

void TERM_Init( void )
{
    // Clear the screen and display information
    UART_SendMessage( "\e[2J\e[2H" ); // Clear screen and home cursor
    UART_SendMessage( "\e(B" );  // ASCII Set
    UART_SendMessage( "\e[0m" ); // Normal Attributes

    UART_SendMessage( "-----------------------------------\r\n" );
    UART_SendMessage(  "Firmware    : EL 507 - LAB 12\r\n" );
    UART_SendMessage(  "System      : Speed Control \r\n" );
    UART_SendMessage(  "Company     : Confederation College\r\n" );
    UART_SendMessage(  "Platform    : TM4C123GH6PM \r\n" );
    UART_SendMessage(  "Date        : 2025-12-10\r\n" );
    UART_SendMessage(  "Programmer  : Mohit Sethiya\r\n" );
    UART_SendMessage( "-----------------------------------\r\n" );
    TERM_SetPos(0,12 );
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_SetPos( uint8_t x, uint8_t y )
// PURPOSE  : Moves the cursor to the specified x and y position
//----------------------------------------------------------------------------

void TERM_SetPos( uint8_t x, uint8_t y )
{
    char sBuffer[ 16 ];
    sprintf( sBuffer, "\e[%u;%uH", y, x );
    UART_SendMessage( sBuffer );
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_SaveCursor( void )
// PURPOSE  : Save cursor and attributes
//----------------------------------------------------------------------------

void TERM_SaveCursor( void )
{
    UART_SendMessage( "\e7" );
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_RestoreCursor( void )
// PURPOSE  : Restore cursor and attributes
//----------------------------------------------------------------------------

void TERM_RestoreCursor( void )
{
    UART_SendMessage( "\e8" );
}


//----------------------------------------------------------------------------
// FUNCTION : TERM_Refresh ( void)
// PURPOSE  : Refreshes the terminal
//----------------------------------------------------------------------------

void TERM_Refresh ( void) {
   TERM_Init();
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_Clear( void )
// PURPOSE  : Clear the screen
//----------------------------------------------------------------------------
void TERM_Clear( void )
{
    // Clear the screen and display information
    UART_SendMessage( "\e[2J\e[2H" ); // Clear screen and home cursor
    UART_SendMessage( "\e(B" );  // ASCII Set
    UART_SendMessage( "\e[0m" ); // Normal Attributes
}

//----------------------------------------------------------------------------
// FUNCTION : TERM_DisplayUpTime(void)
// PURPOSE  : Displays the up time of microcontroller
//----------------------------------------------------------------------------

void TERM_DisplayUpTime(void) {
    static uint16_t UpCount = 0;
    static uint8_t seconds = 0;
    static uint8_t minutes = 0;
    char buffer[10];
    TERM_SaveCursor();
    if (UpCount++ >= 1000) {
        UpCount = 0;
        if (++seconds >= 60) {
            seconds = 0;
            minutes++;
        }
        UART_SendMessage( "\e[?25l");
        sprintf(buffer, "%02u:%02u", minutes, seconds);
        UART_SendMessage("\e(B");
        UART_SendMessage("\e[0m");
        TERM_SetPos(16, 9);
        UART_SendMessage(buffer);
        TERM_RestoreCursor();
        UART_SendMessage( "\e[?25h");
    }
}


//----------------------------------------------------------------------------
// FUNCTION : TERM_HideCursor( void )
// PURPOSE  : Hides the cursor
//----------------------------------------------------------------------------

void TERM_HideCursor( void )
{
    UART_SendMessage( "\e[?25l" );
}


//----------------------------------------------------------------------------
// FUNCTION : TERM_ShowCursor( void )
// PURPOSE  : Shows the cursor
//----------------------------------------------------------------------------

void TERM_ShowCursor( void )
{
    UART_SendMessage( "\e[?25h" );
}

//----------------------------------------------------------------------------
// FUNCTION : Cause_of_Reset( uint8_t tempcount )
// PURPOSE  : Displays the reason of the reset
//----------------------------------------------------------------------------

void Cause_of_Reset(uint8_t tempcount) {
    TERM_SaveCursor();
    TERM_SetPos(0, 11);
    switch(tempcount) {
    case 3:
        UART_SendMessage("System Restart: External Reset");
        TERM_SetPos(1,13);
        TERM_SaveCursor();
        break;
    case 4:
        UART_SendMessage("System Restart: Power-on Reset");
        TERM_SetPos(1,13);
        TERM_SaveCursor();
        break;
    case 5:
        UART_SendMessage("System Restart: Brown-out Reset");
        TERM_SetPos(1,13);
        TERM_SaveCursor();
        break;
    case 6:
        UART_SendMessage("System Restart: Watchdog Timer Reset");
        TERM_SetPos(1,13);
        TERM_SaveCursor();
        break;
    case 7:
        UART_SendMessage("System Restart: Software Reset");
        TERM_SetPos(1,13);
        TERM_SaveCursor();
        break;
    case 8:
        UART_SendMessage("System Restart: Main Oscillator Failure Reset");
        TERM_SetPos(1,13);
        TERM_SaveCursor();
        break;
    default:
        UART_SendMessage("System Restart: Power Failure");
        TERM_SetPos(1,13);
        TERM_SaveCursor();
        break;
    }
}

//----------------------------------------------------------------------------
// END TERM.C
//----------------------------------------------------------------------------
