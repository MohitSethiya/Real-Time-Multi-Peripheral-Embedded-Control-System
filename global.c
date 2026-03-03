//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : GLOBAL.C
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
// Global functions for the embedded system.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "Include/global.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

uint32_t g_uiSysFlags;
#pragma NOINIT(g_uiRESC)
uint32_t g_uiRESC;

//----------------------------------------------------------------------------
// FUNCTION : GLOBAL_InitSysFlags( void )
// PURPOSE  : Initializes system event flags to zero
//----------------------------------------------------------------------------

void GLOBAL_InitSysFlags( void )
{
    g_uiSysFlags = 0;
}

//----------------------------------------------------------------------------
// FUNCTION : GLOBAL_SetSysFlag( uint32_t uiSysFlag )
// PURPOSE  : Sets the specified system event flag
//----------------------------------------------------------------------------

void GLOBAL_SetSysFlag( uint32_t uiSysFlag )
{
    BBA( &g_uiSysFlags, uiSysFlag ) = 1;
}

//----------------------------------------------------------------------------
// FUNCTION : GLOBAL_ClearSysFlag( uint32_t uiSysFlag )
// PURPOSE  : Clears the specified system event flag
//----------------------------------------------------------------------------

void GLOBAL_ClearSysFlag( uint32_t uiSysFlag )
{
    BBA( &g_uiSysFlags, uiSysFlag ) = 0;
}

//----------------------------------------------------------------------------
// FUNCTION : GLOBAL_CheckSysFlag( uint32_t uiSysFlag )
// PURPOSE  : Returns the value of the specified system event flag
//----------------------------------------------------------------------------

bool GLOBAL_CheckSysFlag( uint32_t uiSysFlag )
{
    return BBA( &g_uiSysFlags, uiSysFlag );
}

//----------------------------------------------------------------------------
// END GLOBAL.C
//----------------------------------------------------------------------------
