#include "./KEY/bsp_key.h"

/* @brief configure the GPIO pins for the KEY1 
 * @param None
 * @retval None
 */

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
    // open the clock of the GPIO port
    KEY1_GPIO_CLK_ENABLE();
    KEY2_GPIO_CLK_ENABLE();


	GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
	HAL_GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief  scan the key value
 * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral for STM32F1xx devices or (A..I) to select the GPIO peripheral for STM32F0xx/STM32F4xx devices.
 * @param  GPIO_Pin: specifies the port bit to read.
 * @retval the key value
 */
uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	// scan to see if the key is pressed
	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==KEY_ON)
	{
		// wait for the key to be released
		while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==KEY_ON);
		{
		return KEY_ON;
		}
	}
	else return KEY_OFF;
}
