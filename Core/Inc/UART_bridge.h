#ifndef _UART_BRIDGE_
#define _UART_BRIDGE_

#include <stdint.h>

void UART_Bridge_init(void);
void USART1_IRQ_Handler(void);
void USART2_IRQ_Handler(void);

#endif // _UART_BRIDGE_
