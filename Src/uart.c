#include "uart.h"
#include <string.h>
#include "gpio.h"

#define BUFFER_SIZE   512

static struct sCyclicBuffer
{
    uint8_t data[BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
}
rx_buffer = {0}, tx_buffer = {0};

void UART1Init(void)
{
    // USART1: PA9=TX PA10=RX
    // Enable GPIOA,USART1 clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
    // GPIO alternate-function for TX, input floating for RX
    GPIOA->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_0 | GPIO_CRH_CNF9_1;
    // Enable RXNE interrupt
    USART1->CR1 |=  USART_CR1_RXNEIE;    
    // Set baudrate 115200
    USART1->BRR = (2 << 4) + 3;
    // Enable USART1 interrupt
    NVIC_EnableIRQ(USART1_IRQn);
    // Enable transmitter and receiver
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
     // Enable USART1
    USART1->CR1 |= USART_CR1_UE;    
}


static inline void UART1TransmitNext(void)
{
    USART1->DR = tx_buffer.data[tx_buffer.tail++];
    if (tx_buffer.tail >= BUFFER_SIZE) tx_buffer.tail = 0;
    --tx_buffer.count;
}

void UART1Transmit(uint8_t *data, uint16_t size)
{
    if (size < 1) return;
    // Put data to rx_buffer
    USART1->CR1 &= ~USART_CR1_TXEIE;
    for(uint16_t i = 0; i < size; i++)
    {
        if (tx_buffer.count == BUFFER_SIZE) break;
        tx_buffer.data[tx_buffer.head++] = data[i];
        if (tx_buffer.head >= BUFFER_SIZE) tx_buffer.head = 0;
        ++tx_buffer.count;
    }
    // Transmit first byte
    while (!(USART1->SR & USART_SR_TXE));
    UART1TransmitNext();
    USART1->CR1 |= USART_CR1_TXEIE;
}

void UART1TransmitStr(char* str)
{
    UART1Transmit((uint8_t*)str,strlen(str));
    UART1Transmit((uint8_t*)"\r\n",2);
}

void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_TXE)
    {
        if (tx_buffer.count > 0)
        {
           UART1TransmitNext();
        }
        else
        {
            USART1->CR1 &= ~USART_CR1_TXEIE;   
        }
    }
    if (USART1->SR & USART_SR_RXNE)
    {
        rx_buffer.data[rx_buffer.head++] = USART1->DR;
        if (rx_buffer.head >= BUFFER_SIZE) rx_buffer.head = 0;
        if (rx_buffer.count == BUFFER_SIZE-1) 
        {
            rx_buffer.tail++;
            if (rx_buffer.tail >= BUFFER_SIZE) rx_buffer.tail = 0;
        }
        else
        {
            rx_buffer.count++;
        }
    }
}