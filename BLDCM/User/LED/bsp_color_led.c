#include "bsp_color_led.h"


/* breif:  LED GPIO Configuration
 * param:  none
 * return: none
 */

static void COLOR_TIMx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // enable the clock of GPIOA
    RCC_APB2PeriphClockCmd(COLOR_TIM_GPIO_CLK, ENABLE);
    
    // IO configuration, because the pin of the red using the second function of the pinout
    // so we define the remap function 
    COLOR_GPIO_REMAP_FUN();

    // configure the pinout of the LED
    // Red
    GPIO_InitStructure.GPIO_Pin = COLOR_RED_TIM_LED_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// we configure the pin as the alternate function
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COLOR_RED_TIM_LED_PORT, &GPIO_InitStructure);

    // Green
    GPIO_InitStructure.GPIO_Pin = COLOR_GREEN_TIM_LED_PIN ;
    GPIO_Init(COLOR_GREEN_TIM_LED_PORT, &GPIO_InitStructure);

    // Blue
    GPIO_InitStructure.GPIO_Pin = COLOR_BLUE_TIM_LED_PIN ;
    GPIO_Init(COLOR_BLUE_TIM_LED_PORT, &GPIO_InitStructure);
}

static void COLOR_TIMx_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // enable the clock fo the timer 
    COLOR_TIM_APBxClock_FUN(COLOR_TIM_CLK, ENABLE);
    
    // configure the timer as the PWM mode
    TIM_TimeBaseStructure.TIM_Period = 255;

    // the frequency of the timer is 72MHz/255 = 282.35KHz
    TIM_TimeBaseStructure.TIM_Prescaler = 1999;

    // the frequency of the timer is 72MHz/1999/255 = 282.35Hz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    // the counter of the timer is upcounting
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(COLOR_TIMx, &TIM_TimeBaseStructure);

    // configure the PWM mode
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

    // enable the channel and preload
    COLOR_RED_TIM_OCxInit(COLOR_TIMx, &TIM_OCInitStructure);
    COLOR_RED_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);

    COLOR_GREEN_TIM_OCxInit(COLOR_TIMx, &TIM_OCInitStructure);
    COLOR_GREEN_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);

    COLOR_BLUE_TIM_OCxInit(COLOR_TIMx, &TIM_OCInitStructure);
    COLOR_BLUE_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);

    // enable the COLOR_TIMx Reload register ARR
    TIM_ARRPreloadConfig(COLOR_TIMx, ENABLE);

    // enable the timer
    TIM_Cmd(COLOR_TIMx, ENABLE);

}

void COLOR_TIMx_LED_init(void)
{
	COLOR_TIMx_GPIO_Config();
	COLOR_TIMx_Mode_Config();
}

void SetRGBColor(uint32_t rgb)
{
    uint8_t red = (rgb >> 16) & 0xFF;
    uint8_t green = (rgb >> 8) & 0xFF;
    uint8_t blue = rgb & 0xFF;

    COLOR_TIMx->COLOR_RED_CCRx = red;
    COLOR_TIMx->COLOR_GREEN_CCRx = green;
    COLOR_TIMx->COLOR_BLUE_CCRx = blue;
}


void SetColorValue(uint8_t r, uint8_t g, uint8_t b)
{
    COLOR_TIMx->COLOR_RED_CCRx = r;
    COLOR_TIMx->COLOR_GREEN_CCRx = g;
    COLOR_TIMx->COLOR_BLUE_CCRx = b;
}
