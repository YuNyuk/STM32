/*
 * button.c
 *
 *  Created on: Apr 9, 2024
 *      Author: IOT18
 */
#include <stdio.h>
#include "button.h"

static void button_dummy(void *);

static BUTTON_T gBtnObjs[] = {
		  {USER_Btn_GPIO_Port, USER_Btn_Pin, 80, 0, 0, 0, button_dummy,{true,	0}},
		  {NULL,		 	   0,  		 	 0,  0, 0, 0, NULL,		   {true,	0}}
		};

void button_init(void)
{

}

void button_regcbf(uint16_t idx, FUNC_CBF cbf)
{
	gBtnObjs[idx].cbf = cbf;
}

void button_check(void)
{
  BUTTON_T *p = &gBtnObjs[0];

  for (uint8_t i=0; p->cbf != NULL; i++) {
	  p->count++;
	  p->count %= p->period;
	  //gBtnObjs[i].count++;
	  //gBtnObjs[i].count %= gBtnObjs[i].period;
	  //p[i].count++;
	  //p[i].count %= p[i].period;

	  /* if(p->count > p->period)
		  p->count =0; */
	  if(p->count == 0) {
		  p->curr = HAL_GPIO_ReadPin(p->port, p->pin);
		  if(p->prev == 0 && p->curr == 1){ // rising
			p->sts.edge = true;
			// p->sts.pushed_count = 0;
			p->cbf((void *)&(p->sts));
		  } else if(p->prev == 1 && p-> curr == 0) { // falling
			  p->sts.edge = false;
			  p->cbf((void *)&(p->sts));
		  } else if(p->prev == 1 && p->curr == 1){ // pushing
			  if(p->sts.pushed_count < 100)
				  p->sts.pushed_count++;
		  }
			else { //released
				p->sts.pushed_count = 0;
		  }
		  p->prev = p->curr;
	  }

	  p++; // p = p + 1;
  }
}
void button_led_toggle(void *arg) {
    // 사용자 버튼의 상태 정보를 arg 매개변수를 통해 받아옴
    BUTTON_STS *pSts = (BUTTON_STS *)arg;

    // 버튼 상태가 rising edge일 때(LED가 꺼져 있고 버튼을 누른 경우)
    if (pSts->edge == true) {
        // LED를 켬
        HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
    }
    // 버튼 상태가 falling edge일 때(LED가 켜져 있고 버튼을 놓은 경우)
    else {
        // LED를 끔
        HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

    }
}

static void button_dummy(void *)
{
	printf("I'm dummy\r\n");
	return;
}
