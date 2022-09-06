#include "bsp_motor_tim.h"

// TIM复用功能引脚初始化
static void TIMx_GPIO_Config(void)
{
   /*定义一个GPIO_InitTypeDef类型的结构体*/
   GPIO_InitTypeDef GPIO_InitStructure;

   /*开启定时器相关的GPIO外设时钟*/
   MOTOR_OCPWM1_GPIO_CLK_ENABLE();
   MOTOR_OCNPWM1_GPIO_CLK_ENABLE();
   MOTOR_OCPWM2_GPIO_CLK_ENABLE();
   MOTOR_OCNPWM2_GPIO_CLK_ENABLE();
   MOTOR_OCPWM3_GPIO_CLK_ENABLE();
   MOTOR_OCNPWM3_GPIO_CLK_ENABLE();

   /* 定时器功能引脚初始化 */
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;   // 推挽输出模式

   GPIO_InitStructure.Pin = MOTOR_OCNPWM1_PIN;
   HAL_GPIO_Init(MOTOR_OCNPWM1_GPIO_PORT, &GPIO_InitStructure);

   GPIO_InitStructure.Pin = MOTOR_OCNPWM2_PIN;
   HAL_GPIO_Init(MOTOR_OCNPWM2_GPIO_PORT, &GPIO_InitStructure);

   GPIO_InitStructure.Pin = MOTOR_OCNPWM3_PIN;
   HAL_GPIO_Init(MOTOR_OCNPWM3_GPIO_PORT, &GPIO_InitStructure);

   /* 通道 2 */
   GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;

   GPIO_InitStructure.Pin = MOTOR_OCPWM1_PIN;
   GPIO_InitStructure.Alternate = MOTOR_OCPWM1_AF;
   HAL_GPIO_Init(MOTOR_OCPWM1_GPIO_PORT, &GPIO_InitStructure);

   GPIO_InitStructure.Pin = MOTOR_OCPWM2_PIN;
   GPIO_InitStructure.Alternate = MOTOR_OCPWM2_AF;
   HAL_GPIO_Init(MOTOR_OCPWM2_GPIO_PORT, &GPIO_InitStructure);

   /* 通道 3 */
   GPIO_InitStructure.Pin = MOTOR_OCPWM3_PIN;
   GPIO_InitStructure.Alternate = MOTOR_OCPWM3_AF;
   HAL_GPIO_Init(MOTOR_OCPWM3_GPIO_PORT, &GPIO_InitStructure);
}

// 配置TIM的模式
static void TIM_Mode_Config(void)
{
   // 开启TIMx_CLK,x[1,8]
   MOTOR_TIM_CLK_ENABLE();
   /* 定义定时器的句柄即确定定时器寄存器的基地址*/
   htimx_bldcm.Instance = MOTOR_TIM;
   /* 累计 TIM_Period个后产生一个更新或者中断*/
   //当定时器从0计数到999，即为1000次，为一个定时周期
   htimx_bldcm.Init.Period = PWM_PERIOD_COUNT - 1;
   // 高级控制定时器时钟源TIMxCLK = HCLK=216MHz // NOTICE, NEED TO CHECK
   // 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=1MHz
   htimx_bldcm.Init.Prescaler = PWM_PRESCALER_COUNT - 1;
   // 采样时钟分频
   htimx_bldcm.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
   // 计数方式
   htimx_bldcm.Init.CounterMode=TIM_COUNTERMODE_UP;
   // 重复计数器
   htimx_bldcm.Init.RepetitionCounter=0;
   // 初始化定时器TIMx, x[1,8]
   HAL_TIM_PWM_Init(&htimx_bldcm);

   /*PWM模式配置*/
   //配置为PWM模式1
   TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
   TIM_OCInitStructure.Pulse = 0;                         // 默认必须要初始为0
   TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
   TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
   TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
   TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;

   HAL_TIM_PWM_ConfigChannel(&htimx_bldcm,&TIM_OCInitStructure,TIM_CHANNEL_1);    // 初始化通道 1 输出 PWM
   HAL_TIM_PWM_ConfigChannel(&htimx_bldcm,&TIM_OCInitStructure,TIM_CHANNEL_2);    // 初始化通道 2 输出 PWM
   HAL_TIM_PWM_ConfigChannel(&htimx_bldcm,&TIM_OCInitStructure,TIM_CHANNEL_3);    // 初始化通道 3 输出 PWM

   /* 配置触发源 */
   HAL_TIMEx_ConfigCommutationEvent(&htimx_bldcm, TIM_COM_TS_ITRx, TIM_COMMUTATION_SOFTWARE);

   /* 开启定时器通道1输出PWM */
   HAL_TIM_PWM_Start(&htimx_bldcm,TIM_CHANNEL_1);

   /* 开启定时器通道2输出PWM */
   HAL_TIM_PWM_Start(&htimx_bldcm,TIM_CHANNEL_2);

   /* 开启定时器通道3输出PWM */
   HAL_TIM_PWM_Start(&htimx_bldcm,TIM_CHANNEL_3);
}

static void hall_tim_init(void)
{
   TIM_HallSensor_InitTypeDef  hall_sensor_onfig;

   /* 基本定时器外设时钟使能 */
   HALL_TIM_CLK_ENABLE();

   /* 定时器基本功能配置 */
   htimx_hall.Instance = HALL_TIM;
   htimx_hall.Init.Prescaler = HALL_PRESCALER_COUNT - 1;       // 预分频
   htimx_hall.Init.CounterMode = TIM_COUNTERMODE_UP;           // 向上计数
   htimx_hall.Init.Period = HALL_PERIOD_COUNT - 1;             // 计数周期
   htimx_hall.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;     // 时钟分频

   hall_sensor_onfig.IC1Prescaler = TIM_ICPSC_DIV1;            // 输入捕获分频
   hall_sensor_onfig.IC1Polarity = TIM_ICPOLARITY_BOTHEDGE;    // 输入捕获极性
   hall_sensor_onfig.IC1Filter = 10;                           // 输入滤波
   hall_sensor_onfig.Commutation_Delay = 0U;                   // 不使用延迟触发
   HAL_TIMEx_HallSensor_Init(&htimx_hall,&hall_sensor_onfig);

   HAL_NVIC_SetPriority(HALL_TIM_IRQn, 0, 0);    // 设置中断优先级
   HAL_NVIC_EnableIRQ(HALL_TIM_IRQn);            // 使能中断
}

static void hall_tim_init(void)
{
   TIM_HallSensor_InitTypeDef  hall_sensor_onfig;

   /* 基本定时器外设时钟使能 */
   HALL_TIM_CLK_ENABLE();

   /* 定时器基本功能配置 */
   htimx_hall.Instance = HALL_TIM;
   htimx_hall.Init.Prescaler = HALL_PRESCALER_COUNT - 1;       // 预分频
   htimx_hall.Init.CounterMode = TIM_COUNTERMODE_UP;           // 向上计数
   htimx_hall.Init.Period = HALL_PERIOD_COUNT - 1;             // 计数周期
   htimx_hall.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;     // 时钟分频

   hall_sensor_onfig.IC1Prescaler = TIM_ICPSC_DIV1;            // 输入捕获分频
   hall_sensor_onfig.IC1Polarity = TIM_ICPOLARITY_BOTHEDGE;    // 输入捕获极性
   hall_sensor_onfig.IC1Filter = 10;                           // 输入滤波
   hall_sensor_onfig.Commutation_Delay = 0U;                   // 不使用延迟触发
   HAL_TIMEx_HallSensor_Init(&htimx_hall,&hall_sensor_onfig);

   HAL_NVIC_SetPriority(HALL_TIM_IRQn, 0, 0);    // 设置中断优先级
   HAL_NVIC_EnableIRQ(HALL_TIM_IRQn);            // 使能中断
}