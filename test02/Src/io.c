/*
 * io.c
 *
 *  Created on: Apr 8, 2024
 *      Author: IOT18
 */

#include "main.h"
#include "io.h"
#include "stdbool.h"

extern UART_HandleTypeDef huart3;
#define D_PRINTF_UART_HANDLE	huart3

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&D_PRINTF_UART_HANDLE, (uint8_t*)&ch, 1, 0xffff);
	return ch;
}
