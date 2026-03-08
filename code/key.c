#include "stm32g4xx.h"                  // Device header
#include "stdint.h"
#include "led.h"

extern int count;

uint8_t B1_state;
uint8_t B1_last_state;
uint8_t B2_state;
uint8_t B2_last_state;
uint8_t B3_state;
uint8_t B3_last_state;
uint8_t B4_state;
uint8_t B4_last_state;


void key_scan ()
{
	B1_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	B2_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	B3_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	B4_state = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	if (B1_state == 0 && B1_last_state == 1) //
	{
		count++;	
	}
	
	if (B2_state == 0 && B2_last_state == 1) //
	{
		count--;
	}
	
	if (B3_state == 0 && B3_last_state == 1) //
	{
		led_show(2,1);
	}
	
	if (B4_state == 0 && B4_last_state == 1)
	{
		led_show(2,0);
	}
		
	B1_last_state=B1_state;
	B2_last_state=B2_state;
	B3_last_state=B3_state;
	B4_last_state=B4_state;
}
