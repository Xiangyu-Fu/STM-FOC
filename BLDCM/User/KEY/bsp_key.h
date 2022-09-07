#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f1xx.h"
#include "main.h"

//KEY1
#define KEY1_GPIO_PIN			    GPIO_Pin_0
#define KEY1_GPIO_PORT		        GPIOA
#define KEY1_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

//KEY2
#define KEY2_GPIO_PIN			    GPIO_Pin_13
#define KEY2_GPIO_PORT		        GPIOC
#define KEY2_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

//KEY VALUE
#define KEY_ON 	1
#define KEY_OFF 0

// Declarations of function
void KEY_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /*__BSP_KEY_H*/
