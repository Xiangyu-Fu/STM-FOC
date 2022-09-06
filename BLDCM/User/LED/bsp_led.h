#ifndef __BSP_LED_H  // 这个名称的问题？
#define __BSP_LED_H

#include "stm32f10x.h"

/*为了提高程序的可以执行，跟硬件相关的都定义成宏*/
#define LED_G_GPIO_PIN		GPIO_Pin_0
#define LED_R_GPIO_PIN		GPIO_Pin_5
#define LED_B_GPIO_PIN		GPIO_Pin_1

#define LED_GPIO_PORT			GPIOB
#define LED_GPIO_CLK			RCC_APB2Periph_GPIOB
#define DELAY_TIME				((uint32_t)0xFFF)

#define 	ON 		1
#define 	OFF 	0

#define		LED_G(x) 				if(x) \
																GPIO_ResetBits(LED_GPIO_PORT, LED_G_GPIO_PIN); \
													else 	GPIO_SetBits(LED_GPIO_PORT, LED_G_GPIO_PIN);
#define		LED_R(x) 				if(x) \
																GPIO_ResetBits(LED_GPIO_PORT, LED_R_GPIO_PIN); \
													else 	GPIO_SetBits(LED_GPIO_PORT, LED_R_GPIO_PIN);
#define		LED_B(x) 				if(x) \
																GPIO_ResetBits(LED_GPIO_PORT, LED_B_GPIO_PIN); \
													else 	GPIO_SetBits(LED_GPIO_PORT, LED_B_GPIO_PIN);
						
#define LED_G_TOGGLE 	{LED_GPIO_PORT->ODR ^= LED_G_GPIO_PIN;}
#define LED_R_TOGGLE 	{LED_GPIO_PORT->ODR ^= LED_R_GPIO_PIN;}

void LED_GPIO_Config(uint32_t pin);

#endif /*__BSP_LED_H*/

