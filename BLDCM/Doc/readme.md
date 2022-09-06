## TIM advanced timer
输出比较结构体 `TIM_OCInitTypeDef`

```c
    // 互补输出通道空闲电平极性配置
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
```

TIM 重载寄存器ARR --> Review the TIM

## DEBUG
From last night, I stuck in a bug that I found that I cannot open the breathing led, even I checked all of the code but I couldn't find anything. But compared with the example code, I found that the clock of TIM was not open. I use the same clock function on the two periperals. So,  next time, please notice that you must open the clock for all peripheral correctly.


# TIM Review
## RCC 系统时钟树 
RCC reset clock control 复位和时钟控制器 APB1, 最大36MHz。 APB2， 最大72MB。

## TIM　classification
IN STM32F2 serial production, except for the interconnect type, there are a total of 8 timers,  divided into basic timers, general-purpose timers and adcanced timers. The `basic timer` TIM6 and TIM7 are 16-bit count-up-only timer that can only be timed and have no external IO. 

TIM6&7是基本定时器，只能用来定时。

TIM2,3,4,5 是通用定时器，可以做到定时，输出比较， 输入捕捉，每个定时器有个四个外部的IO。->外部IO的作用？

TIM1,8 是高级定时器，有三相电机互补输出信号。

## 基本定时器
PSC预分频器， 
CNT是一个16位的计数器，最大计数为65535，当技术达到自动重装在寄存器的时候产生更新事件。
ARR自动重装载寄存器，是个16位的寄存器，储存计数器能计数的最大数值，如果达到这个值，并且使能中断了的话，定时器就会产生中断。

### 定时器初始化结构体
```c
typedef struct {
    uint16_t TIM_Prescaler;          // 预分频器
    uint16_t TIM_CounterMode;        // 计数模式，三种计数模式，向上，向下，和中心对齐
    uint32_t TIM_Period;             // 定时器周期， 设定自动重载寄存器ARR的值。在事件生成时更新到影子寄存器。
    uint16_t TIM_ClockDivision;      // 时钟分频，设置定时器时钟CK_INT频率与数字滤波器采样时钟频率分频比。
    uint8_t TIM_RepetitionCounter;   // 重复计算器， 属于高级控制寄存器专用寄存器位，利用它可以非常容易控制输出PWM的个数。
} TIM_TimeBaseInitTypeDef;
```

基本定时器模式配置：
定时器时钟，内部时钟`CK_INT = 72M`->自动重装载寄存器ARR的值`TIM_Period = 1000`->时钟预分频器`TIM_Prescaler = 71` -> （驱动计数器的时钟`CK_CNT = CK_INT/ (71 + 1) = 1M`, 预分频得到驱动计数器时钟）->初始化定时器 -> 清除中断标志位 -> 开启计数器中断 -> 使能计数器 

## 高级定时器

![advanced timer block diagramm](https://doc.embedfire.com/mcu/stm32/f103badao/std/zh/latest/_images/TIMsen002.png)


1. 时钟源

高级控制定时器有四个时钟源可选：

- 内部时钟源CK_INT

- 外部时钟模式1：外部输入引脚TIx（x=1,2,3,4）

- 外部时钟模式2：外部触发输入ETR

- 内部触发输入(ITRx)

2. 控制器
高级控制定时器控制器包括触发控制器、从模式控制器以及编码器接口。
- 出发控制器用来针对片内外设输出触发信号，为其他定时器提供时钟或者出发DAC/ADC转换
- 编码发接口专门的针对编码器技术而设计。
- 从模式控制器可以控制计数器复位启动，递增/递减,计数

3. 时基单元
高级控制定时器时基单元功能包括四个寄存器，分别是计数器寄存器(CNT)、预分频器寄存器(PSC)、自动重载寄存器(ARR)和重复计数器寄存器(RCR)。其中重复计数器RCR是高级定时器独有，通用和基本定时器没有。前面三个寄存器都是16位有效，TIMx_RCR寄存器是8位有效。
- 预分频器PSC，有一个输入时钟CK_PSC和一个输出时钟CK_CNT。
- 计数器CNT， 有三种计数模式，分别为递增计数模式、递减计数模式和递增/递减(中心对齐)计数模式。
- 自动重载寄存器ARR, 自动重载寄存器ARR用来存放与计数器CNT比较的值，如果两个值相等就递减重复计数器。
- 重复计数器RCR 在定时器发生上溢或下溢事件是递减重复计数器的值，只有当重复计数器为0时才会生成更新事件。

4. 输入捕获
输入捕获可以对输入的信号的上升沿、下降沿或者双边沿进行捕获，常用的有测量输入信号的脉宽，测量PWM输入信号的频率和占空比这两种。

输入捕获的大概的原理就是，当捕获到信号的跳变沿的时候，把计数器CNT的值锁存到捕获寄存器CCR中，把前后两次捕获到的CCR寄存器中的值相减， 就可以算出脉宽或者频率。

5. 输出比较
通过定时器的外部引脚对外输出控制信号，有冻结、将通道X（x=1,2,3,4）设置为匹配时输出有效电平、 将通道X设置为匹配时输出无效电平、翻转、强制变为无效电平、强制变为有效电平、PWM1和PWM2这八种模式， 具体使用哪种模式由寄存器CCMRx的位OCxM[2:0]配置。

5.4 输出引脚
输出比较的输出信号最终是通过定时器的外部IO来输出的，分别为CH1/2/3/4， 其中前面三个通道还有互补的输出通道CH1/2/3N。

6. 断路功能 
点击刹车功能


## 输入捕获应用
1. 测量脉宽或者频率

2. PWM输入模式

## 输出捕获应用
1. PWM输出模式
PWM模式分为两种 
![](https://doc.embedfire.com/mcu/stm32/f103badao/std/zh/latest/_images/TIMsen02.png)

下面我们以PWM1模式来讲解，以计数器CNT计数的方向不同还分为边沿对齐模式和中心对齐模式。PWM信号主要都是用来控制电机， 一般的电机控制用的都是边沿对齐模式，FOC电机一般用中心对齐模式。

1.1 PWM边沿对齐模式 
0->ARR，然后从0开始，并产生计数器上溢事件。

（比较寄存器CCR）

![](https://doc.embedfire.com/mcu/stm32/f103badao/std/zh/latest/_images/TIMsen014.png)

在边沿对齐模式下，计数器CNT只工作在一种模式，递增或者递减模式。这里我们以CNT工作在递增模式为例，ARR=8，CCR=4，CNT从0开始计数，当CNT`<`CCR的值时，OCxREF为有效的高电平，于此同时，比较中断寄存器CCxIF置位, `CCR=<CNT<=ARR`时，OCxREF为无效的低电平。然后CNT又从0开始计数并生成计数器上溢事件，以此循环往复。

1.2 PWM中心对齐模式
![](https://doc.embedfire.com/mcu/stm32/f103badao/std/zh/latest/_images/TIMsen015.png)

在中心对齐模式下，计数器CNT是工作做递增/递减模式下。开始的时候，计数器CNT从 0 开始计数到自动重载值减1(ARR-1)， 生成计数器上溢事件；然后从自动重载值开始向下计数到 1 并生成计数器下溢事件。之后从0 开始重新计数。

图 PWM1模式的中心对齐波形 是PWM1模式的中心对齐波形，ARR=8，CCR=4。 第一阶段计数器CNT工作在递增模式下，从0开始计数，当`CNT<CCR`的值时，OCxREF为有效的高电平， 当`CCR=<CNT<<ARR`时，OCxREF为无效的低电平。第二阶段计数器CNT工作在递减模式，从ARR的值开始递减， 当CNT>CCR时，OCxREF为无效的低电平，当CCR=>CNT>=1时，OCxREF为有效的高电平。

在波形图上我们把波形分为两个阶段，第一个阶段是计数器CNT工作在递增模式的波形，这个阶段我们又分为①和②两个阶段， 第二个阶段是计数器CNT工作在递减模式的波形，这个阶段我们又分为③和④两个阶段。要说中心对齐模式下的波形有什么特征的话， 那就是①和③阶段的时间相等，②和④阶段的时间相等。

中心对齐模式又分为中心对齐模式1/2/3 三种，具体由寄存器CR1位CMS[1:0]配置。 具体的区别就是比较中断中断标志位CCxIF在何时置1：中心模式1在CNT递减计数的时候置1， 中心对齐模式2在CNT递增计数时置1，中心模式3在CNT递增和递减计数时都置1。


## 定时器初始化结构体
时基结构体` `用于定时器基础参数设置，与TIM_TimeBaseInit函数配合使用完成配置。
```c
typedef struct {
    uint16_t TIM_Prescaler;          // 预分频器
    uint16_t TIM_CounterMode;        // 计数模式
    uint32_t TIM_Period;             // 定时器周期
    uint16_t TIM_ClockDivision;      // 时钟分频
    uint8_t TIM_RepetitionCounter;   // 重复计算器
} TIM_TimeBaseInitTypeDef;
```

输出比较结构体`TIM_OCInitTypeDef`用于输出比较模式，与TIM_OCxInit函数配合使用完成指定定时器输出通道初始化配置。高级控制定时器有四个定时器通道，使用时都必须单独设置。

```c
typedef struct {
    uint16_t TIM_OCMode;        // 比较输出模式,总共有八种，常用的为PWM1/PWM2。
    uint16_t TIM_OutputState;   // 比较输出使能, 决定最终的输出比较信号OCx是否通过外部引脚输出。它设定TIMx_CCER寄存器CCxE/CCxNE位的值
    uint16_t TIM_OutputNState;  // 比较互补输出使能
    uint32_t TIM_Pulse;         // 脉冲宽度, 实际设定比较寄存器CCR的值，决定脉冲宽度。可设置范围为0至65535。
    uint16_t TIM_OCPolarity;    // 输出极性, 比较输出极性，可选OCx为高电平有效或低电平有效。
    uint16_t TIM_OCNPolarity;   // 互补输出极性
    uint16_t TIM_OCIdleState;   // 空闲状态下比较输出状态
    uint16_t TIM_OCNIdleState;  // 空闲状态下比较互补输出状态
} TIM_OCInitTypeDef;
```

输入捕获结构体T`IM_ICInitTypeDef`用于输入捕获模式，与TIM_ICInit函数配合使用完成定时器输入通道初始化配置。 如果使用PWM输入模式需要与TIM_PWMIConfig函数配合使用完成定时器输入通道初始化配置。

```c
typedef struct {
    uint16_t TIM_Channel;      // 输入通道选择
    uint16_t TIM_ICPolarity;   // 输入捕获触发选择
    uint16_t TIM_ICSelection;  // 输入捕获选择
    uint16_t TIM_ICPrescaler;  // 输入捕获预分频器
    uint16_t TIM_ICFilter;     // 输入捕获滤波器
} TIM_ICInitTypeDef;
```


断路和死区结构体`TIM_BDTRInitTypeDef`用于断路和死区参数的设置，属于高级定时器专用，用于配置断路时通道输出状态，以及死区时间。 它与TIM_BDTRConfig函数配置使用完成参数配置。这个结构体的成员只对应BDTR这个寄存器，有关成员的具体使用配置请参考手册BDTR寄存器的详细描述。

```c
typedef struct {
    uint16_t TIM_OSSRState;        // 运行模式下的关闭状态选择
    uint16_t TIM_OSSIState;        // 空闲模式下的关闭状态选择
    uint16_t TIM_LOCKLevel;        // 锁定配置
    uint16_t TIM_DeadTime;         // 死区时间
    uint16_t TIM_Break;            // 断路输入使能控制
    uint16_t TIM_BreakPolarity;    // 断路输入极性
    uint16_t TIM_AutomaticOutput;  // 自动输出使能
} TIM_BDTRInitTypeDef;
```
