#ifndef _GPIO_
#define _GPIO_

#include "stm32f1xx.h"

void    GPIOGreenLEDInit(void);
void    GPIOGreenLEDState(uint8_t state);
void    GPIOUserButtonInit(void);
uint8_t GPIOUserButtonPushed(void);

#endif
