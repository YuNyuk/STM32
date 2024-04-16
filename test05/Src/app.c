/*
 * app.c
 *
 *  Created on: Apr 15, 2024
 *      Author: iot00
 */

#include <stdio.h>
#include "cmsis_os.h"
#include "io.h"
#include "uart.h"
#include "cli.h"
#include "gpio.h"
#include "polling.h"
#include "app.h"

static void app_init(void);

#if 0
#define log(fmt, ...) \
    printf("[%s: %d][%s] " fmt "\t\t\t (%s, %s)\n", \
    __FILE__, __LINE__, __func__, __DATE__, __TIME__);
#else
#define log(fmt, ...)
#endif

void app(void)
{
	printf("System Start...\r\n");
	log();
	osKernelInitialize();
	app_init();
	osKernelStart();
}

static void app_init(void)
{
	io_exti_init();
	uart_init();

	cli_init();
	polling_init();
}

