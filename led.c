//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LED.C
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
// Controls the LED and LED sequencer using a software encoded state machine.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "Include/led.h"
#include "Include/cause_of_reset.h"
#include "Include/uart.h"
#include "Include/term.h"
#include "Include/pcf8574a.h"
//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
static uint8_t tempcount = 0;
uint8_t uiData;
//----------------------------------------------------------------------------
// FUNCTION : LED_Init( void )
// PURPOSE  : Initializes the GPIO pin to control the LED
//----------------------------------------------------------------------------

void LED_Init( void )
{
    // Enable GPIO Port F
    HWREG( SYSCTL_RCGCGPIO ) |= 0x20;

    // Configure PF1 as an output
    HWREG( GPIO_PORTF_BASE + GPIO_O_DEN ) |= 0x0E;
    HWREG( GPIO_PORTF_BASE + GPIO_O_DIR ) |= 0x0E;
    HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x0E << 2 ) ) = 0;

    // Configure PF4 as an switch 2
    HWREG(GPIO_PORTF_BASE + GPIO_O_DEN) |= 0x10;
    HWREG(GPIO_PORTF_BASE + GPIO_O_DIR) &= ~0x10;
    HWREG(GPIO_PORTF_BASE + GPIO_O_PUR) |= 0x10;

    // Configure PF4 as an switch 0
    HWREG( GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x4C4F434B;
    HWREG( GPIO_PORTF_BASE + GPIO_O_CR) |= 0X01;

    HWREG(GPIO_PORTF_BASE + GPIO_O_DEN) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_DIR) &= ~0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_PUR) |= 0x01;

    tempcount = cause_of_reset();
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LED_LED1( bool bOn )
// PURPOSE  : Controls the state of the LED
//----------------------------------------------------------------------------

void LED_LED1( bool bOn )
{
    // Control the state of the LED based on bOn
    HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x02 << 2 ) ) = bOn ? 0x02 : 0;
    /*UART_SendMessage( "\e[?25l");
    TERM_SaveCursor();
    TERM_SetPos(17,5);
    UART_SendMessage(bOn ? "\e(0`\e(B" : " ");
    TERM_RestoreCursor();
    UART_SendMessage( "\e[?25h");*/
    return;
}

void LED_LED2( bool bOn )
{
    // Control the state of the LED based on bOn
    HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x04 << 2 ) ) = bOn ? 0x04 : 0;
    UART_SendMessage( "\e[?25l");
    TERM_SaveCursor();
    TERM_SetPos(17,6);
    UART_SendMessage(bOn ? "\e(0`\e(B" : " ");
    TERM_RestoreCursor();
    UART_SendMessage( "\e[?25h");
    return;
}

void LED_LED3( bool bOn )
{
    // Control the state of the LED based on bOn
    HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x08 << 2 ) ) = bOn ? 0x08 : 0;
    /*UART_SendMessage("LED 3: ");
    UART_SendMessage( "\e[?25l");
    TERM_SaveCursor();
    TERM_SetPos(9,2);
    UART_SendMessage(bOn ? "\e(0`\e(B" : " ");
    TERM_RestoreCursor();
    UART_SendMessage( "\e[?25h");*/
    UART_SendMessage(bOn ? "LED 3 ON" : "LED 3 OFF" );
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LED_LED4( bool bOn )
// PURPOSE  : Controls the state of the LED
//----------------------------------------------------------------------------


void LED_LED4( bool bOn ) {
    uint8_t uiData;
    PCF8574A_Read( PCF8574A_SA, &uiData );
    uiData = bOn ? uiData & ~PCF8574A_LED4 : uiData | PCF8574A_LED4;
    uiData |= PCF8574A_INPUTS;
    PCF8574A_Write( PCF8574A_SA, uiData );
}

//----------------------------------------------------------------------------
// FUNCTION : LED_LED5( bool bOn )
// PURPOSE  : Controls the state of the LED
//----------------------------------------------------------------------------
void LED_LED5( bool bOn ) {
    uint8_t uiData;
    PCF8574A_Read( PCF8574A_SA, &uiData );
    uiData = bOn ? uiData & ~PCF8574A_LED5 : uiData | PCF8574A_LED5;
    uiData |= PCF8574A_INPUTS;
    PCF8574A_Write( PCF8574A_SA, uiData );
}

//----------------------------------------------------------------------------
// FUNCTION : LED_LED6( bool bOn )
// PURPOSE  : Controls the state of the LED
//----------------------------------------------------------------------------
void LED_LED6( bool bOn ) {
    uint8_t uiData;
    PCF8574A_Read( PCF8574A_SA, &uiData );
    uiData = bOn ? uiData & ~PCF8574A_LED6 : uiData | PCF8574A_LED6;
    uiData |= PCF8574A_INPUTS;
    PCF8574A_Write( PCF8574A_SA, uiData );
}

//----------------------------------------------------------------------------
// FUNCTION : LED_FSM( bool bReset )
// PURPOSE  : LED Finite State Machine control
//----------------------------------------------------------------------------
void LED_FSM( bool bReset ) {
    static uint8_t uiSB = 0;
    static uint16_t uiTimer = 0;
    // Reset the state machine if necessary
    if( bReset )
    {
        uiSB = LED_FSM_RESET;
    }
    // Clock the state machine
    switch( uiSB ) {
    case LED_FSM_RESET:
        LED_LED1( 0 );
        uiSB    = LED_FSM_PAUSE1;
        uiTimer = LED_TIME_PAUSE1;
        Cause_of_Reset(tempcount);
        break;
    case LED_FSM_PAUSE1:
        if( !--uiTimer ) {
            uiSB    = LED_FSM_FPON;
            uiTimer = LED_TIME_FPON;
            LED_LED1( 1 );
        }
        break;
    case LED_FSM_FPON:
        if( !--uiTimer ) {
            tempcount--;
            uiSB    = LED_FSM_FPOFF;
             uiTimer = LED_TIME_FPOFF;
             LED_LED1( 0 );
        }
        if(!tempcount) {
            tempcount=0;
                 uiSB    = LED_FSM_PAUSE2;
                 uiTimer = LED_TIME_PAUSE2;
                 LED_LED1( 0 );
        }
        break;
    case LED_FSM_FPOFF:
        if( !--uiTimer ) {
            uiSB    = LED_FSM_FPON;
            uiTimer = LED_TIME_FPON;
            LED_LED1( 1 );
        }
        break;
     case LED_FSM_PAUSE2:
        if( !--uiTimer ) {
            uiSB    = LED_FSM_HBON;
            uiTimer = LED_TIME_HBON;
            LED_LED1( 1 );
        }
        break;
     case LED_FSM_HBON:
        if( !--uiTimer ) {
            uiSB    = LED_FSM_HBOFF;
            uiTimer = LED_TIME_HBOFF;
            LED_LED1( 0 );
        }
        break;
     case LED_FSM_HBOFF:
        if( !--uiTimer ) {
            uiSB    = LED_FSM_HBON;
            uiTimer = LED_TIME_HBON;
            LED_LED1( 1 );
        }
        break;
    default: // Unknown State
        LED_LED1( 0 );
        uiSB = LED_FSM_RESET;
        break;
    }
    return;
}


bool Toggle_LED2(void) {
    static bool state = true;
    state = !state;
    LED_LED2(state);
    UART_SendMessage("\e[?25l");
    TERM_SaveCursor();
    TERM_SetPos(17,6);
    UART_SendMessage(state ? "\e(0`\e(B" : " ");
    TERM_RestoreCursor();
    UART_SendMessage("\e[?25h");
    return state;
}

bool Toggle_LED3(void) {
    static bool state = true;
    state = !state;
    LED_LED3(state);
    UART_SendMessage("\e[?25l");
    TERM_SaveCursor();
    TERM_SetPos(17,7);
    UART_SendMessage(state ? "\e(0`\e(B" : " ");
    TERM_RestoreCursor();
    UART_SendMessage("\e[?25h");
    return state;
}



//----------------------------------------------------------------------------
// END LED.C
//----------------------------------------------------------------------------
