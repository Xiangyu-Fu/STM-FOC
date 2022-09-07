# HAL 库
## Introduction to the hal repo

### 1. Include
在Include文件夹中包含了 的是位于CMSIS标准的核内设备函数层的Cortex-M核通用的头文件， 它们的作用是为那些采用Cortex-M核设计SOC的芯片商设计的芯片外设提供一个进入内核的接口， 定义了一些内核相关的寄存器(类似我们前面写的stm32F103xx.h文件，但定义的是内核部分的寄存器)。


### 2. Divce 
Device文件夹下的是具体芯片直接相关的文件，包含启动文件、芯片外设寄存器定义、 系统时钟初始化功能的一些文件，这是由ST公司提供的。

system_stm32f1xx.c文件

文件目录：\Drivers\CMSIS\ Device\ST\stm32f1xx\Source\Templates

这个文件包含了STM32芯片上电后初始化系统时钟、扩展外部存储器用的函数，例如我们前两章提到供启动文件调用的“SystemInit”函数，用于上电后初始化时钟，该函数的定义就存储在system_stm32f1xx.c文件。STM32F103系列的芯片，调用库的这个SystemInit函数后，系统时钟被初始化为72MHz，如有需要可以修改这个文件的内容，设置成自己所需的时钟频率。

启动文件

文件目录：\Drivers\CMSIS\Device\ST\stm32f1xx\Source\Templates

在这个目录下，还有很多文件夹，如“ARM”、“gcc”、“iar”等，这些文件夹下包含了对应编译平台的汇编启动文件，在实际使用时要根据编译平台来选择。我们使用的MDK启动文件在“ARM”文件夹中。其中的“strartup_STM32F103xx.s”即为STM32F103芯片的启动文件，前面两章工程中使用的启动文件就是从这里复制过去的，strartup_STM32F103xe.s(xe适用于大容量产品)可兼容F103的 ve,ze,rc 芯片。如果使用其它型号的芯片，要在此处文件夹选择其他对应的启动文件。

stm32F103xx.h文件

文件目录：\Drivers\CMSIS\Device\ST\stm32f1xx\Include

stm32F103xx.h 这个文件非常重要，是一个STM32芯片底层相关的文件。它是我们前两章自己定义的“stm32F103xx.h”文件的完整版，包含了STM32中所有的外设寄存器地址和结构体类型定义，在使用到STM32 HAL库的地方都要包含这个头文件。


### 3. STM32F1xx_HAL_Driver
STM32F1xx_HAL_Driver文件夹下有inc（include的缩写）跟src（source的简写）这两个文件夹， 这里的文件属于CMSIS之外的的、芯片片上外设部分。src里面是每个设备外设的驱动源程序， inc则是相对应的外设头文件。**src及inc文件夹是ST的HAL库的主要内容。**


在src 和inc文件夹里的就是ST公司针对每个STM32外设而编写的库函数文件，每个外设对应一个*.c* 和 .h 后缀的文件。我们把这类外设文件统称为：stm32f1xx_hal_ppp.c 或stm32f1xx_hal_ppp.h文件，PPP表示外设名称。 如在上一章中我们自建的stm32f1xx_hal_gpio.c及stm32f1xx_hal_gpio.h文件，就属于这一类。


如针对模数转换`(ADC)`外设，在src文件夹下有一个`stm32f1xx_hal_adc.c`源文件， 在`inc`文件夹下有一个`stm32f1xx_hal_adc.h`头文件，若我们开发的工程中用到了STM32内部的ADC， 则至少要把这两个文件包含到工程里。这里很类似于ESP32的库，需要调用的时候包含相应的文件。

### 4. stm32f1xx_it.c、 stm32f1xx_hal_conf.h文件
我们在用库建立一个完整的工程时， 还需要添加这个目录下`src`文件夹中`stm32f1xx_it.c`和`inc`文件夹中 和`stm32f1xx_it.h`、`stm32f1xx_hal_conf.h`这三个文件。

**`stm32f1xx_it.c`**：这个文件是专门用来编写**中断服务函数**的，在我们修改前， 这个文件已经定义了一些系统异常(特殊中断)的接口，其它普通中断服务函数由我们自己添加。 但是我们怎么知道这些中断服务函数的接口如何写？是不是可以自定义呢？答案当然不是的， 这些都有可以在汇编启动文件中找到，在学习中断和启动文件的时候我们会详细介绍

**`stm32f1xx_hal_conf.h`**：这个文件被包含进stm32f103xx.h 文件。STM32HAL库支持所有STM32F1型号的芯片，但有的型号芯片外设功能比较多，所以使用这个**配置文件**根据芯片型号增减ST库的外设文件, 另外**时钟源配置**也是在这里进行设置。


`stm32f1xx_hal_conf.h` 这个文件还可配置是否使用“断言” 编译选项，
```c
   #ifdef  USE_FULL_ASSERT

   /**
   * @brief  The assert_param macro is used for  parameters check.
   * @param  expr: If expr is false, it calls assert_failed function
   *   which reports the name of the source file and the source
   *   line number of the call that failed.
   *   If expr is true, it returns no value.
   * @retval None
   */
   #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t
   *)__FILE__, __LINE__))
   /* Exported functions ---------------------------------- */
   void assert_failed(uint8_t* file, uint32_t line);
   #else
   #define assert_param(expr) ((void)0)
   #endif /* USE_FULL_ASSERT */
```

在ST的HAL库函数中，一般会包含输入参数检查，即上述代码中的“assert_param”宏，当参数不符合要求时，会调用“assert_failed”函数，这个函数默认是空的。

实际开发中使用断言时，先通过定义USE_FULL_ASSERT 宏来使能断言，然后定义“assert_failed”函数， 通常我们会让它调用printf函数输出错误说明。使能断言后，程序运行时会检查函数的输入参数， 当软件经过测试，可发布时，会取消USE_FULL_ASSERT宏来去掉断言功能，使程序全速运行。