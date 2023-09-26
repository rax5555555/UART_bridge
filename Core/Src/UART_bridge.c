#include "main.h"
#include "cmsis_os.h"
#include "uart_bridge.h"

osThreadId UART1_TaskHandle;
osThreadId UART2_TaskHandle;

void StartUART1_Task(void const * argument);
void StartUART2_Task(void const * argument);

xQueueHandle uart1_to_uart2_queue;
xQueueHandle uart2_to_uart1_queue;

void UART_Bridge_init(void)
{
	LL_USART_EnableIT_RXNE(USART1);
	LL_USART_EnableIT_RXNE(USART2);

	uart1_to_uart2_queue = xQueueCreate(1, sizeof(uint8_t));
	uart2_to_uart1_queue = xQueueCreate(1, sizeof(uint8_t));

	osThreadDef(UART1_Task, StartUART1_Task, osPriorityNormal, 0, 128);
	UART1_TaskHandle = osThreadCreate(osThread(UART1_Task), NULL);

	osThreadDef(UART2_Task, StartUART2_Task, osPriorityNormal, 0, 128);
	UART2_TaskHandle = osThreadCreate(osThread(UART2_Task), NULL);

	osKernelStart();
}

void USART1_IRQ_Handler()
{
	BaseType_t xPriorityWoken = pdFALSE;
	uint8_t recieve = LL_USART_ReceiveData8(USART1);
	xQueueSendFromISR(uart1_to_uart2_queue, &recieve, &xPriorityWoken);
	portYIELD_FROM_ISR(xPriorityWoken);
}

void USART2_IRQ_Handler()
{
	BaseType_t xPriorityWoken = pdFALSE;
	uint8_t recieve = LL_USART_ReceiveData8(USART2);
	xQueueSendFromISR(uart2_to_uart1_queue, &recieve, &xPriorityWoken);
	portYIELD_FROM_ISR(xPriorityWoken);
}

void StartUART1_Task(void const * argument)
{
	uint8_t data;

	for(;;)
	{
		if (xQueueReceive(uart2_to_uart1_queue, &data, portMAX_DELAY))
		{
			while (!LL_USART_IsActiveFlag_TXE(USART1)) {}
			LL_USART_TransmitData8(USART1, data);
		}		
		//osDelay(1);
	}
}

void StartUART2_Task(void const * argument)
{
	uint8_t data;

	for(;;)
	{
		if (xQueueReceive(uart1_to_uart2_queue, &data, portMAX_DELAY))
		{
			while (!LL_USART_IsActiveFlag_TXE(USART2)) {}
			LL_USART_TransmitData8(USART2, data);
		}		
		//osDelay(1);
	}
}
