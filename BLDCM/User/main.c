#include "stm32f10x.h" 
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_usart.h"
#include "stdio.h"
#include "bsp_color_led.h"
#include "bsp_breathing.h"

__IO uint32_t rgb_color = 0xFFFFFF;

void delay(uint32_t count)
{
	for(;count!=0;count--);
}


int main()
{     
	// initialise the mode of USART as 115200 8-N-1
	USART_Config();

	/* 初始化呼吸灯 */
	TIMx_Breathing_Init();
	while (1) 
	{
		rgb_color= 0xFF00FF;
		delay(0x0FFFFFF);
		rgb_color= 0x00FFFF;
		delay(0x0FFFFFF);
		rgb_color= 0xFFFF00;
		delay(0x0FFFFFF);
	}
}
