/*********************************************************************************************/

【*】 程序简介 
-工程名称：无刷电机-串口控制
-实验平台: 野火STM32 F103 霸道开发板

【 ！】功能简介：
通过串口发送指令,设置电机方向和速度.
指令示例:
d 0
方向正转
v 1000
速度设置为1000
注意指令结尾需要换行,或在串口助手中勾选发送新行,一次只能发送一条指令

【 ！】实验操作：

接线:
注意接线是有序的,使用了LCD显示屏引出的接口:
电机驱动板 5V_IN\GND 		<----> 开发板 5V\GND
电机驱动板 U+\V+\W+ 		<----> 开发板 PE9\PE11\PE13
电机驱动板 U-\V-\W- 		<----> 开发板 PE8\PE10\PE12
电机驱动板 SD\GND				<----> 开发板 PD15\GND
电机驱动板 HU\HV\HW\GND	<----> 开发板 PD12(丝印为BK)\PD13\PD14\GND --> 编码器

下载本程序，复位开发板即可。

【 ！！】注意事项：
无

/*********************************************************************************************/

【*】 引脚分配

定时器TIM1输出PWM信号:
三路PWM输出到PE9\PE11\PE13上连接到驱动板上的U+\V+\W+接口.

电机使能引脚:
电机驱动板 SD <----> 开发板 PD15

编码器:
霍尔编码器信号从HU\HV\HW引脚输出,接入开发板PD12(丝印为BK)\PD13\PD14定时器捕获.
/*********************************************************************************************/

【*】 时钟

A.晶振：
-外部高速晶振：25MHz
-RTC晶振：32.768KHz

B.各总线运行时钟：
-系统时钟 = SYCCLK = AHB1 = 72MHz
-APB2 = 36MHz 
-APB1 = 36MHz

C.浮点运算单元：
  不使用


/*********************************************************************************************/

【*】 联系我们

-野火论坛    :http://www.firebbs.cn
-资料中心    :http://doc.embedfire.com
-淘宝店铺    :https://fire-stm32.taobao.com

/*********************************************************************************************/