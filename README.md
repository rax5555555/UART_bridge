### USART BRIDGE

Проект собран в Keil uVision под микроконтроллер STM32f401CCU6 с использованием CubeMX.
Взаимодействие с UART производилось через библиотеку LL.
В качестве RTOS использовался FreeRTOS CMSIS_V1.
Описание алгоритма лежит в Core/Src/UART_bridge.c

Во время прихода данных на UART по прерыванию в обработчике байты складываются в очередь Queue, в тасках байты берутся из очереди и отправляются в соответствующий UART

![alt text](https://github.com/rax5555555/UART_bridge/blob/main/img/HW.jpg)

![alt text](https://github.com/rax5555555/UART_bridge/blob/main/img/SW.jpg)