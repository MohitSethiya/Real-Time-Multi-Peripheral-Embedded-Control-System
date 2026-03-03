//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : TERM.H
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
// INCLUSION LOCK
//----------------------------------------------------------------------------

#ifndef TERM_H_
#define TERM_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <Include/global.h>
#include <Include/uart.h>

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void TERM_Init( void );

void TERM_SetPos( uint8_t x, uint8_t y );

void TERM_SaveCursor( void );
void TERM_RestoreCursor( void );
void TERM_Clear( void );
void TERM_DisplayCursor(bool display);
void TERM_Refresh(void);
void TERM_DisplayUpTime(void);
void TERM_Timer(void);
void TERM_HideCursor(void);
void TERM_ShowCursor(void);
void Cause_of_Reset(uint8_t tempcount);
//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#endif // TERM_H_

//----------------------------------------------------------------------------
// END TERM.H
//----------------------------------------------------------------------------
