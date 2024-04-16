/*
 * app.c
 *
 *  Created on: Apr 8, 2024
 *      Author: IOT18
 */
#include <stdio.h>
#include <stdbool.h>
#include "button.h"
#include "main.h"
#include "adc.h"

void button_callback(void *arg);
void button_callback_2(void *arg);
void button_led_toggle(void);


void button_callback_2(void *arg)
{
	static uint8_t count = 0;

		BUTTON_STS *pSts = (BUTTON_STS *)arg;

		if(pSts->edge == true) {
			printf("rising\r\n");
			button_led_toggle();
		}
		else if(pSts->edge == false) {
			printf("falling:period = %d\r\n", pSts->pushed_count);

			button_led_toggle();
		}

		printf("2.count = %d\r\n",count);
		count ++;
		count %= 10;
		if(count == 0) {
			button_regcbf(0, button_callback);
			printf("cbf changed to callback\r\n");
		}
}

void adc_callback(void *arg)
{
	printf("adc value = %d\r\n", *(uint16_t*)arg);
}

void button_callback(void *arg)
{
	static uint8_t count = 0;

	BUTTON_STS *pSts = (BUTTON_STS *)arg;

	if(pSts->edge == true) {
		printf("rising\r\n");
		button_led_toggle();
	}

	else if(pSts->edge == false) {
		printf("falling:period = %d\r\n", pSts->pushed_count);

		button_led_toggle();
	}

	printf("1.count = %d\r\n",count);
	count ++;
	count %= 10;
	if(count == 0) {
		button_regcbf(0, button_callback_2);
		printf("cbf changed to callback_2\r\n");
	}
}

typedef struct {
	uint32_t period, count;
	bool flag;
	void(*cbf)(void);
}THR_T; // thread type

THR_T gThrObjs[]={
		{ .period = 1, 		.cbf = button_check	},
		{ .period = 500, 	.cbf = adc_check		},
		{ .cbf = NULL			}
};

static void init(void)
{
	button_init();
	button_regcbf(0, button_callback);

	adc_init();
	adc_regcbf(0, adc_callback);
}


void app(void)
{
	uint32_t thr_idx = 0;
	uint32_t tick_prev, tick_curr;

	init();

	tick_prev = tick_curr = HAL_GetTick();
	while(1) {
		tick_curr = HAL_GetTick();
		for (int i=0; gThrObjs[i].cbf != NULL; i++) {
			if(tick_curr - tick_prev >= 1) {
				gThrObjs[i].count++;
				gThrObjs[i].count %= gThrObjs[i].period;
				if(gThrObjs[i].count == 0)
					gThrObjs[i].flag = true;
				tick_prev = tick_curr;
			}
		}
		{
			if(gThrObjs[thr_idx].flag == true) {
				gThrObjs[thr_idx].flag = false;
				gThrObjs[thr_idx].cbf();
			}
			thr_idx++;
			if(gThrObjs[thr_idx].cbf == NULL)
				thr_idx = 0;
		}
		// Message Pump
	}

	/* uint32_t tick_prev, tick_curr;

	button_init();
	button_regcbf(0, button_callback);

	init();

	// tick_prev = tick_curr = HAL_GetTick();

	while (1) {
		for (int i=0; gThrObjs[i].cbf != NULL; i++) {
			gThrObjs[i].curr = HAL_GetTick();
		if (gThrObjs[i].curr - gThrObjs[i].prev >= 1){
			gThrObjs[i].cbf();
			gThrObjs[i].prev = gThrObjs[i].curr;
		}
		}
	} */
}

// #define bsp_read_btn1()	HAL_GPIO_ReadPin(USER_Btn_GPIO_Port,USER_Btn_Pin)
#if 0
#define bsp_delay(x)	HAL_Delay(x)


bool bsp_read_btn1(void)
{
	return HAL_GPIO_ReadPin(USER_Btn_GPIO_Port,USER_Btn_Pin) == GPIO_PIN_SET ? true : false;
}

void app(void)
{
	//uint16_t btn;
	//GPIO_TypeDef *pPort = GPIOC; // (GPIO_TypeDef*)0x40020800;
	bool sts;

	printf("System Started...\r\n");
	while(1) {
		bsp_delay(200);

		//btn = *(uint16_t*)0x40020810; // Port C IDR
		//btn = pPort->IDR;


		//if(btn & (0x1 << 13))
		//  printf("Blue button Pushed!!!\r\n");
		// or
		//if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
		//			printf("Blue button Pushed!!!\r\n");
		// or
		sts = bsp_read_btn1();
		if (sts == true)
			printf("Blue button Pushed!!!\r\n");

	}
}

#endif
