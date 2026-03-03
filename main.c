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

#include <Include/global.h>
#include <stdio.h>

#include "Include/sysclk.h"
#include "Include/systick.h"
#include "Include/probe.h"

#include "Include/led.h"

// Serial Communication
#include "Include/uart.h"
#include "Include/queue.h"
#include "Include/el507.h"

// Terminal Interfacing
#include "Include/term.h"

// Contact Bounce
#include "Include/contact.h"
#include "Include/switch.h"

// LCD Module
#include "Include/lcd.h"

// Data Acquisition
#include "Include/adc.h"

// External Peripherals
#include "Include/i2c.h"
#include "Include/pcf8574a.h"
#include "Include/mcp7940m.h"
#include "Include/max518bcpa.h"

// H-Bridge Motor Driver
#include "Include/motor.h"
#include "Include/qei.h"


//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
extern char* QUOTE_Retrieve (void);
extern MOTOR_CONTROL_PARAMS g_MCP;


//----------------------------------------------------------------------------
// PROGRAM OPTIONS
//----------------------------------------------------------------------------
#define DEBOUNCE
#define Max_Number 25
#define Min_Number -25
#define fVREFP 3.303f
#define fVREFN 0.0f


//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------
#ifdef DEBOUNCE

CONTACT g_SW3; // PF0
CONTACT g_SW2; // PF4
CONTACT g_SW4;
CONTACT g_SW5;
CONTACT g_SW6;

#endif

float g_avoltage;
float g_fIntTemp;
float g_fIntTemp_k;
float g_fExtTemp;
float g_fExtTemp_F;
float g_rpm;

char g_sBuffer[80];

extern uint16_t g_uiADCSamples[3];

uint8_t  g_aRTCData[ 8 ];
uint16_t g_uiRTCCounter;
uint8_t  g_modeselection;
uint16_t press_time = 0;


//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void Initialize(void);
void HandleSysTick(void);
void HandleSwitches(uint8_t uiData, int32_t *iTC, bool *motor_flag);
void HandleADC(void);
void HandleRTC(void);
void HandleUART(void);


//----------------------------------------------------------------------------
// FUNCTION : Initialize( void )
// PURPOSE  : Initializes the embedded system
//----------------------------------------------------------------------------
void Initialize( void )
{
    // Initialize all system flags
    GLOBAL_InitSysFlags();

    // Initialize peripherals
    LED_Init();
    PROBE_Init();
    SYSTICK_Init();
    SYSCLK_Init();

    UART_Init();
    SWITCH_Init();
    TERM_Init();
    LCD_Init();
    ADC_Init();

    I2C_Init();
    PCF8574A_Init();
    g_uiRTCCounter = 1;
    MCP7940M_Init();

    MOTOR_Init(&g_MCP);
    QEI_Init(g_MCP.fdt);
    return;
}


//----------------------------------------------------------------------------
// FUNCTION : main( void )
// PURPOSE  : Program entry and main execution loop
//----------------------------------------------------------------------------
void main(void)
{
    Initialize();

    int32_t iTC = 1;
    uint8_t uiData;
    bool motor_flag = 0;

    g_MCP.fSP = 0.0f;

#ifdef DEBOUNCE
    CONTACT_Init(&g_SW3, 20, 1);
    CONTACT_Init(&g_SW2, 20, 1);
    CONTACT_Init(&g_SW4, 20, 1);
    CONTACT_Init(&g_SW5, 20, 1);
    CONTACT_Init(&g_SW6, 20, 1);
#endif

    LCD_Menu(1);
    contrast_adj(30);

    while (1)
    {
        __wfi();

        PCF8574A_Read(PCF8574A_SA, &uiData);

        HandleSysTick();
        HandleSwitches(uiData, &iTC, &motor_flag);
        HandleADC();
        HandleRTC();
        HandleUART();

        if (motor_flag)
        {
            press_time++;
        }
    }
}


//----------------------------------------------------------------------------
// FUNCTION : HandleSysTick(void)
// PURPOSE  : Processes periodic system tick operations
//----------------------------------------------------------------------------
void HandleSysTick(void)
{
    static uint8_t uiConvInterval = 1;

    if(GLOBAL_CheckSysFlag(SYSFLAGS_SYS_TICK))
    {
        GLOBAL_ClearSysFlag(SYSFLAGS_SYS_TICK);

        if(!--uiConvInterval)
        {
            ADC_SS0_Trigger();
            uiConvInterval = 100;
        }

        LED_FSM(0);
    }
}


//----------------------------------------------------------------------------
// FUNCTION : HandleSwitches(uint8_t uiData, int32_t *iTC, bool *motor_flag)
// PURPOSE  : Handles switch inputs and motor control
//----------------------------------------------------------------------------
void HandleSwitches(uint8_t uiData, int32_t *iTC, bool *motor_flag)
{
#ifdef DEBOUNCE

    if (CONTACT_Sample(&g_SW3, HWREG(GPIO_PORTF_BASE + GPIO_O_DATA + (0x01 << 2))))
    {
        if (!g_SW3.bOutput)
        {
            *iTC = (--(*iTC) < 1) ? 3 : *iTC;
            LCD_Menu(*iTC);
        }
    }

    if (CONTACT_Sample(&g_SW2, HWREG(GPIO_PORTF_BASE + GPIO_O_DATA + (0x10 << 2))))
    {
        if (!g_SW2.bOutput)
        {
            *iTC = (++(*iTC) > 3) ? 1 : *iTC;
            LCD_Menu(*iTC);
        }
    }

    if (CONTACT_Sample(&g_SW4, uiData & PCF8574A_SW4))
    {
        if (!g_SW4.bOutput)
        {
            LED_LED4(1);
            press_time = 0;
            *motor_flag = 1;
        }
        else
        {
            LED_LED4(0);
            *motor_flag = 0;

            if (press_time < 1000)
                g_MCP.fSP = (g_MCP.fSP + 100.0f > 12000.0f) ? 12000.0f : (g_MCP.fSP + 100.0f);
            else
                g_MCP.fSP = (g_MCP.fSP + 1000.0f > 12000.0f) ? 10000.0f : (g_MCP.fSP + 1000.0f);

            press_time = 0;
        }
    }

    if (CONTACT_Sample(&g_SW6, uiData & PCF8574A_SW6))
    {
        if (!g_SW6.bOutput)
        {
            LED_LED6(1);
            press_time = 0;
            *motor_flag = 1;
        }
        else
        {
            LED_LED6(0);
            *motor_flag = 0;

            if (press_time < 1000)
                g_MCP.fSP = (g_MCP.fSP - 100.0f < 0.0f) ? 0.0f : (g_MCP.fSP - 100.0f);
            else
                g_MCP.fSP = (g_MCP.fSP - 1000.0f < 0.0f) ? 0.0f : (g_MCP.fSP - 1000.0f);

            press_time = 0;
        }
    }

#endif
}


//----------------------------------------------------------------------------
// FUNCTION : HandleADC(void)
// PURPOSE  : Processes ADC readings and sensor data
//----------------------------------------------------------------------------
void HandleADC(void)
{
    uint16_t aValues[3];

    if (GLOBAL_CheckSysFlag(SYSFLAGS_ADC_SS0))
    {
        GLOBAL_ClearSysFlag(SYSFLAGS_ADC_SS0);

        if (ADC_SS0_Read(aValues, NUM_ELEMENTS(aValues)) == NUM_ELEMENTS(aValues))
        {
            float fAIN4 = aValues[0];
            float fAIN5 = aValues[1];
            g_fIntTemp = aValues[2];

            g_avoltage = (fAIN4 / 4096.0f) * (fVREFP - fVREFN);

            if (g_modeselection == 1)
            {
                g_rpm = (12000 * (g_avoltage / (fVREFP - fVREFN)));
                g_MCP.fSP = g_rpm;
            }

            g_fExtTemp = (fAIN5 / 81.225) - 2.4f;
            g_fExtTemp_F = (g_fExtTemp * 1.8) + 32;
            g_fIntTemp_k = 147.5f - (75.0f * (fVREFP - fVREFN) * (g_fIntTemp / 4096.0f)) + 273.15f + 13.2f;

            while (ADC_SS0_Read(aValues, 1));
        }
    }
}


//----------------------------------------------------------------------------
// FUNCTION : HandleRTC(void)
// PURPOSE  : Updates and manages real-time clock data
//----------------------------------------------------------------------------
void HandleRTC(void)
{
    if (!--g_uiRTCCounter)
    {
        g_uiRTCCounter = 500;
        MCP7940M_Read(0, g_aRTCData, 8);
    }
}


//----------------------------------------------------------------------------
// FUNCTION : HandleUART(void)
// PURPOSE  : Processes UART communication and commands
//----------------------------------------------------------------------------
void HandleUART(void)
{
    if (GLOBAL_CheckSysFlag(SYSFLAGS_UART_RXD))
    {
        GLOBAL_ClearSysFlag(SYSFLAGS_UART_RXD);

        uint8_t uiData;

        while (UART_GetChar(&uiData))
        {
            // Capital letter detection
            if (uiData >= 0x41 && uiData <= 0x5A)
            {
                LED_LED2(1);
            }

            // Convert lowercase to uppercase
            EL507_ToUpper((char*)&uiData);

            switch ((char)uiData)
            {
                case 'q':
                    UART_SendMessage("\r \n");
                    UART_SendEncodedMessage(QUOTE_Retrieve(), 0x5a);
                    break;

                case 0x0D: // Enter key
                    UART_PutChar(0x0D);
                    UART_PutChar(0x0A);
                    break;

                case '[':
                    TERM_Refresh();
                    break;

                case 0x12: // Ctrl+R (Reset)
                    HWREG(APINT) = 0x05FA0004;
                    break;

                case 0x17: // Ctrl+W (Watchdog reset)
                    HWREG(WATCHDOG_LOCK) = 0x1ACCE551;
                    HWREG(WATCHDOG_LOAD) = 0;
                    HWREG(WATCHDOG_CTL) |= 0x03;
                    HWREG(WATCHDOG_LOCK) = 0;
                    break;

                case 'A':
                    TERM_Clear();
                    g_modeselection = 1;
                    UART_SendMessage("\tMode: Automatic Control \r\n ");
                    break;

                case 'C':
                    TERM_Clear();
                    sprintf(g_sBuffer,
                            "\t20%02X-%02X-%02X %02X:%02X:%02X\r\n",
                            g_aRTCData[6],
                            g_aRTCData[5] & 0x1f,
                            g_aRTCData[4] & 0x3f,
                            g_aRTCData[2] & 0x1f,
                            g_aRTCData[1] & 0x7f,
                            g_aRTCData[0] & 0x7f);
                    UART_SendMessage(g_sBuffer);
                    break;

                case 'E':
                    UART_SendMessage("\tLab 12: Embedded System \r\n");
                    break;

                case 'F':
                    TERM_Clear();
                    g_MCP.fSP = 12000.0f;
                    UART_SendMessage("\tMotor speed set to 100% of maximum (12000.0 RPM)\r\n");
                    break;

                case 'I':
                    TERM_Clear();
                    UART_SendMessage("\tEL 507 Embedded Control Systems II\r\n");
                    UART_SendMessage("\tEmbedded System v1.0.\r\n\n");

                    sprintf(g_sBuffer,
                            "\tDate: 20%02X-%02X-%02X\r\n",
                            g_aRTCData[6],
                            g_aRTCData[5] & 0x1f,
                            g_aRTCData[4] & 0x3f);
                    UART_SendMessage(g_sBuffer);

                    sprintf(g_sBuffer,
                            "\tTime: %02X:%02X:%02X\r\n",
                            g_aRTCData[2] & 0x1f,
                            g_aRTCData[1] & 0x7f,
                            g_aRTCData[0] & 0x7f);
                    UART_SendMessage(g_sBuffer);
                    break;

                case 'L':
                    TERM_Clear();
                    LED_LED3(!HWREG(GPIO_PORTF_BASE + GPIO_O_DATA + (0X08 << 2)));
                    break;

                case 'M':
                    TERM_Clear();
                    g_modeselection = 0;
                    UART_SendMessage("\tMode: Manual Control \r\n ");
                    break;

                case 'R':
                    TERM_Clear();
                    sprintf(g_sBuffer, "\tSpeed: %4.1f RPM\r\n", QEI_GetSpeed());
                    UART_SendMessage(g_sBuffer);
                    break;

                case 'T':
                    TERM_Clear();
                    sprintf(g_sBuffer, "\tAmbient Temp: %.1f \xC2\xBA""C\r\n", g_fExtTemp);
                    UART_SendMessage(g_sBuffer);
                    sprintf(g_sBuffer, "\tInternal Temp: %.1f \xC2\xBA""C\r\n", g_fIntTemp_k - 273.15f);
                    UART_SendMessage(g_sBuffer);
                    break;

                case 'V':
                    sprintf(g_sBuffer, "\tPD3 Voltage: %.3f V\r\n", g_avoltage);
                    UART_SendMessage(g_sBuffer);
                    break;

                case '+':
                    LED_LED2(!HWREG(GPIO_PORTF_BASE + GPIO_O_DATA + (0X04 << 2)));
                    break;

                case '?':
                    TERM_Clear();
                    UART_SendMessage("\tAvailable Commands:\r\n\n");
                    UART_SendMessage("\tC     - Display the date and time\r\n");
                    UART_SendMessage("\tT     - Display the ambient and internal temperature\r\n");
                    UART_SendMessage("\tR     - Display the speed of the output shaft in RPM\r\n");
                    UART_SendMessage("\t0-9,F - Sets the speed of the output shaft\r\n");
                    UART_SendMessage("\tA     - Change the mode of control to automatic\r\n");
                    UART_SendMessage("\tM     - Change the mode of control to manual\r\n");
                    UART_SendMessage("\tI     - Display system information\r\n");
                    UART_SendMessage("\tL     - Toggles the state of LED3\r\n\n");
                    UART_SendMessage("\t<Ctrl>+R - Reset the embedded system\r\n");
                    break;

                default:
                    UART_PutChar(uiData);
                    break;
            }

            // Speed control (0–9) in MANUAL mode
            if ((char)uiData >= '0' && (char)uiData <= '9')
            {
                TERM_Clear();

                if (g_modeselection == 0)
                {
                    switch ((char)uiData)
                    {
                        case '0':
                            g_MCP.fSP = 0.0f;
                            break;
                        case '1':
                            g_MCP.fSP = 1200.0f;
                            break;
                        case '2':
                            g_MCP.fSP = 2400.0f;
                            break;
                        case '3':
                            g_MCP.fSP = 3600.0f;
                            break;
                        case '4':
                            g_MCP.fSP = 4800.0f;
                            break;
                        case '5':
                            g_MCP.fSP = 6000.0f;
                            break;
                        case '6':
                            g_MCP.fSP = 7200.0f;
                            break;
                        case '7':
                            g_MCP.fSP = 8400.0f;
                            break;
                        case '8':
                            g_MCP.fSP = 9600.0f;
                            break;
                        case '9':
                            g_MCP.fSP = 10800.0f;
                            break;
                    }
                }
            }
        }
    }
}





//----------------------------------------------------------------------------
// END MAIN.C
//----------------------------------------------------------------------------
