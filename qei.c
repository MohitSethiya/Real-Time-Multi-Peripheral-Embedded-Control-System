//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : QEI.C
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
// Quadrature encoder interface support functions.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "Include/qei.h"
#include "Include/motor.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

extern MOTOR_CONTROL_PARAMS g_MCP;

//----------------------------------------------------------------------------
// FUNCTION : QEI0_IntHandler( void )
// PURPOSE  : Interrupt handler for QEI0 (control loop timer)
//----------------------------------------------------------------------------

void QEI0_IntHandler( void )
{
    // Acknowledge the interrupt
    HWREG( QEI0_BASE + QEI_O_ISC ) |= ( 1 << 1 );

    // Control the motor
    MOTOR_PID( &g_MCP );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : QEI_Init( float fdt )
// PURPOSE  : Quadrature encoder interface initialization.
//----------------------------------------------------------------------------

void QEI_Init( float fdt )
{
    // Configure the Quadrature Encoder Interface peripheral
    HWREG( SYSCTL_SRQEI ) |=  0x00000001; // Reset the QEI peripheral
    HWREG( SYSCTL_SRQEI ) &= ~0x00000001; // Remove reset

    HWREG( SYSCTL_RCGCQEI  ) |= 0x00000001; // Enable Clock for QEI 0
    HWREG( SYSCTL_RCGCGPIO ) |= 0x00000008; // Enable Clock for Port D

    // PD7 is a special pin (locked by default) - unlock PD7
    HWREG( GPIO_PORTD_BASE + GPIO_O_LOCK ) = 0x4C4F434B;
    HWREG( GPIO_PORTD_BASE + GPIO_O_CR   ) = 0x80;

    // Configure PD6 and PD7 for QEI 0
    HWREG( GPIO_PORTD_BASE + GPIO_O_DEN   ) |=  0xC0;
    HWREG( GPIO_PORTD_BASE + GPIO_O_DIR   ) &= ~0xC0;
    HWREG( GPIO_PORTD_BASE + GPIO_O_AFSEL ) |=  0xC0;
    HWREG( GPIO_PORTD_BASE + GPIO_O_PCTL  ) &=  0x00FFFFFF;
    HWREG( GPIO_PORTD_BASE + GPIO_O_PCTL  ) |=  0x66000000;

    // Calculate the load value based on the "delta t" argument fdt:
    uint32_t uiLoad = ( uint32_t )( ( QEI_SYSCLK * fdt ) + 0.5f ) - 1UL;

    // Configure QEI0 (80 MHz System Clock)
    HWREG( QEI0_BASE + QEI_O_CTL  )  = 0x00000628;
    HWREG( QEI0_BASE + QEI_O_LOAD )  = uiLoad;
    HWREG( QEI0_BASE + QEI_O_CTL  ) |= 0x00000001;

    // Enable the QEI0 interrupt (timer only)
    HWREG( QEI0_BASE + QEI_O_INTEN ) = ( 1 << 1 );
    HWREG( NVIC_EN0 ) = ( 1 << 13 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : QEI_GetSpeed( void )
// PURPOSE  : Returns the speed of the motor in RPM.
//----------------------------------------------------------------------------

float QEI_GetSpeed( void )
{
    // Calculates the speed of the 180SH-EN motor shaft in RPM (rotations per
    // minute).
    //
    // The motor encoder produces 11 pulses per motor shaft revolution
    // QEI0 is configured to produce 4 counts per pulse:
    //      1 count for PhA rising
    //      1 count for PhB rising
    //      1 count for PhA falling
    //      1 count for PhB falling
    //
    // The SPEED register contains the number of counts within the timer
    // interval, delta-t (LOAD register).
    //
    // The system clock (in Hz) is defined in the header file as QEI_SYSCLK.
    //
    // The time interval, delta-t is determined by:
    //
    //     dt = LOAD / QEI_SYSCLK
    //
    // The number of encoder events per second is:
    //
    //     EPSenc = SPEED / dt
    //
    // The number of rotations of the motor shaft per second is:
    //
    //     RPSmot = EPSenc / ( 11 * 7 )
    //
    // The number of rotations of the motor shaft per minute is:
    //
    //     RPMmot = RPSmot * 60
    //

    float fdt     = ( ( float )HWREG( QEI0_BASE + QEI_O_LOAD ) + 1.0f ) / QEI_SYSCLK;
    float fEPSenc = ( float )HWREG( QEI0_BASE + QEI_O_SPEED ) / fdt;
    float fRPSmot = fEPSenc / ( 4.0f * 11.0f );
    float fRPMmot = fRPSmot * 60.0f;

    return fRPMmot;
}

//----------------------------------------------------------------------------
// END QEI.C
//----------------------------------------------------------------------------
