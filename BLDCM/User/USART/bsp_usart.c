#include "bsp_usart.h"

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// 串口中断优先级配置
	// NVIC_Configuration();
	
	// 使能串口接收中断
	// USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}


void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)       // 解操作，获得USART上的值
{
	 USART_SendData(pUSARTx, ch);
	 
	 while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data)
{
	uint8_t temp_h, temp_l;
	
	temp_h = (data&0xff00) >> 8;
	temp_l = data&0xff;
	
	USART_SendData(pUSARTx, temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(pUSARTx, temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
}

void Usart_SendString(USART_TypeDef *pUSARTx, char *str)     
{
	 unsigned int k=0;
	 
	 do{
			Usart_SendByte(pUSARTx, *(str+k));
			k++;
	 }while (*(str+k) != '\0'); 
	 
	 
	 while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET){}
}

// redirect the printf and scanf function
int fputc(int ch, FILE *f)
{
	/* transmit a byte to USART*/
	USART_SendData(DEBUG_USARTx, (uint8_t) ch);
	
	/* Awaiting for transmitting */
	while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
	
	return (ch);
}

// redirect the scanf function to USART, after redirecting you can use scanf() and getchar() etc.
int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE)  == RESET); // 读取寄存器非空
	
	return (int)USART_ReceiveData(DEBUG_USARTx);
}

void Show_Message(void)
{
	printf("\r\n 这是一个通过串口通信指令控制RGB的彩灯实验\n");
	printf("使用USART参数为： %d 8-n-1\n", DEBUG_USART_BAUDRATE);
	printf("开发板接到指令后控制RGB彩灯的颜色， 指令对应如下：\n");
	printf("指令 ----- 彩灯颜色\n");
	printf("	 1 ----- 红\n");
	printf("	 2 ----- 绿\n");
	printf("	 3 ----- 蓝\n");
	printf("	 4 ----- 黄\n");
	printf("	 5 ----- 紫\n");
	printf("	 6 ----- 青\n");
	printf("	 7 ----- 白\n");
	printf("	 8 ----- 灭\n");
}
