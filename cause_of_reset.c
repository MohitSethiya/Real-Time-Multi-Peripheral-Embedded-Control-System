/*
 * cause_of_reset.c
 *
 *  Created on: Oct 6, 2025
 *      Author: msethiya
 */




#include "Include/cause_of_reset.h"

static uint8_t number = 0;

uint8_t cause_of_reset(void) {
    // It it was anything other than MOSC, treat as normal, otherwise
    // preserve the state of g_uiRESC
    if(HWREG( RSC_BASE + RSC_OFFSET ) & 0x0000003F) {
        g_uiRESC = HWREG(RSC_BASE + RSC_OFFSET);
    }
    HWREG( RSC_BASE + RSC_OFFSET ) = ~0x0001003F;
    if(g_uiRESC & (0x01)) {
        g_uiRESC &= ~(0x01);
        number = 3;
    }
    if(g_uiRESC &(0x02)) {
        g_uiRESC &= ~(0x02);
        number = 4;
    }
    if(g_uiRESC &(0x04)) {
        g_uiRESC &= ~(0x04);
        number = 5;
    }
    if(g_uiRESC &(0x08)) {
        g_uiRESC &= ~(0x08);
        number = 6;
    }
    if(g_uiRESC &(0x10)) {
        g_uiRESC &= ~(0x10);
        number = 7;
    }
    if(g_uiRESC &(0x10000)) {
            g_uiRESC &= ~(0x10000);
            number = 8;
    }
    return number;
}
