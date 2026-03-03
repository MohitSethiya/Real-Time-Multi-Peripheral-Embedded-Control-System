    //----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MAIN.C
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
// Main program entry.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "Include/global.h"

#include "Include/Watchdog.h"

void WD_Init( void )
{
#ifdef USE_WD

    //Initialize Watchdog timer
    HWREG(WATCHDOG_LOAD + WATCHDOG_RCGCWD) = 0x01;

    // Wait for the WDT0 pheripheral to be ready
    while( !WATCHDOG_PWRD &0x01 );

    HWREG( WATCHDOG_LOAD + WATCHDOG_LOCK ) = 0x1ACCE551;    // Unlock
    HWREG( WATCHDOG_LOAD ) = WD_RELOAD;     // Load count
    HWREG( WATCHDOG_LOAD + WATCHDOG_WDTCTL ) = 0x07;        // Enable
    HWREG( WATCHDOG_LOAD + WATCHDOG_LOCK ) = 0;             // Lock
#endif
    return;
}

void WD_Reload( void )
{
    HWREG( WATCHDOG_LOAD + WATCHDOG_LOCK ) = 0x1ACCE551;          // Unlock
    HWREG( WATCHDOG_LOAD ) = WD_RELOAD;   // Reload count // 250ms timeout
    HWREG( WATCHDOG_LOAD + WATCHDOG_LOCK ) = 0;                   // Lock

}
