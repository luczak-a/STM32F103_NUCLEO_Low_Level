#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f1xx.h"

#define TIMEOUT_COUNT  3
#define TIMEOUT_BUTTON 0
#define TIMEOUT_LED    1
#define TIMEOUT_UART1  2

void Timer2Init(void);
void TimeoutSet(uint32_t timeout, uint32_t value);
uint8_t TimeoutOccured(uint32_t timeout);

#endif /* end of include guard: _TIMER_H_ */
