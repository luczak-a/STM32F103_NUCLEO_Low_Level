#include "gpio.h"
#include "timer.h"

static uint8_t user_button_pushed = 0;

void GPIOGreenLEDInit(void)
{
  // GPIOA clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  // output push-pull 2MHz
  GPIOA->CRL |= GPIO_CRL_MODE5_0;
  GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
}

void GPIOGreenLEDState(uint8_t state)
{
  if (state)
    GPIOA->BSRR = GPIO_BSRR_BS5;
  else
    GPIOA->BSRR = GPIO_BSRR_BR5;
}

void GPIOUserButtonInit(void)
{
  // User button - PC13 floating input with EXTI
  // active low
  //GPIOC clock enable
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
  //EXTI13 on GPIOC
  AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC;
  //Line 13 interurpt is not masked
  EXTI->IMR |= EXTI_IMR_IM13;
  //Rising trigger enabled
  EXTI->RTSR |= EXTI_RTSR_RT13;
  //Enable EXTI15_10 interrupt in NVIC
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

uint8_t GPIOUserButtonPushed(void)
{
  if (user_button_pushed) 
  {
    user_button_pushed = 0;
    return 1;
  }
  return 0;
}



void EXTI15_10_IRQHandler(void)
{
  if (EXTI->PR & EXTI_PR_PR13)
  {
    // Timeout is used for debouncing (500ms)
    if(TimeoutOccured(TIMEOUT_BUTTON))
    {
      user_button_pushed = 1;
      TimeoutSet(TIMEOUT_BUTTON,200);
    }
    EXTI->PR |= EXTI_PR_PR13;
  }
}