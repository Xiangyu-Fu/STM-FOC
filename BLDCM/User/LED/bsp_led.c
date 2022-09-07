// board support package 
#include "./LED/bsp_led.h"
/**
 * @brief  The GPIO for LED
 * @param  None
 * @retval None
 */
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	LED1_GPIO_CLK_ENABLE();
    LED2_GPIO_CLK_ENABLE();
    LED3_GPIO_CLK_ENABLE();


	GPIO_InitStruct.GPIO_Pin = LED1_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.GPIO_Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LED2_PIN;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LED3_PIN;
	HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);

	LED_RGBOFF;
}


