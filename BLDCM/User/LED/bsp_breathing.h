#ifndef __BSP_BREATHING_H
#define __BSP_BREATHING_H

#include "stm32f10x.h"

// the number of points in the PWM table
extern uint16_t  POINT_NUM	;

// the frequence of hte output wave
extern __IO uint16_t period_class ;

#define AMPLITUDE_CLASS 64

#define RED_LIGHT 1
#define GREEN_LIGHT 2
#define BLUE_LIGHT 3

/* the configuration of the timer */
#define BRE_TIMx                    		TIM3

// the interruption of the timer
#define BRE_TIMx_IRQn               		TIM3_IRQn            // the interrupt of the timer
#define BRE_TIMx_IRQHandler         		TIM3_IRQHandler

// the clock of the timer
#define BRE_TIM_APBxClock_FUN				RCC_APB1PeriphClockCmd  // TIM clock enable function
#define BRE_TIMx_CLK                		RCC_APB1Periph_TIM3
#define BRE_TIM_GPIO_CLK           			(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO)

/* RED LIGHT */
#define BRE_GPIO_REMAP_FUN()        		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE)
#define BRE_RED_TIM_LED_PIN             	GPIO_Pin_5
#define BRE_RED_TIM_LED_PORT            	GPIOB

#define BRE_RED_TIM_OCxInit             	TIM_OC2Init // the channel of the red light. 1~4
#define BRE_RED_TIM_OCxPreloadConfig    	TIM_OC2PreloadConfig
#define BRE_RED_CCRx                    	CCR2

/* GREEN LIGHT */
#define BRE_GREEN_TIM_LED_PIN             	GPIO_Pin_0
#define BRE_GREEN_TIM_LED_PORT            	GPIOB

#define BRE_GREEN_TIM_OCxInit             	TIM_OC3Init // the channel of the green light. 1~4
#define BRE_GREEN_TIM_OCxPreloadConfig    	TIM_OC3PreloadConfig
#define BRE_GREEN_CCRx                    	CCR3

/* BLUE LIGHT */
#define BRE_BLUE_TIM_LED_PIN             	GPIO_Pin_1
#define BRE_BLUE_TIM_LED_PORT            	GPIOB

#define BRE_BLUE_TIM_OCxInit             	TIM_OC4Init // the channel of the blue light. 1~4
#define BRE_BLUE_TIM_OCxPreloadConfig    	TIM_OC4PreloadConfig
#define BRE_BLUE_CCRx                    	CCR4

void TIMx_Breathing_Init(void);

#endif
