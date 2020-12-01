#include "stm32f1xx.h"
#include "system_clock.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"

int main(void)
{
  static uint8_t led_state = 0;

  RCCSystemClockConfig();
  GPIOGreenLEDInit();
  GPIOUserButtonInit();
  Timer2Init();
  UART1Init();

  UART1TransmitStr("Hello, this is STM32F103 Nucleo Low-Level UART Project");

  while(1)
  {
    if(TimeoutOccured(TIMEOUT_LED))
    {
      GPIOGreenLEDState(led_state = !led_state);
      TimeoutSet(TIMEOUT_LED,1000);
    }

    if (GPIOUserButtonPushed())
    {
      UART1TransmitStr("Blue button pressed");
    }
  }

  return 0;
}
