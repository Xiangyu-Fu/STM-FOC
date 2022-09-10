/**
 * Stucture of this file:
 * - bldcm_init()
 *      | - TIMx_Configuration()    // from bsp_motor_tim.h  
 *      | - hall_tim_config()       // from bsp_motor_tim.h
 *      | - sd_gpio_config() 
 * - motor control   
 *      | - set_bldcm_speed() 
 *      | - set_bldcm_direction() 
 *      | - get_bldcm_direction() 
 *      | - set_bldcm_enable()     
 *      | - set_bldcm_disable()   
 * - USART commands
 *      | - deal_serial_data() -> show_help()
 * @copyright Copyright (c) 2022
 * 
 */
# include ".\BLDCM\bsp_bldcm_control.h"
# include "./USART/bsp_usart.h"
# include <stdlib.h>
# include <math.h>

// private variables
static bldcm_data_t bldcm_data;

// local functions
static void sd_gpio_config(void);

/**
 * @brief Initialize the BLDCM
 */
void bldcm_init(void)
{
  TIMx_Configuration();    // 电机控制定时器，引脚初始化
  hall_tim_config();       // 霍尔传感器初始化
  sd_gpio_config();        // sd 引脚初始化
}


static void sd_gpio_config(void)
{   
    GPIO_InitTypeDef GPIO_InitStruct;

    SHUTDOWN_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = SHUTDOWN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(SHUTDOWN_PORT, &GPIO_InitStruct);
}

/**
  * @brief  set the speed of the motor
  */
void set_bldcm_speed(uint16_t v)
{
  bldcm_data.dutyfactor = v;
  
  set_pwm_pulse(v);    
}

/**
  * @brief  set the direction of the motor
  */
void set_bldcm_direction(motor_dir_t dir)
{
  bldcm_data.direction = dir;
}

/**
  * @brief  get the motor direction
  */
motor_dir_t get_bldcm_direction(void)
{
  return bldcm_data.direction;
}

/**
  * @brief  Enable the motor
  */
void set_bldcm_enable(void)
{
  BLDCM_ENABLE_SD();
	HAL_Delay(1);
  hall_enable();
}

/**
  * @brief  Disable the motor
  */
void set_bldcm_disable(void)
{
    hall_disable();

    stop_pwm_output();

    BLDCM_DISABLE_SD();
}


////////////////////// MOTOR CONTROL /////////////////////////

/**
  * @brief  打印帮助命令
  * @param  无
  * @retval 无
  */
void show_help(void)
{
    printf("——————————————野火直流无刷电机驱动演示程序——————————————\n\r");
    printf("输入命令(以回车结束)：\n\r");
    printf("< ? >        -帮助菜单\n\r");
    printf("v [data]     -设置电机的速度（范围：0—%d）\n\r", PWM_PERIOD_COUNT);
    printf("d [data]     -设置电机的方向，%d:正向转，%d:反向转\n\r", MOTOR_FWD, MOTOR_REV);
}

/**
  * @brief  处理串口接收到的数据
  * @param  无
  * @retval 无
  */
void deal_serial_data(void)
{
    static char showflag =1;
    int dec_temp=0;
    int speed_temp=0;
    
    //接收到正确的指令才为1
    char okCmd = 0;
  
    if (showflag)
    {
      show_help();
      showflag = !showflag;
    }

    //检查是否接收到指令
    if(receive_cmd == 1) // this bit set by usart interrupt
    {
      if(UART_RxBuffer[0] == 'v' || UART_RxBuffer[0] == 'V')
      {
        //设置速度
        if(UART_RxBuffer[1] == ' ')
        {
          speed_temp = atoi((char const *)UART_RxBuffer+2);
          if(speed_temp>=0 && speed_temp <= PWM_PERIOD_COUNT)
          {
            set_bldcm_speed(speed_temp);
            set_bldcm_enable();
            printf("\n\r速度: %d\n\r", speed_temp);
            okCmd = 1;
          }
        }
      }
      else if(UART_RxBuffer[0] == 'd')
      {
        //设置方向
        if(UART_RxBuffer[1] == ' ')
        {
          dec_temp = atoi((char const *)UART_RxBuffer+2);

          if(dec_temp>=0)
          {
            set_bldcm_direction(dec_temp);
            printf("\n\r方向:%s\n\r", dec_temp ? "反向转" : "正向转");
            okCmd = 1;
          }
        }
      }
      else if(UART_RxBuffer[0] == '?')
      {
        //打印帮助命令
        show_help();
        okCmd = 1;
      }
      //如果指令有无则打印帮助命令
      if(okCmd != 1)
      {
        printf("\n\r 输入有误，请重新输入...\n\r");
        show_help();
      }

      //清空串口接收缓冲数组
      receive_cmd = 0;
      uart_FlushRxBuffer();

    }
}
