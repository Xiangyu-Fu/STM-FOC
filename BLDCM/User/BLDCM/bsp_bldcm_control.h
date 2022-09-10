#ifndef __BSP_MOTOR_TIM_H
#define __BSP_MOTOR_TIM_H

#include "stm32f1xx.h"
#include "main.h"
#include "./TIM/bsp_motor_tim.h"

// PIN define
/////////////////////////
#define SHUTDOWN_PIN                		GPIO_PIN_15
#define SHUTDOWN_PORT               		GPIOD
#define SHUTDOWN_GPIO_CLK_ENABLE()  		__HAL_RCC_GPIOD_CLK_ENABLE()

// Enable/Disable PIN of the SD 
#define BLDCM_ENABLE_SD()              	HAL_GPIO_WritePin(SHUTDOWN_PORT, SHUTDOWN_PIN, GPIO_PIN_SET)
#define BLDCM_DISABLE_SD()             	HAL_GPIO_WritePin(SHUTDOWN_PORT, SHUTDOWN_PIN, GPIO_PIN_RESET)

// Enumerate the motor direction
typedef enum
{
    MOTOR_FWD = 0,
    MOTOR_REV,
} motor_dir_t;

typedef struct 
{
    motor_dir_t 	direction;
    uint16_t 			dutyfactor;
    uint8_t  			is_enable;
    uint32_t 			lock_timeout;
} bldcm_data_t;


void bldcm_init(void);
void set_bldcm_speed(uint16_t v);
void set_bldcm_direction(motor_dir_t dir);
motor_dir_t get_bldcm_direction(void);
void set_bldcm_enable(void);
void set_bldcm_disable(void);
void deal_serial_data(void);


#endif
