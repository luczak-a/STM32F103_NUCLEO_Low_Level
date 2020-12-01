#ifndef _UART_
#define _UART_

#include "stm32f1xx.h"

void UART1Init(void);
void UART1Transmit(uint8_t *data, uint16_t size);
void UART1TransmitStr(char* str);

#endif