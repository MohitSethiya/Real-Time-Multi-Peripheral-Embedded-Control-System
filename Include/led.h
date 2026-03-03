//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LED.H
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

#ifndef LED_H_
#define LED_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <Include/global.h>

#define TIME_ADJ -94
//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void LED_Init( void );

void LED_LED1( bool bOn );
void LED_LED2( bool bOn );
void LED_LED3( bool bOn );
void LED_LED4( bool bOn );
void LED_LED5( bool bOn );
void LED_LED6( bool bOn );
void LED_FSM( bool bReset );


bool Toggle_LED2(void);
bool Toggle_LED3(void);

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define LED_TIME_HBON    250 // Heartbeat On-Time
#define LED_TIME_HBOFF   750 // Heartbeat Off-Time
#define LED_TIME_FPON    125 // Fast Pulse On-Time
#define LED_TIME_FPOFF   375 // Fast Pulse Off-Time
#define LED_TIME_PAUSE2  2375// Pause 2 Time
#define LED_TIME_PAUSE1  2000 + TIME_ADJ // Pause 1 Time

enum LED_FSM_STATES
{
    LED_FSM_RESET = 0,
    LED_FSM_PAUSE1,
    LED_FSM_FPON,
    LED_FSM_FPOFF,
    LED_FSM_PAUSE2,
    LED_FSM_HBON,
    LED_FSM_HBOFF,
};



#endif // LED_H_

//----------------------------------------------------------------------------
// END LED.H
//----------------------------------------------------------------------------
