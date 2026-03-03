//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : UART.C
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
// Provides code to support UART0 for a serial communication interface.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "Include/uart.h"
#include "Include/queue.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

QUEUE* g_pQueueTransmit;
QUEUE* g_pQueueReceive;

//----------------------------------------------------------------------------
// FUNCTION : UART0_IntHandler( void )
// PURPOSE  : Interrupt handler
//----------------------------------------------------------------------------

void UART0_IntHandler( void )
{
    uint32_t uiIntStatus = HWREG( UART0_BASE + UART_O_MIS );

    // Check for transmit (FIFO empty) interrupt
    if( uiIntStatus & ( 1 << 5 ) )
    {
        // Clear interrupt
        HWREG( UART0_BASE + UART_O_ICR ) = ( 1 << 5 );

        // Continue transmitting anything left in the queue
        UART_TransmitFromQueue();
    }

    // Check for receive interrupt
    if( uiIntStatus & ( 1 << 4 ) )
    {
        // Clear interrupt
        HWREG( UART0_BASE + UART_O_ICR ) = ( 1 << 4 );

        // Transfer received data to queue
        UART_ReceiveToQueue();

        // Set a global flag to indicate that data was received
        GLOBAL_SetSysFlag( SYSFLAGS_UART_RXD );
    }
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_ReceiveToQueue( void )
// PURPOSE  : Receives information from UART0 and enqueues it to the queue
//----------------------------------------------------------------------------

void UART_ReceiveToQueue( void )
{
    uint8_t uiData;

    // Read data and queue
    uiData = HWREG( UART0_BASE + UART_O_DR );
    QUEUE_Enqueue( g_pQueueReceive, uiData );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_TransmitFromQueue( void )
// PURPOSE  : Dequeues information from the a queue and sends it via UART0
//----------------------------------------------------------------------------

void UART_TransmitFromQueue( void )
{
    uint8_t uiData;

    // If the transmitter holding register is empty, start a transmission
    if( HWREG( UART0_BASE + UART_O_FR ) & ( 1 << 7 ) )
    {
        // Remove byte from queue and transmit
        if( QUEUE_Dequeue( g_pQueueTransmit, &uiData ) )
        {
            HWREG( UART0_BASE + UART_O_DR ) = uiData;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_GetChar( uint8_t *puiData )
// PURPOSE  : Gets one character from the receive queue
//----------------------------------------------------------------------------

bool UART_GetChar( uint8_t *puiData )
{
    bool bResult = false;

    UART0_IntDisable();
    {
        bResult = QUEUE_Dequeue( g_pQueueReceive, puiData );
    }
    UART0_IntEnable();

    return bResult;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_PutChar( uint8_t uiData )
// PURPOSE  : Places one character into the transmit queue
//----------------------------------------------------------------------------

bool UART_PutChar( uint8_t uiData )
{
    bool bResult = false;

    UART0_IntDisable();
    {
        if( QUEUE_Enqueue( g_pQueueTransmit, uiData ) )
        {
            UART_TransmitFromQueue();
            bResult = true;
        }
    }
    UART0_IntEnable();

    return bResult;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_SendMessage( char* sMessage )
// PURPOSE  : Writes a message to the transmit queue
//----------------------------------------------------------------------------

void UART_SendMessage( char* sMessage )
{
    // Enqueue entire message using the UART transmit queue
    while( *sMessage )
    {
        if( UART_PutChar( *(uint8_t*)sMessage ) )
        {
            sMessage++;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_SendEncodedMessage( char* sMessage, uint8_t uiKey )
// PURPOSE  : Writes an encoded message the transmit queue
//----------------------------------------------------------------------------

void UART_SendEncodedMessage( char* sMessage, uint8_t uiKey )
{
    while( *sMessage )
    {
        if( UART_PutChar( *(uint8_t*)sMessage ^ uiKey ) )
        {
            sMessage++;
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_Init( void )
// PURPOSE  : Initializes the UART peripheral
//----------------------------------------------------------------------------

void UART_Init( void )
{
    // UART 0 Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCUART ) |= 0x01;

    // UART 0 Sleep Mode Clock Gating Control
    HWREG( SYSCTL_SCGCUART ) |= 0x01;

    // GPIO Port A Run Mode Clock Gating Control (Port A)
    HWREG( SYSCTL_RCGCGPIO ) |= 0x01;

    // GPIO Port A Digital Enable (UART U0Rx/U0Tx = PA0/PA1)
    HWREG( GPIO_PORTA_BASE + GPIO_O_DEN ) |= 0x03;

    // GPIO Port A Alternate Function Select (UART U0Rx/U0Tx = PA0/PA1)
    HWREG( GPIO_PORTA_BASE + GPIO_O_AFSEL ) |= 0x03;

    // GPIO Port A Port Control (assign as U0Rx and U0Tx)
    HWREG( GPIO_PORTA_BASE + GPIO_O_PCTL ) = ( 1 << 0 ) | ( 1 << 4 );

    // Initialize control (default)
    HWREG( UART0_BASE + UART_O_CTL ) = 0;

    // Compute the fractional baud rate divisor
    uint32_t uiDivisor = ( ( ( UART_CLOCK * 8 ) / UART_BAUDRATE ) + 1 ) / 2;

    // Set the baud rate
    HWREG( UART0_BASE + UART_O_IBRD ) = uiDivisor / 64;
    HWREG( UART0_BASE + UART_O_FBRD ) = uiDivisor % 64;

    // Set parity, data length, and number of stop bits
    HWREG( UART0_BASE + UART_O_LCRH ) = ( 0x03 << 5 );

    // Enable the UART in two operations
    // Bit 9: RXE
    // Bit 8: TXE
    // Bit 0: UARTEN
    HWREG( UART0_BASE + UART_O_CTL ) = ( ( 1 << 9 ) | ( 1 << 8 ) | ( 1 << 0 ) );

    // UART Interrupt Mask
    // Bit 5: TXIM
    // Bit 4: RXIM
    HWREG( UART0_BASE + UART_O_IM ) = ( ( 1 << 5 ) | ( 1 << 4 ) );

    // Enable Interrupts for UART0
    UART0_IntEnable();

    // Create a transmit queue
    g_pQueueTransmit = QUEUE_Create( 1024 );

    // Create a receive queue
    g_pQueueReceive = QUEUE_Create( 32 );

    return;
}

//----------------------------------------------------------------------------
// END UART.C
//----------------------------------------------------------------------------
