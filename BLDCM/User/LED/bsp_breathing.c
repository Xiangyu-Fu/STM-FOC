#include "bsp_breathing.h"

/* breif:  LED GPIO Configuration
 * param:  none
 * return: none
 */
 

static void TIMx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // enable the clock of GPIO
    RCC_APB2PeriphClockCmd(BRE_TIM_GPIO_CLK, ENABLE); 
    
    // IO configuration, because the pin of the red using the second function of the pinout
    // so we define the remap function 
    BRE_GPIO_REMAP_FUN();

    // configure the pinout of the LED
    GPIO_InitStructure.GPIO_Pin = BRE_RED_TIM_LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// we configure the pin as the alternate function
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BRE_RED_TIM_LED_PORT, &GPIO_InitStructure);

    // green light
    GPIO_InitStructure.GPIO_Pin = BRE_GREEN_TIM_LED_PIN;
    GPIO_Init(BRE_GREEN_TIM_LED_PORT, &GPIO_InitStructure);

    // blue light
    GPIO_InitStructure.GPIO_Pin = BRE_BLUE_TIM_LED_PIN;
    GPIO_Init(BRE_BLUE_TIM_LED_PORT, &GPIO_InitStructure);
}


uint16_t indexWave[] = {
    1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5,
    6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 22,
    25, 28, 32, 36, 41, 47, 53, 61, 69, 79,
    89, 102, 116, 131, 149, 170, 193, 219,
    250, 284, 323, 367, 417, 474, 539, 613,
    697, 792, 901, 1024, 1024, 901, 792, 697,
    613, 539, 474, 417, 367, 323, 284, 250,
    219, 193, 170, 149, 131, 116, 102, 89,
    79, 69, 61, 53, 47, 41, 36, 32, 28, 25,
    22, 19, 17, 15, 13, 11, 10, 9, 8, 7, 6,
    5, 5, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1
};




__IO uint16_t period_class = 1;

// calculate how many elements in the PWM table
uint16_t POINT_NUM = sizeof(indexWave) / sizeof(indexWave[0]);

static void NVIC_Config_PWM(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // configure the preemption priority of the timer
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    // enable the timer interrupt
    NVIC_InitStructure.NVIC_IRQChannel = BRE_TIMx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void TIMx_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // enable the clock of timer
    BRE_TIM_APBxClock_FUN(BRE_TIMx_CLK, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = (512 - 1);
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(BRE_TIMx, &TIM_TimeBaseStructure);

    // configure the PWM mode
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

    // RGB CHANNELS
    BRE_RED_TIM_OCxInit(BRE_TIMx, &TIM_OCInitStructure);
    BRE_RED_TIM_OCxPreloadConfig(BRE_TIMx, TIM_OCPreload_Enable);

    BRE_GREEN_TIM_OCxInit(BRE_TIMx, &TIM_OCInitStructure);
    BRE_GREEN_TIM_OCxPreloadConfig(BRE_TIMx, TIM_OCPreload_Enable);

    BRE_BLUE_TIM_OCxInit(BRE_TIMx, &TIM_OCInitStructure);
    BRE_BLUE_TIM_OCxPreloadConfig(BRE_TIMx, TIM_OCPreload_Enable);

		TIM_ARRPreloadConfig(BRE_TIMx, ENABLE); // Enable the ARR reload register

    // enable the timer
    TIM_Cmd(BRE_TIMx, ENABLE);
    TIM_ITConfig(BRE_TIMx, TIM_IT_Update, ENABLE);
    NVIC_Config_PWM();
}

void TIMx_Breathing_Init(void)
{
    TIMx_GPIO_Config();
    TIMx_Mode_Config();
}
