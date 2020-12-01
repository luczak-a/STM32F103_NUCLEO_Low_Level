#include "system_clock.h"

void RCCSystemClockConfig(void)
{
    // Driven from HSE 8MHz
    // AHB 8MHz/2 = 4 MHz
    // APB1, APB2 4MHz
    RCC->CFGR |= RCC_CFGR_HPRE_DIV2;
}
