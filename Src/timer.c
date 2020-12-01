#include "timer.h"

static uint32_t timeout_array[TIMEOUT_COUNT] = {0};

void Timer2Init(void)
{
  // Enable TIM2 clock
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  // Reload value 4000: 1kHz
  TIM2->ARR = 4000;
  // Update interurpt enable
  TIM2->DIER |= TIM_DIER_UIE;
  // Enable TIM2 interrupt in NVIC
  NVIC_EnableIRQ(TIM2_IRQn);
  // Enable counter
  TIM2->CR1 |= TIM_CR1_CEN;
}


void TimeoutSet(uint32_t timeout, uint32_t value)
{
  timeout_array[timeout] = value;
}


uint8_t TimeoutOccured(uint32_t timeout)
{
  return (uint8_t)(timeout_array[timeout] == 0);
}


void TIM2_IRQHandler(void)
{
  TIM2->SR = 0;
  for(uint32_t i = 0; i < TIMEOUT_COUNT; i++)
  {
    if (timeout_array[i])
      --timeout_array[i];
  }
}
