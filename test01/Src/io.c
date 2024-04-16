/*
 * io.c
 *
 *  Created on: Mar 29, 2024
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


#define PORTB_BASE		0x40020400 //GPIOB 레지스터 값
#define ODR_OFFSET		0x14 // GPIOB 오프셋 값

void led1_on(void)
{
	// 0x40020414
	*(uint32_t*)(PORTB_BASE + ODR_OFFSET) |= (0x1 << 0);
}

void led1_off(void)
{
	*(uint32_t*)(PORTB_BASE + ODR_OFFSET) &= ~(0x1 << 0);
}

/* void led2_on(void)
{
	*(uint32_t*)(PORTB_BASE + ODR_OFFSET) |= (0x1<<7);
}

void led2_off(void)
{
	*(uint32_t*)(PORTB_BASE + ODR_OFFSET) &= ~(0x1<<7);
} */

void led3_on(void)
{
	*(uint32_t*)(PORTB_BASE + ODR_OFFSET) |= (0x1<<14);
}

void led3_off(void)
{
	*(uint32_t*)(PORTB_BASE + ODR_OFFSET) &= ~(0x1<<14);
}


void led2_onoff(bool on_flag)
{
#define LD2_PORTB_BSRR_ADDR	0x40020418
#define LD2_BIT				7

	uint32_t bsrr_addr = LD2_PORTB_BSRR_ADDR;
	uint32_t bit = on_flag ? (0x1 << LD2_BIT):(0x1 << (16+LD2_BIT));

	*(uint32_t *)bsrr_addr = bit;
}

typedef struct LED_S {
	uint32_t bsrr_addr;
	uint16_t on, off;
} LED_T;

#define LED_MAX		3

const struct LED_S gLedObjs[LED_MAX] = {
		{ 0x40020418,	0,	16+0 }
		{ 0x40020418,	7,	16+7
};





